#ifndef REGISTRATIONCOMPLETEPAGE_H
#define REGISTRATIONCOMPLETEPAGE_H

#include <QWidget>
#include "userinfopage.h"

QT_BEGIN_NAMESPACE
namespace Ui { class RegistrationCompletePage; }
QT_END_NAMESPACE

class RegistrationCompletePage : public QWidget
{
    Q_OBJECT

public:
    explicit RegistrationCompletePage(QWidget *parent = nullptr);
    ~RegistrationCompletePage();

    void setUserInfo(const UserInfo& userInfo);
    void reset();
    void showProgress();
    void showSuccess();
    void showError(const QString& message);

signals:
    void registrationRequested();
    void registrationCompleted();
    void backClicked();

private slots:
    void onCompleteRegistration();
    void onRetryRegistration();
    void onBackToHome();

private:
    Ui::RegistrationCompletePage *ui;
    
    // 用户信息
    UserInfo m_userInfo;
    
    // 状态
    enum State {
        Ready,      // 准备提交
        Progress,   // 提交中
        Success,    // 成功
        Error       // 失败
    };
    State m_state;
    
    // 私有方法
    void updateUI();
};

#endif // REGISTRATIONCOMPLETEPAGE_H
