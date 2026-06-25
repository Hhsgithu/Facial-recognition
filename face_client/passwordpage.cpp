#include "passwordpage.h"
#include "ui_passwordpage.h"
#include <QDebug>
#include <QPushButton>
#include <QPalette>
#include <QJsonDocument>
#include <QJsonObject>
#include <QHostAddress>

PasswordPage::PasswordPage(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::PasswordPage)
    , inputPassword("")
    , m_socket(nullptr)
    , m_reconnectTimer(nullptr)
    , is_connected(false)
    , is_verifying(false)
{
    ui->setupUi(this);
    
    // 设置固定大小和不透明背景
    setFixedSize(1024, 600);
    setAutoFillBackground(true);
    setAttribute(Qt::WA_OpaquePaintEvent, true);
    
    QPalette pal = palette();
    pal.setColor(QPalette::Window, QColor(245, 247, 250));
    setPalette(pal);

    // 初始化网络组件
    m_socket = new QTcpSocket(this);
    m_reconnectTimer = new QTimer(this);
    
    // 连接网络信号槽
    connect(m_socket, &QTcpSocket::connected, this, &PasswordPage::onConnected);
    connect(m_socket, &QTcpSocket::disconnected, this, &PasswordPage::onDisconnected);
    connect(m_socket, &QTcpSocket::readyRead, this, &PasswordPage::recv_data);
    connect(m_socket, QOverload<QAbstractSocket::SocketError>::of(&QAbstractSocket::error),
            this, &PasswordPage::onSocketError);
    
    // 连接重连定时器
    connect(m_reconnectTimer, &QTimer::timeout, this, &PasswordPage::timer_connect);
    m_reconnectTimer->start(5000);

    // 连接数字按钮
    connect(ui->btn0, &QPushButton::clicked, this, &PasswordPage::onNumberButtonClicked);
    connect(ui->btn1, &QPushButton::clicked, this, &PasswordPage::onNumberButtonClicked);
    connect(ui->btn2, &QPushButton::clicked, this, &PasswordPage::onNumberButtonClicked);
    connect(ui->btn3, &QPushButton::clicked, this, &PasswordPage::onNumberButtonClicked);
    connect(ui->btn4, &QPushButton::clicked, this, &PasswordPage::onNumberButtonClicked);
    connect(ui->btn5, &QPushButton::clicked, this, &PasswordPage::onNumberButtonClicked);
    connect(ui->btn6, &QPushButton::clicked, this, &PasswordPage::onNumberButtonClicked);
    connect(ui->btn7, &QPushButton::clicked, this, &PasswordPage::onNumberButtonClicked);
    connect(ui->btn8, &QPushButton::clicked, this, &PasswordPage::onNumberButtonClicked);
    connect(ui->btn9, &QPushButton::clicked, this, &PasswordPage::onNumberButtonClicked);

    // 连接功能按钮
    connect(ui->btnDelete, &QPushButton::clicked, this, &PasswordPage::onDeleteButtonClicked);
    connect(ui->btnConfirm, &QPushButton::clicked, this, &PasswordPage::onConfirmButtonClicked);
    connect(ui->backBtn, &QPushButton::clicked, this, &PasswordPage::onBackButtonClicked);

    // 初始化显示
    updatePasswordDisplay();
    updateConnectionStatus(false);
    clearStatus();

    qDebug() << "[密码页面] 密码解锁页面初始化完成";
}

PasswordPage::~PasswordPage()
{
    delete ui;
}

void PasswordPage::onNumberButtonClicked()
{
    QPushButton* button = qobject_cast<QPushButton*>(sender());
    if (button) {
        QString number = button->text();
        if (inputPassword.length() < 10) {  // 限制最大长度
            inputPassword += number;
            updatePasswordDisplay();
            qDebug() << "[密码页面] 输入数字:" << number << "当前密码长度:" << inputPassword.length();
        }
    }
}

void PasswordPage::onDeleteButtonClicked()
{
    if (!inputPassword.isEmpty()) {
        inputPassword.chop(1);
        updatePasswordDisplay();
        qDebug() << "[密码页面] 删除一位，当前密码长度:" << inputPassword.length();
    }
}

void PasswordPage::onConfirmButtonClicked()
{
    qDebug() << "[密码页面] 确认按钮点击，密码长度:" << inputPassword.length();
    
    // 检查是否正在验证中
    if (is_verifying) {
        qDebug() << "[密码页面] 正在验证中，请稍候...";
        return;
    }
    
    // 检查密码是否为空
    if (inputPassword.isEmpty()) {
        qDebug() << "[密码页面] 密码为空，不发送验证请求";
        showStatus("请输入密码", "rgb(255, 152, 0)");
        return;
    }
    
    // 检查网络连接状态
    if (!is_connected || m_socket->state() != QTcpSocket::ConnectedState) {
        qDebug() << "[密码页面] 未连接服务器，尝试重新连接...";
        showStatus("网络未连接，正在重连...", "rgb(255, 152, 0)");
        connectToServer();
        return;
    }
    
    // 设置验证中状态
    is_verifying = true;
    showStatus("正在验证...", "rgb(24, 144, 255)");
    ui->btnConfirm->setEnabled(false);
    
    // 构建JSON请求
    QJsonObject jsonObj;
    jsonObj["type"] = "password_verify";
    jsonObj["password"] = inputPassword;
    
    QJsonDocument doc(jsonObj);
    QByteArray jsonData = doc.toJson(QJsonDocument::Compact);
    
    // 发送JSON数据到服务器
    qint64 bytesWritten = m_socket->write(jsonData);
    m_socket->flush();
    
    qDebug() << "[密码页面] 发送密码验证请求:" << jsonData << "字节数:" << bytesWritten;
}

void PasswordPage::onBackButtonClicked()
{
    qDebug() << "[密码页面] 返回按钮点击";
    resetPassword();
    clearStatus();
    is_verifying = false;
    emit backToHomeClicked();
}

void PasswordPage::updatePasswordDisplay()
{
    // 显示为星号，但保留实际密码用于验证
    QString displayText = QString(inputPassword.length(), '*');
    ui->passwordDisplay->setText(displayText);
}

void PasswordPage::resetPassword()
{
    inputPassword.clear();
    updatePasswordDisplay();
}

void PasswordPage::connectToServer()
{
    if (is_connected && m_socket->state() == QTcpSocket::ConnectedState) {
        return;
    }
    
    if (m_socket->state() == QTcpSocket::ConnectingState) {
        return;
    }
    
    if (m_socket->state() != QTcpSocket::UnconnectedState) {
        m_socket->abort();
    }
    
    qDebug() << "[密码页面] 连接服务器 192.168.2.220:8888 ...";
    m_socket->connectToHost("192.168.2.220", 8888);
}

void PasswordPage::timer_connect()
{
    if (is_connected && m_socket->state() == QTcpSocket::ConnectedState) {
        return;
    }
    connectToServer();
}

void PasswordPage::onConnected()
{
    qDebug() << "[密码页面] ✓ 服务器连接成功";
    is_connected = true;
    m_reconnectTimer->stop();
    updateConnectionStatus(true);
}

void PasswordPage::onDisconnected()
{
    qDebug() << "[密码页面] 服务器连接断开";
    is_connected = false;
    updateConnectionStatus(false);
    m_reconnectTimer->start(5000);
}

void PasswordPage::onSocketError(QAbstractSocket::SocketError socketError)
{
    qDebug() << "[密码页面] 网络错误:" << socketError << m_socket->errorString();
    is_connected = false;
    
    // 如果正在验证中，显示网络错误
    if (is_verifying) {
        is_verifying = false;
        showStatus("网络错误，请重试", "rgb(245, 34, 45)");
    }
    
    updateConnectionStatus(false);
}

void PasswordPage::updateConnectionStatus(bool connected)
{
    qDebug() << "[密码页面] 连接状态:" << (connected ? "已连接" : "未连接");
    
    // 更新连接状态标签
    if (connected) {
        ui->connectionLabel->setText("网络状态: 已连接");
        ui->connectionLabel->setStyleSheet("font-size: 12px; color: rgb(82, 196, 26); background: transparent;");
        // 连接成功时启用确认按钮（如果不在验证中）
        if (!is_verifying) {
            ui->btnConfirm->setEnabled(true);
            ui->btnConfirm->setStyleSheet(
                "QPushButton {"
                "    background-color: rgb(82, 196, 26);"
                "    border: none;"
                "    border-radius: 12px;"
                "    font-size: 18px;"
                "    font-weight: bold;"
                "    color: white;"
                "}"
                "QPushButton:pressed {"
                "    background-color: rgb(60, 170, 10);"
                "}"
            );
        }
    } else {
        ui->connectionLabel->setText("网络状态: 未连接");
        ui->connectionLabel->setStyleSheet("font-size: 12px; color: rgb(245, 34, 45); background: transparent;");
        // 未连接时禁用确认按钮
        ui->btnConfirm->setEnabled(false);
        ui->btnConfirm->setStyleSheet(
            "QPushButton {"
            "    background-color: rgb(200, 200, 200);"
            "    border: none;"
            "    border-radius: 12px;"
            "    font-size: 18px;"
            "    font-weight: bold;"
            "    color: white;"
            "}"
        );
    }
}

void PasswordPage::showStatus(const QString& message, const QString& color)
{
    ui->statusLabel->setText(message);
    ui->statusLabel->setStyleSheet(QString("font-size: 16px; color: %1; background: transparent;").arg(color));
}

void PasswordPage::clearStatus()
{
    ui->statusLabel->setText("");
}

void PasswordPage::recv_data()
{
    qDebug() << "[密码页面] 收到服务器响应";
    
    QByteArray data = m_socket->readAll();
    qDebug() << "[密码页面] 响应数据:" << data;
    
    // 重置验证状态
    is_verifying = false;
    ui->btnConfirm->setEnabled(is_connected);
    
    // 解析JSON响应
    QJsonParseError parseError;
    QJsonDocument doc = QJsonDocument::fromJson(data, &parseError);
    
    if (parseError.error != QJsonParseError::NoError) {
        qDebug() << "[密码页面] JSON解析错误:" << parseError.errorString();
        showStatus("响应解析错误", "rgb(245, 34, 45)");
        return;
    }
    
    QJsonObject obj = doc.object();
    QString type = obj.value("type").toString();
    
    // 检查是否为密码验证结果
    if (type != "password_result") {
        qDebug() << "[密码页面] 非密码验证响应，忽略";
        return;
    }
    
    bool success = obj.value("success").toBool();
    QString userId = obj.value("user_id").toString();
    QString userName = obj.value("user_name").toString();
    QString message = obj.value("message").toString();
    
    if (success) {
        qDebug() << "[密码页面] ✓ 密码验证成功 - 用户:" << userName << "ID:" << userId;
        showStatus("验证成功", "rgb(82, 196, 26)");
        resetPassword();
        emit verifySuccess(userId, userName);
    } else {
        qDebug() << "[密码页面] ✗ 密码验证失败:" << message;
        // 显示错误提示
        QString errorMsg = message.isEmpty() ? "密码错误，请重新输入" : message;
        showStatus(errorMsg, "rgb(245, 34, 45)");
        resetPassword();
    }
}

