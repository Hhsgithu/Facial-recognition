#ifndef HARDWARECONTROLLER_H
#define HARDWARECONTROLLER_H

#include <QObject>
#include <QTimer>
#include "fingerprintcontroller.h"
#include "servocontroller.h"

/**
 * @brief 硬件控制器统一管理类
 * 
 * 集成FingerprintController和ServoController，提供硬件初始化和清理流程。
 * 协调指纹认证和门控操作，实现完整的指纹解锁流程。
 */
class HardwareController : public QObject
{
    Q_OBJECT

public:
    explicit HardwareController(QObject *parent = nullptr);
    ~HardwareController();

    /**
     * @brief 初始化指纹模块
     * @return true表示初始化成功，false表示初始化失败
     */
    bool initializeFingerprint();

    /**
     * @brief 初始化舵机
     * @return true表示初始化成功，false表示初始化失败
     */
    bool initializeServo();

    /**
     * @brief 清理所有硬件资源
     */
    void cleanup();

    /**
     * @brief 开始指纹验证
     * @return true表示成功启动验证，false表示启动失败
     */
    bool startFingerprintVerification();

    /**
     * @brief 停止指纹验证
     */
    void stopFingerprintVerification();

    /**
     * @brief 开始指纹采集（用于注册）
     * @return true表示成功启动采集，false表示启动失败
     */
    bool startFingerprintCapture();

    /**
     * @brief 停止指纹采集
     */
    void stopFingerprintCapture();

    /**
     * @brief 检查指纹模块是否就绪
     * @return true表示就绪，false表示未就绪
     */
    bool isFingerprintReady() const;

    /**
     * @brief 开门操作
     * @return true表示操作成功，false表示操作失败
     */
    bool openDoor();

    /**
     * @brief 关门操作
     * @return true表示操作成功，false表示操作失败
     */
    bool closeDoor();

    /**
     * @brief 设置舵机角度
     * @param angle 角度值（0-180度）
     * @return true表示设置成功，false表示设置失败
     */
    bool setServoAngle(int angle);

signals:
    /**
     * @brief 指纹捕获信号
     * @param userId 用户ID，大于0表示验证成功，0表示验证失败
     */
    void fingerprintCaptured(int userId);

    /**
     * @brief 指纹错误信号
     * @param error 错误描述信息
     */
    void fingerprintError(const QString& error);

    /**
     * @brief 指纹采集信号
     * @param data 采集到的指纹数据
     */
    void fingerprintCaptured(const QByteArray& data);

    /**
     * @brief 舵机位置改变信号
     * @param angle 新的角度值
     */
    void servoPositionChanged(int angle);

    /**
     * @brief 门操作完成信号
     */
    void doorOperationComplete();

private slots:
    /**
     * @brief 处理指纹验证结果
     * @param userId 用户ID
     */
    void onFingerprintResult(int userId);

    /**
     * @brief 处理指纹设备错误
     * @param error 错误信息
     */
    void onFingerprintDeviceError(const QString& error);

    /**
     * @brief 处理指纹采集结果
     * @param data 采集到的指纹数据
     */
    void onFingerprintCaptureResult(const QByteArray& data);

    /**
     * @brief 处理指纹采集错误
     * @param error 错误信息
     */
    void onFingerprintCaptureError(const QString& error);

    /**
     * @brief 处理舵机角度变化
     * @param angle 新角度
     */
    void onServoAngleChanged(int angle);

    /**
     * @brief 处理舵机操作完成
     */
    void onServoOperationComplete();

    /**
     * @brief 处理舵机设备错误
     * @param error 错误信息
     */
    void onServoDeviceError(const QString& error);

    /**
     * @brief 舵机超时处理（用于自动关门）
     */
    void onServoTimeout();

private:
    FingerprintController* fingerprintCtrl;  ///< 指纹控制器
    ServoController* servoCtrl;              ///< 舵机控制器
    QTimer* servoTimer;                      ///< 舵机定时器（用于自动关门）
    bool fingerprintReady;                   ///< 指纹模块就绪状态
    bool servoReady;                         ///< 舵机就绪状态

    static const int DOOR_OPEN_DURATION = 5000; ///< 开门持续时间（毫秒）
};

#endif // HARDWARECONTROLLER_H