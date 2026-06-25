#ifndef REGISTRATIONKEYPAGE_H
#define REGISTRATIONKEYPAGE_H

#include <QWidget>
#include <QPushButton>
#include <QLabel>
#include <QTimer>

QT_BEGIN_NAMESPACE
namespace Ui { class RegistrationKeyPage; }
QT_END_NAMESPACE

class RegistrationKeyPage : public QWidget
{
    Q_OBJECT

public:
    explicit RegistrationKeyPage(QWidget *parent = nullptr);
    ~RegistrationKeyPage();

    void clearInput();
    void showVerificationStatus(bool success, const QString& message);

signals:
    void backToHomeClicked();
    void keyVerified(const QString& key);

private slots:
    void onVerifyClicked();
    void onBackClicked();
    void onRetryClicked();
    void onVerificationTimeout();
    
    // 数字键盘按钮槽函数
    void onNumberButtonClicked(const QString& number);
    void onDeleteButtonClicked();

private:
    Ui::RegistrationKeyPage *ui;
    QTimer* m_timeoutTimer;
    int m_retryCount;
    QString m_lastKey;
    QString m_currentKey;  // 当前输入的密钥
    
    void setVerifyButtonEnabled(bool enabled);
    void showLoadingState();
    void hideLoadingState();
    void enableRetry();
    void updateKeyDisplay();
    
    static const int MAX_RETRY_COUNT = 3;
    static const int VERIFICATION_TIMEOUT = 10000; // 10 seconds
    static const int MAX_KEY_LENGTH = 8; // 最大密钥长度（8位纯数字）
};

#endif // REGISTRATIONKEYPAGE_H