#include "fingerprintmanager.h"

FingerprintManager::FingerprintManager(QObject *parent)
    : QObject(parent), database(nullptr)
{
    qDebug() << "[FingerprintManager] 指纹管理器初始化";
}

FingerprintManager::~FingerprintManager()
{
    qDebug() << "[FingerprintManager] 指纹管理器销毁";
}

void FingerprintManager::setDatabase(QSqlDatabase* db)
{
    if (db == nullptr) {
        // 如果传入nullptr，则使用默认数据库连接
        // 我们不能存储临时对象的指针，所以改为在需要时获取数据库连接
        database = nullptr;
        qDebug() << "[FingerprintManager] 将使用默认数据库连接";
    } else {
        database = db;
        qDebug() << "[FingerprintManager] 数据库连接已设置";
    }
}

FingerprintManager::VerificationResult FingerprintManager::verifyFingerprint(int userId, int fingerprintId)
{
    qDebug() << "[FingerprintManager] 开始验证指纹，用户ID:" << userId;
    qDebug() << "[DEBUG] fingerprint_id = " << fingerprintId << " (传入的指纹ID)";
    
    // 获取数据库连接
    QSqlDatabase db;
    if (database != nullptr) {
        db = *database;
    } else {
        db = QSqlDatabase::database();  // 使用默认数据库连接
    }
    
    if (!db.isOpen()) {
        qDebug() << "[FingerprintManager] 数据库未连接";
        return VerificationResult(false, "", "", "", "数据库连接错误");
    }
    
    // 检查用户ID是否有效（大于0表示指纹识别成功，0表示失败）
    if (userId <= 0) {
        qDebug() << "[FingerprintManager] 无效的用户ID:" << userId;
        qDebug() << "[DEBUG] fingerprint_id = " << fingerprintId << " (无效的用户ID，记录失败)";
        
        // 记录失败的考勤（employeeID为NULL表示未知用户）
        QSqlQuery failQuery(db);
        failQuery.prepare("INSERT INTO attendance(employeeID, unlock_type, result, fingerprint_id) VALUES(NULL, 'fingerprint', 'fail', ?)");
        failQuery.bindValue(0, fingerprintId);
        if (failQuery.exec()) {
            qDebug() << "[FingerprintManager] 失败考勤记录成功 (fingerprint_id=" << fingerprintId << ")";
        } else {
            qDebug() << "[FingerprintManager] 失败考勤记录失败:" << failQuery.lastError().text();
        }
        
        return VerificationResult(false, "", "", "", "指纹未注册或不匹配");
    }
    
    // 根据指纹ID查询对应的员工信息
    QSqlQuery query(db);
    query.prepare("SELECT e.employeeID, e.name, '软件部' as department "
                  "FROM employee e "
                  "INNER JOIN fingerprints f ON e.employeeID = f.employee_id "
                  "WHERE f.fingerprint_id = ?");
    query.bindValue(0, userId);
    
    if (!query.exec()) {
        qDebug() << "[FingerprintManager] 数据库查询失败:" << query.lastError().text();
        qDebug() << "[DEBUG] fingerprint_id = " << fingerprintId << " (数据库查询失败)";
        return VerificationResult(false, "", "", "", "数据库查询错误");
    }
    
    if (query.next()) {
        QString employeeId = query.value("employeeID").toString();
        QString name = query.value("name").toString();
        QString department = query.value("department").toString();
        
        qDebug() << "[FingerprintManager] 指纹验证成功";
        qDebug() << "  员工ID:" << employeeId;
        qDebug() << "  姓名:" << name;
        qDebug() << "  部门:" << department;
        qDebug() << "[DEBUG] fingerprint_id = " << fingerprintId << " (验证成功)";
        
        // 记录考勤
        QSqlQuery attendanceQuery(db);
        attendanceQuery.prepare("INSERT INTO attendance(employeeID, unlock_type, result, fingerprint_id) VALUES(?, 'fingerprint', 'success', ?)");
        attendanceQuery.bindValue(0, employeeId.toInt());
        attendanceQuery.bindValue(1, fingerprintId);
        
        if (attendanceQuery.exec()) {
            qDebug() << "[FingerprintManager] 考勤记录成功 (fingerprint_id=" << fingerprintId << ")";
        } else {
            qDebug() << "[FingerprintManager] 考勤记录失败:" << attendanceQuery.lastError().text();
        }
        
        return VerificationResult(true, employeeId, name, department, "指纹验证成功");
    } else {
        qDebug() << "[FingerprintManager] 未找到对应的员工记录，指纹ID:" << userId;
        qDebug() << "[DEBUG] fingerprint_id = " << fingerprintId << " (未找到对应员工)";
        
        // 记录失败的考勤（employeeID为NULL表示未知用户）
        QSqlQuery failQuery(db);
        failQuery.prepare("INSERT INTO attendance(employeeID, unlock_type, result, fingerprint_id) VALUES(NULL, 'fingerprint', 'fail', ?)");
        failQuery.bindValue(0, fingerprintId);
        if (failQuery.exec()) {
            qDebug() << "[FingerprintManager] 失败考勤记录成功 (fingerprint_id=" << fingerprintId << ")";
        } else {
            qDebug() << "[FingerprintManager] 失败考勤记录失败:" << failQuery.lastError().text();
        }
        
        return VerificationResult(false, "", "", "", "指纹未注册或不匹配");
    }
}

bool FingerprintManager::registerFingerprint(const QString& employeeId, const QByteArray& fingerprintData)
{
    qDebug() << "[FingerprintManager] 开始注册指纹，员工ID:" << employeeId;
    
    QSqlDatabase db = getDatabase();
    if (!db.isOpen()) {
        qDebug() << "[FingerprintManager] 数据库未连接";
        emit registrationCompleted(false, "数据库连接错误");
        return false;
    }
    
    // 检查员工是否存在
    if (!isEmployeeExists(employeeId)) {
        qDebug() << "[FingerprintManager] 员工不存在:" << employeeId;
        emit registrationCompleted(false, "员工不存在");
        return false;
    }
    
    // 检查是否已经注册过指纹
    if (isFingerprintRegistered(employeeId)) {
        qDebug() << "[FingerprintManager] 员工已注册指纹:" << employeeId;
        emit registrationCompleted(false, "该员工已注册指纹");
        return false;
    }
    
    // 生成新的指纹ID（简单递增方式）
    QSqlQuery maxIdQuery(db);
    maxIdQuery.exec("SELECT COALESCE(MAX(fingerprint_id), 0) + 1 FROM fingerprints");
    int newFingerprintId = 1;
    if (maxIdQuery.next()) {
        newFingerprintId = maxIdQuery.value(0).toInt();
    }
    
    // 确保指纹ID不会重复（额外安全检查）
    while (isFingerprintIdUsed(newFingerprintId)) {
        newFingerprintId++;
        if (newFingerprintId > 1000) { // 防止无限循环
            qDebug() << "[FingerprintManager] 指纹ID生成失败，超出范围";
            emit registrationCompleted(false, "指纹ID生成失败");
            return false;
        }
    }
    
    // 使用事务确保数据一致性
    if (!db.transaction()) {
        qDebug() << "[FingerprintManager] 事务开始失败";
        emit registrationCompleted(false, "数据库事务错误");
        return false;
    }
    
    // 插入指纹记录
    QSqlQuery insertQuery(db);
    insertQuery.prepare("INSERT INTO fingerprints(employee_id, fingerprint_id, fingerprint_data) VALUES(?, ?, ?)");
    insertQuery.bindValue(0, employeeId.toInt());
    insertQuery.bindValue(1, newFingerprintId);
    insertQuery.bindValue(2, fingerprintData);
    
    if (insertQuery.exec()) {
        // 提交事务
        if (db.commit()) {
            qDebug() << "[FingerprintManager] 指纹注册成功";
            qDebug() << "  员工ID:" << employeeId;
            qDebug() << "  指纹ID:" << newFingerprintId;
            emit registrationCompleted(true, QString("指纹注册成功，指纹ID: %1").arg(newFingerprintId));
            return true;
        } else {
            qDebug() << "[FingerprintManager] 事务提交失败";
            db.rollback();
            emit registrationCompleted(false, "数据库事务提交失败");
            return false;
        }
    } else {
        qDebug() << "[FingerprintManager] 指纹注册失败:" << insertQuery.lastError().text();
        db.rollback();
        emit registrationCompleted(false, "指纹注册失败: " + insertQuery.lastError().text());
        return false;
    }
}

bool FingerprintManager::registerFingerprintFromNetwork(const QString& employeeId, const QString& base64FingerprintData)
{
    qDebug() << "[FingerprintManager] 从网络注册指纹，员工ID:" << employeeId;
    
    // 解码Base64数据
    QByteArray fingerprintData = QByteArray::fromBase64(base64FingerprintData.toUtf8());
    
    if (fingerprintData.isEmpty()) {
        qDebug() << "[FingerprintManager] Base64解码失败或数据为空";
        emit registrationCompleted(false, "指纹数据格式错误");
        return false;
    }
    
    // 调用标准注册方法
    return registerFingerprint(employeeId, fingerprintData);
}

int FingerprintManager::getNextAvailableFingerprintId()
{
    QSqlDatabase db = getDatabase();
    if (!db.isOpen()) {
        return -1;
    }
    
    QSqlQuery maxIdQuery(db);
    maxIdQuery.exec("SELECT COALESCE(MAX(fingerprint_id), 0) + 1 FROM fingerprints");
    int nextId = 1;
    if (maxIdQuery.next()) {
        nextId = maxIdQuery.value(0).toInt();
    }
    
    // 确保ID不重复
    while (isFingerprintIdUsed(nextId) && nextId <= 1000) {
        nextId++;
    }
    
    return nextId <= 1000 ? nextId : -1;
}

bool FingerprintManager::deleteFingerprint(const QString& employeeId)
{
    qDebug() << "[FingerprintManager] 删除指纹，员工ID:" << employeeId;
    
    QSqlDatabase db = getDatabase();
    if (!db.isOpen()) {
        qDebug() << "[FingerprintManager] 数据库未连接";
        return false;
    }
    
    // 检查员工是否存在指纹
    if (!isFingerprintRegistered(employeeId)) {
        qDebug() << "[FingerprintManager] 员工未注册指纹:" << employeeId;
        return false;
    }
    
    // 使用事务确保数据一致性
    if (!db.transaction()) {
        qDebug() << "[FingerprintManager] 事务开始失败";
        return false;
    }
    
    QSqlQuery deleteQuery(db);
    deleteQuery.prepare("DELETE FROM fingerprints WHERE employee_id = ?");
    deleteQuery.bindValue(0, employeeId.toInt());
    
    if (deleteQuery.exec()) {
        int deletedRows = deleteQuery.numRowsAffected();
        if (deletedRows > 0 && db.commit()) {
            qDebug() << "[FingerprintManager] 删除指纹成功，影响行数:" << deletedRows;
            return true;
        } else {
            qDebug() << "[FingerprintManager] 事务提交失败或无数据删除";
            db.rollback();
            return false;
        }
    } else {
        qDebug() << "[FingerprintManager] 删除指纹失败:" << deleteQuery.lastError().text();
        db.rollback();
        return false;
    }
}

QStringList FingerprintManager::getRegisteredUsers()
{
    qDebug() << "[FingerprintManager] 获取已注册指纹的用户列表";
    
    QStringList userList;
    
    QSqlDatabase db = getDatabase();
    if (!db.isOpen()) {
        qDebug() << "[FingerprintManager] 数据库未连接";
        return userList;
    }
    
    QSqlQuery query(db);
    query.exec("SELECT e.employeeID, e.name FROM employee e "
               "INNER JOIN fingerprints f ON e.employeeID = f.employee_id "
               "ORDER BY e.employeeID");
    
    while (query.next()) {
        QString employeeId = query.value("employeeID").toString();
        QString name = query.value("name").toString();
        userList.append(QString("%1 - %2").arg(employeeId, name));
    }
    
    qDebug() << "[FingerprintManager] 找到" << userList.size() << "个已注册用户";
    return userList;
}

bool FingerprintManager::createFingerprintTable()
{
    qDebug() << "[FingerprintManager] 创建指纹表";
    
    QSqlDatabase db = getDatabase();
    if (!db.isOpen()) {
        qDebug() << "[FingerprintManager] 数据库未连接";
        return false;
    }
    
    QSqlQuery query(db);
    
    // 创建指纹表
    QString createTableSql = 
        "CREATE TABLE IF NOT EXISTS fingerprints("
        "id INTEGER PRIMARY KEY AUTOINCREMENT,"
        "employee_id INTEGER NOT NULL,"
        "fingerprint_id INTEGER NOT NULL,"
        "fingerprint_data BLOB,"
        "create_time TEXT DEFAULT(datetime('now','localtime')),"
        "FOREIGN KEY (employee_id) REFERENCES employee(employeeID),"
        "UNIQUE(employee_id),"
        "UNIQUE(fingerprint_id))";
    
    if (!query.exec(createTableSql)) {
        qDebug() << "[FingerprintManager] 创建指纹表失败:" << query.lastError().text();
        return false;
    }
    
    // 创建索引
    query.exec("CREATE INDEX IF NOT EXISTS idx_fingerprints_employee_id ON fingerprints(employee_id)");
    query.exec("CREATE INDEX IF NOT EXISTS idx_fingerprints_fingerprint_id ON fingerprints(fingerprint_id)");
    
    qDebug() << "[FingerprintManager] 指纹表创建成功";
    return true;
}

bool FingerprintManager::logAttendance(const QString& employeeId, const QString& result)
{
    QSqlDatabase db = getDatabase();
    if (!db.isOpen()) {
        qDebug() << "[FingerprintManager] 数据库未连接，无法记录考勤";
        return false;
    }
    
    QSqlQuery attendanceQuery(db);
    attendanceQuery.prepare("INSERT INTO attendance(employeeID, unlock_type, result) VALUES(?, 'fingerprint', ?)");
    attendanceQuery.bindValue(0, employeeId.toInt());
    attendanceQuery.bindValue(1, result);
    
    if (attendanceQuery.exec()) {
        qDebug() << "[FingerprintManager] 考勤记录成功 - 员工ID:" << employeeId << "结果:" << result;
        return true;
    } else {
        qDebug() << "[FingerprintManager] 考勤记录失败:" << attendanceQuery.lastError().text();
        return false;
    }
}

bool FingerprintManager::isValidUserId(int userId)
{
    return userId > 0;
}

QString FingerprintManager::getUserInfo(int userId)
{
    QSqlDatabase db = getDatabase();
    if (!db.isOpen()) {
        return QString();
    }
    
    QSqlQuery query(db);
    query.prepare("SELECT e.name FROM employee e "
                  "INNER JOIN fingerprints f ON e.employeeID = f.employee_id "
                  "WHERE f.fingerprint_id = ?");
    query.bindValue(0, userId);
    
    if (query.exec() && query.next()) {
        return query.value("name").toString();
    }
    
    return QString();
}

bool FingerprintManager::isEmployeeExists(const QString& employeeId)
{
    QSqlDatabase db = getDatabase();
    if (!db.isOpen()) {
        return false;
    }
    
    QSqlQuery query(db);
    query.prepare("SELECT COUNT(*) FROM employee WHERE employeeID = ?");
    query.bindValue(0, employeeId.toInt());
    
    if (query.exec() && query.next()) {
        return query.value(0).toInt() > 0;
    }
    
    return false;
}

bool FingerprintManager::isFingerprintRegistered(const QString& employeeId)
{
    QSqlDatabase db = getDatabase();
    if (!db.isOpen()) {
        return false;
    }
    
    QSqlQuery query(db);
    query.prepare("SELECT COUNT(*) FROM fingerprints WHERE employee_id = ?");
    query.bindValue(0, employeeId.toInt());
    
    if (query.exec() && query.next()) {
        return query.value(0).toInt() > 0;
    }
    
    return false;
}

bool FingerprintManager::isFingerprintIdUsed(int fingerprintId)
{
    QSqlDatabase db = getDatabase();
    if (!db.isOpen()) {
        return false;
    }
    
    QSqlQuery query(db);
    query.prepare("SELECT COUNT(*) FROM fingerprints WHERE fingerprint_id = ?");
    query.bindValue(0, fingerprintId);
    
    if (query.exec() && query.next()) {
        return query.value(0).toInt() > 0;
    }
    
    return false;
}
QSqlDatabase FingerprintManager::getDatabase() const
{
    if (database != nullptr) {
        return *database;
    } else {
        return QSqlDatabase::database();  // 使用默认数据库连接
    }
}