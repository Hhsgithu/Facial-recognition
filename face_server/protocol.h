#ifndef PROTOCOL_H
#define PROTOCOL_H

#include <QString>
#include <QJsonObject>
#include <QJsonDocument>

/**
 * @file protocol.h
 * @brief 多模态门禁系统通信协议定义
 * 
 * 定义客户端与服务器之间的JSON消息格式，包括注册相关的所有消息类型。
 * 符合需求9.2和9.3的规范。
 */

namespace Protocol {

// ============================================================================
// 消息类型常量定义
// ============================================================================

namespace MessageType {
    // 注册相关消息类型
    const QString KEY_VERIFY = "key_verify";                    // 注册密钥验证请求
    const QString KEY_VERIFY_RESULT = "key_verify_result";      // 注册密钥验证响应
    const QString USER_REGISTER = "user_register";              // 用户注册请求
    const QString USER_REGISTER_RESULT = "user_register_result"; // 用户注册响应
    
    // 认证相关消息类型
    const QString FACE_VERIFY = "face_verify";                  // 人脸认证请求
    const QString PASSWORD_VERIFY = "password_verify";          // 密码认证请求
    const QString FINGERPRINT_VERIFY = "fingerprint_verify";    // 指纹认证请求
    const QString AUTH_RESULT = "auth_result";                  // 认证响应
}

// ============================================================================
// 注册密钥验证消息
// ============================================================================

/**
 * @brief 注册密钥验证请求消息
 * 
 * 客户端发送给服务器验证注册密钥有效性的请求。
 * 对应需求1.2：WHEN 用户输入注册密钥时，THE Client SHALL 将密钥发送到Server进行验证
 */
struct KeyVerifyRequest {
    QString type = MessageType::KEY_VERIFY;  // 消息类型
    QString key;                             // 注册密钥
    
    /**
     * @brief 转换为JSON对象
     * @return QJsonObject JSON格式的消息
     */
    QJsonObject toJson() const {
        QJsonObject obj;
        obj["type"] = type;
        obj["key"] = key;
        return obj;
    }
    
    /**
     * @brief 从JSON对象创建请求
     * @param json JSON对象
     * @return KeyVerifyRequest 请求对象
     */
    static KeyVerifyRequest fromJson(const QJsonObject& json) {
        KeyVerifyRequest request;
        request.type = json["type"].toString();
        request.key = json["key"].toString();
        return request;
    }
};

/**
 * @brief 注册密钥验证响应消息
 * 
 * 服务器返回给客户端的密钥验证结果。
 * 对应需求1.13：IF 注册密钥无效或过期，THEN THE Client SHALL 显示错误提示并拒绝注册
 */
struct KeyVerifyResponse {
    QString type = MessageType::KEY_VERIFY_RESULT;  // 消息类型
    bool success;                                   // 验证是否成功
    QString message;                                // 响应消息
    
    /**
     * @brief 转换为JSON对象
     * @return QJsonObject JSON格式的消息
     */
    QJsonObject toJson() const {
        QJsonObject obj;
        obj["type"] = type;
        obj["success"] = success;
        obj["message"] = message;
        return obj;
    }
    
    /**
     * @brief 从JSON对象创建响应
     * @param json JSON对象
     * @return KeyVerifyResponse 响应对象
     */
    static KeyVerifyResponse fromJson(const QJsonObject& json) {
        KeyVerifyResponse response;
        response.type = json["type"].toString();
        response.success = json["success"].toBool();
        response.message = json["message"].toString();
        return response;
    }
};

// ============================================================================
// 用户注册消息
// ============================================================================

/**
 * @brief 用户信息结构
 * 
 * 用户注册时的基本信息。
 * 对应需求1.4：WHEN 用户填写基本信息（姓名、电话、地址）时，THE Client SHALL 验证信息完整性
 */
struct UserInfo {
    QString name;     // 用户姓名
    QString phone;    // 电话号码
    QString address;  // 地址
    QString sex;      // 性别："男" 或 "女"
    
    /**
     * @brief 转换为JSON对象
     * @return QJsonObject JSON格式的用户信息
     */
    QJsonObject toJson() const {
        QJsonObject obj;
        obj["name"] = name;
        obj["phone"] = phone;
        obj["address"] = address;
        obj["sex"] = sex;
        return obj;
    }
    
    /**
     * @brief 从JSON对象创建用户信息
     * @param json JSON对象
     * @return UserInfo 用户信息对象
     */
    static UserInfo fromJson(const QJsonObject& json) {
        UserInfo info;
        info.name = json["name"].toString();
        info.phone = json["phone"].toString();
        info.address = json["address"].toString();
        info.sex = json["sex"].toString();
        return info;
    }
    
    /**
     * @brief 验证用户信息完整性
     * @return bool 信息是否完整
     */
    bool isValid() const {
        return !name.trimmed().isEmpty() && 
               !phone.trimmed().isEmpty() && 
               !address.trimmed().isEmpty() &&
               !sex.trimmed().isEmpty();
    }
};

/**
 * @brief 用户注册请求消息
 * 
 * 客户端发送给服务器的完整用户注册数据。
 * 对应需求1.10：WHEN 所有注册信息完成时，THE Server SHALL 将用户信息存储到数据库
 */
struct UserRegisterRequest {
    QString type = MessageType::USER_REGISTER;  // 消息类型
    QString registrationKey;                    // 注册密钥
    UserInfo userInfo;                          // 用户基本信息
    QString password;                           // 登录密码
    QString faceData;                           // 人脸数据（Base64编码）
    QString fingerprintData;                    // 指纹数据（Base64编码）
    
    /**
     * @brief 转换为JSON对象
     * @return QJsonObject JSON格式的消息
     */
    QJsonObject toJson() const {
        QJsonObject obj;
        obj["type"] = type;
        obj["registration_key"] = registrationKey;
        obj["user_info"] = userInfo.toJson();
        obj["password"] = password;
        
        if (!faceData.isEmpty()) {
            obj["face_data"] = faceData;
        }
        
        if (!fingerprintData.isEmpty()) {
            obj["fingerprint_data"] = fingerprintData;
        }
        
        return obj;
    }
    
    /**
     * @brief 从JSON对象创建请求
     * @param json JSON对象
     * @return UserRegisterRequest 请求对象
     */
    static UserRegisterRequest fromJson(const QJsonObject& json) {
        UserRegisterRequest request;
        request.type = json["type"].toString();
        request.registrationKey = json["registration_key"].toString();
        request.userInfo = UserInfo::fromJson(json["user_info"].toObject());
        request.password = json["password"].toString();
        request.faceData = json["face_data"].toString();
        request.fingerprintData = json["fingerprint_data"].toString();
        return request;
    }
    
    /**
     * @brief 验证注册请求数据完整性
     * @return bool 数据是否完整有效
     */
    bool isValid() const {
        return !registrationKey.isEmpty() && 
               userInfo.isValid() && 
               !password.isEmpty();
    }
};

/**
 * @brief 用户注册响应消息
 * 
 * 服务器返回给客户端的注册结果。
 * 对应需求1.11：WHEN 注册成功时，THE Server SHALL 向Client返回注册成功确认
 * 对应需求1.12：WHEN Client收到注册成功确认时，THE Client SHALL 显示注册成功提示并返回首页
 */
struct UserRegisterResponse {
    QString type = MessageType::USER_REGISTER_RESULT;  // 消息类型
    bool success;                                      // 注册是否成功
    int userId;                                        // 用户ID（成功时返回）
    QString message;                                   // 响应消息
    
    /**
     * @brief 转换为JSON对象
     * @return QJsonObject JSON格式的消息
     */
    QJsonObject toJson() const {
        QJsonObject obj;
        obj["type"] = type;
        obj["success"] = success;
        obj["user_id"] = userId;
        obj["message"] = message;
        return obj;
    }
    
    /**
     * @brief 从JSON对象创建响应
     * @param json JSON对象
     * @return UserRegisterResponse 响应对象
     */
    static UserRegisterResponse fromJson(const QJsonObject& json) {
        UserRegisterResponse response;
        response.type = json["type"].toString();
        response.success = json["success"].toBool();
        response.userId = json["user_id"].toInt();
        response.message = json["message"].toString();
        return response;
    }
};

// ============================================================================
// 认证相关消息（为完整性包含，但不是本任务重点）
// ============================================================================

/**
 * @brief 人脸认证请求消息
 * 
 * 对应需求3.3：WHEN 图像预处理完成时，THE Client SHALL 将处理后的人脸图像数据发送到Server
 */
struct FaceVerifyRequest {
    QString type = MessageType::FACE_VERIFY;  // 消息类型
    QString imageData;                        // 人脸图像数据（Base64编码）
    
    QJsonObject toJson() const {
        QJsonObject obj;
        obj["type"] = type;
        obj["image_data"] = imageData;
        return obj;
    }
    
    static FaceVerifyRequest fromJson(const QJsonObject& json) {
        FaceVerifyRequest request;
        request.type = json["type"].toString();
        request.imageData = json["image_data"].toString();
        return request;
    }
};

/**
 * @brief 密码认证请求消息
 * 
 * 对应需求4.3：WHEN 用户点击确认时，THE Client SHALL 将输入的密码发送到Server进行验证
 */
struct PasswordVerifyRequest {
    QString type = MessageType::PASSWORD_VERIFY;  // 消息类型
    QString password;                             // 密码
    
    QJsonObject toJson() const {
        QJsonObject obj;
        obj["type"] = type;
        obj["password"] = password;
        return obj;
    }
    
    static PasswordVerifyRequest fromJson(const QJsonObject& json) {
        PasswordVerifyRequest request;
        request.type = json["type"].toString();
        request.password = json["password"].toString();
        return request;
    }
};

/**
 * @brief 指纹认证请求消息
 * 
 * 对应需求5.4：WHEN 指纹数据采集完成时，THE Client SHALL 将指纹数据发送到Server进行比对
 */
struct FingerprintVerifyRequest {
    QString type = MessageType::FINGERPRINT_VERIFY;  // 消息类型
    QString fingerprintData;                         // 指纹数据（Base64编码）
    
    QJsonObject toJson() const {
        QJsonObject obj;
        obj["type"] = type;
        obj["fingerprint_data"] = fingerprintData;
        return obj;
    }
    
    static FingerprintVerifyRequest fromJson(const QJsonObject& json) {
        FingerprintVerifyRequest request;
        request.type = json["type"].toString();
        request.fingerprintData = json["fingerprint_data"].toString();
        return request;
    }
};

/**
 * @brief 认证响应消息
 * 
 * 服务器返回的认证结果，适用于人脸、密码、指纹认证。
 */
struct AuthResponse {
    QString type = MessageType::AUTH_RESULT;  // 消息类型
    bool success;                             // 认证是否成功
    UserInfo userInfo;                        // 用户信息（成功时返回）
    QString message;                          // 响应消息
    
    QJsonObject toJson() const {
        QJsonObject obj;
        obj["type"] = type;
        obj["success"] = success;
        obj["message"] = message;
        
        if (success) {
            obj["user_info"] = userInfo.toJson();
        }
        
        return obj;
    }
    
    static AuthResponse fromJson(const QJsonObject& json) {
        AuthResponse response;
        response.type = json["type"].toString();
        response.success = json["success"].toBool();
        response.message = json["message"].toString();
        
        if (json.contains("user_info")) {
            response.userInfo = UserInfo::fromJson(json["user_info"].toObject());
        }
        
        return response;
    }
};

// ============================================================================
// 工具函数
// ============================================================================

/**
 * @brief 将QJsonObject转换为紧凑的JSON字符串
 * @param obj JSON对象
 * @return QByteArray JSON字符串的字节数组
 */
inline QByteArray toJsonBytes(const QJsonObject& obj) {
    QJsonDocument doc(obj);
    return doc.toJson(QJsonDocument::Compact);
}

/**
 * @brief 从JSON字节数组解析为QJsonObject
 * @param data JSON字节数组
 * @return QJsonObject 解析后的JSON对象
 */
inline QJsonObject fromJsonBytes(const QByteArray& data) {
    QJsonParseError error;
    QJsonDocument doc = QJsonDocument::fromJson(data, &error);
    
    if (error.error != QJsonParseError::NoError) {
        return QJsonObject();
    }
    
    return doc.object();
}

/**
 * @brief 验证消息类型是否为注册相关消息
 * @param messageType 消息类型
 * @return bool 是否为注册相关消息
 */
inline bool isRegistrationMessage(const QString& messageType) {
    return messageType == MessageType::KEY_VERIFY ||
           messageType == MessageType::KEY_VERIFY_RESULT ||
           messageType == MessageType::USER_REGISTER ||
           messageType == MessageType::USER_REGISTER_RESULT;
}

/**
 * @brief 验证消息类型是否为认证相关消息
 * @param messageType 消息类型
 * @return bool 是否为认证相关消息
 */
inline bool isAuthenticationMessage(const QString& messageType) {
    return messageType == MessageType::FACE_VERIFY ||
           messageType == MessageType::PASSWORD_VERIFY ||
           messageType == MessageType::FINGERPRINT_VERIFY ||
           messageType == MessageType::AUTH_RESULT;
}

} // namespace Protocol

#endif // PROTOCOL_H