#include "doorsimulator.h"
#include "hardwarecontroller.h"
#include <QDebug>

DoorSimulator::DoorSimulator(QObject *parent)
    : QObject(parent)
    , m_closeTimer(new QTimer(this))
    , m_doorOpen(false)
    , m_hwController(nullptr)
{
    // 设置定时器为单次触发
    m_closeTimer->setSingleShot(true);
    
    // 连接定时器超时信号到关门槽函数
    connect(m_closeTimer, &QTimer::timeout, this, &DoorSimulator::onCloseTimeout);
    
    qDebug() << "[门禁模拟器] DoorSimulator初始化完成";
}

DoorSimulator::~DoorSimulator()
{
    if (m_closeTimer->isActive()) {
        m_closeTimer->stop();
    }
    qDebug() << "[门禁模拟器] DoorSimulator销毁";
}

bool DoorSimulator::isOpen() const
{
    return m_doorOpen;
}

void DoorSimulator::setHardwareController(HardwareController* controller)
{
    m_hwController = controller;
    qDebug() << "[门禁模拟器] 硬件控制器已设置";
}

void DoorSimulator::openDoor()
{
    qDebug() << "[门禁模拟器] 开门请求";
    
    // 如果定时器正在运行，先停止（重置计时）
    if (m_closeTimer->isActive()) {
        m_closeTimer->stop();
        qDebug() << "[门禁模拟器] 重置开门定时器";
    }
    
    // 设置门状态为打开
    if (!m_doorOpen) {
        m_doorOpen = true;
        qDebug() << "[门禁模拟器] 门已打开";
        emit doorOpened();
        
        // 控制真实舵机开门
        if (m_hwController) {
            // 确保舵机已初始化
            if (!m_hwController->initializeServo()) {
                qWarning() << "[门禁模拟器] 舵机初始化失败，尝试开门";
            }
            
            qDebug() << "[门禁模拟器] 控制舵机开门（0°）";
            if (!m_hwController->openDoor()) {
                qWarning() << "[门禁模拟器] 舵机开门失败";
            }
        }
    }
    
    // 启动5秒定时器
    m_closeTimer->start(DOOR_OPEN_DURATION);
    qDebug() << "[门禁模拟器] 启动" << DOOR_OPEN_DURATION / 1000 << "秒关门定时器";
}

void DoorSimulator::onCloseTimeout()
{
    qDebug() << "[门禁模拟器] 关门定时器超时";
    
    if (m_doorOpen) {
        m_doorOpen = false;
        qDebug() << "[门禁模拟器] 门已关闭";
        
        // 控制真实舵机关门
        if (m_hwController) {
            // 确保舵机已初始化
            if (!m_hwController->initializeServo()) {
                qWarning() << "[门禁模拟器] 舵机初始化失败，尝试关门";
            }
            
            qDebug() << "[门禁模拟器] 控制舵机关门（90°）";
            if (!m_hwController->closeDoor()) {
                qWarning() << "[门禁模拟器] 舵机关门失败";
            }
        }
        
        emit doorClosed();
    }
}
