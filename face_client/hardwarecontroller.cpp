#include "hardwarecontroller.h"
#include <QDebug>

HardwareController::HardwareController(QObject *parent)
    : QObject(parent)
    , fingerprintCtrl(new FingerprintController(this))
    , servoCtrl(new ServoController(this))
    , servoTimer(new QTimer(this))
    , fingerprintReady(false)
    , servoReady(false)
{
    // 连接指纹控制器信号 - 验证模式
    connect(fingerprintCtrl, &FingerprintController::verificationResult,
            this, &HardwareController::onFingerprintResult);
    connect(fingerprintCtrl, &FingerprintController::deviceError,
            this, &HardwareController::onFingerprintDeviceError);
    
    // 连接指纹控制器信号 - 采集模式
    connect(fingerprintCtrl, &FingerprintController::captureResult,
            this, &HardwareController::onFingerprintCaptureResult);
    connect(fingerprintCtrl, &FingerprintController::captureError,
            this, &HardwareController::onFingerprintCaptureError);

    // 连接舵机控制器信号
    connect(servoCtrl, &ServoController::angleChanged,
            this, &HardwareController::onServoAngleChanged);
    connect(servoCtrl, &ServoController::operationComplete,
            this, &HardwareController::onServoOperationComplete);
    connect(servoCtrl, &ServoController::deviceError,
            this, &HardwareController::onServoDeviceError);

    // 设置舵机定时器（用于自动关门）
    servoTimer->setSingleShot(true);
    servoTimer->setInterval(DOOR_OPEN_DURATION);
    connect(servoTimer, &QTimer::timeout, this, &HardwareController::onServoTimeout);

    qDebug() << "[硬件控制器] HardwareController初始化完成";
}

HardwareController::~HardwareController()
{
    cleanup();
    qDebug() << "[硬件控制器] HardwareController析构完成";
}

bool HardwareController::initializeFingerprint()
{
    qDebug() << "[硬件控制器] 初始化指纹模块";
    
    fingerprintReady = fingerprintCtrl->openDevice();
    if (fingerprintReady) {
        qDebug() << "[硬件控制器] 指纹模块初始化成功";
    } else {
        qWarning() << "[硬件控制器] 指纹模块初始化失败";
    }
    
    return fingerprintReady;
}

bool HardwareController::initializeServo()
{
    qDebug() << "[硬件控制器] 初始化舵机";
    
    servoReady = servoCtrl->openDevice();
    if (servoReady) {
        qDebug() << "[硬件控制器] 舵机初始化成功";
        // 初始化时设置舵机为关门状态
        servoCtrl->closeDoor();
    } else {
        qWarning() << "[硬件控制器] 舵机初始化失败";
    }
    
    return servoReady;
}

void HardwareController::cleanup()
{
    qDebug() << "[硬件控制器] 开始清理硬件资源";
    
    try {
        // 停止正在进行的操作
        stopFingerprintVerification();
        stopFingerprintCapture();
        
        if (servoTimer && servoTimer->isActive()) {
            servoTimer->stop();
            qDebug() << "[硬件控制器] 舵机定时器已停止";
        }
        
        // 需求2.5: 确保设备文件描述符正确关闭
        if (fingerprintCtrl) {
            fingerprintCtrl->closeDevice();
            fingerprintReady = false;
            qDebug() << "[硬件控制器] 指纹设备已关闭";
        }
        
        if (servoCtrl) {
            // 关门后再关闭设备
            if (servoReady) {
                try {
                    servoCtrl->closeDoor();
                    qDebug() << "[硬件控制器] 舵机已关门";
                } catch (...) {
                    qWarning() << "[硬件控制器] 舵机关门异常，继续清理";
                }
            }
            servoCtrl->closeDevice();
            servoReady = false;
            qDebug() << "[硬件控制器] 舵机设备已关闭";
        }
        
        qDebug() << "[硬件控制器] 硬件资源清理完成";
        
    } catch (...) {
        qWarning() << "[硬件控制器] 硬件资源清理过程中发生异常";
        // 需求2.5: 异常情况下的资源清理 - 强制重置状态
        fingerprintReady = false;
        servoReady = false;
    }
}

bool HardwareController::startFingerprintVerification()
{
    if (!fingerprintReady) {
        qWarning() << "[硬件控制器] 指纹模块未就绪，无法开始验证";
        return false;
    }
    
    qDebug() << "[硬件控制器] 开始指纹验证";
    return fingerprintCtrl->startVerification();
}

void HardwareController::stopFingerprintVerification()
{
    if (fingerprintReady) {
        qDebug() << "[硬件控制器] 停止指纹验证";
        fingerprintCtrl->stopVerification();
    }
}

bool HardwareController::startFingerprintCapture()
{
    if (!fingerprintReady) {
        qWarning() << "[硬件控制器] 指纹模块未就绪，无法开始采集";
        return false;
    }
    
    qDebug() << "[硬件控制器] 开始指纹采集";
    return fingerprintCtrl->startCapture();
}

void HardwareController::stopFingerprintCapture()
{
    if (fingerprintReady) {
        qDebug() << "[硬件控制器] 停止指纹采集";
        fingerprintCtrl->stopCapture();
    }
}

bool HardwareController::isFingerprintReady() const
{
    return fingerprintReady;
}

bool HardwareController::openDoor()
{
    if (!servoReady) {
        qWarning() << "[硬件控制器] 舵机未就绪，无法开门";
        return false;
    }
    
    qDebug() << "[硬件控制器] 执行开门操作";
    
    // 开门（不启动自动关门定时器，由DoorSimulator管理）
    bool result = servoCtrl->openDoor();
    if (result) {
        qDebug() << "[硬件控制器] 开门成功";
    }
    
    return result;
}

bool HardwareController::closeDoor()
{
    if (!servoReady) {
        qWarning() << "[硬件控制器] 舵机未就绪，无法关门";
        return false;
    }
    
    qDebug() << "[硬件控制器] 执行关门操作";
    
    // 关门（不需要停止定时器，由DoorSimulator管理）
    return servoCtrl->closeDoor();
}

bool HardwareController::setServoAngle(int angle)
{
    if (!servoReady) {
        qWarning() << "[硬件控制器] 舵机未就绪，无法设置角度";
        return false;
    }
    
    return servoCtrl->setAngle(angle);
}

void HardwareController::onFingerprintResult(int userId)
{
    qDebug() << "[硬件控制器] 收到指纹验证结果，用户ID:" << userId;
    emit fingerprintCaptured(userId);
}

void HardwareController::onFingerprintDeviceError(const QString& error)
{
    qWarning() << "[硬件控制器] 指纹设备错误:" << error;
    fingerprintReady = false;
    emit fingerprintError(error);
}

void HardwareController::onFingerprintCaptureResult(const QByteArray& data)
{
    qDebug() << "[硬件控制器] 收到指纹采集结果，数据大小:" << data.size();
    emit fingerprintCaptured(data);
}

void HardwareController::onFingerprintCaptureError(const QString& error)
{
    qWarning() << "[硬件控制器] 指纹采集错误:" << error;
    emit fingerprintError(error);
}

void HardwareController::onServoAngleChanged(int angle)
{
    qDebug() << "[硬件控制器] 舵机角度变化:" << angle << "度";
    emit servoPositionChanged(angle);
}

void HardwareController::onServoOperationComplete()
{
    qDebug() << "[硬件控制器] 舵机操作完成";
    emit doorOperationComplete();
}

void HardwareController::onServoDeviceError(const QString& error)
{
    qWarning() << "[硬件控制器] 舵机设备错误:" << error;
    servoReady = false;
    
    // 舵机错误时停止自动关门定时器
    servoTimer->stop();
}

void HardwareController::onServoTimeout()
{
    qDebug() << "[硬件控制器] 自动关门定时器超时，执行关门操作";
    closeDoor();
}
