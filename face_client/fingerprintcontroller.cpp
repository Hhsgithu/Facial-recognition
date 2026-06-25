#include "fingerprintcontroller.h"
#include <QDebug>
#include <QThread>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include <cstring>

const char* FingerprintController::DEVICE_PATH = "/dev/sfm17";

FingerprintController::FingerprintController(QObject *parent)
    : QObject(parent)
    , deviceFd(-1)
    , notifier(nullptr)
    , timeoutTimer(new QTimer(this))
    , isVerifying(false)
    , isCapturing(false)
    , lastUserId(0)
{
    // 设置超时定时器
    timeoutTimer->setSingleShot(true);
    timeoutTimer->setInterval(VERIFY_TIMEOUT);
    connect(timeoutTimer, &QTimer::timeout, this, &FingerprintController::onVerificationTimeout);
    
    qDebug() << "[指纹控制器] FingerprintController初始化完成";
}

FingerprintController::~FingerprintController()
{
    closeDevice();
    qDebug() << "[指纹控制器] FingerprintController析构完成";
}

bool FingerprintController::openDevice()
{
    if (deviceFd >= 0) {
        qDebug() << "[指纹控制器] 设备已经打开";
        return true;
    }

    // 打开设备文件
    deviceFd = open(DEVICE_PATH, O_RDWR | O_NONBLOCK);
    if (deviceFd < 0) {
        QString errorMsg = QString("无法打开指纹设备 %1: %2").arg(DEVICE_PATH).arg(strerror(errno));
        qWarning() << "[指纹控制器]" << errorMsg;
        emit deviceError(errorMsg);
        return false;
    }

    // 创建套接字通知器用于异步读取
    notifier = new QSocketNotifier(deviceFd, QSocketNotifier::Read, this);
    connect(notifier, &QSocketNotifier::activated, this, &FingerprintController::readFingerprintData);
    notifier->setEnabled(false); // 初始状态禁用

    qDebug() << "[指纹控制器] 指纹设备打开成功:" << DEVICE_PATH;
    return true;
}

void FingerprintController::closeDevice()
{
    if (deviceFd < 0) {
        return;
    }

    qDebug() << "[指纹控制器] 开始关闭指纹设备";
    
    try {
        // 停止正在进行的验证
        stopVerification();

        // 清理套接字通知器
        if (notifier) {
            notifier->setEnabled(false);
            delete notifier;
            notifier = nullptr;
            qDebug() << "[指纹控制器] 套接字通知器已清理";
        }

        // 需求2.5: 确保设备文件描述符正确关闭
        if (close(deviceFd) == 0) {
            qDebug() << "[指纹控制器] 设备文件描述符关闭成功";
        } else {
            qWarning() << "[指纹控制器] 设备文件描述符关闭失败:" << strerror(errno);
        }
        deviceFd = -1;

        qDebug() << "[指纹控制器] 指纹设备已关闭";
        
    } catch (...) {
        qWarning() << "[指纹控制器] 关闭设备过程中发生异常";
        // 需求2.5: 异常情况下的资源清理
        deviceFd = -1;
        if (notifier) {
            delete notifier;
            notifier = nullptr;
        }
    }
}

bool FingerprintController::isDeviceOpen() const
{
    return deviceFd >= 0;
}

bool FingerprintController::startVerification()
{
    if (!isDeviceOpen()) {
        QString errorMsg = "指纹设备未打开";
        qWarning() << "[指纹控制器]" << errorMsg;
        emit deviceError(errorMsg);
        return false;
    }

    if (isVerifying) {
        qDebug() << "[指纹控制器] 指纹验证已在进行中";
        return true;
    }

    // 发送验证命令
    unsigned char cmd = CMD_VERIFY;
    ssize_t bytesWritten = write(deviceFd, &cmd, 1);
    if (bytesWritten != 1) {
        QString errorMsg = QString("发送指纹验证命令失败: %1").arg(strerror(errno));
        qWarning() << "[指纹控制器]" << errorMsg;
        emit deviceError(errorMsg);
        return false;
    }

    // 启用套接字通知器和超时定时器
    isVerifying = true;
    lastUserId = 0;
    notifier->setEnabled(true);
    timeoutTimer->start();

    qDebug() << "[指纹控制器] 指纹验证已启动，等待用户输入";
    return true;
}

void FingerprintController::stopVerification()
{
    if (!isVerifying) {
        return;
    }

    // 停止超时定时器
    timeoutTimer->stop();

    // 禁用套接字通知器
    if (notifier) {
        notifier->setEnabled(false);
    }

    isVerifying = false;
    qDebug() << "[指纹控制器] 指纹验证已停止";
}

int FingerprintController::getLastUserId() const
{
    return lastUserId;
}

void FingerprintController::readFingerprintData()
{
    if ((!isVerifying && !isCapturing) || deviceFd < 0) {
        return;
    }

    if (isVerifying) {
        // 验证模式：读取用户ID数据
        int userId = 0;
        ssize_t bytesRead = read(deviceFd, &userId, sizeof(userId));
        
        if (bytesRead == sizeof(userId)) {
            lastUserId = userId;
            
            // 停止验证过程
            stopVerification();
            
            qDebug() << "[指纹控制器] 指纹验证完成，用户ID:" << userId;
            emit verificationResult(userId);
        } else if (bytesRead < 0 && errno != EAGAIN && errno != EWOULDBLOCK) {
            // 读取错误（排除非阻塞读取的正常情况）
            QString errorMsg = QString("读取指纹数据失败: %1").arg(strerror(errno));
            qWarning() << "[指纹控制器]" << errorMsg;
            
            stopVerification();
            emit deviceError(errorMsg);
        }
    } else if (isCapturing) {
        // 采集模式：读取用户ID（3C3R注册完成后返回的ID）
        int userId = 0;
        ssize_t bytesRead = read(deviceFd, &userId, sizeof(userId));
        
        if (bytesRead == sizeof(userId)) {
            lastUserId = userId;
            
            // 停止采集过程
            stopCapture();
            
            qDebug() << "[指纹控制器] 指纹采集完成，用户ID:" << userId;
            
            // 将用户ID转换为QByteArray（作为指纹数据的标识）
            QByteArray fingerprintData;
            fingerprintData.append(reinterpret_cast<const char*>(&userId), sizeof(userId));
            
            emit captureResult(fingerprintData);
        } else if (bytesRead < 0 && errno != EAGAIN && errno != EWOULDBLOCK) {
            // 读取错误（排除非阻塞读取的正常情况）
            QString errorMsg = QString("读取指纹采集数据失败: %1").arg(strerror(errno));
            qWarning() << "[指纹控制器]" << errorMsg;
            
            stopCapture();
            emit captureError(errorMsg);
        }
    }
}

void FingerprintController::onVerificationTimeout()
{
    if (!isVerifying) {
        return;
    }

    qDebug() << "[指纹控制器] 指纹验证超时";
    
    // 停止验证并发送超时结果
    stopVerification();
    lastUserId = 0;
    emit verificationResult(0); // 0表示验证失败/超时
}

bool FingerprintController::startCapture()
{
    if (!isDeviceOpen()) {
        QString errorMsg = "指纹设备未打开";
        qWarning() << "[指纹控制器]" << errorMsg;
        emit deviceError(errorMsg);
        return false;
    }

    if (isCapturing) {
        qDebug() << "[指纹控制器] 指纹采集已在进行中";
        return true;
    }

    // 发送采集命令（3C3R注册）
    unsigned char cmd = CMD_CAPTURE;
    ssize_t bytesWritten = write(deviceFd, &cmd, 1);
    if (bytesWritten != 1) {
        QString errorMsg = QString("发送指纹采集命令失败: %1").arg(strerror(errno));
        qWarning() << "[指纹控制器]" << errorMsg;
        emit captureError(errorMsg);
        return false;
    }

    // 启用套接字通知器和超时定时器
    isCapturing = true;
    lastCaptureData.clear();
    lastUserId = 0;
    notifier->setEnabled(true);
    timeoutTimer->setInterval(CAPTURE_TIMEOUT);
    timeoutTimer->start();

    qDebug() << "[指纹控制器] 指纹采集已启动，等待用户输入（3次按压）";
    return true;
}

void FingerprintController::stopCapture()
{
    if (!isCapturing) {
        return;
    }

    // 停止超时定时器
    timeoutTimer->stop();
    timeoutTimer->setInterval(VERIFY_TIMEOUT); // 恢复默认超时时间

    // 禁用套接字通知器
    if (notifier) {
        notifier->setEnabled(false);
    }

    isCapturing = false;
    qDebug() << "[指纹控制器] 指纹采集已停止";
}

void FingerprintController::onCaptureTimeout()
{
    if (!isCapturing) {
        return;
    }

    qDebug() << "[指纹控制器] 指纹采集超时";
    
    // 停止采集并发送超时错误
    stopCapture();
    emit captureError("指纹采集超时");
}
