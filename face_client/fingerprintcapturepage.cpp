#include "fingerprintcapturepage.h"
#include "ui_fingerprintcapturepage.h"
#include <QDebug>

FingerprintCapturePage::FingerprintCapturePage(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::FingerprintCapturePage)
    , m_hardwareController(nullptr)
{
    ui->setupUi(this);
    
    // 设置固定大小和背景
    setFixedSize(1024, 600);
    setAutoFillBackground(true);
    
    QPalette pal = palette();
    pal.setColor(QPalette::Window, QColor(245, 247, 250));
    setPalette(pal);

    // 连接信号槽
    connect(ui->backBtn, &QPushButton::clicked, this, &FingerprintCapturePage::onBackClicked);
    connect(ui->startCaptureBtn, &QPushButton::clicked, this, &FingerprintCapturePage::onStartCapture);

    qDebug() << "[指纹采集页面] 指纹采集页面初始化完成";
}

FingerprintCapturePage::~FingerprintCapturePage()
{
    delete ui;
}

void FingerprintCapturePage::setUserInfo(const UserInfo& userInfo)
{
    m_userInfo = userInfo;
    ui->userNameLabel->setText(QString("用户：%1").arg(userInfo.name));
    qDebug() << "[指纹采集页面] 设置用户信息:" << userInfo.name;
}

void FingerprintCapturePage::setHardwareController(HardwareController* controller)
{
    m_hardwareController = controller;
    
    if (m_hardwareController) {
        // 连接指纹采集信号
        connect(m_hardwareController, static_cast<void(HardwareController::*)(const QByteArray&)>(&HardwareController::fingerprintCaptured),
                this, &FingerprintCapturePage::onFingerprintCaptured);
        connect(m_hardwareController, &HardwareController::fingerprintError,
                this, &FingerprintCapturePage::onFingerprintError);
        
        qDebug() << "[指纹采集页面] 硬件控制器已设置";
    }
}

void FingerprintCapturePage::startCapture()
{
    qDebug() << "[指纹采集页面] 开始采集流程";
    
    // 重置按钮状态
    ui->startCaptureBtn->setEnabled(true);
    
    // 初始化指纹模块
    if (m_hardwareController) {
        qDebug() << "[指纹采集页面] 初始化指纹模块";
        if (!m_hardwareController->initializeFingerprint()) {
            qWarning() << "[指纹采集页面] 指纹模块初始化失败";
            updateStatus("指纹模块初始化失败，请检查设备", "rgb(255, 77, 79)");
            return;
        }
        qDebug() << "[指纹采集页面] 指纹模块初始化成功";
    }
    
    updateStatus("点击\"开始采集\"按钮开始", "rgb(100, 100, 100)");
}

void FingerprintCapturePage::onStartCapture()
{
    qDebug() << "[指纹采集页面] 用户点击开始采集";

    if (!m_hardwareController) {
        updateStatus("硬件控制器未初始化", "rgb(255, 77, 79)");
        qWarning() << "[指纹采集页面] 硬件控制器未初始化";
        return;
    }

    ui->startCaptureBtn->setEnabled(false);
    updateStatus("请将手指放在指纹传感器上...", "rgb(24, 144, 255)");

    // 开始指纹采集
    m_hardwareController->startFingerprintCapture();
}

void FingerprintCapturePage::onFingerprintCaptured(const QByteArray& data)
{
    qDebug() << "[指纹采集页面] 指纹采集成功，数据大小:" << data.size();
    
    m_fingerprintData = data;
    updateStatus("指纹采集成功！", "rgb(82, 196, 26)");
    
    // 短暂延迟后发送完成信号
    QTimer::singleShot(1000, this, [this]() {
        emit fingerprintCaptureCompleted(m_fingerprintData);
    });
}

void FingerprintCapturePage::onFingerprintError(const QString& error)
{
    qDebug() << "[指纹采集页面] 指纹采集失败:" << error;
    
    updateStatus(QString("指纹采集失败: %1").arg(error), "rgb(255, 77, 79)");
    
    // 停止当前采集
    if (m_hardwareController) {
        m_hardwareController->stopFingerprintCapture();
    }
    
    // 重新启用按钮，允许用户重试
    ui->startCaptureBtn->setEnabled(true);
    
    qDebug() << "[指纹采集页面] 已重置状态，用户可以重试";
}

void FingerprintCapturePage::onBackClicked()
{
    qDebug() << "[指纹采集页面] 用户点击返回";
    emit backClicked();
}

void FingerprintCapturePage::updateStatus(const QString& message, const QString& color)
{
    ui->statusLabel->setText(message);
    ui->statusLabel->setStyleSheet(QString("color: %1; font-size: 14px;").arg(color));
}
