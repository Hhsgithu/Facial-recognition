#ifndef USERINFOPAGE_H
#define USERINFOPAGE_H

#include <QWidget>
#include <QLineEdit>
#include <QPushButton>
#include <QLabel>
#include <QEvent>
#include <QRadioButton>

QT_BEGIN_NAMESPACE
namespace Ui { class UserInfoPage; }
QT_END_NAMESPACE

struct UserInfo {
    QString name;
    QString phone;
    QString address;
    QString sex;   // 性别："男" 或 "女"
    
    bool isValid() const {
        return !name.trimmed().isEmpty() && 
               !phone.trimmed().isEmpty() && 
               !address.trimmed().isEmpty() &&
               !sex.trimmed().isEmpty();
    }
};

class UserInfoPage : public QWidget
{
    Q_OBJECT

public:
    explicit UserInfoPage(QWidget *parent = nullptr);
    ~UserInfoPage();

    void clearInput();
    UserInfo getUserInfo() const;

signals:
    void backToHomeClicked();
    void userInfoCompleted(const UserInfo& userInfo);

private slots:
    void onNextClicked();
    void onBackClicked();
    void onInputChanged();

private:
    Ui::UserInfoPage *ui;

    void validateInput();
    void setNextButtonEnabled(bool enabled);
    bool isPhoneValid(const QString& phone) const;
};

#endif // USERINFOPAGE_H