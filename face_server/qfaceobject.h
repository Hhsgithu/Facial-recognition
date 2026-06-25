#ifndef QFACEOBJECT_H
#define QFACEOBJECT_H
#include <QObject>
#include <seeta/FaceDatabase.h>
#include <seeta/FaceDetector.h>
#include <seeta/FaceLandmarker.h>
#include <seeta/FaceRecognizer.h>
#include <seeta/FaceTracker.h>
#include <seeta/FaceEngine.h>
#include <opencv.hpp>

//人脸数据存储,人脸检测,人脸识别
class QFaceObject : public QObject
{
    Q_OBJECT
public:
    explicit QFaceObject(QObject *parent = nullptr);
    ~QFaceObject();
public slots:
    int64_t face_register(cv::Mat& faceImage);
    // 查询（原始结果）：返回最相似的人脸ID与相似度；不做阈值裁决
    bool face_query_raw(cv::Mat& faceImage, int64_t* outFaceId, float* outSimilarity);
    // 查询（兼容接口）：内部用阈值裁决，返回通过的faceId，否则-1
    int face_query(cv::Mat& faceImage);
    bool face_delete(int64_t faceId);  // 删除指定ID的人脸数据
    int64_t get_face_count();  // 获取人脸数据库中的人脸数量

signals:
    void send_faceid(int64_t);

private:
    seeta::FaceEngine *fengineptr;

};

#endif // QFACEOBJECT_H
