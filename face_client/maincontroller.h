#ifndef MAINCONTROLLER_H
#define MAINCONTROLLER_H

#include <QWidget>
#include <QLabel>
#include "homepage.h"
#include "facerecognition.h"
#include "passwordpage.h"
#include "fingerprintpage.h"
#include "registrationkeypage.h"
#include "userinfopage.h"
#include "biometriccollectionpage.h"
#include "facecapturepage.h"
#include "fingerprintcapturepage.h"
#include "passwordsetuppage.h"
#include "registrationcompletepage.h"
#include "doorsimulator.h"
#include "hardwarecontroller.h"
#include "networkmanager.h"


class MainController : public QWidget
{
    Q_OBJECT

public:
    explicit MainController(QWidget *parent = nullptr);
    ~MainController();

private:
     // 声明 ui 成员变量

private slots:
    void showHomePage();
    void showFaceRecognitionPage();
    void showPasswordUnlockPage();
    void showFingerprintUnlockPage();
    void showRegistrationKeyPage();
    void showUserInfoPage();
    void showBiometricCollectionPage();
    
    // 新注册流程页面跳转
    void showFaceCapturePage();
    void showFingerprintCapturePage();
    void showPasswordSetupPage();
    void showRegistrationCompletePage();
    
    // 门禁相关槽函数
    void onDoorOpened();
    void onDoorClosed();
    
    // 注册相关槽函数
    void onKeyVerified(const QString& key);
    void onUserInfoCompleted(const UserInfo& userInfo);
    void onRegistrationCompleted(const UserInfo& userInfo, const QByteArray& faceData, 
                                const QByteArray& fingerprintData, const QString& password);
    
    // 新注册流程数据处理
    void onFaceCaptureCompleted(const QByteArray& faceData);
    void onFingerprintCaptureCompleted(const QByteArray& fingerprintData);
    void onPasswordSetupCompleted(const QString& password);
    void onRegistrationRequested();
    void onNewRegistrationCompleted();
    
    // 网络管理器相关槽函数
    void onKeyVerificationResult(bool success, const QString& message);
    void onRegistrationResult(bool success, const QString& message, int fingerprintId);
    void onNetworkConnected();
    void onNetworkDisconnected();
    void onNetworkError(const QString& error);

private:
    void showDoorOpenStatus();
    void hideDoorOpenStatus();

    HomePage *homePage;
    FaceRecognition *faceRecognitionPage;
    PasswordPage *passwordUnlockPage;
    FingerprintPage *fingerprintUnlockPage;
    RegistrationKeyPage *registrationKeyPage;
    UserInfoPage *userInfoPage;
    BiometricCollectionPage *biometricCollectionPage;
    
    // 新注册流程页面
    FaceCapturePage *faceCapturePage;
    FingerprintCapturePage *fingerprintCapturePage;
    PasswordSetupPage *passwordSetupPage;
    RegistrationCompletePage *registrationCompletePage;
    
    // 门禁模拟器
    DoorSimulator *doorSimulator;
    
    // 硬件控制器
    HardwareController *hardwareController;
    
    // 网络管理器
    NetworkManager *networkManager;
    
    // 注册流程数据
    struct RegistrationData {
        QString registrationKey;
        UserInfo userInfo;
        QByteArray faceData;
        QByteArray fingerprintData;
        QString password;
    };
    RegistrationData m_registrationData;
    
    // 旧注册流程数据（保留兼容）
    QString m_currentRegistrationKey;
    UserInfo m_currentUserInfo;
    
    // 开门状态显示标签
    QLabel *doorStatusLabel;
    
    // 是否使用新注册流程
    bool m_useNewRegistrationFlow;
};

#endif // MAINCONTROLLER_H

