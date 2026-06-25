#ifndef DOORSIMULATOR_H
#define DOORSIMULATOR_H

#include <QObject>
#include <QTimer>

// 前向声明
class HardwareController;

/**
 * @brief 门禁模拟器类
 * 
 * 模拟门禁开门状态，开门后保持5秒然后自动关闭。
 * 用于人脸识别成功或密码验证成功后的开门模拟。
 */
class DoorSimulator : public QObject
{
    Q_OBJECT

public:
    explicit DoorSimulator(QObject *parent = nullptr);
    ~DoorSimulator();
    
    /**
     * @brief 设置硬件控制器
     * @param controller 硬件控制器指针
     */
    void setHardwareController(HardwareController* controller);

    /**
     * @brief 查询门是否处于打开状态
     * @return true表示门已打开，false表示门已关闭
     */
    bool isOpen() const;

public slots:
    /**
     * @brief 开门操作
     * 
     * 触发开门，启动5秒定时器，5秒后自动关门。
     * 如果门已经打开，则重置定时器。
     */
    void openDoor();

signals:
    /**
     * @brief 门已打开信号
     */
    void doorOpened();

    /**
     * @brief 门已关闭信号
     */
    void doorClosed();

private slots:
    /**
     * @brief 关门定时器超时处理
     */
    void onCloseTimeout();

private:
    QTimer* m_closeTimer;              ///< 关门定时器
    bool m_doorOpen;                   ///< 门状态标志
    HardwareController* m_hwController; ///< 硬件控制器指针

    static const int DOOR_OPEN_DURATION = 5000; ///< 开门持续时间（毫秒）
};

#endif // DOORSIMULATOR_H
