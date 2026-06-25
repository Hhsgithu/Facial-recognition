#ifndef NETWORKMANAGER_H
#define NETWORKMANAGER_H

#include <QObject>
#include <QTcpSocket>
#include <QTimer>
#include <QJsonObject>
#include <QJsonDocument>
#include "userinfopage.h"

class NetworkManager : public QObject
{
    Q_OBJECT

public:
    explicit NetworkManager(QObject *parent = nullptr);
    ~NetworkManager();

    // 注册相关方法
    void verifyRegistrationKey(const QString& key);
    void sendRegistrationData(const QString& registrationKey, const UserInfo& userInfo, const QByteArray& faceData, 
                             const QByteArray& fingerprintData, const QString& password);
    
    // 认证相关方法（保留现有接口）
    void sendFaceVerification(const QByteArray& faceImage);
    void sendPasswordVerification(const QString& password);
    void sendFingerprintVerification(const QByteArray& fingerprintData);
    
    // 连接管理
    void connectToServer();
    void disconnectFromServer();
    bool isConnected() const;

signals:
    // 注册相关信号
    void keyVerificationResult(bool success, const QString& message);
    void registrationResult(bool success, const QString& message, int fingerprintId);
    
    // 认证相关信号
    void authenticationResult(bool success, const UserInfo& userInfo, const QString& message);
    
    // 连接状态信号
    void connected();
    void disconnected();
    void connectionError(const QString& error);

private slots:
    void onConnected();
    void onDisconnected();
    void onReadyRead();
    void onSocketError(QAbstractSocket::SocketError socketError);
    void onReconnectTimer();

private:
    // 网络相关
    QTcpSocket* m_socket;
    QTimer* m_reconnectTimer;
    QString m_serverAddress;
    quint16 m_serverPort;
    bool m_isConnected;
    
    // 数据处理相关
    QByteArray m_receiveBuffer;
    
    // 内部方法
    void setupConnection();
    void processReceivedData();
    void sendJsonMessage(const QJsonObject& message);
    QJsonObject parseJsonMessage(const QByteArray& data);
    
    // 消息处理方法
    void handleKeyVerificationResponse(const QJsonObject& response);
    void handleRegistrationResponse(const QJsonObject& response);
    void handleAuthenticationResponse(const QJsonObject& response);
    void handleTimeSyncResponse(const QJsonObject& response);
    
    // 工具方法
    QString encodeBase64(const QByteArray& data);
    QByteArray decodeBase64(const QString& data);
    
    // 常量
    static const int RECONNECT_INTERVAL = 3000; // 3秒重连间隔
    static const int MAX_RECONNECT_ATTEMPTS = 5;
    int m_reconnectAttempts;
};

#endif // NETWORKMANAGER_H