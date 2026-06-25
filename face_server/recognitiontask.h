#ifndef RECOGNITIONTASK_H
#define RECOGNITIONTASK_H

#include <QObject>
#include <QRunnable>
#include <opencv2/opencv.hpp>
#include <QString>
#include <QMutex>
#include "clientinfo.h"

// 前向声明
class QFaceObject;

// 识别任务类，用于线程池处理（继承QRunnable）
class RecognitionTask : public QObject, public QRunnable
{
    Q_OBJECT

public:
    explicit RecognitionTask(QObject *parent = nullptr);
    
    // 设置任务数据
    void setTaskData(cv::Mat image, ClientInfo* clientInfo, QFaceObject* faceObject);
    
    // QRunnable接口
    void run() override;

signals:
    void taskCompleted(int64_t faceid, ClientInfo* clientInfo);  // 任务完成信号
    void coordReady(QString coord, ClientInfo* clientInfo);      // 坐标就绪信号

private:
    cv::Mat taskImage;
    ClientInfo* clientInfo;
    QFaceObject* faceObject;  // SeetaFace引擎对象
    
    // 人脸检测分类器（共享使用，线程安全）
    static cv::CascadeClassifier cascade;
    static QMutex cascadeMutex;
    
    // 检测到的人脸区域
    cv::Rect detectedFaceRect;
    bool hasDetectedFace;
    
    void detectFace();  // 检测人脸
    int64_t recognizeFace();  // 识别人脸（调用SeetaFace）
    cv::Mat extractFaceRegion(const cv::Mat& image, const cv::Rect& faceRect);  // 提取人脸区域
    cv::Mat preprocessFaceImage(const cv::Mat& faceImage);  // 预处理人脸图片（内部转发）

public:
    // 注册和认证共用的预处理函数，保证特征空间一致
    // 可在 registerwin.cpp 中直接调用
    static cv::Mat preprocessForSeetaFace(const cv::Mat& faceImage);
};

#endif // RECOGNITIONTASK_H

