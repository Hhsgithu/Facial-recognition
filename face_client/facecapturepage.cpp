#include "facecapturepage.h"
#include "ui_facecapturepage.h"
#include <QDebug>
#include <QThread>

FaceCapturePage::FaceCapturePage(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::FaceCapturePage)
    , m_hardwareController(nullptr)
    , m_previewTimer(new QTimer(this))
    , m_faceCaptureState(FaceInitializing)
    , m_confirmButton(nullptr)
    , m_retakeButton(nullptr)
    , m_previewLabel(nullptr)
    , m_confirmImageLabel(nullptr)
    , m_faceDetected(false)
    , m_captureCountdown(0)
    , m_bestSharpness(0.0)
    , m_captureTimer(new QTimer(this))
{
    ui->setupUi(this);
    
    // 设置固定大小和背景
    setFixedSize(1024, 600);
    setAutoFillBackground(true);
    
    QPalette pal = palette();
    pal.setColor(QPalette::Window, QColor(245, 247, 250));
    setPalette(pal);

    // 初始化人脸检测器
    QString cascadePath = "haarcascade_frontalface_default.xml";
    if (!m_faceClassifier.load(cascadePath.toStdString())) {
        qWarning() << "[人脸采集页面] 无法加载人脸检测器:" << cascadePath;
    }

    // 连接信号槽
    connect(ui->backBtn, &QPushButton::clicked, this, &FaceCapturePage::onBackClicked);
    connect(ui->startCaptureBtn, &QPushButton::clicked, this, &FaceCapturePage::onStartCapture);
    connect(m_previewTimer, &QTimer::timeout, this, &FaceCapturePage::onPreviewTimer);
    connect(m_captureTimer, &QTimer::timeout, this, &FaceCapturePage::onCaptureTimerTick);
    
    // 创建手动拍照相关的UI组件
    // 预览标签 (440x330) - 在左侧预览容器中
    m_previewLabel = new QLabel(ui->previewContainer);
    m_previewLabel->setFixedSize(440, 330);
    m_previewLabel->setAlignment(Qt::AlignCenter);
    m_previewLabel->setStyleSheet("background-color: rgb(50, 50, 50); border: 2px solid rgb(200, 200, 200); border-radius: 8px;");
    m_previewLabel->move(30, 40);
    m_previewLabel->hide();
    
    // 确认图像标签 (300x225) - 居中显示
    m_confirmImageLabel = new QLabel(this);
    m_confirmImageLabel->setFixedSize(300, 225);
    m_confirmImageLabel->setAlignment(Qt::AlignCenter);
    m_confirmImageLabel->setStyleSheet("background-color: rgb(50, 50, 50); border: 2px solid rgb(200, 200, 200); border-radius: 8px;");
    m_confirmImageLabel->move(362, 220);
    m_confirmImageLabel->hide();
    
    // 确认按钮 - 在确认图像下方
    m_confirmButton = new QPushButton("确认", this);
    m_confirmButton->setFixedSize(120, 50);
    m_confirmButton->setStyleSheet(
        "QPushButton {"
        "   background-color: rgb(82, 196, 26);"
        "   color: white;"
        "   border: none;"
        "   border-radius: 8px;"
        "   font-size: 16px;"
        "   font-weight: bold;"
        "}"
        "QPushButton:pressed {"
        "   background-color: rgb(70, 170, 20);"
        "}"
    );
    m_confirmButton->move(572, 470);
    m_confirmButton->hide();
    connect(m_confirmButton, &QPushButton::clicked, this, &FaceCapturePage::onConfirmButtonClicked);
    
    // 重拍按钮 - 在确认按钮左侧
    m_retakeButton = new QPushButton("重拍", this);
    m_retakeButton->setFixedSize(120, 50);
    m_retakeButton->setStyleSheet(
        "QPushButton {"
        "   background-color: rgb(255, 193, 7);"
        "   color: white;"
        "   border: none;"
        "   border-radius: 8px;"
        "   font-size: 16px;"
        "   font-weight: bold;"
        "}"
        "QPushButton:pressed {"
        "   background-color: rgb(230, 170, 0);"
        "}"
    );
    m_retakeButton->move(432, 470);
    m_retakeButton->hide();
    connect(m_retakeButton, &QPushButton::clicked, this, &FaceCapturePage::onRetakeButtonClicked);

    qDebug() << "[人脸采集页面] 人脸采集页面初始化完成";
}

FaceCapturePage::~FaceCapturePage()
{
    cleanupCamera();
    delete ui;
}

void FaceCapturePage::setUserInfo(const UserInfo& userInfo)
{
    m_userInfo = userInfo;
    ui->userNameLabel->setText(QString("用户：%1").arg(userInfo.name));
    qDebug() << "[人脸采集页面] 设置用户信息:" << userInfo.name;
}

void FaceCapturePage::setHardwareController(HardwareController* controller)
{
    m_hardwareController = controller;
}

void FaceCapturePage::startCapture()
{
    qDebug() << "[人脸采集页面] 开始采集流程";
    
    // 清理之前的捕获帧
    if (!m_capturedFrame.empty()) {
        m_capturedFrame.release();
        qDebug() << "[人脸采集页面] 清理之前的捕获帧";
    }
    
    // 停止多帧采集定时器（如果正在运行）
    if (m_captureTimer->isActive()) {
        m_captureTimer->stop();
    }
    m_captureCountdown = 0;
    m_bestSharpness = -1.0;

    // 重置状态
    m_faceCaptureState = FaceInitializing;
    
    // 隐藏确认界面组件
    m_confirmImageLabel->hide();
    m_confirmButton->hide();
    m_retakeButton->hide();
    
    // 显示预览容器和控制容器
    ui->previewContainer->show();
    ui->controlContainer->show();
    ui->previewPlaceholder->show();
    m_previewLabel->hide();
    
    // 重置按钮
    ui->startCaptureBtn->setText("开始采集");
    ui->startCaptureBtn->setEnabled(true);
    
    updateStatus("点击\"开始采集\"按钮开始", "rgb(100, 100, 100)");
}

void FaceCapturePage::onStartCapture()
{
    // 复用同一个按钮：第一次点击开启预览；预览中再次点击直接拍照
    if (m_faceCaptureState == FacePreviewing) {
        onCaptureButtonClicked();
        return;
    }

    qDebug() << "[人脸采集页面] 用户点击开始采集";
    ui->startCaptureBtn->setEnabled(false);
    startLivePreview();
}

void FaceCapturePage::onBackClicked()
{
    qDebug() << "[人脸采集页面] 用户点击返回";
    
    cleanupCamera();
    emit backClicked();
}

bool FaceCapturePage::initializeCamera()
{
    qDebug() << "[人脸采集页面] 初始化摄像头";
    
    // 如果摄像头已打开，先释放再重新打开
    if (m_camera.isOpened()) {
        qDebug() << "[人脸采集页面] 检测到摄像头已打开，先释放";
        m_camera.release();
        QThread::msleep(200);
    }

    qDebug() << "[人脸采集页面] 尝试打开摄像头...";
    
    // 尝试多个摄像头索引
    int cameraIndices[] = {0, 1, 2, -1};
    
    for (int i = 0; cameraIndices[i] != -2; i++) {
        int index = cameraIndices[i];
        qDebug() << "[人脸采集页面] 尝试打开摄像头索引:" << index;
        
        if (index == -1) {
            m_camera.open(cv::CAP_ANY);
        } else {
            m_camera.open(index);
        }
        
        if (m_camera.isOpened()) {
            qDebug() << "[人脸采集页面] ✓ 摄像头打开成功，索引:" << index;
            
            // 统一设置为 640x480，与认证摄像头分辨率一致
            m_camera.set(cv::CAP_PROP_FRAME_WIDTH, 640);
            m_camera.set(cv::CAP_PROP_FRAME_HEIGHT, 480);
            m_camera.set(cv::CAP_PROP_FPS, 20);
            
            // 测试读取一帧
            cv::Mat testFrame;
            if (m_camera.read(testFrame) && !testFrame.empty()) {
                qDebug() << "[人脸采集页面] ✓ 摄像头测试成功";
                qDebug() << "  实际分辨率:" << testFrame.cols << "x" << testFrame.rows;
                return true;
            } else {
                qDebug() << "[人脸采集页面] 摄像头打开但无法读取图像，尝试下一个";
                m_camera.release();
            }
        }
        
        if (i == 2) {
            cameraIndices[i+1] = -2;
        }
    }
    
    qDebug() << "[人脸采集页面] ✗ 所有摄像头索引都无法打开";
    return false;
}

void FaceCapturePage::startLivePreview()
{
    qDebug() << "[人脸采集页面] 启动实时预览";
    
    if (!initializeCamera()) {
        m_faceCaptureState = FaceError;
        updateStatus("摄像头初始化失败，请检查设备连接", "rgb(255, 77, 79)");
        ui->startCaptureBtn->setEnabled(true);
        return;
    }
    
    m_faceCaptureState = FacePreviewing;
    m_faceDetected = false;
    
    // 显示预览标签；并让“开始采集”按钮切换为“拍照”
    ui->previewPlaceholder->hide();
    m_previewLabel->show();
    ui->startCaptureBtn->setText("拍照");
    ui->startCaptureBtn->setEnabled(true);
    
    // 启动预览定时器
    m_previewTimer->start(50);
    
    updateStatus("请正视摄像头，点击拍照按钮", "rgb(100, 100, 100)");
}

void FaceCapturePage::stopLivePreview()
{
    qDebug() << "[人脸采集页面] 停止实时预览";
    
    // 只停止预览相关的资源，不释放m_capturedFrame
    if (m_previewTimer && m_previewTimer->isActive()) {
        m_previewTimer->stop();
        qDebug() << "[人脸采集页面] 停止预览定时器";
    }
    
    // 释放当前帧
    if (!m_currentFrame.empty()) {
        m_currentFrame.release();
        qDebug() << "[人脸采集页面] 释放当前帧";
    }
    
    // 释放摄像头
    if (m_camera.isOpened()) {
        m_camera.release();
        qDebug() << "[人脸采集页面] 释放摄像头";
        QThread::msleep(100);
    }
    
    qDebug() << "[人脸采集页面] 实时预览停止完成";
}

void FaceCapturePage::cleanupCamera()
{
    qDebug() << "[人脸采集页面] 清理摄像头资源";
    
    // 停止定时器
    if (m_previewTimer && m_previewTimer->isActive()) {
        m_previewTimer->stop();
        qDebug() << "[人脸采集页面] 停止预览定时器";
    }
    
    // 释放图像数据
    if (!m_currentFrame.empty()) {
        m_currentFrame.release();
        qDebug() << "[人脸采集页面] 释放当前帧";
    }
    
    if (!m_capturedFrame.empty()) {
        m_capturedFrame.release();
        qDebug() << "[人脸采集页面] 释放捕获帧";
    }
    
    // 释放摄像头
    if (m_camera.isOpened()) {
        m_camera.release();
        qDebug() << "[人脸采集页面] 释放摄像头";
        QThread::msleep(200);
    }
    
    // 重置状态
    m_faceCaptureState = FaceInitializing;
    m_faceDetected = false;
    m_detectedFaces.clear();
    
    qDebug() << "[人脸采集页面] 摄像头资源清理完成";
}

void FaceCapturePage::onPreviewTimer()
{
    if (!m_camera.read(m_currentFrame) || m_currentFrame.empty()) {
        return;
    }
    
    // 等比例显示预览（避免拉伸）
    cv::Mat rgbFrame;
    cv::cvtColor(m_currentFrame, rgbFrame, cv::COLOR_BGR2RGB);
    QImage img(rgbFrame.data, rgbFrame.cols, rgbFrame.rows, rgbFrame.step, QImage::Format_RGB888);

    const QPixmap pixmap = QPixmap::fromImage(img.copy());
    m_previewLabel->setPixmap(
        pixmap.scaled(m_previewLabel->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation)
    );
}

void FaceCapturePage::onCaptureButtonClicked()
{
    if (m_currentFrame.empty()) {
        updateStatus("无法获取摄像头图像", "rgb(255, 77, 79)");
        return;
    }
    
    qDebug() << "[人脸采集页面] 用户点击拍照，开始多帧采集";
    
    m_faceCaptureState = FaceCapturing;
    ui->startCaptureBtn->setEnabled(false);
    updateStatus(QString("正在采集最佳帧（0/%1）...").arg(kCaptureFrames), "rgb(24, 144, 255)");

    // 重置多帧采集状态
    m_captureCountdown = kCaptureFrames;
    m_bestSharpness = -1.0;
    m_capturedFrame.release();

    // 每 80ms 采一帧，共采 kCaptureFrames 帧
    m_captureTimer->start(80);
}

void FaceCapturePage::onCaptureTimerTick()
{
    if (m_currentFrame.empty()) return;

    cv::Mat frame = m_currentFrame.clone();
    double sharpness = computeSharpness(frame);

    int collected = kCaptureFrames - m_captureCountdown + 1;
    qDebug() << "[多帧采集] 第" << collected << "帧，清晰度:" << sharpness;

    if (sharpness > m_bestSharpness) {
        m_bestSharpness = sharpness;
        m_capturedFrame = frame;
        qDebug() << "[多帧采集] 更新最佳帧，清晰度:" << sharpness;
    }

    updateStatus(QString("正在采集最佳帧（%1/%2）...").arg(collected).arg(kCaptureFrames),
                 "rgb(24, 144, 255)");

    m_captureCountdown--;
    if (m_captureCountdown <= 0) {
        m_captureTimer->stop();
        qDebug() << "[多帧采集] 采集完成，最终清晰度:" << m_bestSharpness;

        // 停止预览，切换到确认界面
        stopLivePreview();
        switchToConfirmView();
    }
}

void FaceCapturePage::switchToConfirmView()
{
    qDebug() << "[人脸采集页面] 切换到确认界面";
    
    // 检查捕获帧是否有效
    if (m_capturedFrame.empty()) {
        qWarning() << "[人脸采集页面] 错误：捕获帧为空";
        updateStatus("图像捕获失败，请重试", "rgb(255, 77, 79)");
        switchToPreviewView();
        startLivePreview();
        return;
    }
    
    m_faceCaptureState = FaceConfirming;
    
    // 显示捕获的图像
    cv::Mat rgbFrame;
    cv::cvtColor(m_capturedFrame, rgbFrame, cv::COLOR_BGR2RGB);
    QImage img(rgbFrame.data, rgbFrame.cols, rgbFrame.rows, rgbFrame.step, QImage::Format_RGB888);

    const QPixmap pixmap = QPixmap::fromImage(img.copy());
    m_confirmImageLabel->setPixmap(
        pixmap.scaled(m_confirmImageLabel->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation)
    );
    
    // 显示确认界面，隐藏预览界面
    m_previewLabel->hide();
    ui->previewContainer->hide();
    ui->controlContainer->hide();
    
    m_confirmImageLabel->show();
    m_confirmButton->show();
    m_retakeButton->show();
    
    updateStatus("请确认图像质量", "rgb(24, 144, 255)");
}

void FaceCapturePage::switchToPreviewView()
{
    qDebug() << "[人脸采集页面] 切换到预览界面";
    
    m_faceCaptureState = FacePreviewing;
    
    // 显示预览界面，隐藏确认界面
    ui->previewContainer->show();
    ui->controlContainer->show();
    m_previewLabel->show();
    ui->startCaptureBtn->setText("拍照");
    ui->startCaptureBtn->setEnabled(true);
    
    m_confirmImageLabel->hide();
    m_confirmButton->hide();
    m_retakeButton->hide();
    
    updateStatus("请正视摄像头，点击拍照按钮", "rgb(100, 100, 100)");
}

void FaceCapturePage::onConfirmButtonClicked()
{
    qDebug() << "[人脸采集页面] 用户确认图像";
    
    if (m_capturedFrame.empty()) {
        qWarning() << "[人脸采集页面] 错误：确认时捕获帧为空";
        updateStatus("图像数据无效，请重新拍照", "rgb(255, 77, 79)");
        onRetakeButtonClicked();
        return;
    }
    
    // 直接将 640x480 彩色原图编码为 JPEG 发送给服务器
    // 服务器端会做 Haar 检测 + 裁剪 + preprocessForSeetaFace，与认证流程完全一致
    QByteArray faceData = encodeFrameForServer(m_capturedFrame);
    
    m_faceCaptureState = FaceCompleted;
    updateStatus("人脸采集成功", "rgb(82, 196, 26)");
    
    emit faceCaptureCompleted(faceData);
}

void FaceCapturePage::onRetakeButtonClicked()
{
    qDebug() << "[人脸采集页面] 用户重拍";
    
    // 停止多帧采集定时器
    if (m_captureTimer->isActive()) {
        m_captureTimer->stop();
    }
    m_captureCountdown = 0;
    m_bestSharpness = -1.0;

    // 释放捕获的帧
    if (!m_capturedFrame.empty()) {
        m_capturedFrame.release();
        qDebug() << "[人脸采集页面] 释放捕获帧";
    }
    
    // 返回预览界面
    switchToPreviewView();
    startLivePreview();
}

void FaceCapturePage::updateStatus(const QString& message, const QString& color)
{
    ui->statusLabel->setText(message);
    ui->statusLabel->setStyleSheet(QString("color: %1; font-size: 14px;").arg(color));
}

// 计算帧清晰度（Laplacian 方差，值越大越清晰）
double FaceCapturePage::computeSharpness(const cv::Mat& frame)
{
    cv::Mat gray, lap;
    if (frame.channels() == 3) {
        cv::cvtColor(frame, gray, cv::COLOR_BGR2GRAY);
    } else {
        gray = frame;
    }
    cv::Laplacian(gray, lap, CV_64F);
    cv::Scalar mean, stddev;
    cv::meanStdDev(lap, mean, stddev);
    return stddev[0] * stddev[0];  // 方差
}

// 将原始 640x480 彩色帧编码为 JPEG，直接发送给服务器
// 服务器端负责 Haar 检测 + 裁剪 + preprocessForSeetaFace，与认证流程完全一致
QByteArray FaceCapturePage::encodeFrameForServer(const cv::Mat& image)
{
    std::vector<uchar> buffer;
    std::vector<int> params = {cv::IMWRITE_JPEG_QUALITY, 90};
    cv::imencode(".jpg", image, buffer, params);
    qDebug() << "[人脸采集] 编码原始帧:" << image.cols << "x" << image.rows
             << "→" << buffer.size() << "字节 (JPEG Q90)";
    return QByteArray(reinterpret_cast<const char*>(buffer.data()), buffer.size());
}
