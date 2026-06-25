#include "loginwindow.h"
#include "ui_loginwindow.h"
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>
#include <QTimer>

LoginWindow::LoginWindow(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::LoginWindow)
{
    ui->setupUi(this);
    setFixedSize(450, 580);
    initDatabase();
}

LoginWindow::~LoginWindow()
{
    delete ui;
}

void LoginWindow::initDatabase()
{
    QSqlQuery query;
    query.exec("CREATE TABLE IF NOT EXISTS admin("
               "id INTEGER PRIMARY KEY AUTOINCREMENT,"
               "username VARCHAR(64) UNIQUE NOT NULL,"
               "password VARCHAR(128) NOT NULL,"
               "create_time TIMESTAMP DEFAULT (datetime('now','localtime')))");
}

void LoginWindow::on_loginBtn_clicked()
{
    QString username = ui->loginUsername->text().trimmed();
    QString password = ui->loginPassword->text();
    
    if(username.isEmpty() || password.isEmpty()) {
        ui->loginMsgLabel->setText("请输入账号和密码");
        return;
    }
    
    QSqlQuery query;
    query.prepare("SELECT * FROM admin WHERE username = ? AND password = ?");
    query.addBindValue(username);
    query.addBindValue(password);
    
    if(query.exec() && query.next()) {
        ui->loginMsgLabel->setStyleSheet("color: rgb(82, 196, 26); font-size: 14px; background: transparent;");
        ui->loginMsgLabel->setText("登录成功！");
        qDebug() << "[登录] 管理员登录成功:" << username;
        emit loginSuccess();
    } else {
        ui->loginMsgLabel->setStyleSheet("color: rgb(245, 34, 45); font-size: 14px; background: transparent;");
        ui->loginMsgLabel->setText("账号或密码错误");
    }
}

void LoginWindow::on_registerBtn_clicked()
{
    QString username = ui->regUsername->text().trimmed();
    QString password = ui->regPassword->text();
    QString confirmPwd = ui->regConfirmPwd->text();
    QString key = ui->regKey->text().trimmed();
    
    ui->regMsgLabel->setStyleSheet("color: rgb(245, 34, 45); font-size: 14px; background: transparent;");
    
    if(username.isEmpty()) {
        ui->regMsgLabel->setText("请输入账号");
        return;
    }
    if(password.isEmpty()) {
        ui->regMsgLabel->setText("请输入密码");
        return;
    }
    if(password != confirmPwd) {
        ui->regMsgLabel->setText("两次密码输入不一致");
        return;
    }
    if(key != REGISTER_KEY) {
        ui->regMsgLabel->setText("请输入正确的注册密钥 (Key)");
        return;
    }
    
    // 检查账号是否存在
    QSqlQuery checkQuery;
    checkQuery.prepare("SELECT id FROM admin WHERE username = ?");
    checkQuery.addBindValue(username);
    if(checkQuery.exec() && checkQuery.next()) {
        ui->regMsgLabel->setText("该账号已存在");
        return;
    }
    
    // 注册
    QSqlQuery insertQuery;
    insertQuery.prepare("INSERT INTO admin(username, password) VALUES(?, ?)");
    insertQuery.addBindValue(username);
    insertQuery.addBindValue(password);
    
    if(insertQuery.exec()) {
        ui->regMsgLabel->setStyleSheet("color: rgb(82, 196, 26); font-size: 14px; background: transparent;");
        ui->regMsgLabel->setText("注册成功！即将跳转登录...");
        qDebug() << "[注册] 管理员注册成功:" << username;
        
        // 清空输入
        ui->regUsername->clear();
        ui->regPassword->clear();
        ui->regConfirmPwd->clear();
        ui->regKey->clear();
        
        // 延迟跳转到登录页
        QTimer::singleShot(1500, this, &LoginWindow::on_toLoginBtn_clicked);
    } else {
        ui->regMsgLabel->setText("注册失败，请重试");
        qDebug() << "[注册] 失败:" << insertQuery.lastError().text();
    }
}

void LoginWindow::on_toRegisterBtn_clicked()
{
    ui->loginMsgLabel->clear();
    ui->regMsgLabel->clear();
    ui->stackedWidget->setCurrentIndex(1);
}

void LoginWindow::on_toLoginBtn_clicked()
{
    ui->loginMsgLabel->clear();
    ui->regMsgLabel->clear();
    ui->regMsgLabel->setStyleSheet("color: rgb(245, 34, 45); font-size: 14px; background: transparent;");
    ui->stackedWidget->setCurrentIndex(0);
}
