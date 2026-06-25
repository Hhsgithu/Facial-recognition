#include "managewin.h"
#include "ui_managewin.h"
#include "mainwidget.h"
#include <QSqlQuery>
#include <QSqlQueryModel>
#include <QSqlError>
#include <QMessageBox>
#include <QDebug>

ManageWin::ManageWin(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ManageWin),
    m_userModel(nullptr),
    m_recordModel(nullptr),
    m_keyModel(nullptr),
    m_registrationKeyManager(nullptr),
    m_mainWidget(nullptr)
{
    ui->setupUi(this);
    
    this->setStyleSheet(
        "ManageWin { background-color: rgb(245, 247, 250); }"
        
        "QGroupBox { font-size: 14px; font-weight: bold; color: rgb(50, 50, 50); "
        "border: 1px solid rgb(220, 220, 220); border-radius: 8px; margin-top: 10px; padding-top: 10px; background: white; }"
        "QGroupBox::title { subcontrol-origin: margin; left: 15px; padding: 0 5px; }"
        
        "QPushButton { background-color: rgb(82, 196, 26); color: white; border: none; border-radius: 8px; "
        "padding: 8px 16px; font-size: 13px; font-weight: bold; }"
        "QPushButton:hover { background-color: rgb(60, 170, 10); }"
        
        "QPushButton#deleteUserBtn, QPushButton#deleteRecordBtn { background-color: rgb(245, 34, 45); }"
        "QPushButton#deleteUserBtn:hover, QPushButton#deleteRecordBtn:hover { background-color: rgb(220, 20, 30); }"
        
        "QPushButton#clearRecordsBtn { background-color: rgb(250, 173, 20); }"
        "QPushButton#clearRecordsBtn:hover { background-color: rgb(220, 150, 10); }"
        
        "QTableView { background-color: white; border: none; border-radius: 8px; "
        "gridline-color: rgb(240, 240, 240); selection-background-color: rgb(82, 196, 26); "
        "selection-color: white; font-size: 12px; }"
        "QTableView::item { padding: 6px; }"
        "QHeaderView::section { background-color: rgb(250, 250, 250); color: rgb(100, 100, 100); "
        "padding: 8px; border: none; font-weight: bold; font-size: 12px; }"
    );
    
    // 注意：不需要手动连接信号，Qt的auto-connect机制会自动连接
    // 槽函数命名为 on_<objectName>_<signalName> 时会自动连接
    // 手动连接会导致槽函数被调用两次！
    
    // 初始化时加载数据
    refreshUserList();
    refreshRecordList();
}

ManageWin::~ManageWin()
{
    delete ui;
}

void ManageWin::setRegistrationKeyManager(RegistrationKeyManager* manager)
{
    m_registrationKeyManager = manager;
    if (m_registrationKeyManager) {
        qDebug() << "[ManageWin] 注册密钥管理器已设置";
        refreshKeyList();
    }
}

void ManageWin::setMainWidget(QWidget* mainWidget)
{
    m_mainWidget = mainWidget;
    if (m_mainWidget) {
        qDebug() << "[ManageWin] MainWidget指针已设置";
    }
}

void ManageWin::refreshUserList()
{
    if(m_userModel) {
        delete m_userModel;
    }
    m_userModel = new QSqlQueryModel(this);
    
    QString sql = "SELECT "
                  "e.employeeID AS '用户ID', "
                  "e.name AS '姓名', "
                  "e.sex AS '性别', "
                  "e.phone AS '电话', "
                  "COALESCE(e.password, '未设置') AS '密码', "
                  "COALESCE(CAST(e.faceID AS TEXT), '未注册') AS '人脸ID', "
                  "COALESCE(CAST(f.fingerprint_id AS TEXT), '未注册') AS '指纹ID' "
                  "FROM employee e "
                  "LEFT JOIN fingerprints f ON e.employeeID = f.employee_id "
                  "ORDER BY e.employeeID ASC";
    
    qDebug() << "[ManageWin] 刷新用户列表";
    qDebug() << "[DEBUG] 用户列表查询SQL: " << sql;
    
    m_userModel->setQuery(sql);
    
    if(m_userModel->lastError().isValid()) {
        qDebug() << "[错误] 用户列表查询失败: " << m_userModel->lastError().text();
    } else {
        qDebug() << "[ManageWin] 用户列表刷新完成，总记录数: " << m_userModel->rowCount();
    }
    
    ui->userTableView->setModel(m_userModel);
    ui->userTableView->resizeColumnsToContents();
}

void ManageWin::refreshRecordList()
{
    if(m_recordModel) {
        delete m_recordModel;
    }
    m_recordModel = new QSqlQueryModel(this);
    
    QString sql = "SELECT "
                  "a.attendanceID AS '记录ID', "
                  "a.employeeID AS '用户ID', "
                  "e.name AS '姓名', "
                  "CASE a.unlock_type "
                  "  WHEN 'face' THEN '人脸' "
                  "  WHEN 'password' THEN '密码' "
                  "  WHEN 'fingerprint' THEN '指纹' "
                  "  ELSE a.unlock_type "
                  "END AS '方式', "
                  "CASE a.result "
                  "  WHEN 'success' THEN '成功' "
                  "  WHEN 'fail' THEN '失败' "
                  "  ELSE a.result "
                  "END AS '结果', "
                  "COALESCE(CAST(a.face_id AS TEXT), '-') AS '人脸ID', "
                  "COALESCE(CAST(a.fingerprint_id AS TEXT), '-') AS '指纹ID', "
                  "datetime(a.attendanceTime) AS '时间' "
                  "FROM attendance a "
                  "LEFT JOIN employee e ON a.employeeID = e.employeeID "
                  "ORDER BY a.attendanceTime DESC LIMIT 100";
    
    qDebug() << "[ManageWin] 刷新识别记录列表";
    qDebug() << "[DEBUG] 识别记录查询SQL: " << sql;
    
    m_recordModel->setQuery(sql);
    
    if(m_recordModel->lastError().isValid()) {
        qDebug() << "[错误] 识别记录查询失败: " << m_recordModel->lastError().text();
    } else {
        qDebug() << "[ManageWin] 识别记录列表刷新完成，总记录数: " << m_recordModel->rowCount();
    }
    
    ui->recordTableView->setModel(m_recordModel);
    ui->recordTableView->resizeColumnsToContents();
}

void ManageWin::refreshKeyList()
{
    if (!m_registrationKeyManager) {
        qDebug() << "[ManageWin] Registration key manager not initialized, skip refresh";
        return;
    }
    
    if(m_keyModel) {
        delete m_keyModel;
    }
    m_keyModel = new QSqlQueryModel(this);
    
    QString sql = "SELECT "
                  "key_value AS '注册密钥', "
                  "max_uses AS '最大使用次数', "
                  "used_count AS '已使用次数', "
                  "(max_uses - used_count) AS '剩余次数', "
                  "datetime(expires_at) AS '过期时间', "
                  "datetime(created_at) AS '创建时间', "
                  "CASE WHEN is_active = 1 THEN '是' ELSE '否' END AS '有效', "
                  "CASE "
                  "  WHEN is_active = 0 THEN '已禁用' "
                  "  WHEN expires_at <= datetime('now') THEN '已过期' "
                  "  WHEN used_count >= max_uses THEN '已用完' "
                  "  ELSE '可用' "
                  "END AS '状态' "
                  "FROM registration_keys ORDER BY created_at DESC";
    
    m_keyModel->setQuery(sql);
    
    qDebug() << "[ManageWin] Key list refreshed, total:" << m_keyModel->rowCount();
}

void ManageWin::on_refreshUserBtn_clicked()
{
    refreshUserList();
}

void ManageWin::on_refreshRecordBtn_clicked()
{
    refreshRecordList();
}

void ManageWin::on_deleteUserBtn_clicked()
{
    QModelIndexList selected = ui->userTableView->selectionModel()->selectedRows();
    if(selected.isEmpty()) {
        QMessageBox::warning(this, "提示", "请先选择要删除的用户");
        return;
    }
    
    if(QMessageBox::question(this, "确认", 
        QString("确定删除选中的 %1 个用户？\n\n注意：将同时删除其所有识别记录、人脸数据和指纹数据！").arg(selected.size())) 
        != QMessageBox::Yes) {
        return;
    }
    
    // 阻止按钮信号，防止重复触发
    ui->deleteUserBtn->blockSignals(true);
    
    int success = 0;
    int faceDeleted = 0;
    int fingerprintDeleted = 0;
    
    for(const QModelIndex &idx : selected) {
        int userId = m_userModel->data(m_userModel->index(idx.row(), 0)).toInt();
        QString userName = m_userModel->data(m_userModel->index(idx.row(), 1)).toString();
        
        qDebug() << "[ManageWin] 开始删除用户，ID:" << userId << "姓名:" << userName;
        
        // 1. 查询该用户的SeetaFace ID（从映射表）
        QSqlQuery mappingQuery;
        mappingQuery.prepare("SELECT seeta_face_id FROM seeta_face_mapping WHERE employee_id = ?");
        mappingQuery.bindValue(0, userId);
        
        qint64 seetaFaceId = -1;
        if (mappingQuery.exec() && mappingQuery.next()) {
            seetaFaceId = mappingQuery.value(0).toLongLong();
            qDebug() << "[ManageWin] 找到SeetaFace ID:" << seetaFaceId;
        }
        
        // 2. 删除SeetaFace引擎中的人脸数据
        if (seetaFaceId >= 0 && m_mainWidget) {
            // 通过MainWidget删除SeetaFace人脸数据
            MainWidget* mainWidget = qobject_cast<MainWidget*>(m_mainWidget);
            if (mainWidget) {
                bool deleted = mainWidget->deleteFaceById(seetaFaceId);
                if (deleted) {
                    qDebug() << "[ManageWin] ✓ SeetaFace人脸数据已删除，ID:" << seetaFaceId;
                    faceDeleted++;
                } else {
                    qDebug() << "[警告] SeetaFace人脸数据删除失败或不存在，ID:" << seetaFaceId;
                }
            } else {
                qDebug() << "[错误] 无法转换MainWidget指针";
            }
        } else if (seetaFaceId >= 0) {
            qDebug() << "[警告] MainWidget未设置，无法删除SeetaFace人脸数据";
        }
        
        // 3. 删除人脸ID映射表记录
        QSqlQuery deleteMappingQuery;
        deleteMappingQuery.prepare("DELETE FROM seeta_face_mapping WHERE employee_id = ?");
        deleteMappingQuery.bindValue(0, userId);
        if (deleteMappingQuery.exec()) {
            qDebug() << "[ManageWin] ✓ 人脸ID映射已删除";
        } else {
            qDebug() << "[错误] 删除人脸ID映射失败:" << deleteMappingQuery.lastError().text();
        }
        
        // 4. 删除指纹数据
        QSqlQuery deleteFingerprintQuery;
        deleteFingerprintQuery.prepare("DELETE FROM fingerprints WHERE employee_id = ?");
        deleteFingerprintQuery.bindValue(0, userId);
        if (deleteFingerprintQuery.exec()) {
            int rows = deleteFingerprintQuery.numRowsAffected();
            if (rows > 0) {
                qDebug() << "[ManageWin] ✓ 指纹数据已删除，影响行数:" << rows;
                fingerprintDeleted++;
            }
        } else {
            qDebug() << "[错误] 删除指纹数据失败:" << deleteFingerprintQuery.lastError().text();
        }
        
        // 5. 删除考勤记录
        QSqlQuery deleteAttendanceQuery;
        deleteAttendanceQuery.exec(QString("DELETE FROM attendance WHERE employeeID = %1").arg(userId));
        
        // 6. 删除用户记录
        QSqlQuery deleteUserQuery;
        if(deleteUserQuery.exec(QString("DELETE FROM employee WHERE employeeID = %1").arg(userId))) {
            qDebug() << "[ManageWin] ✓ 用户记录已删除";
            success++;
        } else {
            qDebug() << "[错误] 删除用户记录失败:" << deleteUserQuery.lastError().text();
        }
    }
    
    // 先刷新列表
    refreshUserList();
    refreshRecordList();
    
    QString message = QString("成功删除 %1 个用户").arg(success);
    if (faceDeleted > 0) {
        message += QString("\n删除了 %1 个人脸映射").arg(faceDeleted);
    }
    if (fingerprintDeleted > 0) {
        message += QString("\n删除了 %1 个指纹数据").arg(fingerprintDeleted);
    }
    
    // 显示成功消息
    QMessageBox::information(this, "完成", message);
    
    // 恢复按钮信号
    ui->deleteUserBtn->blockSignals(false);
}

void ManageWin::on_deleteRecordBtn_clicked()
{
    QModelIndexList selected = ui->recordTableView->selectionModel()->selectedRows();
    if(selected.isEmpty()) {
        QMessageBox::warning(this, "提示", "请先选择要删除的记录");
        return;
    }
    
    if(QMessageBox::question(this, "确认", 
        QString("确定删除选中的 %1 条记录？").arg(selected.size())) != QMessageBox::Yes) {
        return;
    }
    
    // 阻止按钮信号，防止重复触发
    ui->deleteRecordBtn->blockSignals(true);
    
    int success = 0;
    for(const QModelIndex &idx : selected) {
        int recordId = m_recordModel->data(m_recordModel->index(idx.row(), 0)).toInt();
        
        QSqlQuery q;
        if(q.exec(QString("DELETE FROM attendance WHERE attendanceID = %1").arg(recordId))) {
            success++;
        }
    }
    
    // 先刷新列表
    refreshRecordList();
    
    // 显示成功消息
    QMessageBox::information(this, "完成", QString("成功删除 %1 条记录").arg(success));
    
    // 恢复按钮信号
    ui->deleteRecordBtn->blockSignals(false);
}

void ManageWin::on_clearRecordsBtn_clicked()
{
    if(QMessageBox::question(this, "确认", "确定清空所有识别记录？\n\n此操作不可恢复！") 
        != QMessageBox::Yes) {
        return;
    }
    
    QSqlQuery q;
    QSqlDatabase::database().transaction();
    
    if(q.exec("DELETE FROM attendance")) {
        QSqlDatabase::database().commit();
        QMessageBox::information(this, "完成", "已清空所有识别记录");
        refreshRecordList();
    } else {
        QSqlDatabase::database().rollback();
        QMessageBox::critical(this, "错误", "清空失败: " + q.lastError().text());
    }
}
