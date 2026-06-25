#include "registrationkeypage.h"
#include "ui_registrationkeypage.h"
#include <QDebug>
#include <QPalette>
#include <QTimer>

RegistrationKeyPage::RegistrationKeyPage(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::RegistrationKeyPage)
    , m_timeoutTimer(new QTimer(this))
    , m_retryCount(0)
{
    ui->setupUi(this);
    
    // 设置固定大小和背景
    setFixedSize(1024, 600);
    setAutoFillBackground(true);
    
    QPalette pal = palette();
    pal.setColor(QPalette::Window, QColor(245, 247, 250));
    setPalette(pal);

    // 设置超时定时器
    m_timeoutTimer->setSingleShot(true);
    connect(m_timeoutTimer, &QTimer::timeout, this, &RegistrationKeyPage::onVerificationTimeout);

    // 连接信号槽
    connect(ui->verifyBtn, &QPushButton::clicked, this, &RegistrationKeyPage::onVerifyClicked);
    connect(ui->backBtn, &QPushButton::clicked, this, &RegistrationKeyPage::onBackClicked);

    // 连接数字键盘按钮
    connect(ui->btn0, &QPushButton::clicked, this, [this]() { onNumberButtonClicked("0"); });
    connect(ui->btn1, &QPushButton::clicked, this, [this]() { onNumberButtonClicked("1"); });
    connect(ui->btn2, &QPushButton::clicked, this, [this]() { onNumberButtonClicked("2"); });
    connect(ui->btn3, &QPushButton::clicked, this, [this]() { onNumberButtonClicked("3"); });
    connect(ui->btn4, &QPushButton::clicked, this, [this]() { onNumberButtonClicked("4"); });
    connect(ui->btn5, &QPushButton::clicked, this, [this]() { onNumberButtonClicked("5"); });
    connect(ui->btn6, &QPushButton::clicked, this, [this]() { onNumberButtonClicked("6"); });
    connect(ui->btn7, &QPushButton::clicked, this, [this]() { onNumberButtonClicked("7"); });
    connect(ui->btn8, &QPushButton::clicked, this, [this]() { onNumberButtonClicked("8"); });
    connect(ui->btn9, &QPushButton::clicked, this, [this]() { onNumberButtonClicked("9"); });
    connect(ui->btnDelete, &QPushButton::clicked, this, &RegistrationKeyPage::onDeleteButtonClicked);
    connect(ui->btnConfirm, &QPushButton::clicked, this, &RegistrationKeyPage::onVerifyClicked);

    // 初始状态下验证按钮和确认按钮都不可用
    setVerifyButtonEnabled(false);
    ui->btnConfirm->setEnabled(false);
    
    // 初始化密钥显示
    updateKeyDisplay();

    qDebug() << "[注册密钥页面] 注册密钥页面初始化完成";
}

RegistrationKeyPage::~RegistrationKeyPage()
{
    delete ui;
}

void RegistrationKeyPage::clearInput()
{
    m_currentKey.clear();
    updateKeyDisplay();
    ui->statusLabel->clear();
    m_retryCount = 0;
    m_lastKey.clear();
    m_timeoutTimer->stop();
    setVerifyButtonEnabled(false);
    ui->btnConfirm->setEnabled(false);
    hideLoadingState();
}

void RegistrationKeyPage::onNumberButtonClicked(const QString& number)
{
    if (m_currentKey.length() < MAX_KEY_LENGTH) {
        m_currentKey += number;
        updateKeyDisplay();
        qDebug() << "[注册密钥页面] 输入数字:" << number << "当前长度:" << m_currentKey.length();
        
        // 检查是否达到8位
        if (m_currentKey.length() == MAX_KEY_LENGTH) {
            setVerifyButtonEnabled(true);
            ui->btnConfirm->setEnabled(true);
        }
    }
}

void RegistrationKeyPage::onDeleteButtonClicked()
{
    if (!m_currentKey.isEmpty()) {
        m_currentKey.chop(1);
        updateKeyDisplay();
        qDebug() << "[注册密钥页面] 删除字符，当前长度:" << m_currentKey.length();
        
        // 更新验证按钮和确认按钮状态
        bool isValidLength = (m_currentKey.length() == MAX_KEY_LENGTH);
        setVerifyButtonEnabled(isValidLength);
        ui->btnConfirm->setEnabled(isValidLength);
    }
}

void RegistrationKeyPage::updateKeyDisplay()
{
    // 显示当前输入的密钥
    ui->keyDisplay->setText(m_currentKey);
    
    // 清除状态信息
    if (!m_currentKey.isEmpty()) {
        ui->statusLabel->clear();
        m_retryCount = 0;
        m_timeoutTimer->stop();
        hideLoadingState();
    }
}

void RegistrationKeyPage::onVerifyClicked()
{
    QString key = m_currentKey.trimmed();
    if (key.isEmpty()) {
        ui->statusLabel->setText("请输入注册密钥");
        ui->statusLabel->setStyleSheet("color: rgb(255, 77, 79);");
        return;
    }
    
    // 检查长度
    if (key.length() < MAX_KEY_LENGTH) {
        ui->statusLabel->setText(QString("密钥长度不足，请输入%1位密钥").arg(MAX_KEY_LENGTH));
        ui->statusLabel->setStyleSheet("color: rgb(255, 77, 79);");
        return;
    }
    
    // 检查是否为纯数字
    bool isNumeric = true;
    for (const QChar& ch : key) {
        if (!ch.isDigit()) {
            isNumeric = false;
            break;
        }
    }
    
    if (!isNumeric) {
        ui->statusLabel->setText("密钥必须是8位纯数字");
        ui->statusLabel->setStyleSheet("color: rgb(255, 77, 79);");
        return;
    }

    qDebug() << "[注册密钥页面] 验证注册密钥:" << key << "重试次数:" << m_retryCount;
    
    m_lastKey = key;
    showLoadingState();
    
    // 启动超时定时器
    m_timeoutTimer->start(VERIFICATION_TIMEOUT);
    
    // 发送密钥验证信号
    emit keyVerified(key);
}

void RegistrationKeyPage::onRetryClicked()
{
    if (!m_lastKey.isEmpty() && m_retryCount < MAX_RETRY_COUNT) {
        m_retryCount++;
        qDebug() << "[注册密钥页面] 重试验证密钥:" << m_lastKey << "重试次数:" << m_retryCount;
        
        showLoadingState();
        m_timeoutTimer->start(VERIFICATION_TIMEOUT);
        emit keyVerified(m_lastKey);
    }
}

void RegistrationKeyPage::onVerificationTimeout()
{
    qDebug() << "[注册密钥页面] 验证超时";
    hideLoadingState();
    
    if (m_retryCount < MAX_RETRY_COUNT) {
        ui->statusLabel->setText(QString("验证超时，是否重试？(剩余 %1 次)").arg(MAX_RETRY_COUNT - m_retryCount));
        ui->statusLabel->setStyleSheet("color: rgb(255, 193, 7);");
        enableRetry();
    } else {
        ui->statusLabel->setText("验证失败次数过多，请检查网络连接或联系管理员");
        ui->statusLabel->setStyleSheet("color: rgb(255, 77, 79);");
        ui->verifyBtn->setEnabled(true);
        ui->verifyBtn->setText("重新验证");
        m_retryCount = 0;
    }
}

void RegistrationKeyPage::onBackClicked()
{
    qDebug() << "[注册密钥页面] 返回首页";
    clearInput();
    emit backToHomeClicked();
}

void RegistrationKeyPage::setVerifyButtonEnabled(bool enabled)
{
    ui->verifyBtn->setEnabled(enabled);
    if (enabled) {
        ui->verifyBtn->setStyleSheet(
            "QPushButton {"
            "   background-color: rgb(24, 144, 255);"
            "   color: white;"
            "   border: none;"
            "   border-radius: 8px;"
            "   font-size: 18px;"
            "   font-weight: bold;"
            "}"
            "QPushButton:pressed {"
            "   background-color: rgb(16, 120, 220);"
            "}"
        );
    } else {
        ui->verifyBtn->setStyleSheet(
            "QPushButton {"
            "   background-color: rgb(200, 200, 200);"
            "   color: rgb(150, 150, 150);"
            "   border: none;"
            "   border-radius: 8px;"
            "   font-size: 18px;"
            "   font-weight: bold;"
            "}"
        );
    }
}

void RegistrationKeyPage::showLoadingState()
{
    ui->verifyBtn->setEnabled(false);
    ui->verifyBtn->setText("验证中...");
    ui->statusLabel->setText("正在验证密钥，请稍候...");
    ui->statusLabel->setStyleSheet("color: rgb(24, 144, 255);");
}

void RegistrationKeyPage::hideLoadingState()
{
    ui->verifyBtn->setText("验证密钥");
    bool isValidLength = (m_currentKey.length() == MAX_KEY_LENGTH);
    setVerifyButtonEnabled(isValidLength);
    ui->btnConfirm->setEnabled(isValidLength);
}

void RegistrationKeyPage::enableRetry()
{
    ui->verifyBtn->setEnabled(true);
    ui->verifyBtn->setText("重试");
    ui->verifyBtn->setStyleSheet(
        "QPushButton {"
        "   background-color: rgb(255, 193, 7);"
        "   color: white;"
        "   border: none;"
        "   border-radius: 8px;"
        "   font-size: 18px;"
        "   font-weight: bold;"
        "}"
        "QPushButton:pressed {"
        "   background-color: rgb(230, 170, 0);"
        "}"
    );
    
    // 重新连接到重试槽函数
    disconnect(ui->verifyBtn, &QPushButton::clicked, this, &RegistrationKeyPage::onVerifyClicked);
    connect(ui->verifyBtn, &QPushButton::clicked, this, &RegistrationKeyPage::onRetryClicked);
}

void RegistrationKeyPage::showVerificationStatus(bool success, const QString& message)
{
    qDebug() << "[注册密钥页面] 显示验证状态:" << success << message;
    
    m_timeoutTimer->stop();
    hideLoadingState();
    
    if (success) {
        ui->statusLabel->setText("密钥验证成功！正在跳转...");
        ui->statusLabel->setStyleSheet("color: rgb(82, 196, 26);");
        m_retryCount = 0;
    } else {
        if (m_retryCount < MAX_RETRY_COUNT) {
            ui->statusLabel->setText(QString("%1 是否重试？(剩余 %2 次)").arg(message.isEmpty() ? "密钥验证失败" : message).arg(MAX_RETRY_COUNT - m_retryCount));
            ui->statusLabel->setStyleSheet("color: rgb(255, 77, 79);");
            enableRetry();
        } else {
            ui->statusLabel->setText("验证失败次数过多，请检查密钥或联系管理员");
            ui->statusLabel->setStyleSheet("color: rgb(255, 77, 79);");
            ui->verifyBtn->setEnabled(true);
            ui->verifyBtn->setText("重新验证");
            m_retryCount = 0;
            
            // 重新连接到验证槽函数
            disconnect(ui->verifyBtn, &QPushButton::clicked, this, &RegistrationKeyPage::onRetryClicked);
            connect(ui->verifyBtn, &QPushButton::clicked, this, &RegistrationKeyPage::onVerifyClicked);
        }
    }
}
