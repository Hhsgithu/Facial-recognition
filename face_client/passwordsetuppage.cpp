#include "passwordsetuppage.h"
#include "ui_passwordsetuppage.h"
#include <QDebug>
#include <QTimer>
#include <QEvent>
#include <QMouseEvent>

PasswordSetupPage::PasswordSetupPage(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::PasswordSetupPage)
    , m_isFirstInput(true)
{
    ui->setupUi(this);
    
    // 设置固定大小和背景
    setFixedSize(1024, 600);
    setAutoFillBackground(true);
    
    QPalette pal = palette();
    pal.setColor(QPalette::Window, QColor(245, 247, 250));
    setPalette(pal);

    // 连接信号槽
    connect(ui->backBtn, &QPushButton::clicked, this, &PasswordSetupPage::onBackClicked);
    connect(ui->confirmBtn, &QPushButton::clicked, this, &PasswordSetupPage::onConfirmPassword);
    
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
    connect(ui->btnDelete, &QPushButton::clicked, this, &PasswordSetupPage::onDeleteButtonClicked);
    
    // 初始化密码显示
    updatePasswordDisplay();
    
    // 为密码显示框安装事件过滤器，使其可以响应点击
    ui->passwordDisplay->installEventFilter(this);
    ui->confirmPasswordDisplay->installEventFilter(this);

    qDebug() << "[密码设置页面] 密码设置页面初始化完成";
}

PasswordSetupPage::~PasswordSetupPage()
{
    delete ui;
}

void PasswordSetupPage::setUserInfo(const UserInfo& userInfo)
{
    m_userInfo = userInfo;
    ui->userNameLabel->setText(QString("用户：%1").arg(userInfo.name));
    qDebug() << "[密码设置页面] 设置用户信息:" << userInfo.name;
}

void PasswordSetupPage::startSetup()
{
    qDebug() << "[密码设置页面] 开始设置流程";
    m_password.clear();
    m_confirmPassword.clear();
    m_isFirstInput = true;
    updatePasswordDisplay();
    updateStatus("", "rgb(100, 100, 100)");
    ui->confirmBtn->setEnabled(true);
}

bool PasswordSetupPage::eventFilter(QObject* obj, QEvent* event)
{
    // 处理密码显示框的点击事件
    if (event->type() == QEvent::MouseButtonPress) {
        if (obj == ui->passwordDisplay) {
            // 点击第一个密码框，切换到输入第一个密码
            if (!m_isFirstInput) {
                m_isFirstInput = true;
                updatePasswordDisplay();
                updateStatus("", "rgb(100, 100, 100)");
                qDebug() << "[密码设置页面] 点击密码框，切换到输入密码";
            }
            return true;
        } else if (obj == ui->confirmPasswordDisplay) {
            // 点击确认密码框，切换到输入确认密码
            if (m_isFirstInput && m_password.length() == 6) {
                m_isFirstInput = false;
                updatePasswordDisplay();
                updateStatus("请再次输入密码", "rgb(24, 144, 255)");
                qDebug() << "[密码设置页面] 点击确认密码框，切换到输入确认密码";
            } else if (m_isFirstInput && m_password.length() < 6) {
                updateStatus("请先完成密码输入", "rgb(255, 193, 7)");
                qDebug() << "[密码设置页面] 密码未输入完成，无法切换到确认密码";
            }
            return true;
        }
    }
    
    return QWidget::eventFilter(obj, event);
}

void PasswordSetupPage::onNumberButtonClicked(const QString& number)
{
    if (m_isFirstInput) {
        // 输入第一个密码
        if (m_password.length() < 6) {
            m_password += number;
            updatePasswordDisplay();
            qDebug() << "[密码设置页面] 输入密码数字:" << number << "当前长度:" << m_password.length();
            
            // 如果第一个密码输入完成，自动切换到确认密码
            if (m_password.length() == 6) {
                m_isFirstInput = false;
                updatePasswordDisplay();
                updateStatus("请再次输入密码", "rgb(24, 144, 255)");
            }
        }
    } else {
        // 输入确认密码
        if (m_confirmPassword.length() < 6) {
            m_confirmPassword += number;
            updatePasswordDisplay();
            qDebug() << "[密码设置页面] 输入确认密码数字:" << number << "当前长度:" << m_confirmPassword.length();
        }
    }
}

void PasswordSetupPage::onDeleteButtonClicked()
{
    if (m_isFirstInput) {
        // 删除第一个密码的字符
        if (!m_password.isEmpty()) {
            m_password.chop(1);
            updatePasswordDisplay();
            qDebug() << "[密码设置页面] 删除密码字符，当前长度:" << m_password.length();
        }
    } else {
        // 删除确认密码的字符
        if (!m_confirmPassword.isEmpty()) {
            m_confirmPassword.chop(1);
            updatePasswordDisplay();
            qDebug() << "[密码设置页面] 删除确认密码字符，当前长度:" << m_confirmPassword.length();
        } else if (m_password.length() == 6) {
            // 如果确认密码为空，切换回第一个密码
            m_isFirstInput = true;
            updatePasswordDisplay();
            updateStatus("", "rgb(100, 100, 100)");
        }
    }
}

void PasswordSetupPage::updatePasswordDisplay()
{
    // 更新第一个密码显示
    QString passwordDisplay;
    for (int i = 0; i < m_password.length(); i++) {
        passwordDisplay += "● ";
    }
    ui->passwordDisplay->setText(passwordDisplay.trimmed());
    
    // 更新确认密码显示
    QString confirmPasswordDisplay;
    for (int i = 0; i < m_confirmPassword.length(); i++) {
        confirmPasswordDisplay += "● ";
    }
    ui->confirmPasswordDisplay->setText(confirmPasswordDisplay.trimmed());
    
    // 更新输入框边框颜色，显示当前焦点
    if (m_isFirstInput) {
        ui->passwordDisplay->setStyleSheet(
            "font-size: 32px;"
            "font-weight: bold;"
            "color: rgb(50, 50, 50);"
            "background-color: rgb(250, 250, 250);"
            "border: 2px solid rgb(24, 144, 255);"
            "border-radius: 8px;"
        );
        ui->confirmPasswordDisplay->setStyleSheet(
            "font-size: 32px;"
            "font-weight: bold;"
            "color: rgb(50, 50, 50);"
            "background-color: rgb(250, 250, 250);"
            "border: 1px solid rgb(230, 230, 230);"
            "border-radius: 8px;"
        );
    } else {
        ui->passwordDisplay->setStyleSheet(
            "font-size: 32px;"
            "font-weight: bold;"
            "color: rgb(50, 50, 50);"
            "background-color: rgb(250, 250, 250);"
            "border: 1px solid rgb(230, 230, 230);"
            "border-radius: 8px;"
        );
        ui->confirmPasswordDisplay->setStyleSheet(
            "font-size: 32px;"
            "font-weight: bold;"
            "color: rgb(50, 50, 50);"
            "background-color: rgb(250, 250, 250);"
            "border: 2px solid rgb(24, 144, 255);"
            "border-radius: 8px;"
        );
    }
}

void PasswordSetupPage::onConfirmPassword()
{
    qDebug() << "[密码设置页面] 用户确认密码";

    if (m_password.isEmpty() || m_confirmPassword.isEmpty()) {
        updateStatus("请输入6位数字密码", "rgb(255, 77, 79)");
        return;
    }

    if (m_password != m_confirmPassword) {
        updateStatus("两次输入的密码不一致", "rgb(255, 77, 79)");
        return;
    }

    if (!validatePassword(m_password)) {
        updateStatus("密码必须是6位数字", "rgb(255, 77, 79)");
        return;
    }

    updateStatus("密码设置成功！", "rgb(82, 196, 26)");
    
    ui->confirmBtn->setEnabled(false);
    
    // 短暂延迟后发送完成信号
    QTimer::singleShot(1000, this, [this]() {
        emit passwordSetupCompleted(m_password);
    });
}

void PasswordSetupPage::onBackClicked()
{
    qDebug() << "[密码设置页面] 用户点击返回";
    emit backClicked();
}

bool PasswordSetupPage::validatePassword(const QString& password) const
{
    qDebug() << "[密码设置页面] 验证密码:" << password << "长度:" << password.length();
    
    // 密码必须是6位数字
    if (password.length() != 6) {
        qDebug() << "[密码设置页面] 密码验证失败：长度不是6位";
        return false;
    }

    // 只能包含数字
    for (const QChar& ch : password) {
        if (!ch.isDigit()) {
            qDebug() << "[密码设置页面] 密码验证失败：包含非数字字符" << ch;
            return false;
        }
    }

    qDebug() << "[密码设置页面] 密码验证成功";
    return true;
}

void PasswordSetupPage::updateStatus(const QString& message, const QString& color)
{
    ui->statusLabel->setText(message);
    ui->statusLabel->setStyleSheet(QString("color: %1; font-size: 14px; background: transparent;").arg(color));
}
