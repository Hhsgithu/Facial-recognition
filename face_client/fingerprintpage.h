#ifndef FINGERPRINTPAGE_H
#define FINGERPRINTPAGE_H

#include <QWidget>
#include <QTimer>
#include <QTcpSocket>
#include "hardwarecontroller.h"

QT_BEGIN_NAMESPACE
namespace Ui { class FingerprintPage; }
QT_END_NAMESPACE

class FingerprintPage : public QWidget
{
    Q_OBJECT

public:
    explicit FingerprintPage(QWidget *parent = nullptr);
    ~FingerprintPage();
    
    void setHardwareController(HardwareController* controller);

signals:
    void backToHomeClicked();
    void verifySuccess(const QString& userId, const QString& userName);
    void verifyFailed(const QString& message);

public slots:
    void startAuthentication();
    void stopAuthentication();
    void cleanup(); // 需求8.5: 外部调用的清理方法

private slots:
    void onBackButtonClicked();
    void onAuthenticationTimeout();
    void onFingerprintCaptured(int userId);
    void onFingerprintError(const QString& error);
    
    // 网络相关槽函数
    void onConnected();
    void onDisconnected();
    void onNetworkData();
    void onSocketError(QAbstractSocket::SocketError socketError);
    void onReconnectTimer();

private:
    void updateStatus(const QString& message, const QString& color = "rgb(100, 100, 100)");
    void resetAuthenticationState();
    void enableControls(bool enabled);
    void processAuthenticationResult(int userId);
    void updateConnectionStatus(bool connected);
    void showDetailedStatus(const QString& message, const QString& color, const QString& iconColor);
    void showHardwareStatus(); // 显示硬件状态
    void updateUIForAuthenticationState(bool authenticating); // 更新UI以反映认证状态
    
    // 网络相关方法
    void connectToServer();
    void sendFingerprintVerifyRequest(int userId);
    void processServerResponse(const QByteArray& data);
    
    // 错误处理相关方法
    QString translateHardwareError(const QString& error);
    QString translateNetworkError(QAbstractSocket::SocketError socketError, const QString& errorString);
    void startNetworkRetry();
    void handleAuthenticationFailure(const QString& reason);
    
    // 资源清理相关方法
    void performResourceCleanup();
    void cleanupNetworkResources();
    void cleanupHardwareResources();
    void resetPageState();

    Ui::FingerprintPage *ui;
    QTimer* authTimer;
    bool isAuthenticating;
    HardwareController* hardwareController;
    int currentUserId;
    
    // 网络相关成员
    QTcpSocket* tcpSocket;
    QTimer* reconnectTimer;
    bool isConnected;
    bool isVerifying; // 网络验证中状态标志
    
    // 错误处理相关成员
    int networkRetryCount;
    QTimer* networkRetryTimer;
    static const int MAX_NETWORK_RETRIES = 3;
    static const int NETWORK_RETRY_INTERVAL = 2000; // 2秒
};

#endif // FINGERPRINTPAGE_H