#include "fingerprintpage.h"
#include "ui_fingerprintpage.h"
#include <QDebug>
#include <QJsonDocument>
#include <QJsonObject>
#include <QHostAddress>
#include <QDateTime>

FingerprintPage::FingerprintPage(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::FingerprintPage)
    , authTimer(new QTimer(this))
    , isAuthenticating(false)
    , hardwareController(nullptr)
    , currentUserId(0)
    , tcpSocket(nullptr)
    , reconnectTimer(nullptr)
    , isConnected(false)
    , isVerifying(false)
    , networkRetryCount(0)
    , networkRetryTimer(nullptr)
{
    ui->setupUi(this);
    
    // 设置认证超时定时器
    authTimer->setSingleShot(true);
    authTimer->setInterval(8000); // 8秒超时
    
    // 初始化网络组件
    tcpSocket = new QTcpSocket(this);
    reconnectTimer = new QTimer(this);
    networkRetryTimer = new QTimer(this);
    
    // 连接信号槽
    connect(ui->backBtn, &QPushButton::clicked, this, &FingerprintPage::onBackButtonClicked);
    connect(ui->startBtn, &QPushButton::clicked, this, &FingerprintPage::startAuthentication);
    connect(ui->stopBtn, &QPushButton::clicked, this, &FingerprintPage::stopAuthentication);
    connect(authTimer, &QTimer::timeout, this, &FingerprintPage::onAuthenticationTimeout);
    
    // 连接网络信号槽
    connect(tcpSocket, &QTcpSocket::connected, this, &FingerprintPage::onConnected);
    connect(tcpSocket, &QTcpSocket::disconnected, this, &FingerprintPage::onDisconnected);
    connect(tcpSocket, &QTcpSocket::readyRead, this, &FingerprintPage::onNetworkData);
    connect(tcpSocket, QOverload<QAbstractSocket::SocketError>::of(&QAbstractSocket::error),
            this, &FingerprintPage::onSocketError);
    
    // 连接重连定时器
    connect(reconnectTimer, &QTimer::timeout, this, &FingerprintPage::onReconnectTimer);
    reconnectTimer->start(5000); // 每5秒尝试重连
    
    // 连接网络重试定时器
    networkRetryTimer->setSingleShot(true);
    networkRetryTimer->setInterval(NETWORK_RETRY_INTERVAL);
    
    // 初始化UI状态
    resetAuthenticationState();
    updateConnectionStatus(false); // 初始状态为未连接
    
    // 延迟显示硬件状态（等待硬件控制器设置完成）
    QTimer::singleShot(100, this, &FingerprintPage::showHardwareStatus);
    
    qDebug() << "[指纹页面] FingerprintPage初始化完成";
}

FingerprintPage::~FingerprintPage()
{
    qDebug() << "[指纹页面] FingerprintPage析构开始";
    
    // 析构时需要清理所有资源，包括网络
    try {
        resetPageState();
        cleanupHardwareResources();
        cleanupNetworkResources(); // 析构时才清理网络资源
    } catch (...) {
        qWarning() << "[指纹页面] 析构清理过程中发生异常";
    }
    
    delete ui;
    qDebug() << "[指纹页面] FingerprintPage析构完成";
}

void FingerprintPage::setHardwareController(HardwareController* controller)
{
    if (hardwareController) {
        // 断开之前的连接
        disconnect(hardwareController, nullptr, this, nullptr);
    }
    
    hardwareController = controller;
    
    if (hardwareController) {
        // 连接硬件控制器信号 - 使用int版本的fingerprintCaptured信号（用于验证）
        connect(hardwareController, static_cast<void(HardwareController::*)(int)>(&HardwareController::fingerprintCaptured),
                this, &FingerprintPage::onFingerprintCaptured);
        connect(hardwareController, &HardwareController::fingerprintError,
                this, &FingerprintPage::onFingerprintError);
        
        qDebug() << "[指纹页面] 硬件控制器已设置";
        
        // 显示硬件状态
        showHardwareStatus();
    }
}

void FingerprintPage::startAuthentication()
{
    qDebug() << "[指纹页面] 开始指纹认证";
    
    if (isAuthenticating) {
        qDebug() << "[指纹页面] 认证已在进行中，忽略重复请求";
        return;
    }
    
    if (!hardwareController) {
        qWarning() << "[指纹页面] 硬件控制器未设置";
        showDetailedStatus("硬件控制器未初始化", "rgb(255, 77, 79)", "rgb(255, 77, 79)");
        return;
    }
    
    if (!hardwareController->isFingerprintReady()) {
        qWarning() << "[指纹页面] 指纹模块未就绪";
        showDetailedStatus("指纹设备不可用", "rgb(255, 77, 79)", "rgb(255, 77, 79)");
        return;
    }
    
    isAuthenticating = true;
    currentUserId = 0;
    
    // 更新UI状态 - 需求8.2: 显示适当的状态消息
    updateUIForAuthenticationState(true);
    
    // 启动硬件指纹验证
    if (!hardwareController->startFingerprintVerification()) {
        qWarning() << "[指纹页面] 启动指纹验证失败";
        showDetailedStatus("启动指纹验证失败", "rgb(255, 77, 79)", "rgb(255, 77, 79)");
        resetAuthenticationState();
        return;
    }
    
    // 启动超时定时器 - 需求6.4: 实现认证超时处理
    authTimer->start();
    
    qDebug() << "[指纹页面] 指纹认证已启动，等待用户输入";
}

void FingerprintPage::stopAuthentication()
{
    qDebug() << "[指纹页面] 停止指纹认证";
    
    if (!isAuthenticating) {
        qDebug() << "[指纹页面] 当前没有进行认证，忽略停止请求";
        return;
    }
    
    // 停止硬件指纹验证
    if (hardwareController) {
        hardwareController->stopFingerprintVerification();
    }
    
    // 停止超时定时器
    authTimer->stop();
    
    // 重置状态
    resetAuthenticationState();
    
    qDebug() << "[指纹页面] 指纹认证已停止";
}

void FingerprintPage::cleanup()
{
    qDebug() << "[指纹页面] 外部调用清理方法";
    performResourceCleanup();
}

void FingerprintPage::onBackButtonClicked()
{
    qDebug() << "[指纹页面] 用户点击返回按钮";
    
    // 如果正在认证，先停止认证
    if (isAuthenticating) {
        stopAuthentication();
    }
    
    // 需求8.5: 页面切换时的状态重置
    resetPageState();
    
    emit backToHomeClicked();
}

void FingerprintPage::onAuthenticationTimeout()
{
    qDebug() << "[指纹页面] 指纹认证超时";
    
    // 需求6.4: 认证超时处理
    handleAuthenticationFailure("认证超时");
    
    // 停止硬件指纹验证
    if (hardwareController) {
        hardwareController->stopFingerprintVerification();
    }
}

void FingerprintPage::onFingerprintCaptured(int userId)
{
    qDebug() << "[指纹页面] 收到指纹捕获信号，用户ID:" << userId;
    
    if (!isAuthenticating) {
        qDebug() << "[指纹页面] 当前未在认证状态，忽略指纹结果";
        return;
    }
    
    // 停止超时定时器
    authTimer->stop();
    
    // 处理认证结果
    processAuthenticationResult(userId);
}

void FingerprintPage::onFingerprintError(const QString& error)
{
    qWarning() << "[指纹页面] 指纹错误:" << error;
    
    if (!isAuthenticating) {
        qDebug() << "[指纹页面] 当前未在认证状态，忽略指纹错误";
        return;
    }
    
    // 停止超时定时器
    authTimer->stop();
    
    // 需求6.1: 实现硬件错误的用户友好提示
    QString userFriendlyMessage = translateHardwareError(error);
    showDetailedStatus(userFriendlyMessage, "rgb(255, 77, 79)", "rgb(255, 77, 79)");
    
    // 重置认证状态
    isAuthenticating = false;
    enableControls(true);
    
    emit verifyFailed(userFriendlyMessage);
}

void FingerprintPage::updateStatus(const QString& message, const QString& color)
{
    ui->statusLabel->setText(message);
    ui->statusLabel->setStyleSheet(QString(
        "font-size: 18px;"
        "font-weight: bold;"
        "color: %1;"
        "background: transparent;"
    ).arg(color));
    
    qDebug() << "[指纹页面] 状态更新:" << message;
}

void FingerprintPage::resetAuthenticationState()
{
    currentUserId = 0;
    
    // 更新UI状态 - 需求8.5: 返回首页时重置所有认证状态变量
    updateUIForAuthenticationState(false);
    
    // 停止定时器
    authTimer->stop();
    
    qDebug() << "[指纹页面] 认证状态已重置";
}

void FingerprintPage::processAuthenticationResult(int userId)
{
    qDebug() << "[指纹页面] 处理认证结果，用户ID:" << userId;
    
    currentUserId = userId;
    
    // 根据需求3.3, 3.4, 3.5: ID=0表示认证失败，ID>0表示认证成功
    if (userId == 0) {
        // 认证失败处理
        qDebug() << "[指纹页面] 指纹认证失败 - ID为0";
        handleAuthenticationFailure("指纹未注册或不匹配");
    } else {
        // 指纹识别成功，需要进行网络验证
        qDebug() << "[指纹页面] 指纹认证成功 - 用户ID:" << userId;
        showDetailedStatus("指纹识别成功，正在验证...", "rgb(82, 196, 26)", "rgb(82, 196, 26)");
        
        // 检查网络连接状态 - 需求4.1, 6.3
        if (!isConnected || tcpSocket->state() != QTcpSocket::ConnectedState) {
            qWarning() << "[指纹页面] 未连接服务器，尝试重新连接...";
            showDetailedStatus("网络未连接，正在重连...", "rgb(255, 152, 0)", "rgb(255, 152, 0)");
            connectToServer();
            
            // 连接失败时的处理
            QTimer::singleShot(3000, this, [this]() {
                if (!isConnected) {
                    handleAuthenticationFailure("网络连接失败");
                }
            });
            return;
        }
        
        // 发送网络验证请求 - 需求4.2
        sendFingerprintVerifyRequest(userId);
    }
}

void FingerprintPage::updateConnectionStatus(bool connected)
{
    qDebug() << "[指纹页面] 更新连接状态:" << (connected ? "已连接" : "未连接");
    
    if (connected) {
        ui->connectionLabel->setText("🌐 服务器连接状态: 已连接");
        ui->connectionLabel->setStyleSheet(
            "font-size: 14px;"
            "color: rgb(82, 196, 26);"
            "background: transparent;"
        );
    } else {
        ui->connectionLabel->setText("🌐 服务器连接状态: 未连接");
        ui->connectionLabel->setStyleSheet(
            "font-size: 14px;"
            "color: rgb(255, 77, 79);"
            "background: transparent;"
        );
    }
}

void FingerprintPage::showDetailedStatus(const QString& message, const QString& color, const QString& iconColor)
{
    // 更新状态标签
    ui->statusLabel->setText(message);
    ui->statusLabel->setStyleSheet(QString(
        "font-size: 18px;"
        "font-weight: bold;"
        "color: %1;"
        "background: transparent;"
    ).arg(color));
    
    // 根据状态更新指纹图标和提示
    QString iconText = "👆";
    QString tipText = "请将手指放在指纹传感器上";
    
    if (message.contains("等待指纹输入") || message.contains("正在等待")) {
        iconText = "👆";
        tipText = "请将手指放在指纹传感器上";
        // 添加闪烁效果
        ui->fingerprintIcon->setStyleSheet(QString(
            "font-size: 72px;"
            "color: %1;"
            "background: transparent;"
        ).arg(iconColor));
    } else if (message.contains("识别成功") || message.contains("验证成功")) {
        iconText = "✅";
        tipText = "指纹识别成功！";
    } else if (message.contains("失败") || message.contains("错误") || message.contains("超时")) {
        iconText = "❌";
        tipText = "识别失败，请重试";
    } else if (message.contains("正在验证") || message.contains("正在重连")) {
        iconText = "⏳";
        tipText = "正在处理中...";
    } else if (message.contains("网络")) {
        iconText = "🌐";
        tipText = "网络连接中...";
    }
    
    // 更新指纹图标
    ui->fingerprintIcon->setText(iconText);
    ui->fingerprintIcon->setStyleSheet(QString(
        "font-size: 72px;"
        "color: %1;"
        "background: transparent;"
    ).arg(iconColor));
    
    // 更新提示文字
    ui->fingerprintTip->setText(tipText);
    ui->fingerprintTip->setStyleSheet(QString(
        "font-size: 16px;"
        "color: %1;"
        "background: transparent;"
    ).arg(color));
    
    qDebug() << "[指纹页面] 详细状态更新:" << message;
}

void FingerprintPage::showHardwareStatus()
{
    if (!hardwareController) {
        showDetailedStatus("硬件控制器未初始化", "rgb(255, 77, 79)", "rgb(255, 77, 79)");
        return;
    }
    
    bool fingerprintReady = hardwareController->isFingerprintReady();
    
    if (fingerprintReady) {
        showDetailedStatus("硬件就绪，可以开始识别", "rgb(82, 196, 26)", "rgb(82, 196, 26)");
    } else {
        showDetailedStatus("指纹设备不可用", "rgb(255, 77, 79)", "rgb(255, 77, 79)");
    }
    
    qDebug() << "[指纹页面] 硬件状态 - 指纹模块:" << (fingerprintReady ? "就绪" : "不可用");
}

void FingerprintPage::updateUIForAuthenticationState(bool authenticating)
{
    isAuthenticating = authenticating;
    
    // 更新按钮状态
    enableControls(!authenticating);
    
    // 更新UI视觉反馈
    if (authenticating) {
        // 认证中状态
        ui->fingerprintWidget->setStyleSheet(
            "background-color: rgb(255, 255, 255);"
            "border: 3px solid rgb(82, 196, 26);"
            "border-radius: 20px;"
        );
        
        showDetailedStatus("正在等待指纹输入...", "rgb(82, 196, 26)", "rgb(82, 196, 26)");
    } else {
        // 非认证状态
        ui->fingerprintWidget->setStyleSheet(
            "background-color: rgb(255, 255, 255);"
            "border-radius: 20px;"
        );
        
        showDetailedStatus("等待指纹识别...", "rgb(100, 100, 100)", "rgb(100, 100, 100)");
    }
    
    qDebug() << "[指纹页面] UI状态更新 - 认证中:" << authenticating;
}

void FingerprintPage::connectToServer()
{
    if (isConnected && tcpSocket->state() == QTcpSocket::ConnectedState) {
        return;
    }
    
    if (tcpSocket->state() == QTcpSocket::ConnectingState) {
        return;
    }
    
    if (tcpSocket->state() != QTcpSocket::UnconnectedState) {
        tcpSocket->abort();
    }
    
    qDebug() << "[指纹页面] 连接服务器 192.168.2.220:8888 ...";
    tcpSocket->connectToHost("192.168.2.220", 8888);
}

void FingerprintPage::sendFingerprintVerifyRequest(int userId)
{
    if (!isConnected || tcpSocket->state() != QTcpSocket::ConnectedState) {
        qWarning() << "[指纹页面] 网络未连接，无法发送验证请求";
        handleAuthenticationFailure("网络连接错误");
        return;
    }
    
    isVerifying = true;
    
    // 构建JSON请求 - 需求4.2, 9.1, 9.2
    QJsonObject jsonObj;
    jsonObj["type"] = "fingerprint_verify";
    jsonObj["user_id"] = userId;
    jsonObj["timestamp"] = QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss");
    
    QJsonDocument doc(jsonObj);
    QByteArray jsonData = doc.toJson(QJsonDocument::Compact);
    
    // 发送JSON数据到服务器
    qint64 bytesWritten = tcpSocket->write(jsonData);
    tcpSocket->flush();
    
    qDebug() << "[指纹页面] 发送指纹验证请求:" << jsonData << "字节数:" << bytesWritten;
}

void FingerprintPage::processServerResponse(const QByteArray& data)
{
    qDebug() << "[指纹页面] 收到服务器响应:" << data;
    
    // 重置验证状态
    isVerifying = false;
    
    // 解析JSON响应
    QJsonParseError parseError;
    QJsonDocument doc = QJsonDocument::fromJson(data, &parseError);
    
    if (parseError.error != QJsonParseError::NoError) {
        qWarning() << "[指纹页面] JSON解析错误:" << parseError.errorString();
        handleAuthenticationFailure("服务器响应格式错误");
        return;
    }
    
    QJsonObject obj = doc.object();
    QString type = obj.value("type").toString();
    
    // 检查是否为指纹验证结果
    if (type != "fingerprint_result") {
        qDebug() << "[指纹页面] 非指纹验证响应，忽略";
        return;
    }
    
    bool success = obj.value("success").toBool();
    QString userId = obj.value("user_id").toString();
    QString userName = obj.value("user_name").toString();
    QString message = obj.value("message").toString();
    
    // 重置认证状态
    isAuthenticating = false;
    enableControls(true);
    
    if (success) {
        qDebug() << "[指纹页面] ✓ 指纹验证成功 - 用户:" << userName << "ID:" << userId;
        showDetailedStatus("验证成功", "rgb(82, 196, 26)", "rgb(82, 196, 26)");
        emit verifySuccess(userId, userName);
    } else {
        qDebug() << "[指纹页面] ✗ 指纹验证失败:" << message;
        QString errorMsg = message.isEmpty() ? "指纹验证失败" : message;
        handleAuthenticationFailure(errorMsg);
    }
}

// 网络相关槽函数
void FingerprintPage::onConnected()
{
    qDebug() << "[指纹页面] ✓ 服务器连接成功";
    isConnected = true;
    reconnectTimer->stop();
    updateConnectionStatus(true);
}

void FingerprintPage::onDisconnected()
{
    qDebug() << "[指纹页面] 服务器连接断开";
    isConnected = false;
    updateConnectionStatus(false);
    reconnectTimer->start(5000);
}

void FingerprintPage::onNetworkData()
{
    QByteArray data = tcpSocket->readAll();
    processServerResponse(data);
}

void FingerprintPage::onSocketError(QAbstractSocket::SocketError socketError)
{
    qWarning() << "[指纹页面] 网络错误:" << socketError << tcpSocket->errorString();
    isConnected = false;
    
    // 需求6.3: 添加网络错误的重试机制
    QString userFriendlyError = translateNetworkError(socketError, tcpSocket->errorString());
    
    // 如果正在验证中，显示网络错误
    if (isVerifying) {
        isVerifying = false;
        isAuthenticating = false;
        enableControls(true);
        showDetailedStatus(userFriendlyError, "rgb(255, 77, 79)", "rgb(255, 77, 79)");
        emit verifyFailed(userFriendlyError);
        
        // 启动网络重试机制
        startNetworkRetry();
    }
    
    updateConnectionStatus(false);
}

void FingerprintPage::onReconnectTimer()
{
    if (isConnected && tcpSocket->state() == QTcpSocket::ConnectedState) {
        return;
    }
    connectToServer();
}

void FingerprintPage::enableControls(bool enabled)
{
    // 根据认证状态启用/禁用控件
    // 需求8.1: 认证开始时禁用返回按钮防止中断
    ui->backBtn->setEnabled(enabled);
    ui->startBtn->setEnabled(enabled);
    ui->stopBtn->setEnabled(!enabled && isAuthenticating);
    
    // 更新按钮样式以反映状态
    if (enabled) {
        // 启用状态的样式
        ui->startBtn->setStyleSheet(
            "QPushButton {"
            "    background-color: rgb(82, 196, 26);"
            "    border: none;"
            "    border-radius: 20px;"
            "    font-size: 16px;"
            "    font-weight: bold;"
            "    color: white;"
            "}"
            "QPushButton:pressed {"
            "    background-color: rgb(60, 170, 10);"
            "}"
        );
        
        ui->backBtn->setStyleSheet(
            "QPushButton {"
            "    background-color: rgb(240, 240, 240);"
            "    border: none;"
            "    border-radius: 18px;"
            "    font-size: 14px;"
            "    color: rgb(80, 80, 80);"
            "}"
            "QPushButton:pressed {"
            "    background-color: rgb(220, 220, 220);"
            "}"
        );
    } else {
        // 禁用状态的样式
        ui->startBtn->setStyleSheet(
            "QPushButton {"
            "    background-color: rgb(200, 200, 200);"
            "    border: none;"
            "    border-radius: 20px;"
            "    font-size: 16px;"
            "    font-weight: bold;"
            "    color: rgb(150, 150, 150);"
            "}"
        );
        
        ui->backBtn->setStyleSheet(
            "QPushButton {"
            "    background-color: rgb(200, 200, 200);"
            "    border: none;"
            "    border-radius: 18px;"
            "    font-size: 14px;"
            "    color: rgb(150, 150, 150);"
            "}"
        );
    }
    
    // 停止按钮的样式
    if (isAuthenticating) {
        ui->stopBtn->setStyleSheet(
            "QPushButton {"
            "    background-color: rgb(255, 77, 79);"
            "    border: none;"
            "    border-radius: 20px;"
            "    font-size: 16px;"
            "    font-weight: bold;"
            "    color: white;"
            "}"
            "QPushButton:pressed {"
            "    background-color: rgb(230, 50, 50);"
            "}"
        );
    } else {
        ui->stopBtn->setStyleSheet(
            "QPushButton {"
            "    background-color: rgb(200, 200, 200);"
            "    border: none;"
            "    border-radius: 20px;"
            "    font-size: 16px;"
            "    font-weight: bold;"
            "    color: rgb(150, 150, 150);"
            "}"
        );
    }
    
    qDebug() << "[指纹页面] 控件状态更新 - 启用:" << enabled << "认证中:" << isAuthenticating;
}

// 错误处理相关方法实现

QString FingerprintPage::translateHardwareError(const QString& error)
{
    // 需求6.1: 实现硬件错误的用户友好提示
    if (error.contains("无法打开指纹设备") || error.contains("Permission denied")) {
        return "指纹设备不可用，请检查设备连接";
    } else if (error.contains("Device or resource busy")) {
        return "指纹设备正在被其他程序使用";
    } else if (error.contains("No such file or directory")) {
        return "指纹设备驱动未加载";
    } else if (error.contains("读取指纹数据失败")) {
        return "请重新放置手指";
    } else if (error.contains("发送指纹验证命令失败")) {
        return "指纹设备通信失败，请重试";
    } else if (error.contains("指纹设备未打开")) {
        return "指纹设备未初始化";
    } else {
        // 对于未知错误，提供通用的用户友好消息
        return "指纹识别失败，请重试";
    }
}

QString FingerprintPage::translateNetworkError(QAbstractSocket::SocketError socketError, const QString& errorString)
{
    // 需求6.3: 网络错误的清晰反馈
    switch (socketError) {
        case QAbstractSocket::ConnectionRefusedError:
            return "服务器拒绝连接，请检查服务器状态";
        case QAbstractSocket::RemoteHostClosedError:
            return "服务器连接断开";
        case QAbstractSocket::HostNotFoundError:
            return "无法找到服务器";
        case QAbstractSocket::SocketTimeoutError:
            return "连接超时，请检查网络";
        case QAbstractSocket::NetworkError:
            return "网络错误，请检查网络连接";
        case QAbstractSocket::SocketAccessError:
            return "网络权限错误";
        case QAbstractSocket::SocketResourceError:
            return "网络资源不足";
        default:
            return QString("网络连接失败: %1").arg(errorString);
    }
}

void FingerprintPage::startNetworkRetry()
{
    // 需求6.3: 添加网络错误的重试机制
    if (networkRetryCount >= MAX_NETWORK_RETRIES) {
        qWarning() << "[指纹页面] 网络重试次数已达上限，停止重试";
        networkRetryCount = 0;
        showDetailedStatus("网络连接失败，请检查网络后重试", "rgb(255, 77, 79)", "rgb(255, 77, 79)");
        return;
    }
    
    networkRetryCount++;
    qDebug() << "[指纹页面] 启动网络重试，第" << networkRetryCount << "次";
    
    showDetailedStatus(QString("网络重连中... (%1/%2)").arg(networkRetryCount).arg(MAX_NETWORK_RETRIES), 
                      "rgb(255, 152, 0)", "rgb(255, 152, 0)");
    
    // 延迟重试
    QTimer::singleShot(NETWORK_RETRY_INTERVAL, this, [this]() {
        connectToServer();
        
        // 检查连接结果
        QTimer::singleShot(3000, this, [this]() {
            if (!isConnected) {
                startNetworkRetry(); // 递归重试
            } else {
                networkRetryCount = 0; // 重置重试计数
                showDetailedStatus("网络重连成功", "rgb(82, 196, 26)", "rgb(82, 196, 26)");
            }
        });
    });
}

void FingerprintPage::handleAuthenticationFailure(const QString& reason)
{
    // 需求6.2: 实现认证失败的清晰反馈
    qDebug() << "[指纹页面] 认证失败:" << reason;
    
    QString userMessage;
    if (reason.contains("指纹未注册") || reason.contains("不匹配")) {
        userMessage = "指纹未注册或不匹配，请重试或联系管理员";
    } else if (reason.contains("网络")) {
        userMessage = "网络连接失败，请检查网络后重试";
    } else if (reason.contains("超时")) {
        userMessage = "认证超时，请重新放置手指";
    } else if (reason.contains("服务器")) {
        userMessage = "服务器验证失败，请稍后重试";
    } else {
        userMessage = QString("认证失败: %1").arg(reason);
    }
    
    showDetailedStatus(userMessage, "rgb(255, 77, 79)", "rgb(255, 77, 79)");
    
    // 重置认证状态
    isAuthenticating = false;
    enableControls(true);
    
    emit verifyFailed(userMessage);
}

// 资源清理相关方法实现

void FingerprintPage::performResourceCleanup()
{
    qDebug() << "[指纹页面] 开始执行资源清理";
    
    try {
        // 需求8.5: 页面切换时的状态重置
        resetPageState();
        
        // 需求2.5: 确保设备文件描述符正确关闭
        cleanupHardwareResources();
        
        // 注意：不清理网络资源，保持网络连接持久化
        // 网络连接应该在整个应用程序生命周期内保持
        // cleanupNetworkResources(); // 已移除
        
        qDebug() << "[指纹页面] 资源清理完成（保持网络连接）";
    } catch (...) {
        qWarning() << "[指纹页面] 资源清理过程中发生异常";
        // 需求2.5: 异常情况下的资源清理 - 继续执行其他清理操作
    }
}

void FingerprintPage::cleanupNetworkResources()
{
    qDebug() << "[指纹页面] 清理网络资源";
    
    try {
        // 停止网络相关定时器
        if (reconnectTimer && reconnectTimer->isActive()) {
            reconnectTimer->stop();
            qDebug() << "[指纹页面] 重连定时器已停止";
        }
        
        if (networkRetryTimer && networkRetryTimer->isActive()) {
            networkRetryTimer->stop();
            qDebug() << "[指纹页面] 网络重试定时器已停止";
        }
        
        // 断开网络连接
        if (tcpSocket) {
            if (tcpSocket->state() != QTcpSocket::UnconnectedState) {
                tcpSocket->disconnectFromHost();
                // 等待断开连接，但不超过1秒
                if (tcpSocket->state() != QTcpSocket::UnconnectedState) {
                    tcpSocket->waitForDisconnected(1000);
                }
                qDebug() << "[指纹页面] TCP连接已断开";
            }
        }
        
        // 重置网络状态
        isConnected = false;
        isVerifying = false;
        networkRetryCount = 0;
        
    } catch (...) {
        qWarning() << "[指纹页面] 网络资源清理异常";
    }
}

void FingerprintPage::cleanupHardwareResources()
{
    qDebug() << "[指纹页面] 清理硬件资源";
    
    try {
        // 停止认证超时定时器
        if (authTimer && authTimer->isActive()) {
            authTimer->stop();
            qDebug() << "[指纹页面] 认证超时定时器已停止";
        }
        
        // 停止硬件操作
        if (hardwareController) {
            // 停止指纹验证
            hardwareController->stopFingerprintVerification();
            
            // 清理硬件控制器资源
            hardwareController->cleanup();
            
            qDebug() << "[指纹页面] 硬件控制器资源已清理";
        }
        
    } catch (...) {
        qWarning() << "[指纹页面] 硬件资源清理异常";
    }
}

void FingerprintPage::resetPageState()
{
    qDebug() << "[指纹页面] 重置页面状态";
    
    try {
        // 需求8.5: 返回首页时重置所有认证状态变量
        isAuthenticating = false;
        currentUserId = 0;
        
        // 重置UI状态
        if (ui) {
            // 重置状态显示
            showDetailedStatus("等待指纹识别...", "rgb(100, 100, 100)", "rgb(100, 100, 100)");
            
            // 重置控件状态
            enableControls(true);
            
            // 重置连接状态显示
            updateConnectionStatus(isConnected);
        }
        
        qDebug() << "[指纹页面] 页面状态重置完成";
        
    } catch (...) {
        qWarning() << "[指纹页面] 页面状态重置异常";
    }
}
