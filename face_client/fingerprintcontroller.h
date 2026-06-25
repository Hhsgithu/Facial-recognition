#ifndef FINGERPRINTCONTROLLER_H
#define FINGERPRINTCONTROLLER_H

#include <QObject>
#include <QSocketNotifier>
#include <QTimer>

/**
 * @brief 指纹控制器类
 * 
 * 封装SFM17指纹模块驱动接口，提供指纹设备的打开、关闭和通信功能。
 * 实现指纹验证命令发送和结果读取，包含超时处理和错误处理机制。
 */
class FingerprintController : public QObject
{
    Q_OBJECT

public:
    explicit FingerprintController(QObject *parent = nullptr);
    ~FingerprintController();

    /**
     * @brief 打开指纹设备
     * @return true表示成功打开设备，false表示打开失败
     */
    bool openDevice();

    /**
     * @brief 关闭指纹设备
     */
    void closeDevice();

    /**
     * @brief 检查设备是否已打开
     * @return true表示设备已打开，false表示设备未打开
     */
    bool isDeviceOpen() const;

    /**
     * @brief 开始指纹验证
     * @return true表示成功启动验证，false表示启动失败
     */
    bool startVerification();

    /**
     * @brief 停止指纹验证
     */
    void stopVerification();

    /**
     * @brief 开始指纹采集（用于注册）
     * @return true表示成功启动采集，false表示启动失败
     */
    bool startCapture();

    /**
     * @brief 停止指纹采集
     */
    void stopCapture();

    /**
     * @brief 获取最后一次验证的用户ID
     * @return 用户ID，0表示验证失败或未识别
     */
    int getLastUserId() const;

signals:
    /**
     * @brief 指纹验证结果信号
     * @param userId 用户ID，大于0表示验证成功，0表示验证失败
     */
    void verificationResult(int userId);

    /**
     * @brief 设备错误信号
     * @param error 错误描述信息
     */
    void deviceError(const QString& error);

    /**
     * @brief 指纹采集结果信号
     * @param data 采集到的指纹数据
     */
    void captureResult(const QByteArray& data);

    /**
     * @brief 指纹采集错误信号
     * @param error 错误描述信息
     */
    void captureError(const QString& error);

private slots:
    /**
     * @brief 读取指纹数据
     */
    void readFingerprintData();

    /**
     * @brief 验证超时处理
     */
    void onVerificationTimeout();

    /**
     * @brief 采集超时处理
     */
    void onCaptureTimeout();

private:
    int deviceFd;                    ///< 设备文件描述符
    QSocketNotifier* notifier;       ///< 套接字通知器
    QTimer* timeoutTimer;            ///< 超时定时器
    bool isVerifying;                ///< 是否正在验证
    bool isCapturing;                ///< 是否正在采集
    int lastUserId;                  ///< 最后一次验证的用户ID
    QByteArray lastCaptureData;      ///< 最后一次采集的数据

    static const char* DEVICE_PATH;  ///< 设备路径
    static const int VERIFY_TIMEOUT = 8000; ///< 验证超时时间（毫秒）
    static const int CAPTURE_TIMEOUT = 10000; ///< 采集超时时间（毫秒）
    static const unsigned char CMD_VERIFY = 0x02; ///< 验证命令
    static const unsigned char CMD_CAPTURE = 0x01; ///< 采集命令
};

#endif // FINGERPRINTCONTROLLER_H