#ifndef FACECAPTUREPAGE_H
#define FACECAPTUREPAGE_H

#include <QWidget>
#include <QPushButton>
#include <QLabel>
#include <QTimer>
#include <opencv2/opencv.hpp>
#include "userinfopage.h"
#include "hardwarecontroller.h"

QT_BEGIN_NAMESPACE
namespace Ui { class FaceCapturePage; }
QT_END_NAMESPACE

class FaceCapturePage : public QWidget
{
    Q_OBJECT

public:
    explicit FaceCapturePage(QWidget *parent = nullptr);
    ~FaceCapturePage();

    void setUserInfo(const UserInfo& userInfo);
    void setHardwareController(HardwareController* controller);
    void startCapture();

signals:
    void faceCaptureCompleted(const QByteArray& faceData);
    void backClicked();

private slots:
    void onStartCapture();
    void onPreviewTimer();
    void onCaptureButtonClicked();
    void onCaptureTimerTick();       // 多帧采集定时器回调
    void onConfirmButtonClicked();
    void onRetakeButtonClicked();
    void onBackClicked();

private:
    Ui::FaceCapturePage *ui;
    
    // 用户信息
    UserInfo m_userInfo;
    
    // 硬件控制器（虽然人脸采集不需要，但保持接口一致）
    HardwareController* m_hardwareController;
    
    // 人脸采集相关
    cv::VideoCapture m_camera;
    cv::CascadeClassifier m_faceClassifier;
    QTimer* m_previewTimer;
    
    // 人脸采集状态枚举
    enum FaceCaptureState {
        FaceInitializing,      // 初始化中
        FacePreviewing,        // 预览中
        FaceCapturing,         // 拍照中
        FaceConfirming,        // 确认中
        FaceCompleted,         // 完成
        FaceError              // 错误
    };
    
    // 手动拍照相关成员变量
    FaceCaptureState m_faceCaptureState;
    QPushButton* m_confirmButton;          // 确认按钮
    QPushButton* m_retakeButton;           // 重拍按钮
    QLabel* m_previewLabel;                // 预览标签
    QLabel* m_confirmImageLabel;           // 确认图像标签
    cv::Mat m_capturedFrame;               // 最终选定帧（最清晰）
    cv::Mat m_currentFrame;                // 当前预览帧
    std::vector<cv::Rect> m_detectedFaces; // 检测到的人脸
    bool m_faceDetected;                   // 是否检测到人脸

    // 多帧采集相关
    static constexpr int kCaptureFrames = 5;   // 连续采集帧数
    int m_captureCountdown;                    // 剩余待采集帧数
    double m_bestSharpness;                    // 当前最佳清晰度
    QTimer* m_captureTimer;                    // 多帧采集定时器

    // 私有方法
    bool initializeCamera();
    void startLivePreview();
    void stopLivePreview();
    void cleanupCamera();
    void switchToConfirmView();
    void switchToPreviewView();
    void updateStatus(const QString& message, const QString& color = "rgb(100, 100, 100)");
    QByteArray encodeFrameForServer(const cv::Mat& image);
    double computeSharpness(const cv::Mat& frame);
};

#endif // FACECAPTUREPAGE_H
