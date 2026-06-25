#include "networkmanager.h"
#include <QDebug>
#include <QJsonObject>
#include <QJsonDocument>
#include <QJsonArray>
#include <QAbstractSocket>
#include <QDateTime>
#include <QProcess>

NetworkManager::NetworkManager(QObject *parent)
    : QObject(parent)
    , m_socket(nullptr)
    , m_reconnectTimer(nullptr)
    , m_serverAddress("192.168.2.220")
    , m_serverPort(8888)
    , m_isConnected(false)
    , m_reconnectAttempts(0)
{
    setupConnection();
    qDebug() << "[网络管理器] NetworkManager初始化完成";
}

NetworkManager::~NetworkManager()
{
    if (m_socket) {
        m_socket->disconnectFromHost();
        if (m_socket->state() != QAbstractSocket::UnconnectedState) {
            m_socket->waitForDisconnected(3000);
        }
    }
    qDebug() << "[网络管理器] NetworkManager析构完成";
}

void NetworkManager::setupConnection()
{
    // 创建TCP套接字
    m_socket = new QTcpSocket(this);
    
    // 连接信号槽
    connect(m_socket, &QTcpSocket::connected, this, &NetworkManager::onConnected);
    connect(m_socket, &QTcpSocket::disconnected, this, &NetworkManager::onDisconnected);
    connect(m_socket, &QTcpSocket::readyRead, this, &NetworkManager::onReadyRead);
    connect(m_socket, QOverload<QAbstractSocket::SocketError>::of(&QAbstractSocket::error),
            this, &NetworkManager::onSocketError);
    
    // 创建重连定时器
    m_reconnectTimer = new QTimer(this);
    m_reconnectTimer->setSingleShot(true);
    connect(m_reconnectTimer, &QTimer::timeout, this, &NetworkManager::onReconnectTimer);
    
    qDebug() << "[网络管理器] 网络连接设置完成";
}

void NetworkManager::connectToServer()
{
    if (m_socket->state() == QAbstractSocket::ConnectedState) {
        qDebug() << "[网络管理器] 已经连接到服务器";
        return;
    }
    
    if (m_socket->state() == QAbstractSocket::ConnectingState) {
        qDebug() << "[网络管理器] 正在连接服务器...";
        return;
    }
    
    qDebug() << "[网络管理器] 连接到服务器:" << m_serverAddress << ":" << m_serverPort;
    m_socket->connectToHost(m_serverAddress, m_serverPort);
}

void NetworkManager::disconnectFromServer()
{
    if (m_socket->state() == QAbstractSocket::ConnectedState) {
        qDebug() << "[网络管理器] 断开服务器连接";
        m_socket->disconnectFromHost();
    }
    
    if (m_reconnectTimer->isActive()) {
        m_reconnectTimer->stop();
    }
}

bool NetworkManager::isConnected() const
{
    return m_isConnected;
}

void NetworkManager::verifyRegistrationKey(const QString& key)
{
    qDebug() << "[网络管理器] 验证注册密钥:" << key;
    
    if (!m_isConnected) {
        qDebug() << "[网络管理器] 未连接到服务器，尝试连接...";
        connectToServer();
        // 连接成功后会自动重试，这里先返回错误
        emit keyVerificationResult(false, "网络未连接，请稍后重试");
        return;
    }
    
    QJsonObject message;
    message["type"] = "key_verify";
    message["key"] = key;
    
    sendJsonMessage(message);
}

void NetworkManager::sendRegistrationData(const QString& registrationKey, const UserInfo& userInfo, const QByteArray& faceData, 
                                        const QByteArray& fingerprintData, const QString& password)
{
    qDebug() << "[网络管理器] 发送用户注册数据:" << userInfo.name;
    
    if (!m_isConnected) {
        qDebug() << "[网络管理器] 未连接到服务器";
        emit registrationResult(false, "网络未连接，请检查网络设置", 0);
        return;
    }
    
    QJsonObject message;
    message["type"] = "user_register";
    
    // 注册密钥
    message["registration_key"] = registrationKey;
    
    // 用户信息
    QJsonObject userInfoObj;
    userInfoObj["name"] = userInfo.name;
    userInfoObj["phone"] = userInfo.phone;
    userInfoObj["address"] = userInfo.address;
    userInfoObj["sex"] = userInfo.sex;
    message["user_info"] = userInfoObj;
    
    // 密码
    message["password"] = password;
    
    // 人脸数据（Base64编码）
    if (!faceData.isEmpty()) {
        message["face_data"] = encodeBase64(faceData);
    }
    
    // 指纹数据（Base64编码）和指纹ID
    if (!fingerprintData.isEmpty()) {
        message["fingerprint_data"] = encodeBase64(fingerprintData);
        
        // 从指纹数据中提取硬件指纹ID（假设前4字节是ID）
        if (fingerprintData.size() >= 4) {
            int fingerprintId = *reinterpret_cast<const int*>(fingerprintData.constData());
            message["fingerprint_id"] = fingerprintId;
            qDebug() << "[网络管理器] 指纹ID:" << fingerprintId;
        }
    }
    
    sendJsonMessage(message);
}

void NetworkManager::sendFaceVerification(const QByteArray& faceImage)
{
    qDebug() << "[网络管理器] 发送人脸验证请求";
    
    if (!m_isConnected) {
        emit authenticationResult(false, UserInfo(), "网络未连接");
        return;
    }
    
    QJsonObject message;
    message["type"] = "face_verify";
    message["image_data"] = encodeBase64(faceImage);
    
    sendJsonMessage(message);
}

void NetworkManager::sendPasswordVerification(const QString& password)
{
    qDebug() << "[网络管理器] 发送密码验证请求";
    
    if (!m_isConnected) {
        emit authenticationResult(false, UserInfo(), "网络未连接");
        return;
    }
    
    QJsonObject message;
    message["type"] = "password_verify";
    message["password"] = password;
    
    sendJsonMessage(message);
}

void NetworkManager::sendFingerprintVerification(const QByteArray& fingerprintData)
{
    qDebug() << "[网络管理器] 发送指纹验证请求";
    
    if (!m_isConnected) {
        emit authenticationResult(false, UserInfo(), "网络未连接");
        return;
    }
    
    QJsonObject message;
    message["type"] = "fingerprint_verify";
    message["fingerprint_data"] = encodeBase64(fingerprintData);
    
    sendJsonMessage(message);
}

void NetworkManager::onConnected()
{
    qDebug() << "[网络管理器] 已连接到服务器";
    m_isConnected = true;
    m_reconnectAttempts = 0;
    
    if (m_reconnectTimer->isActive()) {
        m_reconnectTimer->stop();
    }
    
    emit connected();

    // 连接成功即同步时间（用于开发板时间校准）
    QJsonObject req;
    req["type"] = "time_sync";
    sendJsonMessage(req);
}

void NetworkManager::onDisconnected()
{
    qDebug() << "[网络管理器] 与服务器断开连接";
    m_isConnected = false;
    m_receiveBuffer.clear();
    
    emit disconnected();
    
    // 自动重连
    if (m_reconnectAttempts < MAX_RECONNECT_ATTEMPTS) {
        m_reconnectAttempts++;
        qDebug() << "[网络管理器] 将在" << RECONNECT_INTERVAL << "ms后重连 (尝试" << m_reconnectAttempts << "/" << MAX_RECONNECT_ATTEMPTS << ")";
        m_reconnectTimer->start(RECONNECT_INTERVAL);
    } else {
        qDebug() << "[网络管理器] 达到最大重连次数，停止重连";
        emit connectionError("无法连接到服务器，请检查网络设置");
    }
}

void NetworkManager::onReadyRead()
{
    QByteArray data = m_socket->readAll();
    m_receiveBuffer.append(data);
    
    qDebug() << "[网络管理器] 接收到数据:" << data.length() << "字节，缓冲区总大小:" << m_receiveBuffer.length();
    
    // 处理接收到的数据
    processReceivedData();
}

void NetworkManager::onSocketError(QAbstractSocket::SocketError socketError)
{
    QString errorString = m_socket->errorString();
    qDebug() << "[网络管理器] 套接字错误:" << socketError << errorString;
    
    m_isConnected = false;
    emit connectionError(errorString);
}

void NetworkManager::onReconnectTimer()
{
    qDebug() << "[网络管理器] 尝试重新连接...";
    connectToServer();
}

void NetworkManager::processReceivedData()
{
    // 尝试解析JSON消息
    // 处理可能的粘包问题：尝试找到完整的JSON对象
    
    int startPos = 0;
    while (startPos < m_receiveBuffer.length()) {
        // 跳过前导空白字符
        while (startPos < m_receiveBuffer.length() && 
               (m_receiveBuffer[startPos] == ' ' || 
                m_receiveBuffer[startPos] == '\n' || 
                m_receiveBuffer[startPos] == '\r' || 
                m_receiveBuffer[startPos] == '\t')) {
            startPos++;
        }
        
        if (startPos >= m_receiveBuffer.length()) {
            break;
        }
        
        // 查找JSON对象的结束位置
        int braceCount = 0;
        int endPos = startPos;
        bool inString = false;
        bool escape = false;
        
        for (; endPos < m_receiveBuffer.length(); endPos++) {
            char c = m_receiveBuffer[endPos];
            
            if (escape) {
                escape = false;
                continue;
            }
            
            if (c == '\\') {
                escape = true;
                continue;
            }
            
            if (c == '"') {
                inString = !inString;
                continue;
            }
            
            if (!inString) {
                if (c == '{') {
                    braceCount++;
                } else if (c == '}') {
                    braceCount--;
                    if (braceCount == 0) {
                        // 找到完整的JSON对象
                        endPos++;
                        break;
                    }
                }
            }
        }
        
        if (braceCount != 0) {
            // 没有找到完整的JSON对象，等待更多数据
            qDebug() << "[网络管理器] JSON对象不完整，等待更多数据";
            break;
        }
        
        // 提取JSON消息
        QByteArray jsonData = m_receiveBuffer.mid(startPos, endPos - startPos);
        
        // 解析JSON
        QJsonObject jsonObj = parseJsonMessage(jsonData);
        if (!jsonObj.isEmpty()) {
            QString messageType = jsonObj["type"].toString();
            qDebug() << "[网络管理器] 收到消息类型:" << messageType;
            
            if (messageType == "key_verify_result") {
                handleKeyVerificationResponse(jsonObj);
            } else if (messageType == "user_register_result") {
                handleRegistrationResponse(jsonObj);
            } else if (messageType == "auth_result") {
                handleAuthenticationResponse(jsonObj);
            } else if (messageType == "time_sync_result") {
                handleTimeSyncResponse(jsonObj);
            } else {
                qDebug() << "[网络管理器] 未知消息类型:" << messageType;
            }
        }
        
        // 移动到下一个消息
        startPos = endPos;
    }
    
    // 移除已处理的数据
    if (startPos > 0) {
        m_receiveBuffer.remove(0, startPos);
    }
}

void NetworkManager::sendJsonMessage(const QJsonObject& message)
{
    QJsonDocument doc(message);
    QByteArray data = doc.toJson(QJsonDocument::Compact);
    
    qDebug() << "[网络管理器] 发送消息:" << data;
    
    if (m_socket->state() == QAbstractSocket::ConnectedState) {
        m_socket->write(data);
        m_socket->flush();
    } else {
        qDebug() << "[网络管理器] 套接字未连接，无法发送消息";
    }
}

QJsonObject NetworkManager::parseJsonMessage(const QByteArray& data)
{
    QJsonParseError error;
    QJsonDocument doc = QJsonDocument::fromJson(data, &error);
    
    if (error.error != QJsonParseError::NoError) {
        qDebug() << "[网络管理器] JSON解析错误:" << error.errorString();
        return QJsonObject();
    }
    
    return doc.object();
}

void NetworkManager::handleKeyVerificationResponse(const QJsonObject& response)
{
    bool success = response["success"].toBool();
    QString message = response["message"].toString();
    
    qDebug() << "[网络管理器] 密钥验证结果:" << success << message;
    emit keyVerificationResult(success, message);
}

void NetworkManager::handleRegistrationResponse(const QJsonObject& response)
{
    bool success = response["success"].toBool();
    QString message = response["message"].toString();
    
    if (success) {
        int userId = response["user_id"].toInt();
        int fingerprintId = response["fingerprint_id"].toInt();
        qDebug() << "[网络管理器] 注册成功，用户ID:" << userId << "指纹ID:" << fingerprintId << "消息:" << message;
        
        // 发送注册成功信号，包含指纹ID
        emit registrationResult(success, message, fingerprintId);
    } else {
        qDebug() << "[网络管理器] 注册失败:" << message;
        emit registrationResult(success, message, 0);
    }
}

void NetworkManager::handleAuthenticationResponse(const QJsonObject& response)
{
    bool success = response["success"].toBool();
    QString message = response["message"].toString();
    
    UserInfo userInfo;
    if (success && response.contains("user_info")) {
        QJsonObject userInfoObj = response["user_info"].toObject();
        userInfo.name = userInfoObj["name"].toString();
        userInfo.phone = userInfoObj["phone"].toString();
        userInfo.address = userInfoObj["address"].toString();
        userInfo.sex = userInfoObj["sex"].toString();
        
        qDebug() << "[网络管理器] 认证成功，用户:" << userInfo.name;
    } else {
        qDebug() << "[网络管理器] 认证失败:" << message;
    }
    
    emit authenticationResult(success, userInfo, message);
}

void NetworkManager::handleTimeSyncResponse(const QJsonObject& response)
{
    const qint64 epochMs = response.value("server_epoch_ms").toVariant().toLongLong();
    const QString serverTimeStr = response.value("server_time").toString();

    QDateTime serverDt;
    if (epochMs > 0) {
        serverDt = QDateTime::fromMSecsSinceEpoch(epochMs, Qt::LocalTime);
    } else if (!serverTimeStr.isEmpty()) {
        serverDt = QDateTime::fromString(serverTimeStr, "yyyy-MM-dd HH:mm:ss");
        serverDt.setTimeSpec(Qt::LocalTime);
    }

    if (!serverDt.isValid()) {
        qWarning() << "[时间同步] 服务器返回时间无效:" << serverTimeStr << epochMs;
        return;
    }

    qDebug() << "[时间同步] 收到服务器时间:" << serverDt.toString("yyyy-MM-dd HH:mm:ss");

#if defined(Q_OS_LINUX)
    // 需要 root 权限；失败会在日志中体现
    const QString target = serverDt.toString("yyyy-MM-dd HH:mm:ss");
    int rc = QProcess::execute("date", {"-s", target});
    if (rc == 0) {
        qDebug() << "[时间同步] ✓ 已设置系统时间为:" << target;
        // 尝试同步硬件时钟（若系统支持）
        QProcess::execute("hwclock", {"-w"});
    } else {
        qWarning() << "[时间同步] ✗ 设置系统时间失败，返回码:" << rc
                   << "（可能需要root权限）";
    }
#else
    qDebug() << "[时间同步] 非Linux环境不自动设置系统时间，仅记录服务器时间";
#endif
}

QString NetworkManager::encodeBase64(const QByteArray& data)
{
    return data.toBase64();
}

QByteArray NetworkManager::decodeBase64(const QString& data)
{
    return QByteArray::fromBase64(data.toUtf8());
}
