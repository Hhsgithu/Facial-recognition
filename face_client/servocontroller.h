#ifndef SERVOCONTROLLER_H
#define SERVOCONTROLLER_H

#include <QObject>

/**
 * @brief 舵机控制器类
 * 
 * 封装SG90舵机驱动接口，提供舵机设备的打开、关闭和角度控制功能。
 * 实现开门(90°)和关门(0°)的便捷方法，包含设备状态检查和错误处理。
 */
class ServoController : public QObject
{
    Q_OBJECT

public:
    explicit ServoController(QObject *parent = nullptr);
    ~ServoController();

    /**
     * @brief 打开舵机设备
     * @return true表示成功打开设备，false表示打开失败
     */
    bool openDevice();

    /**
     * @brief 关闭舵机设备
     */
    void closeDevice();

    /**
     * @brief 检查设备是否已打开
     * @return true表示设备已打开，false表示设备未打开
     */
    bool isDeviceOpen() const;

    /**
     * @brief 设置舵机角度
     * @param angle 角度值（0-180度）
     * @return true表示设置成功，false表示设置失败
     */
    bool setAngle(int angle);

    /**
     * @brief 获取当前舵机角度
     * @return 当前角度值
     */
    int getCurrentAngle() const;

    /**
     * @brief 开门操作（设置为90度）
     * @return true表示操作成功，false表示操作失败
     */
    bool openDoor();

    /**
     * @brief 关门操作（设置为0度）
     * @return true表示操作成功，false表示操作失败
     */
    bool closeDoor();

signals:
    /**
     * @brief 角度改变信号
     * @param angle 新的角度值
     */
    void angleChanged(int angle);

    /**
     * @brief 操作完成信号
     */
    void operationComplete();

    /**
     * @brief 设备错误信号
     * @param error 错误描述信息
     */
    void deviceError(const QString& error);

private:
    int deviceFd;                    ///< 设备文件描述符
    int currentAngle;                ///< 当前角度

    static const char* DEVICE_PATH;  ///< 设备路径
    static const int OPEN_ANGLE = 0;   ///< 开门角度
    static const int CLOSE_ANGLE = 90; ///< 关门角度（初始状态）
    static const int MIN_ANGLE = 0;    ///< 最小角度
    static const int MAX_ANGLE = 180;  ///< 最大角度
};

#endif // SERVOCONTROLLER_H