#include "homepage.h"
#include "ui_homepage.h"
#include <QDateTime>
#include <QDebug>
#include <QPalette>

HomePage::HomePage(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::HomePage)
{
    ui->setupUi(this);
    
    // 设置固定大小和不透明背景
    setFixedSize(1024, 600);
    setAutoFillBackground(true);
    setAttribute(Qt::WA_OpaquePaintEvent, true);
    
    QPalette pal = palette();
    pal.setColor(QPalette::Window, QColor(245, 247, 250));
    setPalette(pal);

    // 创建注册进度指示器
    registrationProgressLabel = new QLabel(this);
    registrationProgressLabel->setGeometry(0, 580, 1024, 20);
    registrationProgressLabel->setAlignment(Qt::AlignCenter);
    registrationProgressLabel->setStyleSheet(
        "QLabel {"
        "   background-color: rgba(24, 144, 255, 200);"
        "   color: white;"
        "   font-size: 12px;"
        "   font-weight: bold;"
        "   border-radius: 10px;"
        "   padding: 2px 10px;"
        "}"
    );
    registrationProgressLabel->hide();

    // 初始化定时器，每秒更新一次时间
    dateTimeTimer = new QTimer(this);
    connect(dateTimeTimer, &QTimer::timeout, this, &HomePage::updateDateTime);
    dateTimeTimer->start(1000);

    // 立即更新一次时间
    updateDateTime();

    // 连接按钮点击信号
    connect(ui->faceRecognitionBtn, &QPushButton::clicked, this, &HomePage::onFaceRecognitionClicked);
    connect(ui->passwordUnlockBtn, &QPushButton::clicked, this, &HomePage::onPasswordUnlockClicked);
    connect(ui->fingerprintUnlockBtn, &QPushButton::clicked, this, &HomePage::onFingerprintUnlockClicked);
    connect(ui->userRegistrationBtn, &QPushButton::clicked, this, &HomePage::onUserRegistrationClicked);

    qDebug() << "[首页] 首页初始化完成";
}

HomePage::~HomePage()
{
    delete ui;
}

void HomePage::updateDateTime()
{
    QDateTime currentDateTime = QDateTime::currentDateTime();
    
    // 格式化日期：2024年01月01日
    QString dateStr = currentDateTime.toString("yyyy年MM月dd日");
    ui->dateLabel->setText(dateStr);
    
    // 格式化时间：12:00:00
    QString timeStr = currentDateTime.toString("hh:mm:ss");
    ui->timeLabel->setText(timeStr);
    
    // 格式化星期：星期一
    QString weekStr = currentDateTime.toString("dddd");
    ui->weekLabel->setText(weekStr);
}

void HomePage::onFaceRecognitionClicked()
{
    qDebug() << "[首页] 点击人脸识别按钮";
    emit faceRecognitionClicked();
}

void HomePage::onPasswordUnlockClicked()
{
    qDebug() << "[首页] 点击密码解锁按钮";
    emit passwordUnlockClicked();
}

void HomePage::onFingerprintUnlockClicked()
{
    qDebug() << "[首页] 点击指纹解锁按钮";
    emit fingerprintUnlockClicked();
}

void HomePage::onUserRegistrationClicked()
{
    qDebug() << "[首页] 点击用户注册按钮";
    showRegistrationProgress("开始用户注册流程...");
    emit userRegistrationClicked();
}

void HomePage::showRegistrationProgress(const QString& stepText)
{
    registrationProgressLabel->setText(stepText);
    registrationProgressLabel->show();
    registrationProgressLabel->raise();
}

void HomePage::hideRegistrationProgress()
{
    registrationProgressLabel->hide();
}

