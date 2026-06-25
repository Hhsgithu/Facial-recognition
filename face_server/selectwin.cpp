#include "selectwin.h"
#include "ui_selectwin.h"
#include <QSqlQueryModel>
#include <QSqlError>
#include <QDebug>

SelectWin::SelectWin(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SelectWin)
{
    ui->setupUi(this);
    
    this->setStyleSheet(
        "SelectWin { background-color: rgb(245, 247, 250); }"
        
        "QRadioButton { font-size: 14px; color: rgb(50, 50, 50); spacing: 8px; padding: 5px; }"
        "QRadioButton::indicator { width: 18px; height: 18px; border-radius: 9px; "
        "border: 2px solid rgb(200, 200, 200); background-color: white; }"
        "QRadioButton::indicator:checked { background-color: rgb(82, 196, 26); border: 2px solid rgb(82, 196, 26); }"
        
        "QPushButton#selectbt { background-color: rgb(82, 196, 26); color: white; border: none; "
        "border-radius: 12px; padding: 10px 30px; font-size: 14px; font-weight: bold; min-width: 80px; }"
        "QPushButton#selectbt:hover { background-color: rgb(60, 170, 10); }"
        
        "QTableView { background-color: white; border: none; border-radius: 12px; "
        "gridline-color: rgb(240, 240, 240); selection-background-color: rgb(82, 196, 26); "
        "selection-color: white; font-size: 13px; color: rgb(50, 50, 50); }"
        "QTableView::item { padding: 8px; border-bottom: 1px solid rgb(245, 245, 245); }"
        "QHeaderView::section { background-color: rgb(250, 250, 250); color: rgb(120, 120, 120); "
        "padding: 10px; border: none; font-weight: bold; font-size: 13px; }"
    );
    
    model = new QSqlTableModel();
}

SelectWin::~SelectWin()
{
    delete ui;
}

void SelectWin::on_selectbt_clicked()
{
    if(ui->emprb->isChecked())
    {
        QSqlQueryModel *queryModel = new QSqlQueryModel(this);
        
        // 通过LEFT JOIN fingerprints表来检查指纹是否录入
        QString sql = "SELECT "
                      "e.employeeID AS '用户ID', "
                      "e.name AS '姓名', "
                      "e.sex AS '性别', "
                      "e.address AS '地址', "
                      "e.phone AS '电话', "
                      "COALESCE(e.password, '') AS '密码', "
                      "CASE WHEN f.fingerprint_id IS NOT NULL THEN '已录入' ELSE '未录入' END AS '指纹', "
                      "e.faceID AS '人脸ID' "
                      "FROM employee e "
                      "LEFT JOIN fingerprints f ON e.employeeID = f.employee_id "
                      "ORDER BY e.employeeID ASC";
        
        queryModel->setQuery(sql);
        
        if(queryModel->lastError().isValid()) {
            qDebug() << "[查询] 错误: " << queryModel->lastError().text();
        } else {
            qDebug() << "[查询] 用户信息，共" << queryModel->rowCount() << "条";
        }
        
        ui->tableView->setModel(queryModel);
        ui->tableView->resizeColumnsToContents();
    }
    else if(ui->attrb->isChecked())
    {
        QSqlQueryModel *queryModel = new QSqlQueryModel(this);
        
        QString sql = "SELECT "
                      "a.attendanceID AS '记录ID', "
                      "a.employeeID AS '用户ID', "
                      "e.name AS '姓名', "
                      "CASE a.unlock_type "
                      "    WHEN 'face' THEN '人脸识别' "
                      "    WHEN 'password' THEN '密码解锁' "
                      "    WHEN 'fingerprint' THEN '指纹解锁' "
                      "    ELSE a.unlock_type "
                      "END AS '解锁方式', "
                      "CASE a.result "
                      "    WHEN 'success' THEN '成功' "
                      "    WHEN 'fail' THEN '失败' "
                      "    ELSE a.result "
                      "END AS '结果', "
                      "datetime(a.attendanceTime) AS '识别时间' "
                      "FROM attendance a "
                      "LEFT JOIN employee e ON a.employeeID = e.employeeID "
                      "ORDER BY a.attendanceTime DESC";
        
        queryModel->setQuery(sql);
        
        if(queryModel->lastError().isValid()) {
            qDebug() << "[查询] 错误: " << queryModel->lastError().text();
        } else {
            qDebug() << "[查询] 识别记录，共" << queryModel->rowCount() << "条";
        }
        
        ui->tableView->setModel(queryModel);
        ui->tableView->resizeColumnsToContents();
    }
    else if(ui->adminrb->isChecked())
    {
        QSqlQueryModel *queryModel = new QSqlQueryModel(this);
        QString sql = "SELECT "
                      "id AS '管理员ID', "
                      "username AS '账号', "
                      "password AS '密码', "
                      "create_time AS '创建时间' "
                      "FROM admin "
                      "ORDER BY id ASC";
        queryModel->setQuery(sql);
        if(queryModel->lastError().isValid()) {
            qDebug() << "[查询] 管理员信息错误: " << queryModel->lastError().text();
        } else {
            qDebug() << "[查询] 管理员信息，共" << queryModel->rowCount() << "条";
        }
        ui->tableView->setModel(queryModel);
        ui->tableView->resizeColumnsToContents();
    }
}
