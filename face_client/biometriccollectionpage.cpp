#include "biometriccollectionpage.h"
#include "ui_biometriccollectionpage.h"
#include <QDebug>
#include <QPalette>
#include <QMessageBox>
#include <QCryptographicHash>
#include <QThread>

BiometricCollectionPage::BiometricCollectionPage(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::BiometricCollectionPage)
    , m_hardwareController(nullptr)
    , m_faceCaptureTimer(new QTimer(this))
    , m_faceCountdown(0)
    , m_registrationTimer(new QTimer(this))
    , m_registrationRetryCount(0)
    , m_currentStep(FaceCollection)
    , m_faceCaptureState(FaceInitializing)
    , m_confirmButton(nullptr)
    , m_retakeButton(nullptr)
    , m_previewLabel(nullptr)
    , m_confirmImageLabel(nullptr)
    , m_previewTimer(new QTimer(this))
    , m_faceDetected(false)
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
        qWarning() << "[生物特征采集] 无法加载人脸检测器:" << cascadePath;
    }

    // 设置注册超时定时器
    m_registrationTimer->setSingleShot(true);
    connect(m_registrationTimer, &QTimer::timeout, this, &BiometricCollectionPage::onRegistrationTimeout);

    // 连接信号槽
    connect(ui->backBtn, &QPushButton::clicked, this, &BiometricCollectionPage::onBackClicked);
    connect(ui->startFaceCaptureBtn, &QPushButton::clicked, this, &BiometricCollectionPage::onStartFaceCapture);
    connect(ui->startFingerprintBtn, &QPushButton::clicked, this, &BiometricCollectionPage::onStartFingerprintCapture);
    connect(ui->confirmPasswordBtn, &QPushButton::clicked, this, &BiometricCollectionPage::onPasswordConfirmed);
    connect(ui->completeBtn, &QPushButton::clicked, this, &BiometricCollectionPage::onCompleteRegistration);
    
    // 设置密码输入框为可编辑并配置输入法提示
    ui->passwordLineEdit->setReadOnly(false);
    ui->passwordLineEdit->setInputMethodHints(Qt::ImhDigitsOnly);
    ui->confirmPasswordLineEdit->setReadOnly(false);
    ui->confirmPasswordLineEdit->setInputMethodHints(Qt::ImhDigitsOnly);
    
    // 连接定时器
    connect(m_faceCaptureTimer, &QTimer::timeout, this, &BiometricCollectionPage::onFaceCaptureTimer);
    connect(m_previewTimer, &QTimer::timeout, this, &BiometricCollectionPage::onPreviewTimer);
    
    // 创建手动拍照相关的UI组件
    // 预览标签 (440x330) - 在左侧预览容器中
    m_previewLabel = new QLabel(ui->faceCollectionWidget);
    m_previewLabel->setFixedSize(440, 330);
    m_previewLabel->setAlignment(Qt::AlignCenter);
    m_previewLabel->setStyleSheet("background-color: rgb(50, 50, 50); border: 2px solid rgb(200, 200, 200); border-radius: 8px;");
    m_previewLabel->move(60, 70);  // 左侧预览区域
    m_previewLabel->hide();
    
    // 确认图像标签 (300x225) - 居中显示
    m_confirmImageLabel = new QLabel(ui->faceCollectionWidget);
    m_confirmImageLabel->setFixedSize(300, 225);
    m_confirmImageLabel->setAlignment(Qt::AlignCenter);
    m_confirmImageLabel->setStyleSheet("background-color: rgb(50, 50, 50); border: 2px solid rgb(200, 200, 200); border-radius: 8px;");
    m_confirmImageLabel->move(312, 100);  // 居中显示
    m_confirmImageLabel->hide();
    
    // 确认按钮 - 在确认图像下方
    m_confirmButton = new QPushButton("确认", ui->faceCollectionWidget);
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
    m_confirmButton->move(522, 350);
    m_confirmButton->hide();
    connect(m_confirmButton, &QPushButton::clicked, this, &BiometricCollectionPage::onConfirmButtonClicked);
    
    // 重拍按钮 - 在确认按钮左侧
    m_retakeButton = new QPushButton("重拍", ui->faceCollectionWidget);
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
    m_retakeButton->move(382, 350);
    m_retakeButton->hide();
    connect(m_retakeButton, &QPushButton::clicked, this, &BiometricCollectionPage::onRetakeButtonClicked);

    qDebug() << "[生物特征采集] 生物特征采集页面初始化完成";
}

BiometricCollectionPage::~BiometricCollectionPage()
{
    if (m_camera.isOpened()) {
        m_camera.release();
    }
    delete ui;
}

void BiometricCollectionPage::setUserInfo(const UserInfo& userInfo)
{
    m_userInfo = userInfo;
    ui->userNameLabel->setText(QString("用户：%1").arg(userInfo.name));
}

void BiometricCollectionPage::setHardwareController(HardwareController* controller)
{
    m_hardwareController = controller;
    
    if (m_hardwareController) {
        // 连接指纹采集信号 - 使用QByteArray版本的fingerprintCaptured信号（用于采集）
        connect(m_hardwareController, static_cast<void(HardwareController::*)(const QByteArray&)>(&HardwareController::fingerprintCaptured),
                this, &BiometricCollectionPage::onFingerprintCaptured);
        connect(m_hardwareController, &HardwareController::fingerprintError,
                this, &BiometricCollectionPage::onFingerprintError);
    }
}

void BiometricCollectionPage::onFingerprintCaptured(const QByteArray& data)
{
    qDebug() << "[生物特征采集] 指纹采集成功，数据大小:" << data.size();
    
    m_fingerprintData = data;
    updateFingerprintStatus("指纹采集成功", "rgb(82, 196, 26)");
    
    // 切换到密码设置
    switchToStep(PasswordSetting);
}

void BiometricCollectionPage::onFingerprintError(const QString& error)
{
    qDebug() << "[生物特征采集] 指纹采集失败:" << error;
    
    updateFingerprintStatus(QString("指纹采集失败: %1").arg(error), "rgb(255, 77, 79)");
    ui->startFingerprintBtn->setEnabled(true);
}

void BiometricCollectionPage::startCollection()
{
    resetCollection();
    switchToStep(FaceCollection);
}

void BiometricCollectionPage::resetCollection()
{
    qDebug() << "[生物特征采集] 重置采集状态";
    
    // 先清理摄像头资源
    cleanupCamera();
    
    m_faceData.clear();
    m_fingerprintData.clear();
    m_password.clear();
    m_currentStep = FaceCollection;
    m_registrationRetryCount = 0;

    // 重置人脸采集状态
    m_faceCaptureState = FaceInitializing;
    m_faceDetected = false;
    m_detectedFaces.clear();
    
    m_faceCaptureTimer->stop();
    m_registrationTimer->stop();
    hideRegistrationProgress();
    
    // 隐藏手动拍照相关组件
    if (m_previewLabel) m_previewLabel->hide();
    if (m_confirmImageLabel) m_confirmImageLabel->hide();
    if (m_confirmButton) m_confirmButton->hide();
    if (m_retakeButton) m_retakeButton->hide();
    
    // 重新启用开始拍照按钮
    ui->startFaceCaptureBtn->setEnabled(true);
    ui->startFaceCaptureBtn->setText("开始人脸采集");
    updateFaceStatus("点击\"开始人脸采集\"按钮开始", "rgb(100, 100, 100)");
    
    qDebug() << "[生物特征采集] 采集状态重置完成";
}

void BiometricCollectionPage::onBackClicked()
{
    qDebug() << "[生物特征采集] 返回首页";
    
    // 确保摄像头完全释放
    cleanupCamera();
    
    resetCollection();
    emit backToHomeClicked();
}

void BiometricCollectionPage::onStartFaceCapture()
{
    // 复用同一个按钮：第一次点击开启预览；预览中再次点击直接拍照
    if (m_faceCaptureState == FacePreviewing || m_faceCaptureState == FaceReadyToCapture) {
        onCaptureButtonClicked();
        return;
    }

    qDebug() << "[生物特征采集] 开始人脸采集";

    ui->startFaceCaptureBtn->setEnabled(false);
    startLivePreview();
}

void BiometricCollectionPage::onStartFingerprintCapture()
{
    qDebug() << "[生物特征采集] 开始指纹采集";

    if (!m_hardwareController) {
        updateFingerprintStatus("硬件控制器未初始化", "rgb(255, 77, 79)");
        return;
    }

    ui->startFingerprintBtn->setEnabled(false);
    updateFingerprintStatus("请将手指放在指纹传感器上...", "rgb(24, 144, 255)");

    // 开始指纹采集
    m_hardwareController->startFingerprintCapture();
}

void BiometricCollectionPage::onPasswordConfirmed()
{
    QString password = ui->passwordLineEdit->text();
    QString confirmPassword = ui->confirmPasswordLineEdit->text();

    if (password.isEmpty() || confirmPassword.isEmpty()) {
        updatePasswordStatus("请输入6位数字密码", "rgb(255, 77, 79)");
        return;
    }

    if (password != confirmPassword) {
        updatePasswordStatus("两次输入的密码不一致", "rgb(255, 77, 79)");
        return;
    }

    if (!validatePassword(password)) {
        updatePasswordStatus("密码必须是6位数字", "rgb(255, 77, 79)");
        return;
    }

    m_password = password;
    updatePasswordStatus("密码设置成功", "rgb(82, 196, 26)");

    ui->confirmPasswordBtn->setEnabled(false);
    ui->passwordLineEdit->setEnabled(false);
    ui->confirmPasswordLineEdit->setEnabled(false);

    // 切换到完成步骤
    switchToStep(Completed);
}

void BiometricCollectionPage::onCompleteRegistration()
{
    qDebug() << "[生物特征采集] 完成注册";

    if (m_faceData.isEmpty() || m_fingerprintData.isEmpty() || m_password.isEmpty()) {
        QMessageBox::warning(this, "注册未完成", "请完成所有生物特征采集和密码设置");
        return;
    }

    showRegistrationProgress();
    m_registrationTimer->start(REGISTRATION_TIMEOUT);

    emit registrationCompleted(m_userInfo, m_faceData, m_fingerprintData, m_password);
}

void BiometricCollectionPage::onRetryRegistration()
{
    if (m_registrationRetryCount < MAX_REGISTRATION_RETRY) {
        m_registrationRetryCount++;
        qDebug() << "[生物特征采集] 重试注册，第" << m_registrationRetryCount << "次";

        showRegistrationProgress();
        m_registrationTimer->start(REGISTRATION_TIMEOUT);

        emit registrationCompleted(m_userInfo, m_faceData, m_fingerprintData, m_password);
    }
}

void BiometricCollectionPage::onRegistrationTimeout()
{
    qDebug() << "[生物特征采集] 注册超时";
    hideRegistrationProgress();

    if (m_registrationRetryCount < MAX_REGISTRATION_RETRY) {
        ui->completeDescLabel->setText(QString("注册超时，是否重试？\n(剩余 %1 次)").arg(MAX_REGISTRATION_RETRY - m_registrationRetryCount));
        ui->completeDescLabel->setStyleSheet("color: rgb(255, 193, 7); font-size: 16px;");

        ui->completeBtn->setEnabled(true);
        ui->completeBtn->setText("重试注册");
        ui->completeBtn->setStyleSheet(
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

        // 重新连接到重试槽函数
        disconnect(ui->completeBtn, &QPushButton::clicked, this, &BiometricCollectionPage::onCompleteRegistration);
        connect(ui->completeBtn, &QPushButton::clicked, this, &BiometricCollectionPage::onRetryRegistration);
    } else {
        ui->completeDescLabel->setText("注册失败次数过多\n请检查网络连接或联系管理员");
        ui->completeDescLabel->setStyleSheet("color: rgb(255, 77, 79); font-size: 16px;");

        ui->completeBtn->setEnabled(true);
        ui->completeBtn->setText("重新开始");
        m_registrationRetryCount = 0;

        // 重新连接到注册槽函数
        disconnect(ui->completeBtn, &QPushButton::clicked, this, &BiometricCollectionPage::onRetryRegistration);
        connect(ui->completeBtn, &QPushButton::clicked, this, &BiometricCollectionPage::onCompleteRegistration);
    }
}

void BiometricCollectionPage::onFaceCaptureTimer()
{
    if (m_faceCountdown > 0) {
        m_faceCountdown--;
        if (m_faceCountdown > 0) {
            updateFaceStatus(QString("准备采集，请看向摄像头 %1").arg(m_faceCountdown), "rgb(24, 144, 255)");
            return;
        }
    }

    // 开始采集
    cv::Mat frame;
    if (!m_camera.read(frame) || frame.empty()) {
        updateFaceStatus("无法获取摄像头图像", "rgb(255, 77, 79)");
        ui->startFaceCaptureBtn->setEnabled(true);
        m_faceCaptureTimer->stop();
        return;
    }

    // 检测人脸
    cv::Mat grayFrame;
    cv::cvtColor(frame, grayFrame, cv::COLOR_BGR2GRAY);

    std::vector<cv::Rect> faces;
    m_faceClassifier.detectMultiScale(grayFrame, faces, 1.1, 3, 0, cv::Size(80, 80));

    if (faces.empty()) {
        updateFaceStatus("未检测到人脸，请调整位置", "rgb(255, 193, 7)");
        return;
    }
    
    if (faces.size() > 1) {
        updateFaceStatus("检测到多张人脸，请确保只有一人", "rgb(255, 193, 7)");
        return;
    }
    
    // 采集成功
    m_faceData = preprocessFaceImage(frame);
    updateFaceStatus("人脸采集成功", "rgb(82, 196, 26)");
    
    m_faceCaptureTimer->stop();
    m_camera.release();
    
    // 切换到指纹采集
    switchToStep(FingerprintCollection);
}

bool BiometricCollectionPage::initializeCamera()
{
    qDebug() << "[生物特征采集] 初始化摄像头";
    
    // 如果摄像头已打开，先释放再重新打开
    if (m_camera.isOpened()) {
        qDebug() << "[生物特征采集] 检测到摄像头已打开，先释放";
        m_camera.release();
        QThread::msleep(200);  // 等待资源完全释放
    }

    qDebug() << "[生物特征采集] 尝试打开摄像头...";
    
    // 尝试多个摄像头索引
    int cameraIndices[] = {0, 1, 2, -1};  // -1表示自动选择
    
    for (int i = 0; cameraIndices[i] != -2; i++) {
        int index = cameraIndices[i];
        qDebug() << "[生物特征采集] 尝试打开摄像头索引:" << index;
        
        // 尝试打开摄像头
        if (index == -1) {
            m_camera.open(cv::CAP_ANY);
        } else {
            m_camera.open(index);
        }
        
        if (m_camera.isOpened()) {
            qDebug() << "[生物特征采集] ✓ 摄像头打开成功，索引:" << index;
            
            // 设置摄像头参数：先尝试 720p，再根据性能按需调整
            m_camera.set(cv::CAP_PROP_FRAME_WIDTH, 1280);   // 720p 宽
            m_camera.set(cv::CAP_PROP_FRAME_HEIGHT, 720);   // 720p 高
            m_camera.set(cv::CAP_PROP_FPS, 20);            // 降低帧率到20fps
            
            // 测试读取一帧
            cv::Mat testFrame;
            if (m_camera.read(testFrame) && !testFrame.empty()) {
                qDebug() << "[生物特征采集] ✓ 摄像头测试成功";
                qDebug() << "  实际分辨率:" << testFrame.cols << "x" << testFrame.rows;
                return true;
            } else {
                qDebug() << "[生物特征采集] 摄像头打开但无法读取图像，尝试下一个";
                m_camera.release();
            }
        }
        
        // 如果是最后一个索引，标记结束
        if (i == 2) {
            cameraIndices[i+1] = -2;
        }
    }
    
    qDebug() << "[生物特征采集] ✗ 所有摄像头索引都无法打开";
    return false;
}

void BiometricCollectionPage::switchToStep(CollectionStep step)
{
    m_currentStep = step;

    // 隐藏所有步骤
    ui->faceCollectionWidget->hide();
    ui->fingerprintCollectionWidget->hide();
    ui->passwordSettingWidget->hide();
    ui->completionWidget->hide();

    // 显示当前步骤并更新进度指示器
    switch (step) {
    case FaceCollection:
        ui->faceCollectionWidget->show();
        ui->progressLabel->setText("步骤 1/3: 人脸采集");
        break;
    case FingerprintCollection:
        ui->fingerprintCollectionWidget->show();
        ui->progressLabel->setText("步骤 2/3: 指纹采集");
        break;
    case PasswordSetting:
        ui->passwordSettingWidget->show();
        ui->progressLabel->setText("步骤 3/3: 密码设置");
        break;
    case Completed:
        ui->completionWidget->show();
        ui->progressLabel->setText("注册完成");
        break;
    }

    updateProgressIndicator();
}

void BiometricCollectionPage::updateProgressIndicator()
{
    // 更新进度条（如果UI中有进度条的话）
    int progress = 0;
    switch (m_currentStep) {
    case FaceCollection:
        progress = 25;
        break;
    case FingerprintCollection:
        progress = 50;
        break;
    case PasswordSetting:
        progress = 75;
        break;
    case Completed:
        progress = 100;
        break;
    }

    // 如果UI中有进度条，可以在这里更新
    // ui->progressBar->setValue(progress);

    qDebug() << "[生物特征采集] 更新进度:" << progress << "%";
}

void BiometricCollectionPage::showRegistrationProgress()
{
    ui->completeBtn->setEnabled(false);
    ui->completeBtn->setText("注册中...");
    ui->completeDescLabel->setText("正在提交注册信息，请稍候...");
    ui->completeDescLabel->setStyleSheet("color: rgb(24, 144, 255); font-size: 16px;");
}

void BiometricCollectionPage::hideRegistrationProgress()
{
    ui->completeBtn->setText("完成注册");
    ui->completeBtn->setEnabled(true);
    ui->completeBtn->setStyleSheet(
        "QPushButton {"
        "   background-color: rgb(24, 144, 255);"
        "   color: white;"
        "   border: none;"
        "   border-radius: 8px;"
        "   font-size: 16px;"
        "   font-weight: bold;"
        "}"
        "QPushButton:pressed {"
        "   background-color: rgb(16, 120, 220);"
        "}"
    );
}

void BiometricCollectionPage::updateFaceStatus(const QString& message, const QString& color)
{
    ui->faceStatusLabel->setText(message);
    ui->faceStatusLabel->setStyleSheet(QString("color: %1; font-size: 14px;").arg(color));
}

void BiometricCollectionPage::updateFingerprintStatus(const QString& message, const QString& color)
{
    ui->fingerprintStatusLabel->setText(message);
    ui->fingerprintStatusLabel->setStyleSheet(QString("color: %1; font-size: 14px;").arg(color));
}

void BiometricCollectionPage::updatePasswordStatus(const QString& message, const QString& color)
{
    ui->passwordStatusLabel->setText(message);
    ui->passwordStatusLabel->setStyleSheet(QString("color: %1; font-size: 14px;").arg(color));
}

bool BiometricCollectionPage::validatePassword(const QString& password) const
{
    qDebug() << "[生物特征采集] 验证密码:" << password << "长度:" << password.length();
    
    // 密码必须是6位数字
    if (password.length() != 6) {
        qDebug() << "[生物特征采集] 密码验证失败：长度不是6位";
        return false;
    }

    // 只能包含数字
    for (const QChar& ch : password) {
        if (!ch.isDigit()) {
            qDebug() << "[生物特征采集] 密码验证失败：包含非数字字符" << ch;
            return false;
        }
    }

    qDebug() << "[生物特征采集] 密码验证成功";
    return true;
}

QByteArray BiometricCollectionPage::preprocessFaceImage(const cv::Mat& image)
{
    // 轻量级预处理：亮度/对比度增强 + 降噪 + 灰度化 + 缩放
    cv::Mat workImage;
    if (image.cols > 256 || image.rows > 256) {
        cv::resize(image, workImage, cv::Size(256, 256));
    } else {
        workImage = image.clone();
    }

    // 1. 亮度 / 对比度增强（alpha: 对比度, beta: 亮度）
    cv::Mat enhancedImage;
    double alpha = 1.8;   // 推荐范围 1.5 ~ 2.0
    int beta = 25;        // 推荐范围 20 ~ 30
    cv::convertScaleAbs(workImage, enhancedImage, alpha, beta);

    // 2. 轻量级去模糊 / 降噪（高斯模糊，小核，兼顾去噪和细节）
    cv::Mat denoisedImage;
    cv::GaussianBlur(enhancedImage, denoisedImage, cv::Size(3, 3), 0);

    // 3. 灰度化
    cv::Mat grayImage;
    if (denoisedImage.channels() == 3) {
        cv::cvtColor(denoisedImage, grayImage, cv::COLOR_BGR2GRAY);
    } else {
        grayImage = denoisedImage.clone();
    }

    // 4. 轻量级降噪（中值滤波，去除暗图噪点）
    cv::medianBlur(grayImage, grayImage, 3);

    // 5. 调整到标准尺寸 128x128，用于网络传输和服务器进一步增强
    cv::Mat resizedImage;
    cv::resize(grayImage, resizedImage, cv::Size(128, 128));
    
    // 6. JPEG 编码，设置质量 85，兼顾细节和带宽
    std::vector<uchar> buffer;
    std::vector<int> params;
    params.push_back(cv::IMWRITE_JPEG_QUALITY);
    params.push_back(85);
    cv::imencode(".jpg", resizedImage, buffer, params);
    
    return QByteArray(reinterpret_cast<const char*>(buffer.data()), buffer.size());
}

void BiometricCollectionPage::showRegistrationStatus(bool success, const QString& message)
{
    qDebug() << "[生物特征采集] 显示注册状态:" << success << message;
    
    m_registrationTimer->stop();
    hideRegistrationProgress();
    
    if (success) {
        // 注册成功，显示成功状态
        ui->completeDescLabel->setText("注册成功！正在返回首页...");
        ui->completeDescLabel->setStyleSheet("color: rgb(82, 196, 26); font-size: 18px; font-weight: bold;");
        
        // 禁用完成按钮，准备返回首页
        ui->completeBtn->setEnabled(false);
        ui->completeBtn->setText("注册成功");
        ui->completeBtn->setStyleSheet(
            "QPushButton {"
            "   background-color: rgb(82, 196, 26);"
            "   color: white;"
            "   border: none;"
            "   border-radius: 8px;"
            "   font-size: 16px;"
            "   font-weight: bold;"
            "}"
        );
        
        m_registrationRetryCount = 0;
    } else {
        // 注册失败，显示错误状态
        if (m_registrationRetryCount < MAX_REGISTRATION_RETRY) {
            ui->completeDescLabel->setText(QString("%1\n是否重试？(剩余 %2 次)").arg(message.isEmpty() ? "注册失败" : message).arg(MAX_REGISTRATION_RETRY - m_registrationRetryCount));
            ui->completeDescLabel->setStyleSheet("color: rgb(255, 77, 79); font-size: 16px;");
            
            // 重新启用完成按钮，允许重试
            ui->completeBtn->setEnabled(true);
            ui->completeBtn->setText("重试注册");
            ui->completeBtn->setStyleSheet(
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
            
            // 重新连接到重试槽函数
            disconnect(ui->completeBtn, &QPushButton::clicked, this, &BiometricCollectionPage::onCompleteRegistration);
            connect(ui->completeBtn, &QPushButton::clicked, this, &BiometricCollectionPage::onRetryRegistration);
        } else {
            ui->completeDescLabel->setText("注册失败次数过多\n请检查网络连接或联系管理员");
            ui->completeDescLabel->setStyleSheet("color: rgb(255, 77, 79); font-size: 16px;");
            
            ui->completeBtn->setEnabled(true);
            ui->completeBtn->setText("重新开始");
            m_registrationRetryCount = 0;
            
            // 重新连接到注册槽函数
            disconnect(ui->completeBtn, &QPushButton::clicked, this, &BiometricCollectionPage::onRetryRegistration);
            connect(ui->completeBtn, &QPushButton::clicked, this, &BiometricCollectionPage::onCompleteRegistration);
        }
    }
}

// 新增：启动实时预览
void BiometricCollectionPage::startLivePreview()
{
    qDebug() << "[人脸采集] 启动实时预览";
    
    if (!initializeCamera()) {
        m_faceCaptureState = FaceError;
        updateFaceStatus("摄像头初始化失败，请检查设备连接", "rgb(255, 77, 79)");
        return;
    }
    
    m_faceCaptureState = FacePreviewing;
    m_faceDetected = false;
    
    // 显示预览标签；并让“开始人脸采集”按钮切换为“拍照”
    m_previewLabel->show();
    ui->startFaceCaptureBtn->setText("拍照");
    ui->startFaceCaptureBtn->setEnabled(true);
    
    // 启动预览定时器（约 20fps，提高流畅度）
    m_previewTimer->start(50);
    
    updateFaceStatus("请正视摄像头，点击拍照按钮", "rgb(100, 100, 100)");
}

// 新增：停止实时预览
void BiometricCollectionPage::stopLivePreview()
{
    qDebug() << "[人脸采集] 停止实时预览";
    
    // 只停止预览相关的资源，不释放m_capturedFrame（可能在确认界面使用）
    if (m_previewTimer && m_previewTimer->isActive()) {
        m_previewTimer->stop();
        qDebug() << "[人脸采集] 停止预览定时器";
    }
    
    // 释放当前帧（不是捕获帧）
    if (!m_currentFrame.empty()) {
        m_currentFrame.release();
        qDebug() << "[人脸采集] 释放当前帧";
    }
    
    // 释放摄像头
    if (m_camera.isOpened()) {
        m_camera.release();
        qDebug() << "[人脸采集] 释放摄像头";
        
        // 等待一小段时间确保资源完全释放
        QThread::msleep(100);
        qDebug() << "[人脸采集] 等待摄像头资源完全释放";
    }
    
    qDebug() << "[人脸采集] 实时预览停止完成";
}

// 新增：预览定时器回调（简化版，不做人脸检测）
void BiometricCollectionPage::onPreviewTimer()
{
    if (!m_camera.read(m_currentFrame) || m_currentFrame.empty()) {
        return;
    }
    
    // 直接显示预览，不做人脸检测（提高性能）
    cv::Mat displayFrame;
    cv::resize(m_currentFrame, displayFrame, cv::Size(440, 330));
    cv::cvtColor(displayFrame, displayFrame, cv::COLOR_BGR2RGB);
    
    QImage img(displayFrame.data, displayFrame.cols, displayFrame.rows,
               displayFrame.step, QImage::Format_RGB888);
    m_previewLabel->setPixmap(QPixmap::fromImage(img));
}

// 新增：检测人脸并绘制人脸框
void BiometricCollectionPage::detectAndDrawFaces(cv::Mat& frame)
{
    // 转换为灰度图
    cv::Mat grayFrame;
    cv::cvtColor(frame, grayFrame, cv::COLOR_BGR2GRAY);
    
    // 检测人脸
    m_detectedFaces.clear();
    m_faceClassifier.detectMultiScale(grayFrame, m_detectedFaces, 
                                       1.1, 3, 0, cv::Size(80, 80));
    
    // 更新状态
    if (m_detectedFaces.empty()) {
        m_faceDetected = false;
        m_faceCaptureState = FacePreviewing;
        updateFaceStatus("请将面部对准摄像头", "rgb(255, 193, 7)");
    } else if (m_detectedFaces.size() > 1) {
        m_faceDetected = false;
        m_faceCaptureState = FacePreviewing;
        updateFaceStatus("检测到多张人脸，请确保只有一人", "rgb(255, 193, 7)");
    } else {
        m_faceDetected = true;
        m_faceCaptureState = FaceReadyToCapture;
        updateFaceStatus("检测到人脸，可以拍照", "rgb(82, 196, 26)");
        
        // 绘制绿色人脸框（线宽 3 像素）
        cv::Rect face = m_detectedFaces[0];
        cv::rectangle(frame, face, cv::Scalar(0, 255, 0), 3);
    }
}

// 新增：更新拍照按钮状态
void BiometricCollectionPage::updateCaptureButtonState()
{
    // 复用 startFaceCaptureBtn 作为拍照按钮：是否允许拍照取决于是否检测到人脸
    ui->startFaceCaptureBtn->setEnabled(m_faceDetected && m_faceCaptureState == FaceReadyToCapture);
}

// 新增：检查图像质量
bool BiometricCollectionPage::checkImageQuality(const cv::Mat& image, QString& errorMsg)
{
    // 检查人脸大小
    if (!m_detectedFaces.empty()) {
        cv::Rect face = m_detectedFaces[0];
        double faceArea = face.width * face.height;
        double imageArea = image.cols * image.rows;
        double faceRatio = faceArea / imageArea;
        
        if (faceRatio < 0.05) {  // 人脸占比小于 5%
            errorMsg = "人脸过小，请靠近摄像头";
            return false;
        }
    }
    
    // 检查图像清晰度（使用拉普拉斯算子）
    cv::Mat gray, laplacian;
    cv::cvtColor(image, gray, cv::COLOR_BGR2GRAY);
    cv::Laplacian(gray, laplacian, CV_64F);
    
    cv::Scalar mean, stddev;
    cv::meanStdDev(laplacian, mean, stddev);
    
    double variance = stddev.val[0] * stddev.val[0];
    
    if (variance < 100) {  // 方差阈值
        errorMsg = "图像模糊，请重新拍照";
        return false;
    }
    
    return true;
}

// 新增：拍照按钮点击处理（简化版，不做质量检查）
void BiometricCollectionPage::onCaptureButtonClicked()
{
    if (m_currentFrame.empty()) {
        updateFaceStatus("无法获取摄像头图像", "rgb(255, 77, 79)");
        return;
    }
    
    qDebug() << "[人脸采集] 用户点击拍照";
    
    m_faceCaptureState = FaceCapturing;
    
    // 捕获当前帧
    m_capturedFrame = m_currentFrame.clone();
    
    // 停止预览
    stopLivePreview();
    
    // 切换到确认界面
    switchToConfirmView();
}

// 新增：切换到确认界面
void BiometricCollectionPage::switchToConfirmView()
{
    qDebug() << "[人脸采集] 切换到确认界面";
    
    // 检查捕获帧是否有效
    if (m_capturedFrame.empty()) {
        qWarning() << "[人脸采集] 错误：捕获帧为空，无法显示确认界面";
        updateFaceStatus("图像捕获失败，请重试", "rgb(255, 77, 79)");
        // 返回预览界面
        switchToPreviewView();
        startLivePreview();
        return;
    }
    
    m_faceCaptureState = FaceConfirming;
    
    // 显示捕获的图像
    cv::Mat displayFrame;
    cv::resize(m_capturedFrame, displayFrame, cv::Size(300, 225));
    cv::cvtColor(displayFrame, displayFrame, cv::COLOR_BGR2RGB);
    
    QImage img(displayFrame.data, displayFrame.cols, displayFrame.rows,
               displayFrame.step, QImage::Format_RGB888);
    m_confirmImageLabel->setPixmap(QPixmap::fromImage(img));
    
    // 显示确认界面，隐藏预览界面
    m_previewLabel->hide();
    ui->startFaceCaptureBtn->setEnabled(false);
    m_confirmImageLabel->show();
    m_confirmButton->show();
    m_retakeButton->show();
    
    updateFaceStatus("请确认图像质量", "rgb(24, 144, 255)");
}

// 新增：切换到预览界面
void BiometricCollectionPage::switchToPreviewView()
{
    qDebug() << "[人脸采集] 切换到预览界面";
    
    m_faceCaptureState = FacePreviewing;
    
    // 显示预览界面，隐藏确认界面
    m_previewLabel->show();
    ui->startFaceCaptureBtn->setText("拍照");
    ui->startFaceCaptureBtn->setEnabled(true);
    m_confirmImageLabel->hide();
    m_confirmButton->hide();
    m_retakeButton->hide();
    
    updateFaceStatus("请正视摄像头，点击拍照按钮", "rgb(100, 100, 100)");
}

// 新增：确认按钮点击处理
void BiometricCollectionPage::onConfirmButtonClicked()
{
    qDebug() << "[人脸采集] 用户确认图像";
    
    // 处理图像并保存
    m_faceData = preprocessFaceImage(m_capturedFrame);
    
    m_faceCaptureState = FaceCompleted;
    updateFaceStatus("人脸采集成功", "rgb(82, 196, 26)");
    
    // 隐藏确认界面
    m_confirmImageLabel->hide();
    m_confirmButton->hide();
    m_retakeButton->hide();
    
    // 切换到下一步（指纹采集）
    switchToStep(FingerprintCollection);
}

// 新增：重拍按钮点击处理
void BiometricCollectionPage::onRetakeButtonClicked()
{
    qDebug() << "[人脸采集] 用户重拍";
    
    // 释放捕获的帧
    if (!m_capturedFrame.empty()) {
        m_capturedFrame.release();
        qDebug() << "[人脸采集] 释放捕获帧";
    }
    
    // 返回预览界面
    switchToPreviewView();
    startLivePreview();
}

// 新增：清理摄像头资源
void BiometricCollectionPage::cleanupCamera()
{
    qDebug() << "[生物特征采集] 清理摄像头资源";
    
    // 停止定时器
    if (m_previewTimer && m_previewTimer->isActive()) {
        m_previewTimer->stop();
        qDebug() << "[生物特征采集] 停止预览定时器";
    }
    
    if (m_faceCaptureTimer && m_faceCaptureTimer->isActive()) {
        m_faceCaptureTimer->stop();
        qDebug() << "[生物特征采集] 停止人脸采集定时器";
    }
    
    // 释放图像数据
    if (!m_currentFrame.empty()) {
        m_currentFrame.release();
        qDebug() << "[生物特征采集] 释放当前帧";
    }
    
    if (!m_capturedFrame.empty()) {
        m_capturedFrame.release();
        qDebug() << "[生物特征采集] 释放捕获帧";
    }
    
    // 释放摄像头
    if (m_camera.isOpened()) {
        m_camera.release();
        qDebug() << "[生物特征采集] 释放摄像头";
        
        // 等待一小段时间确保资源完全释放
        QThread::msleep(200);
        qDebug() << "[生物特征采集] 等待摄像头资源完全释放";
    }
    
    // 重置状态
    m_faceCaptureState = FaceInitializing;
    m_faceDetected = false;
    m_detectedFaces.clear();
    
    qDebug() << "[生物特征采集] 摄像头资源清理完成";
}
