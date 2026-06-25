#ifndef MANAGEWIN_H
#define MANAGEWIN_H

#include <QWidget>
#include <QSqlTableModel>
#include <QMessageBox>
#include "registrationkeymanager.h"

namespace Ui {
class ManageWin;
}

class ManageWin : public QWidget
{
    Q_OBJECT

public:
    explicit ManageWin(QWidget *parent = nullptr);
    ~ManageWin();
    
    void setRegistrationKeyManager(RegistrationKeyManager* manager);
    void setMainWidget(QWidget* mainWidget);  // 设置MainWidget指针

private slots:
    void on_refreshUserBtn_clicked();
    void on_refreshRecordBtn_clicked();
    void on_deleteUserBtn_clicked();
    void on_deleteRecordBtn_clicked();
    void on_clearRecordsBtn_clicked();

private:
    Ui::ManageWin *ui;
    QSqlQueryModel *m_userModel;
    QSqlQueryModel *m_recordModel;
    QSqlQueryModel *m_keyModel;
    RegistrationKeyManager* m_registrationKeyManager;
    QWidget* m_mainWidget;  // MainWidget指针（用于访问deleteFaceById）
    
    void refreshUserList();
    void refreshRecordList();
    void refreshKeyList();
};

#endif // MANAGEWIN_H
