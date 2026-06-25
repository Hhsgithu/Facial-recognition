#include "mainwidget.h"
#include "loginwindow.h"
#include "registrationkeymanager.h"
#include <QApplication>
#include <QSqlError>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <registerwin.h>
#include <opencv.hpp>
#include <selectwin.h>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    //注册自定义类型
    qRegisterMetaType<cv::Mat>("cv::Mat&");
    qRegisterMetaType<cv::Mat>("cv::Mat");
    qRegisterMetaType<int64_t>("int64_t");

    //连接数据库
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName("server.db");
    if(!db.open())
    {
        qDebug() << db.lastError().text();
        return -1;
    }
    
    //创建员工信息表
    QString createsql = "create table if not exists employee(employeeID integer primary key autoincrement,"
                        "name varchar(256), sex varchar(32), address text, phone text, "
                        "faceID integer unique, headfile text, password text, fingerprintID integer)";
    QSqlQuery query;
    if(!query.exec(createsql))
    {
        qDebug() << query.lastError().text();
        return -1;
    }
    
    //创建指纹表 - 按照设计文档规范
    createsql = "create table if not exists fingerprints("
                "id integer primary key autoincrement,"
                "employee_id integer NOT NULL,"
                "fingerprint_id integer NOT NULL,"
                "fingerprint_data BLOB,"
                "create_time TEXT DEFAULT(datetime('now','localtime')),"
                "FOREIGN KEY (employee_id) REFERENCES employee(employeeID),"
                "UNIQUE(employee_id),"
                "UNIQUE(fingerprint_id))";
    if(!query.exec(createsql))
    {
        qDebug() << query.lastError().text();
        return -1;
    }
    
    // 创建指纹表索引以提高查询性能
    query.exec("CREATE INDEX IF NOT EXISTS idx_fingerprints_employee_id ON fingerprints(employee_id)");
    query.exec("CREATE INDEX IF NOT EXISTS idx_fingerprints_fingerprint_id ON fingerprints(fingerprint_id)");
    
    //创建考勤表
    createsql = "create table if not exists attendance(attendanceID integer primary key autoincrement, "
                "employeeID integer, attendanceTime TimeStamp NOT NULL DEFAULT(datetime('now','localtime')),"
                "unlock_type TEXT NOT NULL DEFAULT 'face', result TEXT NOT NULL DEFAULT 'success')";
    if(!query.exec(createsql))
    {
        qDebug() << query.lastError().text();
        return -1;
    }
    
    //创建注册密钥表 - 按照设计文档规范
    createsql = "create table if not exists registration_keys("
                "id integer primary key autoincrement,"
                "key_value TEXT UNIQUE NOT NULL,"
                "max_uses INTEGER NOT NULL DEFAULT 5,"
                "used_count INTEGER NOT NULL DEFAULT 0,"
                "expires_at DATETIME NOT NULL,"
                "created_at DATETIME DEFAULT(datetime('now','localtime')),"
                "is_active BOOLEAN DEFAULT TRUE)";
    if(!query.exec(createsql))
    {
        qDebug() << query.lastError().text();
        return -1;
    }
    
    // 创建注册密钥表索引以提高查询性能
    query.exec("CREATE INDEX IF NOT EXISTS idx_registration_keys_key_value ON registration_keys(key_value)");
    query.exec("CREATE INDEX IF NOT EXISTS idx_registration_keys_expires_at ON registration_keys(expires_at)");
    query.exec("CREATE INDEX IF NOT EXISTS idx_registration_keys_is_active ON registration_keys(is_active)");
    
    // 创建指纹ID计数器表 - 用于跟踪下一个可用的指纹ID
    createsql = "create table if not exists fingerprint_id_counter("
                "id integer primary key CHECK(id = 1),"  // 只允许一行记录
                "next_fingerprint_id INTEGER NOT NULL DEFAULT 1)";
    if(!query.exec(createsql))
    {
        qDebug() << query.lastError().text();
        return -1;
    }
    
    // 初始化计数器（如果不存在）
    query.exec("INSERT OR IGNORE INTO fingerprint_id_counter(id, next_fingerprint_id) VALUES(1, 1)");
    
    qDebug() << "[数据库] ✓ 指纹ID计数器表创建成功";
    
    // 生成测试注册密钥（仅用于调试）
    #ifdef QT_DEBUG
    qDebug() << "[调试] 生成测试注册密钥...";
    RegistrationKeyManager testKeyManager;
    testKeyManager.generateCustomRegistrationKey("123456", 24 * 365, 100);  // 有效期1年，可使用100次
    testKeyManager.generateCustomRegistrationKey("12345678", 24 * 365, 100);  // 有效期1年，可使用100次
    #endif
    
    // 兼容旧数据库
    QSqlQuery alterQuery;
    alterQuery.exec("ALTER TABLE employee ADD COLUMN password TEXT");
    alterQuery.exec("ALTER TABLE employee ADD COLUMN fingerprintID INTEGER");
    alterQuery.exec("ALTER TABLE attendance ADD COLUMN unlock_type TEXT NOT NULL DEFAULT 'face'");
    alterQuery.exec("ALTER TABLE attendance ADD COLUMN result TEXT NOT NULL DEFAULT 'success'");
    
    // 添加新字段：指纹ID和人脸ID用于识别记录
    qDebug() << "[数据库] 添加attendance表新字段...";
    if(alterQuery.exec("ALTER TABLE attendance ADD COLUMN fingerprint_id INTEGER")) {
        qDebug() << "[数据库] ✓ fingerprint_id字段添加成功";
    } else {
        qDebug() << "[数据库] fingerprint_id字段可能已存在或添加失败:" << alterQuery.lastError().text();
    }
    
    if(alterQuery.exec("ALTER TABLE attendance ADD COLUMN face_id INTEGER")) {
        qDebug() << "[数据库] ✓ face_id字段添加成功";
    } else {
        qDebug() << "[数据库] face_id字段可能已存在或添加失败:" << alterQuery.lastError().text();
    }

    // 创建登录窗口和主窗口
    LoginWindow *loginWindow = new LoginWindow;
    MainWidget *mainWidget = new MainWidget;
    
    // 登录成功后显示主窗口
    QObject::connect(loginWindow, &LoginWindow::loginSuccess, [=]() {
        loginWindow->hide();
        mainWidget->show();
    });
    
    loginWindow->show();
    
    return a.exec();
}
