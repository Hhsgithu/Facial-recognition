#ifndef FINGERPRINTCAPTUREPAGE_H
#define FINGERPRINTCAPTUREPAGE_H

#include <QWidget>
#include <QTimer>
#include "userinfopage.h"
#include "hardwarecontroller.h"

QT_BEGIN_NAMESPACE
namespace Ui { class FingerprintCapturePage; }
QT_END_NAMESPACE

class FingerprintCapturePage : public QWidget
{
    Q_OBJECT

public:
    explicit FingerprintCapturePage(QWidget *parent = nullptr);
    ~FingerprintCapturePage();

    void setUserInfo(const UserInfo& userInfo);
    void setHardwareController(HardwareController* controller);
    void startCapture();

signals:
    void fingerprintCaptureCompleted(const QByteArray& fingerprintData);
    void backClicked();

private slots:
    void onStartCapture();
    void onFingerprintCaptured(const QByteArray& data);
    void onFingerprintError(const QString& error);
    void onBackClicked();

private:
    Ui::FingerprintCapturePage *ui;
    
    // 用户信息
    UserInfo m_userInfo;
    
    // 硬件控制器
    HardwareController* m_hardwareController;
    
    // 指纹数据
    QByteArray m_fingerprintData;
    
    // 私有方法
    void updateStatus(const QString& message, const QString& color = "rgb(100, 100, 100)");
};

#endif // FINGERPRINTCAPTUREPAGE_H
