#include "servocontroller.h"
#include <QDebug>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include <cstring>

const char* ServoController::DEVICE_PATH = "/dev/sg90";

ServoController::ServoController(QObject *parent)
    : QObject(parent)
    , deviceFd(-1)
    , currentAngle(-1) // -1表示未知角度
{
    qDebug() << "[舵机控制器] ServoController初始化完成";
}

ServoController::~ServoController()
{
    closeDevice();
    qDebug() << "[舵机控制器] ServoController析构完成";
}

bool ServoController::openDevice()
{
    if (deviceFd >= 0) {
        qDebug() << "[舵机控制器] 设备已经打开";
        return true;
    }

    // 打开设备文件
    deviceFd = open(DEVICE_PATH, O_WRONLY);
    if (deviceFd < 0) {
        QString errorMsg = QString("无法打开舵机设备 %1: %2").arg(DEVICE_PATH).arg(strerror(errno));
        qWarning() << "[舵机控制器]" << errorMsg;
        emit deviceError(errorMsg);
        return false;
    }

    qDebug() << "[舵机控制器] 舵机设备打开成功:" << DEVICE_PATH;
    return true;
}

void ServoController::closeDevice()
{
    if (deviceFd < 0) {
        return;
    }

    qDebug() << "[舵机控制器] 开始关闭舵机设备";
    
    try {
        // 需求2.5: 确保设备文件描述符正确关闭
        if (close(deviceFd) == 0) {
            qDebug() << "[舵机控制器] 设备文件描述符关闭成功";
        } else {
            qWarning() << "[舵机控制器] 设备文件描述符关闭失败:" << strerror(errno);
        }
        deviceFd = -1;
        currentAngle = -1; // 重置角度状态

        qDebug() << "[舵机控制器] 舵机设备已关闭";
        
    } catch (...) {
        qWarning() << "[舵机控制器] 关闭设备过程中发生异常";
        // 需求2.5: 异常情况下的资源清理
        deviceFd = -1;
        currentAngle = -1;
    }
}

bool ServoController::isDeviceOpen() const
{
    return deviceFd >= 0;
}

bool ServoController::setAngle(int angle)
{
    // 检查设备是否已打开
    if (!isDeviceOpen()) {
        QString errorMsg = "舵机设备未打开";
        qWarning() << "[舵机控制器]" << errorMsg;
        emit deviceError(errorMsg);
        return false;
    }

    // 检查角度范围
    if (angle < MIN_ANGLE || angle > MAX_ANGLE) {
        QString errorMsg = QString("角度值超出范围 [%1, %2]: %3").arg(MIN_ANGLE).arg(MAX_ANGLE).arg(angle);
        qWarning() << "[舵机控制器]" << errorMsg;
        emit deviceError(errorMsg);
        return false;
    }

    // 发送角度值到设备（驱动只接受1字节，0-180）
    unsigned char angleChar = static_cast<unsigned char>(angle);
    ssize_t bytesWritten = write(deviceFd, &angleChar, 1);
    if (bytesWritten != 1) {
        QString errorMsg = QString("设置舵机角度失败: %1").arg(strerror(errno));
        qWarning() << "[舵机控制器]" << errorMsg;
        emit deviceError(errorMsg);
        return false;
    }

    // 更新当前角度并发送信号
    currentAngle = angle;
    qDebug() << "[舵机控制器] 舵机角度已设置为:" << angle << "度";
    
    emit angleChanged(angle);
    emit operationComplete();
    
    return true;
}

int ServoController::getCurrentAngle() const
{
    return currentAngle;
}

bool ServoController::openDoor()
{
    qDebug() << "[舵机控制器] 执行开门操作";
    return setAngle(OPEN_ANGLE);
}

bool ServoController::closeDoor()
{
    qDebug() << "[舵机控制器] 执行关门操作";
    return setAngle(CLOSE_ANGLE);
}