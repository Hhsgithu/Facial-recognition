#include "registrationkeymanager.h"

RegistrationKeyManager::RegistrationKeyManager(QObject *parent)
    : QObject(parent)
{
    qDebug() << "[RegistrationKeyManager] 初始化注册密钥管理器";
}

QString RegistrationKeyManager::generateRegistrationKey(int validHours, int maxUses)
{
    qDebug() << "[RegistrationKeyManager] 生成注册密钥，有效期:" << validHours << "小时，最大使用次数:" << maxUses;
    
    // 生成随机密钥
    QString keyValue = generateRandomKey();
    
    // 计算过期时间
    QDateTime expiresAt = QDateTime::currentDateTime().addSecs(validHours * 3600);
    
    // 插入数据库
    QSqlQuery query;
    query.prepare("INSERT INTO registration_keys (key_value, max_uses, used_count, expires_at, is_active) "
                  "VALUES (?, ?, 0, ?, TRUE)");
    query.bindValue(0, keyValue);
    query.bindValue(1, maxUses);
    query.bindValue(2, expiresAt);
    
    if (!query.exec()) {
        qDebug() << "[错误] 注册密钥插入数据库失败:" << query.lastError().text();
        return QString();
    }
    
    qDebug() << "[RegistrationKeyManager] ✓ 注册密钥生成成功:" << keyValue;
    qDebug() << "  过期时间:" << expiresAt.toString("yyyy-MM-dd hh:mm:ss");
    qDebug() << "  最大使用次数:" << maxUses;
    
    emit keyGenerated(keyValue, validHours, maxUses);
    logKeyOperation("GENERATE", keyValue, QString("有效期%1小时，最大使用%2次").arg(validHours).arg(maxUses));
    
    return keyValue;
}

bool RegistrationKeyManager::generateCustomRegistrationKey(const QString& keyValue, int validHours, int maxUses)
{
    qDebug() << "[RegistrationKeyManager] 生成自定义注册密钥:" << keyValue;
    qDebug() << "  有效期:" << validHours << "小时";
    qDebug() << "  最大使用次数:" << maxUses;
    
    // 检查密钥是否已存在
    QSqlQuery checkQuery;
    checkQuery.prepare("SELECT id FROM registration_keys WHERE UPPER(key_value) = ?");
    checkQuery.bindValue(0, keyValue.toUpper());
    
    if (checkQuery.exec() && checkQuery.next()) {
        qDebug() << "[警告] 密钥已存在，将删除旧密钥";
        // 删除旧密钥
        QSqlQuery deleteQuery;
        deleteQuery.prepare("DELETE FROM registration_keys WHERE UPPER(key_value) = ?");
        deleteQuery.bindValue(0, keyValue.toUpper());
        deleteQuery.exec();
    }
    
    // 计算过期时间
    QDateTime expiresAt = QDateTime::currentDateTime().addSecs(validHours * 3600);
    
    // 插入数据库
    QSqlQuery query;
    query.prepare("INSERT INTO registration_keys (key_value, max_uses, used_count, expires_at, is_active) "
                  "VALUES (?, ?, 0, ?, TRUE)");
    query.bindValue(0, keyValue);
    query.bindValue(1, maxUses);
    query.bindValue(2, expiresAt);
    
    if (!query.exec()) {
        qDebug() << "[错误] 自定义注册密钥插入数据库失败:" << query.lastError().text();
        return false;
    }
    
    qDebug() << "[RegistrationKeyManager] ✓ 自定义注册密钥生成成功:" << keyValue;
    qDebug() << "  过期时间:" << expiresAt.toString("yyyy-MM-dd hh:mm:ss");
    qDebug() << "  最大使用次数:" << maxUses;
    
    emit keyGenerated(keyValue, validHours, maxUses);
    logKeyOperation("GENERATE_CUSTOM", keyValue, QString("有效期%1小时，最大使用%2次").arg(validHours).arg(maxUses));
    
    return true;
}

bool RegistrationKeyManager::verifyRegistrationKey(const QString& keyValue)
{
    // 转换为大写进行验证（不区分大小写）
    QString normalizedKey = keyValue.toUpper().trimmed();
    
    qDebug() << "[RegistrationKeyManager] 验证注册密钥:" << normalizedKey << "(原始:" << keyValue << ")";
    
    if (normalizedKey.isEmpty()) {
        qDebug() << "[RegistrationKeyManager] 密钥为空";
        return false;
    }
    
    // 查询密钥信息（使用UPPER()函数进行大小写不敏感匹配）
    QSqlQuery query;
    query.prepare("SELECT id, max_uses, used_count, expires_at, is_active "
                  "FROM registration_keys WHERE UPPER(key_value) = ?");
    query.bindValue(0, normalizedKey);
    
    if (!query.exec()) {
        qDebug() << "[错误] 查询注册密钥失败:" << query.lastError().text();
        return false;
    }
    
    if (!query.next()) {
        qDebug() << "[RegistrationKeyManager] 密钥不存在:" << normalizedKey;
        logKeyOperation("VERIFY_FAIL", normalizedKey, "密钥不存在");
        return false;
    }
    
    // 获取密钥信息
    int maxUses = query.value("max_uses").toInt();
    int usedCount = query.value("used_count").toInt();
    QDateTime expiresAt = query.value("expires_at").toDateTime();
    bool isActive = query.value("is_active").toBool();
    
    // 检查密钥状态
    if (!isActive) {
        qDebug() << "[RegistrationKeyManager] 密钥已被禁用:" << normalizedKey;
        logKeyOperation("VERIFY_FAIL", normalizedKey, "密钥已禁用");
        return false;
    }
    
    if (QDateTime::currentDateTime() > expiresAt) {
        qDebug() << "[RegistrationKeyManager] 密钥已过期:" << normalizedKey;
        qDebug() << "  过期时间:" << expiresAt.toString("yyyy-MM-dd hh:mm:ss");
        logKeyOperation("VERIFY_FAIL", normalizedKey, "密钥已过期");
        emit keyExpired(normalizedKey);
        return false;
    }
    
    if (usedCount >= maxUses) {
        qDebug() << "[RegistrationKeyManager] 密钥使用次数已达上限:" << normalizedKey;
        qDebug() << "  已使用:" << usedCount << "最大使用:" << maxUses;
        logKeyOperation("VERIFY_FAIL", normalizedKey, "使用次数已达上限");
        return false;
    }
    
    // 密钥验证成功，增加使用次数（使用大写格式更新）
    if (useRegistrationKey(normalizedKey)) {
        int remainingUses = maxUses - (usedCount + 1);
        qDebug() << "[RegistrationKeyManager] ✓ 密钥验证成功:" << normalizedKey;
        qDebug() << "  剩余使用次数:" << remainingUses;
        
        emit keyUsed(normalizedKey, remainingUses);
        logKeyOperation("VERIFY_SUCCESS", normalizedKey, QString("剩余使用次数: %1").arg(remainingUses));
        return true;
    }
    
    return false;
}

QList<RegistrationKey> RegistrationKeyManager::getAllRegistrationKeys()
{
    QList<RegistrationKey> keys;
    
    QSqlQuery query;
    query.prepare("SELECT id, key_value, max_uses, used_count, expires_at, created_at, is_active "
                  "FROM registration_keys ORDER BY created_at DESC");
    
    if (!query.exec()) {
        qDebug() << "[错误] 查询所有注册密钥失败:" << query.lastError().text();
        return keys;
    }
    
    while (query.next()) {
        RegistrationKey key;
        key.id = query.value("id").toInt();
        key.keyValue = query.value("key_value").toString();
        key.maxUses = query.value("max_uses").toInt();
        key.usedCount = query.value("used_count").toInt();
        key.expiresAt = query.value("expires_at").toDateTime();
        key.createdAt = query.value("created_at").toDateTime();
        key.isActive = query.value("is_active").toBool();
        
        keys.append(key);
    }
    
    qDebug() << "[RegistrationKeyManager] 查询到" << keys.size() << "个注册密钥";
    return keys;
}

QList<RegistrationKey> RegistrationKeyManager::getActiveRegistrationKeys()
{
    QList<RegistrationKey> keys;
    
    QSqlQuery query;
    query.prepare("SELECT id, key_value, max_uses, used_count, expires_at, created_at, is_active "
                  "FROM registration_keys WHERE is_active = TRUE AND expires_at > datetime('now') "
                  "ORDER BY created_at DESC");
    
    if (!query.exec()) {
        qDebug() << "[错误] 查询活跃注册密钥失败:" << query.lastError().text();
        return keys;
    }
    
    while (query.next()) {
        RegistrationKey key;
        key.id = query.value("id").toInt();
        key.keyValue = query.value("key_value").toString();
        key.maxUses = query.value("max_uses").toInt();
        key.usedCount = query.value("used_count").toInt();
        key.expiresAt = query.value("expires_at").toDateTime();
        key.createdAt = query.value("created_at").toDateTime();
        key.isActive = query.value("is_active").toBool();
        
        keys.append(key);
    }
    
    qDebug() << "[RegistrationKeyManager] 查询到" << keys.size() << "个活跃注册密钥";
    return keys;
}

bool RegistrationKeyManager::deactivateRegistrationKey(const QString& keyValue)
{
    qDebug() << "[RegistrationKeyManager] 禁用注册密钥:" << keyValue;
    
    // 使用UPPER()函数进行大小写不敏感匹配
    QSqlQuery query;
    query.prepare("UPDATE registration_keys SET is_active = FALSE WHERE UPPER(key_value) = ?");
    query.bindValue(0, keyValue.toUpper());
    
    if (!query.exec()) {
        qDebug() << "[错误] 禁用注册密钥失败:" << query.lastError().text();
        return false;
    }
    
    if (query.numRowsAffected() > 0) {
        qDebug() << "[RegistrationKeyManager] ✓ 注册密钥已禁用:" << keyValue;
        logKeyOperation("DEACTIVATE", keyValue);
        return true;
    } else {
        qDebug() << "[RegistrationKeyManager] 密钥不存在或已禁用:" << keyValue;
        return false;
    }
}

bool RegistrationKeyManager::deleteRegistrationKey(const QString& keyValue)
{
    qDebug() << "[RegistrationKeyManager] 删除注册密钥:" << keyValue;
    
    // 使用UPPER()函数进行大小写不敏感匹配
    QSqlQuery query;
    query.prepare("DELETE FROM registration_keys WHERE UPPER(key_value) = ?");
    query.bindValue(0, keyValue.toUpper());
    
    if (!query.exec()) {
        qDebug() << "[错误] 删除注册密钥失败:" << query.lastError().text();
        return false;
    }
    
    if (query.numRowsAffected() > 0) {
        qDebug() << "[RegistrationKeyManager] ✓ 注册密钥已删除:" << keyValue;
        logKeyOperation("DELETE", keyValue);
        return true;
    } else {
        qDebug() << "[RegistrationKeyManager] 密钥不存在:" << keyValue;
        return false;
    }
}

int RegistrationKeyManager::cleanupExpiredKeys()
{
    qDebug() << "[RegistrationKeyManager] 清理过期注册密钥";
    
    QSqlQuery query;
    query.prepare("DELETE FROM registration_keys WHERE expires_at <= datetime('now')");
    
    if (!query.exec()) {
        qDebug() << "[错误] 清理过期密钥失败:" << query.lastError().text();
        return -1;
    }
    
    int deletedCount = query.numRowsAffected();
    qDebug() << "[RegistrationKeyManager] ✓ 清理了" << deletedCount << "个过期密钥";
    
    if (deletedCount > 0) {
        logKeyOperation("CLEANUP", "", QString("清理了%1个过期密钥").arg(deletedCount));
    }
    
    return deletedCount;
}

RegistrationKey RegistrationKeyManager::getRegistrationKeyInfo(const QString& keyValue)
{
    RegistrationKey key;
    
    // 使用UPPER()函数进行大小写不敏感匹配
    QSqlQuery query;
    query.prepare("SELECT id, key_value, max_uses, used_count, expires_at, created_at, is_active "
                  "FROM registration_keys WHERE UPPER(key_value) = ?");
    query.bindValue(0, keyValue.toUpper());
    
    if (!query.exec()) {
        qDebug() << "[错误] 查询注册密钥信息失败:" << query.lastError().text();
        return key;
    }
    
    if (query.next()) {
        key.id = query.value("id").toInt();
        key.keyValue = query.value("key_value").toString();
        key.maxUses = query.value("max_uses").toInt();
        key.usedCount = query.value("used_count").toInt();
        key.expiresAt = query.value("expires_at").toDateTime();
        key.createdAt = query.value("created_at").toDateTime();
        key.isActive = query.value("is_active").toBool();
    }
    
    return key;
}

QString RegistrationKeyManager::generateRandomKey()
{
    // 字符集：A-Z 和 0-9（36个字符）
    const QString charset = "ABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789";
    const int keyLength = 8;
    
    QString keyValue;
    keyValue.reserve(keyLength);
    
    // 生成8位随机密钥
    for (int i = 0; i < keyLength; ++i) {
        int index = QRandomGenerator::global()->bounded(charset.length());
        keyValue.append(charset[index]);
    }
    
    qDebug() << "[RegistrationKeyManager] 生成8位密钥:" << keyValue;
    
    return keyValue;  // 返回8位大写字母数字组合，如 "A3K9M2P7"
}

bool RegistrationKeyManager::useRegistrationKey(const QString& keyValue)
{
    // 使用UPPER()函数进行大小写不敏感更新
    QSqlQuery query;
    query.prepare("UPDATE registration_keys SET used_count = used_count + 1 WHERE UPPER(key_value) = ?");
    query.bindValue(0, keyValue.toUpper());
    
    if (!query.exec()) {
        qDebug() << "[错误] 更新密钥使用次数失败:" << query.lastError().text();
        return false;
    }
    
    return query.numRowsAffected() > 0;
}

void RegistrationKeyManager::logKeyOperation(const QString& operation, const QString& keyValue, const QString& details)
{
    // 这里可以添加操作日志记录到数据库或文件
    QString logMessage = QString("[%1] %2: %3")
                        .arg(QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss"))
                        .arg(operation)
                        .arg(keyValue);
    
    if (!details.isEmpty()) {
        logMessage += " - " + details;
    }
    
    qDebug() << "[KeyLog]" << logMessage;
}