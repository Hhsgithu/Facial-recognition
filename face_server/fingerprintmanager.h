#ifndef FINGERPRINTMANAGER_H
#define FINGERPRINTMANAGER_H

#include <QObject>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QString>
#include <QStringList>
#include <QByteArray>
#include <QDateTime>
#include <QDebug>

/**
 * FingerprintManager - 指纹管理器
 * 处理指纹相关的服务器端业务逻辑
 * 
 * 功能包括:
 * - 指纹验证逻辑
 * - 用户ID到用户信息的查询
 * - 考勤记录日志功能
 * - 指纹注册管理
 * - 数据库操作
 */
class FingerprintManager : public QObject
{
    Q_OBJECT

public:
    explicit FingerprintManager(QObject *parent = nullptr);
    ~FingerprintManager();
    
    void setDatabase(QSqlDatabase* db);
    
    // 指纹验证结果结构
    struct VerificationResult {
        bool success;
        QString userId;
        QString userName;
        QString department;
        QString message;
        
        VerificationResult() : success(false) {}
        VerificationResult(bool s, const QString& uid, const QString& uname, 
                          const QString& dept, const QString& msg)
            : success(s), userId(uid), userName(uname), department(dept), message(msg) {}
    };
    
    // 指纹验证 - 根据指纹模块返回的用户ID进行验证
    // fingerprintId: 指纹模块返回的指纹ID（用于记录）
    VerificationResult verifyFingerprint(int userId, int fingerprintId);
    
    // 指纹注册管理
    bool registerFingerprint(const QString& employeeId, const QByteArray& fingerprintData);
    bool registerFingerprintFromNetwork(const QString& employeeId, const QString& base64FingerprintData);
    bool deleteFingerprint(const QString& employeeId);
    QStringList getRegisteredUsers();
    int getNextAvailableFingerprintId();
    
    // 数据库操作
    bool createFingerprintTable();
    bool logAttendance(const QString& employeeId, const QString& result);
    
signals:
    void verificationCompleted(const VerificationResult& result);
    void registrationCompleted(bool success, const QString& message);
    
private:
    QSqlDatabase* database;
    
    // 辅助方法：获取数据库连接
    QSqlDatabase getDatabase() const;
    
    bool isValidUserId(int userId);
    QString getUserInfo(int userId);
    bool isEmployeeExists(const QString& employeeId);
    bool isFingerprintRegistered(const QString& employeeId);
    bool isFingerprintIdUsed(int fingerprintId);
};

#endif // FINGERPRINTMANAGER_H