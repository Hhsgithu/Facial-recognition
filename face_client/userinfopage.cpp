#include "userinfopage.h"
#include "ui_userinfopage.h"
#include <QDebug>
#include <QPalette>
#include <QRegularExpression>

UserInfoPage::UserInfoPage(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::UserInfoPage)
{
    ui->setupUi(this);

    // 设置固定大小和背景
    setFixedSize(1024, 600);
    setAutoFillBackground(true);
    
    QPalette pal = palette();
    pal.setColor(QPalette::Window, QColor(245, 247, 250));
    setPalette(pal);

    // 设置输入框为可编辑
    ui->nameLineEdit->setReadOnly(false);
    ui->phoneLineEdit->setReadOnly(false);
    ui->addressLineEdit->setReadOnly(false);

    // 为不同输入框设置合适的 inputMethodHints
    ui->nameLineEdit->setInputMethodHints(Qt::ImhNone);  // 姓名支持中文拼音
    ui->phoneLineEdit->setInputMethodHints(Qt::ImhFormattedNumbersOnly);  // 电话号码仅数字
    ui->addressLineEdit->setInputMethodHints(Qt::ImhNone);  // 地址支持中文拼音

    // 连接信号槽
    connect(ui->nextBtn, &QPushButton::clicked, this, &UserInfoPage::onNextClicked);
    connect(ui->backBtn, &QPushButton::clicked, this, &UserInfoPage::onBackClicked);
    connect(ui->nameLineEdit, &QLineEdit::textChanged, this, &UserInfoPage::onInputChanged);
    connect(ui->phoneLineEdit, &QLineEdit::textChanged, this, &UserInfoPage::onInputChanged);
    connect(ui->addressLineEdit, &QLineEdit::textChanged, this, &UserInfoPage::onInputChanged);

    // 性别二选一（男/女）来自 UI（userinfopage.ui）
    connect(ui->maleRadioBtn, &QRadioButton::toggled, this, &UserInfoPage::onInputChanged);
    connect(ui->femaleRadioBtn, &QRadioButton::toggled, this, &UserInfoPage::onInputChanged);

    // 初始状态下下一步按钮不可用
    setNextButtonEnabled(false);

    qDebug() << "[用户信息页面] 用户信息页面初始化完成";
}

UserInfoPage::~UserInfoPage()
{
    delete ui;
}

void UserInfoPage::clearInput()
{
    ui->nameLineEdit->clear();
    ui->phoneLineEdit->clear();
    ui->addressLineEdit->clear();
    ui->statusLabel->clear();
    if (ui->maleRadioBtn) ui->maleRadioBtn->setChecked(true);
    setNextButtonEnabled(false);
}

UserInfo UserInfoPage::getUserInfo() const
{
    UserInfo info;
    info.name = ui->nameLineEdit->text().trimmed();
    info.phone = ui->phoneLineEdit->text().trimmed();
    info.address = ui->addressLineEdit->text().trimmed();
    info.sex = (ui->maleRadioBtn && ui->maleRadioBtn->isChecked()) ? "男" : "女";
    return info;
}

void UserInfoPage::onNextClicked()
{
    UserInfo userInfo = getUserInfo();
    
    // 验证用户信息
    if (!userInfo.isValid()) {
        ui->statusLabel->setText("请填写完整的用户信息");
        ui->statusLabel->setStyleSheet("color: rgb(255, 77, 79);");
        return;
    }
    
    // 验证手机号格式
    if (!isPhoneValid(userInfo.phone)) {
        ui->statusLabel->setText("请输入正确的手机号码");
        ui->statusLabel->setStyleSheet("color: rgb(255, 77, 79);");
        return;
    }

    qDebug() << "[用户信息页面] 用户信息填写完成:" << userInfo.name << userInfo.phone << userInfo.address;
    
    // 发送用户信息完成信号
    emit userInfoCompleted(userInfo);
}

void UserInfoPage::onBackClicked()
{
    qDebug() << "[用户信息页面] 返回首页";
    clearInput();
    emit backToHomeClicked();
}

void UserInfoPage::onInputChanged()
{
    validateInput();
}

void UserInfoPage::validateInput()
{
    UserInfo userInfo = getUserInfo();
    bool isValid = userInfo.isValid() && isPhoneValid(userInfo.phone);
    
    setNextButtonEnabled(isValid);
    
    // 清除状态信息
    if (isValid) {
        ui->statusLabel->clear();
    }
}

void UserInfoPage::setNextButtonEnabled(bool enabled)
{
    ui->nextBtn->setEnabled(enabled);
    if (enabled) {
        ui->nextBtn->setStyleSheet(
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
        ui->nextBtn->setStyleSheet(
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

bool UserInfoPage::isPhoneValid(const QString& phone) const
{
    // 简单的手机号验证：11位数字，以1开头
    QRegularExpression phoneRegex("^1[3-9]\\d{9}$");
    return phoneRegex.match(phone).hasMatch();
}
