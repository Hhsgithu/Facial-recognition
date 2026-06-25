#ifndef FACERECOGNITION_H
#define FACERECOGNITION_H

#include <QWidget>
#include <QTimer>
#include <QTcpSocket>
#include <opencv2/opencv.hpp>

QT_BEGIN_NAMESPACE
namespace Ui { class FaceRecognition; }
QT_END_NAMESPACE

class FaceRecognition : public QWidget
{
    Q_OBJECT

public:
    explicit FaceRecognition(QWidget *parent = nullptr);
    ~FaceRecognition();

    void startRecognition();
    void stopRecognition();

signals:
    void backToHomeClicked();
    void recognitionSuccess(const QString& userId, const QString& userName);

protected:
    void timerEvent(QTimerEvent *e) override;

private slots:
    void onBackButtonClicked();
    void recv_data();
    void resetState();
    void onTimeout();
    void tryConnect();
    void onConnected();
    void onDisconnected();

private:
    void sendFrame();
    void updateConnectionStatus(bool connected);
    void showUserInfo(const QString &id, const QString &name, 
                      const QString &dept, const QString &time);
    void clearUserInfo();

    Ui::FaceRecognition *ui;

    // 网络
    QTcpSocket msocket;
    QTimer mtimer;          // 重连定时器
    QTimer reset_timer;     // 状态重置定时器
    QTimer response_timer;  // 响应超时定时器
    QByteArray m_recvBuffer;
    bool is_connected;
    bool is_waiting_response;

    // 摄像头
    cv::VideoCapture cap;
    cv::CascadeClassifier cascade;
    bool recognition_started;
    int timer_id;
    int m_frameCount;
    int m_consecutiveFaceFrames = 0;   // 人脸连续出现帧数（用于跳过刚入画的模糊帧）

    // 图像缓冲（复用避免频繁分配）
    cv::Mat m_frame;
    cv::Mat m_displayFrame;
    cv::Mat m_rgbFrame;
    cv::Mat m_smallFrame;
    cv::Mat m_grayFrame;
    cv::Mat m_faceROI;
};

#endif // FACERECOGNITION_H
