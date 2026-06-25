#ifndef PASSWORDPAGE_H
#define PASSWORDPAGE_H

#include <QWidget>
#include <QPushButton>
#include <QLabel>
#include <QTcpSocket>
#include <QTimer>

QT_BEGIN_NAMESPACE
namespace Ui { class PasswordPage; }
QT_END_NAMESPACE

class PasswordPage : public QWidget
{
    Q_OBJECT

public:
    explicit PasswordPage(QWidget *parent = nullptr);
    ~PasswordPage();

signals:
    void backToHomeClicked();
    void verifySuccess(const QString& userId, const QString& userName);

private slots:
    void onNumberButtonClicked();
    void onDeleteButtonClicked();
    void onConfirmButtonClicked();
    void onBackButtonClicked();
    
    // 网络相关槽函数
    void onConnected();
    void onDisconnected();
    void recv_data();
    void onSocketError(QAbstractSocket::SocketError socketError);
    void timer_connect();

private:
    void updatePasswordDisplay();
    void resetPassword();
    void connectToServer();
    void updateConnectionStatus(bool connected);
    void showStatus(const QString& message, const QString& color);
    void clearStatus();

    Ui::PasswordPage *ui;
    QString inputPassword;
    
    // 网络相关成员
    QTcpSocket* m_socket;
    QTimer* m_reconnectTimer;
    bool is_connected;
    bool is_verifying;  // 验证中状态标志
};

#endif // PASSWORDPAGE_H

