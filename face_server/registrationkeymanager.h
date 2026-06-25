#ifndef REGISTRATIONKEYMANAGER_H
#define REGISTRATIONKEYMANAGER_H

#include <QObject>
#include <QString>
#include <QDateTime>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>
#include <QCryptographicHash>
#include <QRandomGenerator>

struct RegistrationKey {
    int id;
    QString keyValue;
    int maxUses;
    int usedCount;
    QDateTime expiresAt;
    QDateTime createdAt;
    bool isActive;
    
    // 便利方法
    bool isExpired() const {
        return QDateTime::currentDateTime() > expiresAt;
    }
    
    bool isUsable() const {
        return isActive && !isExpired() && usedCount < maxUses;
    }
    
    int remainingUses() const {
        return qMax(0, maxUses - usedCount);
    }
};

class RegistrationKeyManager : public QObject
{
    Q_OBJECT

public:
    explicit RegistrationKeyManager(QObject *parent = nullptr);
    
    // 密钥生成
    QString generateRegistrationKey(int validHours = 24, int maxUses = 5);
    
    // 生成指定密钥值的注册密钥（用于测试/调试）
    bool generateCustomRegistrationKey(const QString& keyValue, int validHours = 24, int maxUses = 5);
    
    // 密钥验证
    bool verifyRegistrationKey(const QString& keyValue);
    
    // 密钥管理
    QList<RegistrationKey> getAllRegistrationKeys();
    QList<RegistrationKey> getActiveRegistrationKeys();
    bool deactivateRegistrationKey(const QString& keyValue);
    bool deleteRegistrationKey(const QString& keyValue);
    
    // 清理过期密钥
    int cleanupExpiredKeys();
    
    // 获取密钥详细信息
    RegistrationKey getRegistrationKeyInfo(const QString& keyValue);

signals:
    void keyGenerated(const QString& keyValue, int validHours, int maxUses);
    void keyUsed(const QString& keyValue, int remainingUses);
    void keyExpired(const QString& keyValue);

private:
    QString generateRandomKey();
    bool useRegistrationKey(const QString& keyValue);
    void logKeyOperation(const QString& operation, const QString& keyValue, const QString& details = "");
};

#endif // REGISTRATIONKEYMANAGER_H