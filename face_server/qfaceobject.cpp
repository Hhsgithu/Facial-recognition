#include "qfaceobject.h"
#include <QDebug>
#include <chrono>

QFaceObject::QFaceObject(QObject *parent) : QObject(parent)
{
    //初始化 传入模型
    seeta::ModelSetting FDmode("/home/hhs/face_server/fd_2_00.dat", seeta::ModelSetting::CPU, 0);
    seeta::ModelSetting PDmode("/home/hhs/face_server/pd_2_00_pts5.dat", seeta::ModelSetting::CPU, 0);
    seeta::ModelSetting FRmode("/home/hhs/face_server/fr_2_10.dat", seeta::ModelSetting::CPU, 0);
    this->fengineptr = new seeta::FaceEngine(FDmode, PDmode, FRmode);

    //导入已有的人脸数据库
    this->fengineptr->Load("./face.db");
}

QFaceObject::~QFaceObject()
{
    delete fengineptr;
}

//注册
int64_t QFaceObject::face_register(cv::Mat &faceImage)
{
    //把opencv的Mat数据转成seetface的数据
    SeetaImageData simage;
    simage.data = faceImage.data;
    simage.width = faceImage.cols;
    simage.height = faceImage.rows;
    simage.channels = faceImage.channels();
    
    int64_t faceid = this->fengineptr->Register(simage); //返回人脸ID  
    if(faceid >= 0){
        fengineptr->Save("./face.db");
    }
    return faceid;
}

//查询
bool QFaceObject::face_query_raw(cv::Mat &faceImage, int64_t *outFaceId, float *outSimilarity)
{
    // 记录识别开始时间
    auto query_start = std::chrono::steady_clock::now();
    
    qDebug() << "[SeetaFace] 开始人脸识别查询...";
    qDebug() << "[SeetaFace] 输入图片尺寸: " << faceImage.cols << "x" << faceImage.rows;
    
    // 【性能优化】预处理已经将图片调整到200x200，这里不需要再次resize
    // 直接使用传入的图片，避免不必要的检查和操作
    const cv::Mat& optimizedImage = faceImage;
    
    //把opencv的Mat数据转成seetface的数据
    SeetaImageData simage;
    simage.data = optimizedImage.data;
    simage.width = optimizedImage.cols;
    simage.height = optimizedImage.rows;
    simage.channels = optimizedImage.channels();
    
    float similarity = 0;
    int64_t faceid = fengineptr->Query(simage, &similarity);
    
    auto query_end = std::chrono::steady_clock::now();
    auto query_time = std::chrono::duration_cast<std::chrono::milliseconds>(query_end - query_start);
    
    qDebug() << "[SeetaFace] ✓ 识别完成，耗时: " << query_time.count() << "ms";
    qDebug() << "[SeetaFace] 人脸ID: " << faceid;
    qDebug() << "[SeetaFace] ===== 相似度: " << similarity << " =====";

    if (outFaceId) *outFaceId = faceid;
    if (outSimilarity) *outSimilarity = similarity;

    // faceid < 0 表示数据库为空或Query失败等情况
    return faceid >= 0;
}

int QFaceObject::face_query(cv::Mat &faceImage)
{
    // 兼容旧接口：保留“阈值裁决 + send_faceid”行为
    // 相似度阈值：只要 > 0.5 就算识别成功（用于开门）
    constexpr float kSimilarityThreshold = 0.7f;

    int64_t faceid = -1;
    float similarity = 0.0f;
    const bool ok = face_query_raw(faceImage, &faceid, &similarity);

    qDebug() << "[SeetaFace] 相似度阈值:" << kSimilarityThreshold;

    if (ok && similarity > kSimilarityThreshold) {
        qDebug() << "[SeetaFace] ✓ 相似度" << similarity << ">" << kSimilarityThreshold << "，识别成功！";
        emit send_faceid(faceid);
        return static_cast<int>(faceid);
    }

    qDebug() << "[SeetaFace] ✗ 相似度" << similarity << "<=" << kSimilarityThreshold << "，识别失败";
    qDebug() << "[SeetaFace] 原因: 相似度不足，可能是未注册的人脸或图片质量不佳";
    emit send_faceid(-1);
    return -1;
}

//删除指定ID的人脸数据
bool QFaceObject::face_delete(int64_t faceId)
{
    qDebug() << "[SeetaFace] 尝试删除人脸ID: " << faceId;
    
    // SeetaFace的Delete方法
    bool success = fengineptr->Delete(faceId);
    
    if(success) {
        // 保存到文件
        fengineptr->Save("./face.db");
        qDebug() << "[SeetaFace] ✓ 人脸ID " << faceId << " 删除成功";
    } else {
        // 删除失败可能是因为ID不存在，这是正常的
        qDebug() << "[SeetaFace] 人脸ID " << faceId << " 不存在或已删除";
    }
    
    return success;
}

//获取人脸数据库中的人脸数量
// 注意：SeetaFace可能没有直接获取数量的API
// 我们返回一个估计值，用于清理时的范围判断
int64_t QFaceObject::get_face_count()
{
    // 由于SeetaFace API限制，我们无法直接获取准确的人脸数量
    // 返回一个较大的估计值，用于清理时遍历
    // 实际清理时会尝试删除每个ID，不存在的会自动跳过
    qDebug() << "[SeetaFace] 注意：无法直接获取人脸数量，将使用估计范围进行清理";
    return 100;  // 返回一个估计的最大值，清理时会遍历0-99
}
