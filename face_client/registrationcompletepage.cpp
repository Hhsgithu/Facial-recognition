#include "registrationcompletepage.h"
#include "ui_registrationcompletepage.h"
#include <QDebug>

RegistrationCompletePage::RegistrationCompletePage(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::RegistrationCompletePage)
    , m_state(Ready)
{
    ui->setupUi(this);
    
    // 设置固定大小和背景
    setFixedSize(1024, 600);
    setAutoFillBackground(true);
    
    QPalette pal = palette();
    pal.setColor(QPalette::Window, QColor(245, 247, 250));
    setPalette(pal);

    // 连接信号槽
    connect(ui->backBtn, &QPushButton::clicked, this, &RegistrationCompletePage::onBackToHome);
    connect(ui->completeBtn, &QPushButton::clicked, this, &RegistrationCompletePage::onCompleteRegistration);
    connect(ui->retryBtn, &QPushButton::clicked, this, &RegistrationCompletePage::onRetryRegistration);
    
    // 初始状态
    updateUI();

    qDebug() << "[注册完成页面] 注册完成页面初始化完成";
}

RegistrationCompletePage::~RegistrationCompletePage()
{
    delete ui;
}

void RegistrationCompletePage::setUserInfo(const UserInfo& userInfo)
{
    m_userInfo = userInfo;
    ui->userNameLabel->setText(QString("用户：%1").arg(userInfo.name));
    qDebug() << "[注册完成页面] 设置用户信息:" << userInfo.name;
}

void RegistrationCompletePage::reset()
{
    qDebug() << "[注册完成页面] 重置状态";
    m_state = Ready;
    updateUI();
}

void RegistrationCompletePage::showProgress()
{
    qDebug() << "[注册完成页面] 显示提交进度";
    m_state = Progress;
    updateUI();
}

void RegistrationCompletePage::showSuccess()
{
    qDebug() << "[注册完成页面] 显示注册成功";
    m_state = Success;
    updateUI();
}

void RegistrationCompletePage::showError(const QString& message)
{
    qDebug() << "[注册完成页面] 显示注册失败:" << message;
    m_state = Error;
    ui->statusLabel->setText(message);
    updateUI();
}

void RegistrationCompletePage::onCompleteRegistration()
{
    qDebug() << "[注册完成页面] 用户点击完成注册";
    
    if (m_state == Ready) {
        showProgress();
        emit registrationRequested();
    }
}

void RegistrationCompletePage::onRetryRegistration()
{
    qDebug() << "[注册完成页面] 用户点击重试注册";
    
    if (m_state == Error) {
        showProgress();
        emit registrationRequested();
    }
}

void RegistrationCompletePage::onBackToHome()
{
    qDebug() << "[注册完成页面] 用户点击返回主页";
    
    if (m_state == Success) {
        emit registrationCompleted();
    } else {
        emit backClicked();
    }
}

void RegistrationCompletePage::updateUI()
{
    switch (m_state) {
        case Ready:
            qDebug() << "[注册完成页面] 状态：准备提交";
            ui->iconLabel->setText("✓");
            ui->iconLabel->setStyleSheet("color: rgb(24, 144, 255); font-size: 80px;");
            ui->titleLabel->setText("信息采集完成");
            ui->statusLabel->setText("请点击下方按钮完成注册");
            ui->statusLabel->setStyleSheet("color: rgb(100, 100, 100); font-size: 14px;");
            
            ui->completeBtn->setVisible(true);
            ui->completeBtn->setEnabled(true);
            ui->retryBtn->setVisible(false);
            ui->backBtn->setText("返回");
            ui->backBtn->setEnabled(true);
            break;
            
        case Progress:
            qDebug() << "[注册完成页面] 状态：提交中";
            ui->iconLabel->setText("⟳");
            ui->iconLabel->setStyleSheet("color: rgb(24, 144, 255); font-size: 80px;");
            ui->titleLabel->setText("正在提交注册信息");
            ui->statusLabel->setText("请稍候...");
            ui->statusLabel->setStyleSheet("color: rgb(100, 100, 100); font-size: 14px;");
            
            ui->completeBtn->setVisible(true);
            ui->completeBtn->setEnabled(false);
            ui->retryBtn->setVisible(false);
            ui->backBtn->setEnabled(false);
            break;
            
        case Success:
            qDebug() << "[注册完成页面] 状态：成功";
            ui->iconLabel->setText("✓");
            ui->iconLabel->setStyleSheet("color: rgb(82, 196, 26); font-size: 80px;");
            ui->titleLabel->setText("注册成功！");
            ui->statusLabel->setText("您已成功注册，可以开始使用系统了");
            ui->statusLabel->setStyleSheet("color: rgb(82, 196, 26); font-size: 14px;");
            
            ui->completeBtn->setVisible(false);
            ui->retryBtn->setVisible(false);
            ui->backBtn->setText("返回主页");
            ui->backBtn->setEnabled(true);
            break;
            
        case Error:
            qDebug() << "[注册完成页面] 状态：失败";
            ui->iconLabel->setText("✗");
            ui->iconLabel->setStyleSheet("color: rgb(255, 77, 79); font-size: 80px;");
            ui->titleLabel->setText("注册失败");
            // statusLabel由showError设置
            ui->statusLabel->setStyleSheet("color: rgb(255, 77, 79); font-size: 14px;");
            
            ui->completeBtn->setVisible(false);
            ui->retryBtn->setVisible(true);
            ui->retryBtn->setEnabled(true);
            ui->backBtn->setText("返回");
            ui->backBtn->setEnabled(true);
            break;
    }
}
