#ifndef LOGINWINDOW_H
#define LOGINWINDOW_H

#include <QWidget>

namespace Ui {
class LoginWindow;
}

class LoginWindow : public QWidget
{
    Q_OBJECT

public:
    explicit LoginWindow(QWidget *parent = nullptr);
    ~LoginWindow();

signals:
    void loginSuccess();

private slots:
    void on_loginBtn_clicked();
    void on_registerBtn_clicked();
    void on_toRegisterBtn_clicked();
    void on_toLoginBtn_clicked();

private:
    void initDatabase();
    
    Ui::LoginWindow *ui;
    const QString REGISTER_KEY = "13138543529";
};

#endif // LOGINWINDOW_H
