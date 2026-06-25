#include "facerecognition.h"
#include "ui_facerecognition.h"
#include <QJsonDocument>
#include <QJsonParseError>
#include <QJsonObject>
#include <QDebug>
#include <QThread>

FaceRecognition::FaceRecognition(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::FaceRecognition)
    , is_connected(false)
    , is_waiting_response(false)
    , recognition_started(false)
    , timer_id(-1)
    , m_frameCount(0)
{
    ui->setupUi(this);
    setFixedSize(1024, 600);

    // 隐藏工号和部门（只展示姓名与时间）
    ui->idTitleLabel->hide();
    ui->idValueLabel->hide();
    ui->deptTitleLabel->hide();
    ui->deptValueLabel->hide();

    // 连接信号槽
    connect(ui->backBtn, &QPushButton::clicked, this, &FaceRecognition::onBackButtonClicked);
    connect(&msocket, &QTcpSocket::disconnected, this, &FaceRecognition::onDisconnected);
    connect(&msocket, &QTcpSocket::connected, this, &FaceRecognition::onConnected);
    connect(&msocket, &QTcpSocket::readyRead, this, &FaceRecognition::recv_data);
    connect(&mtimer, &QTimer::timeout, this, &FaceRecognition::tryConnect);
    connect(&reset_timer, &QTimer::timeout, this, &FaceRecognition::resetState);
    connect(&response_timer, &QTimer::timeout, this, &FaceRecognition::onTimeout);
    
    response_timer.setSingleShot(true);
    mtimer.start(5000);

    // 初始化UI
    clearUserInfo();
    updateConnectionStatus(false);
    ui->faceStatusLabel->setText("请将面部对准摄像头");
    ui->resultStatusLabel->setText("等待识别...");

    qDebug() << "=== 人脸识别页面初始化完成 ===";
}

FaceRecognition::~FaceRecognition()
{
    if(cap.isOpened()) cap.release();
    delete ui;
}

void FaceRecognition::onBackButtonClicked()
{
    stopRecognition();
    emit backToHomeClicked();
}

void FaceRecognition::startRecognition()
{
    if(recognition_started) return;

    // 打开摄像头
    const char* devices[] = {"/dev/video2", "/dev/video1", "/dev/video0", "0"};
    for(const char* dev : devices) {
        if(cap.open(dev)) {
            cap.set(cv::CAP_PROP_FRAME_WIDTH, 640);
            cap.set(cv::CAP_PROP_FRAME_HEIGHT, 480);
            cap.set(cv::CAP_PROP_FPS, 15);
            qDebug() << "✓ 摄像头:" << dev;
            break;
        }
    }
    if(!cap.isOpened()) {
        ui->faceStatusLabel->setText("摄像头初始化失败");
        return;
    }

    // 加载分类器
    if(!cascade.load("./haarcascade_frontalface_alt2.xml") && 
       !cascade.load("./haarcascade_frontalface_default.xml")) {
        ui->faceStatusLabel->setText("分类器加载失败");
        return;
    }

    timer_id = startTimer(66);  // 约15fps
    recognition_started = true;
    qDebug() << "=== 人脸识别启动 ===";
}

void FaceRecognition::stopRecognition()
{
    if(!recognition_started) return;
    
    if(timer_id >= 0) {
        killTimer(timer_id);
        timer_id = -1;
    }
    if(cap.isOpened()) cap.release();
    
    recognition_started = false;
    m_recvBuffer.clear();
    is_waiting_response = false;
    clearUserInfo();
}

void FaceRecognition::timerEvent(QTimerEvent *e)
{
    if(e->timerId() != timer_id || !recognition_started) return;

    // 采集帧
    if(!cap.isOpened() || !cap.read(m_frame) || m_frame.empty()) return;

    // 显示画面（缩放到显示区域大小）
    cv::resize(m_frame, m_displayFrame, cv::Size(440, 330));
    cv::cvtColor(m_displayFrame, m_rgbFrame, cv::COLOR_BGR2RGB);
    QImage img(m_rgbFrame.data, m_rgbFrame.cols, m_rgbFrame.rows, 
               m_rgbFrame.step, QImage::Format_RGB888);
    ui->videoLabel->setPixmap(QPixmap::fromImage(img));

    // 非空闲状态或未连接时只显示画面
    if(!is_connected || is_waiting_response) return;

    // 人脸检测：为避免“刚入画的模糊帧”直接送检，这里每帧检测并做连续帧缓冲
    ++m_frameCount;

    // 缩小图像进行人脸检测
    cv::resize(m_frame, m_smallFrame, cv::Size(320, 240));
    cv::cvtColor(m_smallFrame, m_grayFrame, cv::COLOR_BGR2GRAY);

    std::vector<cv::Rect> faces;
    cascade.detectMultiScale(m_grayFrame, faces, 1.2, 3, 0, cv::Size(30, 30));

    if(faces.empty()) {
        m_consecutiveFaceFrames = 0;
        if(m_frameCount % 30 == 0) {
            ui->faceStatusLabel->setText("请将面部对准摄像头");
        }
        return;
    }

    // 检测到人脸：先缓冲几帧，跳过刚入画的模糊帧
    m_consecutiveFaceFrames++;
    constexpr int kSendAfterFrames = 4;   // 第1帧只用于“出现检测”，第4帧再发送
    if (m_consecutiveFaceFrames < kSendAfterFrames) {
        ui->faceStatusLabel->setText("检测到人脸，请保持静止...");
        return;
    }

    // 可选：做一次清晰度检查，模糊则继续等待后续帧
    const cv::Rect faceSmall = faces[0];
    cv::Rect safeSmall = faceSmall & cv::Rect(0, 0, m_grayFrame.cols, m_grayFrame.rows);
    if (safeSmall.width > 0 && safeSmall.height > 0) {
        cv::Mat roiGray = m_grayFrame(safeSmall);
        cv::Mat lap;
        cv::Laplacian(roiGray, lap, CV_64F);
        cv::Scalar mean, stddev;
        cv::meanStdDev(lap, mean, stddev);
        const double variance = stddev[0] * stddev[0];
        constexpr double kMinSharpness = 60.0; // 经验阈值：过小代表运动模糊/失焦
        if (variance < kMinSharpness) {
            ui->faceStatusLabel->setText("画面较模糊，请保持静止...");
            return;
        }
    }

    ui->faceStatusLabel->setText("检测到人脸，正在识别...");
    
    // 保存人脸区域（放大回原始尺寸）
    cv::Rect face = faces[0];
    face.x *= 2; face.y *= 2;
    face.width *= 2; face.height *= 2;
    if(face.x >= 0 && face.y >= 0 && 
       face.x + face.width <= m_frame.cols && 
       face.y + face.height <= m_frame.rows) {
        m_faceROI = m_frame(face).clone();
    }

    sendFrame();
}

void FaceRecognition::sendFrame()
{
    if(!is_connected || is_waiting_response) return;

    // JPEG压缩
    std::vector<uchar> buf;
    std::vector<int> params = {cv::IMWRITE_JPEG_QUALITY, 75};
    cv::imencode(".jpg", m_frame, buf, params);

    // 发送数据
    QByteArray data;
    QDataStream stream(&data, QIODevice::WriteOnly);
    stream.setVersion(QDataStream::Qt_5_9);
    stream << (quint64)buf.size() << QByteArray((char*)buf.data(), buf.size());

    msocket.write(data);
    is_waiting_response = true;
    response_timer.start(8000);
    
    qDebug() << "[发送]" << data.size() << "字节";
}

void FaceRecognition::recv_data()
{
    m_recvBuffer.append(msocket.readAll());

    // 提取完整JSON
    while(!m_recvBuffer.isEmpty()) {
        int start = m_recvBuffer.indexOf('{');
        if(start < 0) { m_recvBuffer.clear(); break; }
        if(start > 0) m_recvBuffer = m_recvBuffer.mid(start);

        // 查找匹配的 '}'
        int depth = 0, end = -1;
        bool inStr = false;
        for(int i = 0; i < m_recvBuffer.size(); i++) {
            char c = m_recvBuffer[i];
            if(c == '"' && (i == 0 || m_recvBuffer[i-1] != '\\')) inStr = !inStr;
            if(!inStr) {
                if(c == '{') depth++;
                else if(c == '}' && --depth == 0) { end = i; break; }
            }
        }
        if(end < 0) break;

        // 解析JSON
        QByteArray json = m_recvBuffer.left(end + 1);
        m_recvBuffer = m_recvBuffer.mid(end + 1);

        QJsonDocument doc = QJsonDocument::fromJson(json);
        if(doc.isNull()) continue;

        QJsonObject obj = doc.object();
        QString empId = obj["employeeID"].toString().trimmed();
        QString name = obj["name"].toString().trimmed();

        // 跳过坐标数据
        if((obj.contains("rect_x") || obj.contains("rect_y")) && 
           (empId.isEmpty() || name.isEmpty())) continue;

        if(!is_waiting_response) continue;

        // 处理识别结果
        is_waiting_response = false;
        response_timer.stop();

        if(empId.isEmpty() || name.isEmpty()) {
            qDebug() << "[识别] 未识别";
            ui->resultStatusLabel->setText("未识别到注册用户");
            ui->faceStatusLabel->setText("请重新对准摄像头");
        } else {
            qDebug() << "[识别] ✓" << name;
            ui->resultStatusLabel->setText("识别成功");
            ui->faceStatusLabel->setText("验证通过");
            showUserInfo(empId, name, obj["department"].toString(), obj["time"].toString());
            
            if(!m_faceROI.empty()) {
                cv::imwrite("./face.jpg", m_faceROI);
                ui->avatarLabel->setStyleSheet("border-radius:60px;border-image:url(./face.jpg);");
            }
            
            emit recognitionSuccess(empId, name);
            reset_timer.start(5000);
            return;  // 识别成功后暂停
        }
    }
}

void FaceRecognition::resetState()
{
    reset_timer.stop();
    is_waiting_response = false;
    clearUserInfo();
    ui->resultStatusLabel->setText("等待识别...");
    ui->faceStatusLabel->setText("请将面部对准摄像头");
}

void FaceRecognition::onTimeout()
{
    is_waiting_response = false;
    ui->faceStatusLabel->setText("响应超时，请重试");
}

void FaceRecognition::tryConnect()
{
    if(is_connected || msocket.state() == QTcpSocket::ConnectingState) return;
    if(msocket.state() != QTcpSocket::UnconnectedState) msocket.abort();
    
    msocket.connectToHost("192.168.2.220", 8888);
}

void FaceRecognition::onConnected()
{
    qDebug() << "[网络] ✓ 已连接";
    is_connected = true;
    mtimer.stop();
    updateConnectionStatus(true);
}

void FaceRecognition::onDisconnected()
{
    qDebug() << "[网络] 断开";
    is_connected = false;
    is_waiting_response = false;
    updateConnectionStatus(false);
    mtimer.start(5000);
}

void FaceRecognition::updateConnectionStatus(bool connected)
{
    ui->statusIndicator->setText(connected ? "● 已连接" : "● 未连接");
    ui->statusIndicator->setStyleSheet(QString("font-size:12px;color:%1;background:transparent;")
        .arg(connected ? "rgb(82,196,26)" : "rgb(245,34,45)"));
}

void FaceRecognition::showUserInfo(const QString &id, const QString &name, 
                                   const QString &dept, const QString &time)
{
    Q_UNUSED(id);
    Q_UNUSED(dept);
    ui->nameValueLabel->setText(name);
    ui->timeValueLabel->setText(time);
}

void FaceRecognition::clearUserInfo()
{
    ui->nameValueLabel->setText("--");
    ui->timeValueLabel->setText("--:--:--");
    ui->avatarLabel->setStyleSheet("background-color:rgb(240,240,240);border-radius:60px;");
}
