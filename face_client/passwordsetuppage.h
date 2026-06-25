#ifndef PASSWORDSETUPPAGE_H
#define PASSWORDSETUPPAGE_H

#include <QWidget>
#include <QLineEdit>
#include "userinfopage.h"

QT_BEGIN_NAMESPACE
namespace Ui { class PasswordSetupPage; }
QT_END_NAMESPACE

class PasswordSetupPage : public QWidget
{
    Q_OBJECT

public:
    explicit PasswordSetupPage(QWidget *parent = nullptr);
    ~PasswordSetupPage();

    void setUserInfo(const UserInfo& userInfo);
    void startSetup();

signals:
    void passwordSetupCompleted(const QString& password);
    void backClicked();

protected:
    bool eventFilter(QObject* obj, QEvent* event) override;

private slots:
    void onConfirmPassword();
    void onBackClicked();
    
    // 数字键盘按钮槽函数
    void onNumberButtonClicked(const QString& number);
    void onDeleteButtonClicked();

private:
    Ui::PasswordSetupPage *ui;
    
    // 用户信息
    UserInfo m_userInfo;
    
    // 密码数据
    QString m_password;
    QString m_confirmPassword;
    bool m_isFirstInput;  // true表示输入第一个密码，false表示输入确认密码
    
    // 私有方法
    void updatePasswordDisplay();
    bool validatePassword(const QString& password) const;
    void updateStatus(const QString& message, const QString& color = "rgb(100, 100, 100)");
};

#endif // PASSWORDSETUPPAGE_H
