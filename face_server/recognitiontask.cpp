#include "recognitiontask.h"
#include "qfaceobject.h"
#include <QDebug>
#include <QHash>
#include <QDateTime>
#include <QVector>
#include <QMutex>
#include <QMutexLocker>
#include <QCoreApplication>
#include <chrono>
#include <opencv2/imgproc/imgproc.hpp>

// 静态成员初始化
cv::CascadeClassifier RecognitionTask::cascade;
QMutex RecognitionTask::cascadeMutex;

RecognitionTask::RecognitionTask(QObject *parent)
    : QObject(parent)
    , QRunnable()
    , clientInfo(nullptr)
    , faceObject(nullptr)
    , hasDetectedFace(false)
{
    // 设置自动删除为false，由主线程管理生命周期
    setAutoDelete(false);
    
    // 优化：延迟加载分类器，只在第一次使用时加载
    // 分类器加载是线程安全的，可以共享使用
    static bool cascadeLoaded = false;
    if (!cascadeLoaded) {
        QMutexLocker locker(&cascadeMutex);
        if (!cascadeLoaded) {
            // 优先从应用程序目录加载分类器，避免依赖工作目录
            QString cascadePath = QCoreApplication::applicationDirPath()
                                  + "/haarcascade_frontalface_alt2.xml";
            if (!cascade.load(cascadePath.toStdString())) {
                // 回退到当前工作目录
                if (!cascade.load("./haarcascade_frontalface_alt2.xml")) {
                    qWarning() << "[错误] 无法加载人脸分类器:" << cascadePath
                               << "或 ./haarcascade_frontalface_alt2.xml";
                } else {
                    qDebug() << "[优化] Cascade分类器已从当前工作目录加载（共享使用）";
                }
            } else {
                qDebug() << "[优化] Cascade分类器已从应用目录加载（共享使用）:"
                         << cascadePath;
            }
            cascadeLoaded = true;
        }
    }
}

void RecognitionTask::setTaskData(cv::Mat image, ClientInfo* info, QFaceObject* faceObj)
{
    // 优化：cv::Mat使用引用计数，clone()实际上是浅拷贝，只有修改时才深拷贝
    // 但为了安全，这里使用clone()确保数据独立
    taskImage = image.clone();
    clientInfo = info;
    faceObject = faceObj;
}

void RecognitionTask::run()
{
    try {
        if (taskImage.empty() || !clientInfo) {
            qDebug() << "[线程池] 任务数据无效";
            emit taskCompleted(-1, clientInfo);
            return;
        }

        qDebug() << "[线程池] 开始处理识别任务，客户端: " << clientInfo->address << ":" << clientInfo->port;
        qDebug() << "[线程池] 输入图片尺寸: " << taskImage.cols << "x" << taskImage.rows;

        // 1. 检测人脸
        detectFace();

        // 2. 如果检测到人脸，进行识别
        int64_t faceid = -1;
        if (hasDetectedFace) {
            faceid = recognizeFace();
        } else {
            qDebug() << "[线程池] 未检测到人脸，跳过识别";
        }

        // 3. 发送结果
        emit taskCompleted(faceid, clientInfo);
    } catch (const cv::Exception &e) {
        qWarning() << "[错误] OpenCV 异常:" << e.what();
        emit taskCompleted(-1, clientInfo);
    } catch (const std::exception &e) {
        qWarning() << "[错误] 标准库异常:" << e.what();
        emit taskCompleted(-1, clientInfo);
    } catch (...) {
        qWarning() << "[错误] 未知异常，任务中止";
        emit taskCompleted(-1, clientInfo);
    }
}

void RecognitionTask::detectFace()
{
    auto detect_start = std::chrono::steady_clock::now();
    
    // 优化：根据图像大小决定是否先resize再转灰度，还是先转灰度再resize
    // 对于大图，先resize再转灰度可以减少计算量
    cv::Mat grayImage;
    cv::Mat resized_gray;
    
    if(taskImage.cols > 320 || taskImage.rows > 240) {
        // 大图：先resize再转灰度，减少灰度转换的计算量
        cv::Mat resized_bgr;
        cv::resize(taskImage, resized_bgr, cv::Size(320, 240), 0, 0, cv::INTER_LINEAR);
        cv::cvtColor(resized_bgr, resized_gray, cv::COLOR_BGR2GRAY);
    } else {
        // 小图：直接转灰度
        cv::cvtColor(taskImage, grayImage, cv::COLOR_BGR2GRAY);
        resized_gray = grayImage;
    }

    // 使用 CLAHE 做局部直方图均衡，提升暗光下的人脸可见度
    cv::Mat clahe_gray;
    try {
        cv::Ptr<cv::CLAHE> clahe = cv::createCLAHE(2.0, cv::Size(8, 8));
        clahe->apply(resized_gray, clahe_gray);
        resized_gray = clahe_gray;
    } catch (const cv::Exception &e) {
        qWarning() << "[检测] CLAHE 处理失败，使用原始灰度图:" << e.what();
    }
    
    std::vector<cv::Rect> faceRects;
    // 优化：使用互斥锁保护共享的cascade分类器（虽然detectMultiScale是线程安全的，但为保险起见）
    {
        QMutexLocker locker(&cascadeMutex);
        if (cascade.empty()) {
            qWarning() << "[错误] Cascade分类器为空，跳过 detectMultiScale 调用";
        } else {
            cascade.detectMultiScale(resized_gray, faceRects, 1.1, 3, 0,
                                     cv::Size(40, 40), cv::Size(200, 200));
        }
    }
    
    auto detect_end = std::chrono::steady_clock::now();
    auto detect_time = std::chrono::duration_cast<std::chrono::milliseconds>(detect_end - detect_start);
    
    if(faceRects.size() > 0) {
        cv::Rect rect = faceRects[0];
        
        // 调整坐标到原始图像
        if(resized_gray.cols != taskImage.cols || resized_gray.rows != taskImage.rows) {
            float scale_x = (float)taskImage.cols / resized_gray.cols;
            float scale_y = (float)taskImage.rows / resized_gray.rows;
            rect.x = (int)(rect.x * scale_x);
            rect.y = (int)(rect.y * scale_y);
            rect.width = (int)(rect.width * scale_x);
            rect.height = (int)(rect.height * scale_y);
        }
        
        // 保存检测到的人脸区域（用于后续识别）
        detectedFaceRect = rect;
        hasDetectedFace = true;
        
        qDebug() << "[线程池] 检测到人脸，耗时: " << detect_time.count() << "ms";
        qDebug() << "[线程池] 人脸位置: x=" << rect.x << " y=" << rect.y 
                 << " 大小: " << rect.width << "x" << rect.height;
        
        // 发送坐标信息
        QString coord = QString("{\"employeeID\":\" \",\"name\":\" \",\"department\":\" \",\"time\":\" \",\"rect_x\":\"%1\",\"rect_y\":\"%2\"}")
                        .arg(rect.x).arg(rect.y);
        emit coordReady(coord, clientInfo);
    } else {
        qDebug() << "[线程池] 未检测到人脸";
        hasDetectedFace = false;
    }
}

cv::Mat RecognitionTask::extractFaceRegion(const cv::Mat& image, const cv::Rect& faceRect)
{
    // 【优化】提取更大的人脸区域，包含更多上下文信息以提高识别准确率
    // 确保人脸区域在图片范围内
    int x = std::max(0, faceRect.x);
    int y = std::max(0, faceRect.y);
    int width = std::min(faceRect.width, image.cols - x);
    int height = std::min(faceRect.height, image.rows - y);
    
    // 【关键优化】增加边界padding到30-40%，包含更多上下文信息
    // 这样可以提高识别准确率，因为SeetaFace需要一定的上下文
    int basePadding = std::min(width, height) / 3;  // 基础padding为人脸大小的1/3
    int padding = std::min(40, basePadding);  // 最多40像素，或人脸大小的1/3
    
    x = std::max(0, x - padding);
    y = std::max(0, y - padding);
    width = std::min(width + 2 * padding, image.cols - x);
    height = std::min(height + 2 * padding, image.rows - y);
    
    // 确保宽高比接近1:1（人脸通常是正方形）
    if(width != height) {
        int size = std::max(width, height);
        // 调整到正方形，保持中心不变
        int center_x = x + width / 2;
        int center_y = y + height / 2;
        x = std::max(0, center_x - size / 2);
        y = std::max(0, center_y - size / 2);
        width = std::min(size, image.cols - x);
        height = std::min(size, image.rows - y);
    }
    
    cv::Rect safeRect(x, y, width, height);
    // 优化：使用ROI引用，延迟拷贝到真正需要时（在预处理时再拷贝）
    cv::Mat faceRegion = image(safeRect);
    
    qDebug() << "[优化] 裁剪人脸区域（含边界）: " << width << "x" << height 
             << " 位置: (" << x << "," << y << ")";
    qDebug() << "[优化] 原始人脸框: " << faceRect.width << "x" << faceRect.height
             << " padding: " << padding << "像素";
    
    return faceRegion;
}

// 静态公共预处理函数：注册和认证共用，保证特征空间一致
// 处理流程：格式统一 → 精确 resize 到 256x256 → 温和 CLAHE（大 tile）
// 不做 Unsharp Mask：深度学习模型内部已有归一化，人为锐化会引入噪声
cv::Mat RecognitionTask::preprocessForSeetaFace(const cv::Mat& faceImage)
{
    // SeetaFace fr_2_10 内部会自行处理尺寸，但给一个适中的固定尺寸
    // 256x256 保留足够细节，又不会让模型做过多下采样
    const int TARGET_SIZE = 256;

    // 1. 格式统一为 BGR 3通道，确保数据连续
    cv::Mat bgr;
    int ch = faceImage.channels();
    if (ch == 1) {
        cv::cvtColor(faceImage, bgr, cv::COLOR_GRAY2BGR);
    } else if (ch == 4) {
        cv::cvtColor(faceImage, bgr, cv::COLOR_BGRA2BGR);
    } else {
        bgr = faceImage.isContinuous() ? faceImage : faceImage.clone();
    }

    // 2. 精确 resize 到 TARGET_SIZE x TARGET_SIZE（center-crop 保持比例）
    cv::Mat resized;
    if (bgr.cols != TARGET_SIZE || bgr.rows != TARGET_SIZE) {
        // 先等比缩放使短边 = TARGET_SIZE，再 center-crop 到正方形
        double scale = std::max((double)TARGET_SIZE / bgr.cols,
                                (double)TARGET_SIZE / bgr.rows);
        int scaledW = (int)std::round(bgr.cols * scale);
        int scaledH = (int)std::round(bgr.rows * scale);
        cv::Mat scaled;
        int interp = (scale < 1.0) ? cv::INTER_AREA : cv::INTER_CUBIC;
        cv::resize(bgr, scaled, cv::Size(scaledW, scaledH), 0, 0, interp);

        // center-crop
        int cx = (scaledW - TARGET_SIZE) / 2;
        int cy = (scaledH - TARGET_SIZE) / 2;
        resized = scaled(cv::Rect(cx, cy, TARGET_SIZE, TARGET_SIZE)).clone();
    } else {
        resized = bgr.clone();
    }

    // 3. 温和 CLAHE：tileSize 与图像尺寸匹配（256/8=32px/格，合理粒度）
    //    clipLimit 降到 1.5，避免过度增强皮肤纹理噪声
    try {
        cv::Mat lab;
        cv::cvtColor(resized, lab, cv::COLOR_BGR2Lab);
        std::vector<cv::Mat> ch3;
        cv::split(lab, ch3);
        cv::Ptr<cv::CLAHE> clahe = cv::createCLAHE(1.5, cv::Size(8, 8));
        clahe->apply(ch3[0], ch3[0]);
        cv::merge(ch3, lab);
        cv::cvtColor(lab, resized, cv::COLOR_Lab2BGR);
    } catch (const cv::Exception& e) {
        qWarning() << "[预处理] CLAHE 失败，跳过增强:" << e.what();
    }

    qDebug() << "[预处理] 输出尺寸:" << resized.cols << "x" << resized.rows;
    return resized;
}

cv::Mat RecognitionTask::preprocessFaceImage(const cv::Mat& faceImage)
{
    return preprocessForSeetaFace(faceImage);
}

int64_t RecognitionTask::recognizeFace()
{
    if(!faceObject) {
        qDebug() << "[线程池] SeetaFace引擎未初始化";
        return -1;
    }
    
    if(!hasDetectedFace || detectedFaceRect.width <= 0 || detectedFaceRect.height <= 0) {
        qDebug() << "[线程池] 无效的人脸区域，跳过识别";
        return -1;
    }
    
    auto total_start = std::chrono::steady_clock::now();
    
    // 【同步预处理】直接提取人脸区域并预处理
    auto extract_start = std::chrono::steady_clock::now();
    cv::Mat faceRegion = extractFaceRegion(taskImage, detectedFaceRect);
    auto extract_end = std::chrono::steady_clock::now();
    auto extract_time = std::chrono::duration_cast<std::chrono::milliseconds>(extract_end - extract_start);
    qDebug() << "[性能] 提取人脸区域耗时: " << extract_time.count() << "ms";
    qDebug() << "[优化] 提取人脸区域: " << faceRegion.cols << "x" << faceRegion.rows
             << " (原始图片: " << taskImage.cols << "x" << taskImage.rows << ")";
    
    // 预处理人脸图片（调整分辨率、格式转换）
    auto preprocess_start = std::chrono::steady_clock::now();
    cv::Mat processedFace = preprocessFaceImage(faceRegion);
    auto preprocess_end = std::chrono::steady_clock::now();
    auto preprocess_time = std::chrono::duration_cast<std::chrono::milliseconds>(preprocess_end - preprocess_start);
    qDebug() << "[性能] 预处理图片耗时: " << preprocess_time.count() << "ms";
    qDebug() << "[优化] 预处理后人脸图片: " << processedFace.cols << "x" << processedFace.rows;
    
    // 使用QFaceObject进行识别（拿到原始相似度，便于做多帧融合）
    // 使用互斥锁保护SeetaFace调用（线程安全）
    auto seeta_start = std::chrono::steady_clock::now();
    static QMutex faceEngineMutex;
    int64_t rawFaceId = -1;
    float rawSimilarity = 0.0f;
    {
        QMutexLocker locker(&faceEngineMutex);
        faceObject->face_query_raw(processedFace, &rawFaceId, &rawSimilarity);
    }
    auto seeta_end = std::chrono::steady_clock::now();
    auto seeta_time = std::chrono::duration_cast<std::chrono::milliseconds>(seeta_end - seeta_start);

    // ===== 多帧融合（按客户端做短窗口聚合）=====
    struct ScoreItem {
        int64_t faceId;
        float similarity;
        qint64 tsMs;
    };
    struct ScoreBuffer {
        QVector<ScoreItem> items;
    };

    static QMutex scoreMutex;
    static QHash<QString, ScoreBuffer> scoreByClient;

    const QString clientKey = clientInfo
        ? QString("%1:%2").arg(clientInfo->address).arg(clientInfo->port)
        : QString("unknown");

    constexpr int kFuseMaxFrames = 5;          // 3~5帧
    constexpr qint64 kFuseWindowMs = 900;      // 约1秒内的帧做融合
    // 相似度阈值：只要 > 0.7 就算识别成功（用于开门）
    constexpr float kSimilarityThreshold = 0.7f;

    const qint64 nowMs = QDateTime::currentMSecsSinceEpoch();
    float fusedBestSim = rawSimilarity;
    int64_t fusedBestId = rawFaceId;
    int fusedCountSameId = 1;

    {
        QMutexLocker locker(&scoreMutex);
        ScoreBuffer &buf = scoreByClient[clientKey];
        buf.items.push_back(ScoreItem{rawFaceId, rawSimilarity, nowMs});

        // 清理窗口外数据
        while (!buf.items.isEmpty() && (nowMs - buf.items.front().tsMs) > kFuseWindowMs) {
            buf.items.pop_front();
        }
        // 控制最大帧数
        while (buf.items.size() > kFuseMaxFrames) {
            buf.items.pop_front();
        }

        // 融合策略：取窗口内“最大相似度”，并统计该id出现次数
        fusedBestSim = -1.0f;
        fusedBestId = -1;
        QHash<int64_t, int> idCount;
        for (const auto &it : buf.items) {
            idCount[it.faceId] = idCount.value(it.faceId, 0) + 1;
            if (it.similarity > fusedBestSim) {
                fusedBestSim = it.similarity;
                fusedBestId = it.faceId;
            }
        }
        fusedCountSameId = idCount.value(fusedBestId, 0);
    }

    // 最终裁决：只要满足相似度阈值就视为识别成功
    int64_t finalFaceId = -1;
    if (fusedBestId >= 0 && fusedBestSim > kSimilarityThreshold) {
        finalFaceId = fusedBestId;
        qDebug() << "[融合] ✓ 通过:"
                 << "bestId=" << fusedBestId
                 << "bestSim=" << fusedBestSim
                 << "count=" << fusedCountSameId
                 << "threshold=" << kSimilarityThreshold;
    } else {
        qDebug() << "[融合] ✗ 未通过:"
                 << "rawId=" << rawFaceId
                 << "rawSim=" << rawSimilarity
                 << "bestId=" << fusedBestId
                 << "bestSim=" << fusedBestSim
                 << "count=" << fusedCountSameId
                 << "threshold=" << kSimilarityThreshold;
        finalFaceId = -1;
    }
    
    auto total_end = std::chrono::steady_clock::now();
    auto total_time = std::chrono::duration_cast<std::chrono::milliseconds>(total_end - total_start);
    
    qDebug() << "\n[性能] ===== 识别性能分析 =====";
    qDebug() << "[性能] 提取人脸区域: " << extract_time.count() << "ms";
    qDebug() << "[性能] 预处理图片: " << preprocess_time.count() << "ms";
    qDebug() << "[性能] SeetaFace识别: " << seeta_time.count() << "ms (主要耗时)";
    qDebug() << "[性能] 识别总耗时: " << total_time.count() << "ms";
    qDebug() << "[性能] =========================";
    qDebug() << "[线程池] 返回人脸ID: " << finalFaceId;
    
    return finalFaceId;
}

