#ifndef BIOMETRICCOLLECTIONPAGE_H
#define BIOMETRICCOLLECTIONPAGE_H

#include <QWidget>
#include <QPushButton>
#include <QLabel>
#include <QLineEdit>
#include <QProgressBar>
#include <QTimer>
#include <QStackedWidget>
#include <opencv2/opencv.hpp>
#include "userinfopage.h"
#include "hardwarecontroller.h"

QT_BEGIN_NAMESPACE
namespace Ui { class BiometricCollectionPage; }
QT_END_NAMESPACE

class BiometricCollectionPage : public QWidget
{
    Q_OBJECT

public:
    explicit BiometricCollectionPage(QWidget *parent = nullptr);
    ~BiometricCollectionPage();

    void setUserInfo(const UserInfo& userInfo);
    void setHardwareController(HardwareController* controller);
    void startCollection();
    void resetCollection();
    void showRegistrationStatus(bool success, const QString& message);

signals:
    void backToHomeClicked();
    void registrationCompleted(const UserInfo& userInfo, const QByteArray& faceData, 
                              const QByteArray& fingerprintData, const QString& password);

private slots:
    void onBackClicked();
    void onStartFaceCapture();
    void onStartFingerprintCapture();
    void onPasswordConfirmed();
    void onCompleteRegistration();
    void onRetryRegistration();
    void onRegistrationTimeout();
    
    // 采集相关槽函数
    void onFaceCaptureTimer();
    void onFingerprintCaptured(const QByteArray& data);
    void onFingerprintError(const QString& error);
    
    // 新增：手动拍照相关槽函数
    void onPreviewTimer();
    void onCaptureButtonClicked();
    void onConfirmButtonClicked();
    void onRetakeButtonClicked();

private:
    Ui::BiometricCollectionPage *ui;
    
    // 数据成员
    UserInfo m_userInfo;
    QByteArray m_faceData;
    QByteArray m_fingerprintData;
    QString m_password;
    
    // 硬件控制器
    HardwareController* m_hardwareController;
    
    // 人脸采集相关
    cv::VideoCapture m_camera;
    cv::CascadeClassifier m_faceClassifier;
    QTimer* m_faceCaptureTimer;
    int m_faceCountdown;
    
    // 新增：人脸采集状态枚举
    enum FaceCaptureState {
        FaceInitializing,      // 初始化中
        FacePreviewing,        // 预览中
        FaceReadyToCapture,    // 准备拍照
        FaceCapturing,         // 拍照中
        FaceConfirming,        // 确认中
        FaceCompleted,         // 完成
        FaceError              // 错误
    };
    
    // 新增：手动拍照相关成员变量
    FaceCaptureState m_faceCaptureState;
    QPushButton* m_confirmButton;          // 确认按钮
    QPushButton* m_retakeButton;           // 重拍按钮
    QLabel* m_previewLabel;                // 预览标签
    QLabel* m_confirmImageLabel;           // 确认图像标签
    QTimer* m_previewTimer;                // 预览定时器
    cv::Mat m_capturedFrame;               // 捕获的帧
    cv::Mat m_currentFrame;                // 当前帧
    std::vector<cv::Rect> m_detectedFaces; // 检测到的人脸
    bool m_faceDetected;                   // 是否检测到人脸
    
    // 注册相关
    QTimer* m_registrationTimer;
    int m_registrationRetryCount;
    
    // 采集状态
    enum CollectionStep {
        FaceCollection,
        FingerprintCollection,
        PasswordSetting,
        Completed
    };
    CollectionStep m_currentStep;
    
    // 私有方法
    void setupUI();
    bool initializeCamera();
    void switchToStep(CollectionStep step);
    void updateFaceStatus(const QString& message, const QString& color = "rgb(100, 100, 100)");
    void updateFingerprintStatus(const QString& message, const QString& color = "rgb(100, 100, 100)");
    void updatePasswordStatus(const QString& message, const QString& color = "rgb(100, 100, 100)");
    void updateProgressIndicator();
    void showRegistrationProgress();
    void hideRegistrationProgress();
    bool validatePassword(const QString& password) const;
    QByteArray preprocessFaceImage(const cv::Mat& image);
    
    // 新增：手动拍照相关方法
    void startLivePreview();
    void stopLivePreview();
    void detectAndDrawFaces(cv::Mat& frame);
    void updateCaptureButtonState();
    void switchToConfirmView();
    void switchToPreviewView();
    bool checkImageQuality(const cv::Mat& image, QString& errorMsg);
    void cleanupCamera();  // 清理摄像头资源
    
    // 常量
    static const int MAX_REGISTRATION_RETRY = 3;
    static const int REGISTRATION_TIMEOUT = 30000; // 30 seconds (增加到30秒)
};

#endif // BIOMETRICCOLLECTIONPAGE_H