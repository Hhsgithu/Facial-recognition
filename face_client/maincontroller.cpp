#include "maincontroller.h"
#include "fingerprintpage.h"
#include <QVBoxLayout>
#include <QDebug>
#include <QPalette>
#include <QThread>

MainController::MainController(QWidget *parent)
    : QWidget(parent)
    , m_useNewRegistrationFlow(true)  // 使用新注册流程
{
    setFixedSize(1024, 600);
    setAutoFillBackground(true);
    
    QPalette pal = palette();
    pal.setColor(QPalette::Window, QColor(245, 247, 250));
    setPalette(pal);

    // 创建硬件控制器
    hardwareController = new HardwareController(this);
    
    // 创建网络管理器
    networkManager = new NetworkManager(this);
    
    // 初始化硬件
    hardwareController->initializeFingerprint();
    hardwareController->initializeServo();

    // 创建各个页面，设置父对象为this
    homePage = new HomePage(this);
    faceRecognitionPage = new FaceRecognition(this);
    passwordUnlockPage = new PasswordPage(this);
    fingerprintUnlockPage = new FingerprintPage(this);
    registrationKeyPage = new RegistrationKeyPage(this);
    userInfoPage = new UserInfoPage(this);
    biometricCollectionPage = new BiometricCollectionPage(this);
    
    // 创建新注册流程页面
    faceCapturePage = new FaceCapturePage(this);
    fingerprintCapturePage = new FingerprintCapturePage(this);
    passwordSetupPage = new PasswordSetupPage(this);
    registrationCompletePage = new RegistrationCompletePage(this);
    
    // 设置指纹页面的硬件控制器
    fingerprintUnlockPage->setHardwareController(hardwareController);
    
    // 设置生物特征采集页面的硬件控制器
    biometricCollectionPage->setHardwareController(hardwareController);
    
    // 设置新注册流程页面的硬件控制器
    faceCapturePage->setHardwareController(hardwareController);
    fingerprintCapturePage->setHardwareController(hardwareController);
    
    // 创建门禁模拟器
    doorSimulator = new DoorSimulator(this);
    
    // 设置门禁模拟器的硬件控制器
    doorSimulator->setHardwareController(hardwareController);
    
    // 创建开门状态显示标签
    doorStatusLabel = new QLabel(this);
    doorStatusLabel->setFixedSize(400, 200);
    doorStatusLabel->move((1024 - 400) / 2, (600 - 200) / 2);
    doorStatusLabel->setAlignment(Qt::AlignCenter);
    doorStatusLabel->setStyleSheet(
        "QLabel {"
        "   background-color: rgba(82, 196, 26, 230);"
        "   color: white;"
        "   font-size: 36px;"
        "   font-weight: bold;"
        "   border-radius: 20px;"
        "}"
    );
    doorStatusLabel->setText("开门中...");
    doorStatusLabel->hide();
    
    // 设置页面位置
    homePage->setGeometry(0, 0, 1024, 600);
    faceRecognitionPage->setGeometry(0, 0, 1024, 600);
    passwordUnlockPage->setGeometry(0, 0, 1024, 600);
    fingerprintUnlockPage->setGeometry(0, 0, 1024, 600);
    registrationKeyPage->setGeometry(0, 0, 1024, 600);
    userInfoPage->setGeometry(0, 0, 1024, 600);
    biometricCollectionPage->setGeometry(0, 0, 1024, 600);
    faceCapturePage->setGeometry(0, 0, 1024, 600);
    fingerprintCapturePage->setGeometry(0, 0, 1024, 600);
    passwordSetupPage->setGeometry(0, 0, 1024, 600);
    registrationCompletePage->setGeometry(0, 0, 1024, 600);
    
    // 初始时只显示首页，隐藏其他页面
    homePage->show();
    homePage->raise();
    faceRecognitionPage->hide();
    passwordUnlockPage->hide();
    fingerprintUnlockPage->hide();
    registrationKeyPage->hide();
    userInfoPage->hide();
    biometricCollectionPage->hide();
    faceCapturePage->hide();
    fingerprintCapturePage->hide();
    passwordSetupPage->hide();
    registrationCompletePage->hide();

    // 连接首页信号
    connect(homePage, &HomePage::faceRecognitionClicked, this, &MainController::showFaceRecognitionPage);
    connect(homePage, &HomePage::passwordUnlockClicked, this, &MainController::showPasswordUnlockPage);
    connect(homePage, &HomePage::fingerprintUnlockClicked, this, &MainController::showFingerprintUnlockPage);
    connect(homePage, &HomePage::userRegistrationClicked, this, &MainController::showRegistrationKeyPage);

     // 初始化 ui

    

    // 连接密码页面返回信号
    connect(passwordUnlockPage, &PasswordPage::backToHomeClicked, this, &MainController::showHomePage);
    
    // 连接密码验证成功信号到门禁模拟器
    connect(passwordUnlockPage, &PasswordPage::verifySuccess, doorSimulator, &DoorSimulator::openDoor);

    // 连接人脸识别页面返回信号
    connect(faceRecognitionPage, &FaceRecognition::backToHomeClicked, this, &MainController::showHomePage);
    
    // 连接人脸识别成功信号到门禁模拟器
    connect(faceRecognitionPage, &FaceRecognition::recognitionSuccess, doorSimulator, &DoorSimulator::openDoor);
    
    // 连接指纹页面返回信号
    connect(fingerprintUnlockPage, &FingerprintPage::backToHomeClicked, this, &MainController::showHomePage);
    
    // 连接指纹验证失败信号
    connect(fingerprintUnlockPage, &FingerprintPage::verifyFailed, this, [this](const QString& message) {
        qDebug() << "[控制器] 指纹验证失败:" << message;
        // 验证失败时不需要特殊处理，页面会自己处理UI反馈
    });
    
    // 连接指纹验证成功信号到门禁模拟器
    connect(fingerprintUnlockPage, &FingerprintPage::verifySuccess, this, [this](const QString& userId, const QString& userName) {
        qDebug() << "[控制器] 指纹验证成功 - 用户:" << userName << "ID:" << userId;
        doorSimulator->openDoor();
    });
    
    // 连接门禁模拟器信号
    connect(doorSimulator, &DoorSimulator::doorOpened, this, &MainController::onDoorOpened);
    connect(doorSimulator, &DoorSimulator::doorClosed, this, &MainController::onDoorClosed);
    
    // 连接注册页面信号
    connect(registrationKeyPage, &RegistrationKeyPage::backToHomeClicked, this, &MainController::showHomePage);
    connect(registrationKeyPage, &RegistrationKeyPage::keyVerified, this, &MainController::onKeyVerified);
    
    connect(userInfoPage, &UserInfoPage::backToHomeClicked, this, &MainController::showHomePage);
    connect(userInfoPage, &UserInfoPage::userInfoCompleted, this, &MainController::onUserInfoCompleted);
    
    connect(biometricCollectionPage, &BiometricCollectionPage::backToHomeClicked, this, &MainController::showHomePage);
    connect(biometricCollectionPage, &BiometricCollectionPage::registrationCompleted, this, &MainController::onRegistrationCompleted);
    
    // 连接新注册流程页面信号
    connect(faceCapturePage, &FaceCapturePage::faceCaptureCompleted, this, &MainController::onFaceCaptureCompleted);
    connect(faceCapturePage, &FaceCapturePage::backClicked, this, &MainController::showUserInfoPage);
    
    connect(fingerprintCapturePage, &FingerprintCapturePage::fingerprintCaptureCompleted, this, &MainController::onFingerprintCaptureCompleted);
    connect(fingerprintCapturePage, &FingerprintCapturePage::backClicked, this, &MainController::showFaceCapturePage);
    
    connect(passwordSetupPage, &PasswordSetupPage::passwordSetupCompleted, this, &MainController::onPasswordSetupCompleted);
    connect(passwordSetupPage, &PasswordSetupPage::backClicked, this, &MainController::showFingerprintCapturePage);
    
    connect(registrationCompletePage, &RegistrationCompletePage::registrationRequested, this, &MainController::onRegistrationRequested);
    connect(registrationCompletePage, &RegistrationCompletePage::registrationCompleted, this, &MainController::onNewRegistrationCompleted);
    connect(registrationCompletePage, &RegistrationCompletePage::backClicked, this, &MainController::showPasswordSetupPage);
    
    // 连接网络管理器信号
    connect(networkManager, &NetworkManager::keyVerificationResult, this, &MainController::onKeyVerificationResult);
    connect(networkManager, &NetworkManager::registrationResult, this, &MainController::onRegistrationResult);
    connect(networkManager, &NetworkManager::connected, this, &MainController::onNetworkConnected);
    connect(networkManager, &NetworkManager::disconnected, this, &MainController::onNetworkDisconnected);
    connect(networkManager, &NetworkManager::connectionError, this, &MainController::onNetworkError);
    
    // 启动网络连接
    networkManager->connectToServer();

    qDebug() << "[控制器] MainController初始化完成";
}

MainController::~MainController()
{
    qDebug() << "[控制器] MainController析构，释放资源...";
    
    // 停止人脸识别（释放摄像头）
    if(faceRecognitionPage) {
        faceRecognitionPage->stopRecognition();
    }
    
    // 清理硬件资源
    if(hardwareController) {
        hardwareController->cleanup();
    }
    
    qDebug() << "[控制器] 资源释放完成";
}

void MainController::showHomePage()
{
    qDebug() << "[控制器] 切换到首页";
    
    // 停止人脸识别
    faceRecognitionPage->stopRecognition();
    
    // 停止指纹认证并清理资源
    if (fingerprintUnlockPage) {
        fingerprintUnlockPage->stopAuthentication();
        fingerprintUnlockPage->cleanup();
    }
    
    // 清理注册页面输入
    if (registrationKeyPage) {
        registrationKeyPage->clearInput();
    }
    if (userInfoPage) {
        userInfoPage->clearInput();
    }
    if (biometricCollectionPage) {
        biometricCollectionPage->resetCollection();
    }
    
    // 清空新注册流程数据
    m_registrationData = RegistrationData();
    
    // 隐藏注册进度指示器
    homePage->hideRegistrationProgress();
    
    // 隐藏其他页面
    faceRecognitionPage->hide();
    passwordUnlockPage->hide();
    fingerprintUnlockPage->hide();
    registrationKeyPage->hide();
    userInfoPage->hide();
    biometricCollectionPage->hide();
    faceCapturePage->hide();
    fingerprintCapturePage->hide();
    passwordSetupPage->hide();
    registrationCompletePage->hide();
    
    // 显示首页
    homePage->show();
    homePage->raise();
}

void MainController::showFaceRecognitionPage()
{
    qDebug() << "[控制器] 切换到人脸识别页面";
    
    // 隐藏其他页面
    homePage->hide();
    passwordUnlockPage->hide();
    fingerprintUnlockPage->hide();
    registrationKeyPage->hide();
    userInfoPage->hide();
    biometricCollectionPage->hide();
    faceCapturePage->hide();
    fingerprintCapturePage->hide();
    passwordSetupPage->hide();
    registrationCompletePage->hide();
    
    // 显示人脸识别页面
    faceRecognitionPage->show();
    faceRecognitionPage->raise();
    faceRecognitionPage->startRecognition();
}

void MainController::showPasswordUnlockPage()
{
    qDebug() << "[控制器] 切换到密码解锁页面";
    
    // 停止人脸识别
    faceRecognitionPage->stopRecognition();
    
    // 隐藏其他页面
    homePage->hide();
    faceRecognitionPage->hide();
    fingerprintUnlockPage->hide();
    registrationKeyPage->hide();
    userInfoPage->hide();
    biometricCollectionPage->hide();
    faceCapturePage->hide();
    fingerprintCapturePage->hide();
    passwordSetupPage->hide();
    registrationCompletePage->hide();
    
    // 显示密码页面
    passwordUnlockPage->show();
    passwordUnlockPage->raise();
}

void MainController::showFingerprintUnlockPage()
{
    qDebug() << "[控制器] 切换到指纹解锁页面";
    
    // 停止人脸识别
    faceRecognitionPage->stopRecognition();
    
    // 隐藏其他页面
    homePage->hide();
    faceRecognitionPage->hide();
    passwordUnlockPage->hide();
    registrationKeyPage->hide();
    userInfoPage->hide();
    biometricCollectionPage->hide();
    faceCapturePage->hide();
    fingerprintCapturePage->hide();
    passwordSetupPage->hide();
    registrationCompletePage->hide();
    
    // 显示指纹页面
    fingerprintUnlockPage->show();
    fingerprintUnlockPage->raise();
    
    // 确保硬件控制器已初始化
    if (hardwareController) {
        // 重新初始化硬件（防止设备被其他进程占用）
        hardwareController->initializeFingerprint();
        hardwareController->initializeServo();
    }
    
    // 自动开始指纹认证
    QTimer::singleShot(500, fingerprintUnlockPage, &FingerprintPage::startAuthentication);
}

void MainController::onDoorOpened()
{
    qDebug() << "[控制器] 门已打开";
    showDoorOpenStatus();
}

void MainController::onDoorClosed()
{
    qDebug() << "[控制器] 门已关闭";
    hideDoorOpenStatus();
    showHomePage();
}

void MainController::showDoorOpenStatus()
{
    qDebug() << "[控制器] 显示开门状态";
    doorStatusLabel->raise();
    doorStatusLabel->show();
}

void MainController::hideDoorOpenStatus()
{
    qDebug() << "[控制器] 隐藏开门状态";
    doorStatusLabel->hide();
}

void MainController::showRegistrationKeyPage()
{
    qDebug() << "[控制器] 切换到注册密钥页面";
    
    // 停止人脸识别
    faceRecognitionPage->stopRecognition();
    
    // 隐藏其他页面
    homePage->hide();
    faceRecognitionPage->hide();
    passwordUnlockPage->hide();
    fingerprintUnlockPage->hide();
    userInfoPage->hide();
    biometricCollectionPage->hide();
    faceCapturePage->hide();
    fingerprintCapturePage->hide();
    passwordSetupPage->hide();
    registrationCompletePage->hide();
    
    // 显示注册密钥页面
    registrationKeyPage->show();
    registrationKeyPage->raise();
}

void MainController::showUserInfoPage()
{
    qDebug() << "[控制器] 切换到用户信息页面";
    
    // 更新首页进度
    homePage->showRegistrationProgress("正在填写用户信息...");
    
    // 隐藏其他页面
    homePage->hide();
    faceRecognitionPage->hide();
    passwordUnlockPage->hide();
    fingerprintUnlockPage->hide();
    registrationKeyPage->hide();
    faceCapturePage->hide();
    fingerprintCapturePage->hide();
    passwordSetupPage->hide();
    registrationCompletePage->hide();
    
    // 显示用户信息页面
    userInfoPage->show();
    userInfoPage->raise();
}

void MainController::onKeyVerified(const QString& key)
{
    qDebug() << "[控制器] 注册密钥验证:" << key;
    
    // 保存当前密钥到新注册数据结构
    m_registrationData.registrationKey = key;
    
    // 保存当前密钥（旧流程兼容）
    m_currentRegistrationKey = key;
    
    // 使用网络管理器验证密钥
    networkManager->verifyRegistrationKey(key);
}

void MainController::onUserInfoCompleted(const UserInfo& userInfo)
{
    qDebug() << "[控制器] 用户信息填写完成:" << userInfo.name << userInfo.phone << userInfo.address;
    
    // 保存用户信息到新注册数据结构
    m_registrationData.userInfo = userInfo;
    
    // 根据配置选择注册流程
    if (m_useNewRegistrationFlow) {
        qDebug() << "[控制器] 使用新注册流程";
        // 跳转到人脸采集页面（新流程）
        showFaceCapturePage();
    } else {
        qDebug() << "[控制器] 使用旧注册流程";
        // 保存用户信息（旧流程兼容）
        m_currentUserInfo = userInfo;
        // 跳转到生物特征采集页面（旧流程）
        showBiometricCollectionPage();
    }
}

void MainController::showBiometricCollectionPage()
{
    qDebug() << "[控制器] 切换到生物特征采集页面";
    
    // 更新首页进度
    homePage->showRegistrationProgress("正在进行生物特征采集...");
    
    // 隐藏其他页面（确保释放摄像头资源）
    homePage->hide();
    faceRecognitionPage->hide();
    passwordUnlockPage->hide();
    fingerprintUnlockPage->hide();
    registrationKeyPage->hide();
    userInfoPage->hide();
    
    // 等待一小段时间确保资源释放
    QThread::msleep(200);
    
    // 设置用户信息并开始采集
    biometricCollectionPage->setUserInfo(m_currentUserInfo);
    biometricCollectionPage->show();
    biometricCollectionPage->raise();
    biometricCollectionPage->startCollection();
}

void MainController::onRegistrationCompleted(const UserInfo& userInfo, const QByteArray& faceData, 
                                           const QByteArray& fingerprintData, const QString& password)
{
    qDebug() << "[控制器] 注册完成:" << userInfo.name;
    qDebug() << "[控制器] 人脸数据大小:" << faceData.size();
    qDebug() << "[控制器] 指纹数据大小:" << fingerprintData.size();
    qDebug() << "[控制器] 密码长度:" << password.length();
    qDebug() << "[控制器] 注册密钥:" << m_currentRegistrationKey;
    
    // 使用网络管理器发送完整的注册数据到服务器（包含注册密钥）
    networkManager->sendRegistrationData(m_currentRegistrationKey, userInfo, faceData, fingerprintData, password);
}

void MainController::onKeyVerificationResult(bool success, const QString& message)
{
    qDebug() << "[控制器] 密钥验证结果:" << success << message;
    
    // 在注册密钥页面显示验证结果
    registrationKeyPage->showVerificationStatus(success, message);
    
    if (success) {
        // 验证成功，更新首页进度并延迟跳转到用户信息页面
        homePage->showRegistrationProgress("密钥验证成功，正在跳转到用户信息填写...");
        QTimer::singleShot(1500, this, &MainController::showUserInfoPage);
    } else {
        // 验证失败，更新首页进度
        homePage->showRegistrationProgress("密钥验证失败，请重试");
    }
}

void MainController::onRegistrationResult(bool success, const QString& message, int fingerprintId)
{
    qDebug() << "[控制器] 注册结果:" << success << message << "指纹ID:" << fingerprintId;
    
    if (m_useNewRegistrationFlow) {
        // 新注册流程
        if (success) {
            // 服务器现在使用硬件分配的指纹ID，ID应该一致
            if (fingerprintId > 0 && !m_registrationData.fingerprintData.isEmpty()) {
                qDebug() << "[控制器] 服务器确认指纹ID:" << fingerprintId << "（与硬件模块ID一致）";
            }
            
            registrationCompletePage->showSuccess();
            homePage->showRegistrationProgress("注册成功！");
            // 3秒后自动返回首页
            QTimer::singleShot(3000, this, &MainController::onNewRegistrationCompleted);
        } else {
            registrationCompletePage->showError(message);
            homePage->showRegistrationProgress("注册失败，请重试");
        }
    } else {
        // 旧注册流程
        // 在生物特征采集页面显示注册结果
        biometricCollectionPage->showRegistrationStatus(success, message);
        
        if (success) {
            // 服务器现在使用硬件分配的指纹ID，ID应该一致
            if (fingerprintId > 0) {
                qDebug() << "[控制器] 服务器确认指纹ID:" << fingerprintId << "（与硬件模块ID一致）";
            }
            
            // 注册成功，更新首页进度并延迟返回首页
            homePage->showRegistrationProgress("注册成功！正在返回首页...");
            QTimer::singleShot(3000, this, &MainController::showHomePage);
        } else {
            // 注册失败，更新首页进度
            homePage->showRegistrationProgress("注册失败，请重试或返回首页");
        }
        // 注册失败时不自动返回，让用户选择重试或返回
    }
}

void MainController::onNetworkConnected()
{
    qDebug() << "[控制器] 网络已连接";
    // TODO: 更新UI显示网络连接状态
}

void MainController::onNetworkDisconnected()
{
    qDebug() << "[控制器] 网络已断开";
    // TODO: 更新UI显示网络断开状态
}

void MainController::onNetworkError(const QString& error)
{
    qDebug() << "[控制器] 网络错误:" << error;
    // TODO: 显示网络错误提示
}

// ==================== 新注册流程页面跳转方法 ====================

void MainController::showFaceCapturePage()
{
    qDebug() << "[控制器] 切换到人脸采集页面（新流程）";
    
    // 更新首页进度
    homePage->showRegistrationProgress("正在进行人脸采集...");
    
    // 隐藏其他页面（确保释放摄像头资源）
    homePage->hide();
    faceRecognitionPage->hide();
    passwordUnlockPage->hide();
    fingerprintUnlockPage->hide();
    registrationKeyPage->hide();
    userInfoPage->hide();
    biometricCollectionPage->hide();
    fingerprintCapturePage->hide();
    passwordSetupPage->hide();
    registrationCompletePage->hide();
    
    // 等待一小段时间确保资源释放
    QThread::msleep(200);
    
    // 设置用户信息并开始采集
    faceCapturePage->setUserInfo(m_registrationData.userInfo);
    faceCapturePage->show();
    faceCapturePage->raise();
    faceCapturePage->startCapture();
}

void MainController::showFingerprintCapturePage()
{
    qDebug() << "[控制器] 切换到指纹采集页面（新流程）";
    
    // 更新首页进度
    homePage->showRegistrationProgress("正在进行指纹采集...");
    
    // 隐藏其他页面
    homePage->hide();
    faceRecognitionPage->hide();
    passwordUnlockPage->hide();
    fingerprintUnlockPage->hide();
    registrationKeyPage->hide();
    userInfoPage->hide();
    biometricCollectionPage->hide();
    faceCapturePage->hide();
    passwordSetupPage->hide();
    registrationCompletePage->hide();
    
    // 设置用户信息并开始采集
    fingerprintCapturePage->setUserInfo(m_registrationData.userInfo);
    fingerprintCapturePage->show();
    fingerprintCapturePage->raise();
    fingerprintCapturePage->startCapture();
}

void MainController::showPasswordSetupPage()
{
    qDebug() << "[控制器] 切换到密码设置页面（新流程）";
    
    // 更新首页进度
    homePage->showRegistrationProgress("正在设置密码...");
    
    // 隐藏其他页面
    homePage->hide();
    faceRecognitionPage->hide();
    passwordUnlockPage->hide();
    fingerprintUnlockPage->hide();
    registrationKeyPage->hide();
    userInfoPage->hide();
    biometricCollectionPage->hide();
    faceCapturePage->hide();
    fingerprintCapturePage->hide();
    registrationCompletePage->hide();
    
    // 设置用户信息并开始设置
    passwordSetupPage->setUserInfo(m_registrationData.userInfo);
    passwordSetupPage->show();
    passwordSetupPage->raise();
    passwordSetupPage->startSetup();
}

void MainController::showRegistrationCompletePage()
{
    qDebug() << "[控制器] 切换到注册完成页面（新流程）";
    
    // 更新首页进度
    homePage->showRegistrationProgress("准备提交注册信息...");
    
    // 隐藏其他页面
    homePage->hide();
    faceRecognitionPage->hide();
    passwordUnlockPage->hide();
    fingerprintUnlockPage->hide();
    registrationKeyPage->hide();
    userInfoPage->hide();
    biometricCollectionPage->hide();
    faceCapturePage->hide();
    fingerprintCapturePage->hide();
    passwordSetupPage->hide();
    
    // 重置页面状态，确保显示"信息采集完成"而非上次的结果
    registrationCompletePage->reset();
    // 设置用户信息
    registrationCompletePage->setUserInfo(m_registrationData.userInfo);
    registrationCompletePage->show();
    registrationCompletePage->raise();
}

// ==================== 新注册流程数据处理方法 ====================

void MainController::onFaceCaptureCompleted(const QByteArray& faceData)
{
    qDebug() << "[控制器] 人脸采集完成，数据大小:" << faceData.size();
    
    // 保存人脸数据
    m_registrationData.faceData = faceData;
    
    // 跳转到指纹采集页面
    showFingerprintCapturePage();
}

void MainController::onFingerprintCaptureCompleted(const QByteArray& fingerprintData)
{
    qDebug() << "[控制器] 指纹采集完成，数据大小:" << fingerprintData.size();
    
    // 保存指纹数据
    m_registrationData.fingerprintData = fingerprintData;
    
    // 跳转到密码设置页面
    showPasswordSetupPage();
}

void MainController::onPasswordSetupCompleted(const QString& password)
{
    qDebug() << "[控制器] 密码设置完成，密码长度:" << password.length();
    
    // 保存密码
    m_registrationData.password = password;
    
    // 跳转到注册完成页面
    showRegistrationCompletePage();
}

void MainController::onRegistrationRequested()
{
    qDebug() << "[控制器] 用户请求提交注册";
    qDebug() << "[控制器] 注册数据 - 姓名:" << m_registrationData.userInfo.name;
    qDebug() << "[控制器] 注册数据 - 人脸数据大小:" << m_registrationData.faceData.size();
    qDebug() << "[控制器] 注册数据 - 指纹数据大小:" << m_registrationData.fingerprintData.size();
    qDebug() << "[控制器] 注册数据 - 密码长度:" << m_registrationData.password.length();
    qDebug() << "[控制器] 注册数据 - 注册密钥:" << m_registrationData.registrationKey;
    
    // 显示提交进度
    registrationCompletePage->showProgress();
    
    // 使用网络管理器发送完整的注册数据到服务器
    networkManager->sendRegistrationData(
        m_registrationData.registrationKey,
        m_registrationData.userInfo,
        m_registrationData.faceData,
        m_registrationData.fingerprintData,
        m_registrationData.password
    );
}

void MainController::onNewRegistrationCompleted()
{
    qDebug() << "[控制器] 新注册流程完成，返回首页";
    
    // 清空注册数据
    m_registrationData = RegistrationData();
    
    // 返回首页
    showHomePage();
}
