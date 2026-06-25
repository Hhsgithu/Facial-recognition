#include "mainwidget.h"
#include "ui_mainwidget.h"
#include <QDateTime>
#include <QDebug>
#include <QSqlQuery>
#include <QSqlError>
#include <QThread>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <string.h>
#include <chrono>
#include <opencv2/opencv.hpp>
#include "fingerprintmanager.h"
#include <QMessageBox>
#include <QSqlQueryModel>

MainWidget::MainWidget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::MainWidget)
{
    ui->setupUi(this);

    // 设置主窗口样式 - 与客户端统一的浅色风格
    this->setStyleSheet(
        // 主窗口背景 - 与客户端一致的浅灰色
        "MainWidget {"
        "    background-color: rgb(245, 247, 250);"
        "}"
        
        // TabWidget样式 - 浅色系、圆角
        "QTabWidget::pane {"
        "    border: none;"
        "    border-radius: 16px;"
        "    background-color: rgb(255, 255, 255);"
        "    top: -1px;"
        "}"
        "QTabBar::tab {"
        "    background-color: rgb(250, 250, 250);"
        "    color: rgb(120, 120, 120);"
        "    border: none;"
        "    border-top-left-radius: 12px;"
        "    border-top-right-radius: 12px;"
        "    padding: 12px 30px;"
        "    margin-right: 4px;"
        "    font-size: 14px;"
        "    font-weight: bold;"
        "}"
        "QTabBar::tab:selected {"
        "    background-color: rgb(255, 255, 255);"
        "    color: rgb(50, 50, 50);"
        "    border-bottom: 3px solid rgb(82, 196, 26);"
        "}"
        "QTabBar::tab:hover:!selected {"
        "    background-color: rgb(245, 245, 245);"
        "    color: rgb(80, 80, 80);"
        "}"
        
        // 图片显示标签样式 - 与客户端视频区域一致
        "QLabel#piclb {"
        "    background-color: rgb(30, 30, 30);"
        "    border: none;"
        "    border-radius: 12px;"
        "    min-height: 200px;"
        "    color: rgb(150, 150, 150);"
        "}"
    );

    qDebug() << "=== 服务器启动（多客户端+线程池版本）===";
    qDebug() << "初始化服务器...";

    // QtcpServer当有客户端连接会发送newConnection
    connect(&mserver, &QTcpServer::newConnection, this, &MainWidget::accept_client);

    // 监听,启动服务器
    if(mserver.listen(QHostAddress::Any, 8888)) {
        qDebug() << "✓ 服务器启动成功";
        qDebug() << "  监听地址: 任意地址(0.0.0.0)";
        qDebug() << "  监听端口: 8888";
        qDebug() << "  等待客户端连接...";
    } else {
        qDebug() << "✗ 服务器启动失败: " << mserver.errorString();
        return;
    }

    // 初始化线程池
    threadPool = new QThreadPool(this);
    int maxThreads = QThread::idealThreadCount();  // 获取CPU核心数
    threadPool->setMaxThreadCount(maxThreads > 0 ? maxThreads : 4);  // 默认4个线程
    qDebug() << "✓ 线程池初始化完成";
    qDebug() << "  最大线程数: " << threadPool->maxThreadCount();

    // 初始化SeetaFace引擎（使用qfaceobject的方式，避免ModelSetting依赖）
    faceObject = new QFaceObject(this);
    qDebug() << "✓ SeetaFace引擎初始化完成";

    // 初始化指纹管理器
    fingerprintManager = new FingerprintManager(this);
    // 注意：QSqlDatabase::database()返回的是默认数据库连接的引用
    // 我们需要通过不同的方式传递数据库连接
    fingerprintManager->setDatabase(nullptr);  // 先设置为nullptr，让FingerprintManager内部获取数据库连接
    
    // 确保指纹表存在
    if (!fingerprintManager->createFingerprintTable()) {
        qDebug() << "✗ 指纹表创建失败";
    }
    
    qDebug() << "✓ 指纹管理器初始化完成";
    
    // 初始化注册密钥管理器
    registrationKeyManager = new RegistrationKeyManager(this);
    qDebug() << "✓ 注册密钥管理器初始化完成";
    
    // 清理过期密钥
    int cleanedKeys = registrationKeyManager->cleanupExpiredKeys();
    if (cleanedKeys > 0) {
        qDebug() << "✓ 清理了" << cleanedKeys << "个过期注册密钥";
    }
    
    // 初始化注册密钥表格模型
    keyModel = new QSqlQueryModel(this);
    ui->keyTableView->setModel(keyModel);
    refreshKeyTable();
    
    // 设置管理窗口的MainWidget指针
    if (ui->manageWidget) {
        ui->manageWidget->setMainWidget(this);  // 设置MainWidget指针
        qDebug() << "✓ 管理窗口已初始化";
    }

    // 给模型绑定表格
    model.setTable("employee");
    if(model.select()) {
        qDebug() << "✓ 数据库连接成功";
        qDebug() << "  员工表记录数: " << model.rowCount();
    } else {
        qDebug() << "✗ 数据库连接失败";
    }

    // 初始化数据库结构（创建映射表等）
    initializeDatabaseSchema();

    // 清理孤立的人脸数据（数据库中不存在的用户）
    cleanupOrphanedFaceData();

    qDebug() << "=== 服务器初始化完成 ===";
}

MainWidget::~MainWidget()
{
    // 清理所有客户端
    QMutexLocker locker(&clientsMutex);
    for(auto it = clients.begin(); it != clients.end(); ++it) {
        if(it.value()->socket) {
            it.value()->socket->disconnectFromHost();
            it.value()->socket->deleteLater();
        }
        delete it.value();
    }
    clients.clear();
    
    // 等待线程池完成
    threadPool->waitForDone(5000);
    
    delete ui;
}

void MainWidget::accept_client()
{
    // 获取新客户端连接
    QTcpSocket* newSocket = mserver.nextPendingConnection();

    if(!newSocket) {
        return;
    }

    QString clientAddress = newSocket->peerAddress().toString();
    quint16 clientPort = newSocket->peerPort();

        qDebug() << "\n=== 新客户端连接 ===";
        qDebug() << "✓ 客户端连接成功";
        qDebug() << "  客户端IP: " << clientAddress;
        qDebug() << "  客户端端口: " << clientPort;
    qDebug() << "  当前客户端数: " << (clients.size() + 1);

    // 创建客户端信息
    ClientInfo* clientInfo = new ClientInfo(newSocket, clientAddress, clientPort);
    
    // 添加到客户端列表（线程安全）
    {
        QMutexLocker locker(&clientsMutex);
        clients[newSocket] = clientInfo;
    }

    // 连接信号槽
    connect(newSocket, &QTcpSocket::readyRead, this, [this, newSocket]() {
        this->read_data();
    });
    
    connect(newSocket, &QTcpSocket::disconnected, this, [this, newSocket]() {
        this->client_disconnected();
        });

    qDebug() << "  等待接收人脸数据...";
}

void MainWidget::read_data()
{
    // 获取发送数据的Socket
    QTcpSocket* socket = qobject_cast<QTcpSocket*>(sender());
    if(!socket) {
        return;
    }

    // 查找客户端信息
    ClientInfo* clientInfo = findClientInfo(socket);
    if(!clientInfo) {
        qDebug() << "[错误] 未找到客户端信息";
        return;
    }

    if(!socket->bytesAvailable()) {
        return;
    }

    // 如果正在处理，忽略新数据
    if(clientInfo->is_processing) {
        static QMap<QTcpSocket*, int> skip_counts;
        skip_counts[socket] = skip_counts.value(socket, 0) + 1;
        if(skip_counts[socket] % 10 == 0) {
            qDebug() << "[接收] 客户端 " << clientInfo->address << ":" << clientInfo->port 
                     << " 正在处理，忽略新数据 (已忽略" << skip_counts[socket] << "次)";
        }
        return;
    }

    // 先peek数据检测是否为JSON格式
    // 如果jsonBuffer中已有数据，说明正在接收JSON，继续按JSON处理
    QByteArray peekData = socket->peek(1024);  // 先查看前1024字节
    
    if(!clientInfo->jsonBuffer.isEmpty() || isJsonData(peekData)) {
        // JSON数据处理 - 支持大数据包分包接收
        // 累积接收数据直到收到完整的JSON
        clientInfo->jsonBuffer.append(socket->readAll());
        
        // 尝试解析JSON，检查是否完整
        QJsonParseError parseError;
        QJsonDocument doc = QJsonDocument::fromJson(clientInfo->jsonBuffer, &parseError);
        
        if(parseError.error == QJsonParseError::NoError) {
            // JSON解析成功，说明数据完整
            QByteArray jsonData = clientInfo->jsonBuffer;
            clientInfo->jsonBuffer.clear();  // 清空缓冲区
            
            qDebug() << "\n[接收] ===== 接收到JSON请求 =====";
            qDebug() << "[接收] 客户端: " << clientInfo->address << ":" << clientInfo->port;
            qDebug() << "[接收] 数据大小: " << jsonData.size() << "字节";
            qDebug() << "[接收] JSON内容(前500字符): " << QString::fromUtf8(jsonData.left(500));
            
            QJsonObject obj = doc.object();
            QString type = obj.value("type").toString();
            
            qDebug() << "[接收] 请求类型: " << type;
            
            // 如果是用户注册请求，打印registration_key字段用于调试
            if(type == "user_register") {
                QString regKey = obj.value("registration_key").toString();
                qDebug() << "[接收] registration_key字段值: \"" << regKey << "\" (长度:" << regKey.length() << ")";
                qDebug() << "[接收] registration_key字段是否存在: " << obj.contains("registration_key");
            }
            
            if(type == "password_verify") {
                QString password = obj.value("password").toString();
                qDebug() << "[处理] 开始密码验证处理...";
                clientInfo->is_processing = true;
                clientInfo->process_start_time = std::chrono::steady_clock::now();
                processPasswordVerify(password, clientInfo);
            } else if(type == "fingerprint_verify") {
                int userId = obj.value("user_id").toInt();
                qDebug() << "[处理] 开始指纹验证处理...";
                clientInfo->is_processing = true;
                clientInfo->process_start_time = std::chrono::steady_clock::now();
                processFingerprintVerify(userId, clientInfo);
            } else if(type == "fingerprint_register") {
                QString employeeId = obj.value("employee_id").toString();
                QString fingerprintData = obj.value("fingerprint_data").toString();
                qDebug() << "[处理] 开始指纹注册处理...";
                clientInfo->is_processing = true;
                clientInfo->process_start_time = std::chrono::steady_clock::now();
                processFingerprintRegister(employeeId, fingerprintData, clientInfo);
            } else if(type == "key_verify") {
                QString keyValue = obj.value("key").toString();
                qDebug() << "[处理] 开始注册密钥验证处理...";
                clientInfo->is_processing = true;
                clientInfo->process_start_time = std::chrono::steady_clock::now();
                processKeyVerify(keyValue, clientInfo);
            } else if(type == "user_register") {
                qDebug() << "[处理] 开始用户注册处理...";
                clientInfo->is_processing = true;
                clientInfo->process_start_time = std::chrono::steady_clock::now();
                processUserRegister(obj, clientInfo);
            } else if(type == "get_valid_fingerprint_ids") {
                qDebug() << "[处理] 开始获取有效指纹ID列表...";
                clientInfo->is_processing = true;
                clientInfo->process_start_time = std::chrono::steady_clock::now();
                processGetValidFingerprintIds(clientInfo);
            } else if(type == "time_sync") {
                // 时间同步：客户端连接后主动请求服务器时间
                const QDateTime now = QDateTime::currentDateTime();
                QJsonObject responseObj;
                responseObj["type"] = "time_sync_result";
                responseObj["server_time"] = now.toString("yyyy-MM-dd HH:mm:ss");
                responseObj["server_epoch_ms"] = static_cast<qint64>(now.toMSecsSinceEpoch());

                QJsonDocument doc(responseObj);
                QByteArray responseData = doc.toJson(QJsonDocument::Compact);

                qDebug() << "[时间同步] 发送服务器时间给客户端:" << clientInfo->address << ":" << clientInfo->port
                         << responseObj["server_time"].toString();

                socket->write(responseData);
                socket->flush();
            } else {
                qDebug() << "[错误] 未知的请求类型: " << type;
            }
        } else {
            // JSON解析失败，可能数据还没接收完，继续等待
            qDebug() << "[接收] JSON数据未完整接收，当前缓冲区大小: " << clientInfo->jsonBuffer.size() << "字节，继续等待...";
            
            // 防止缓冲区无限增长（设置10MB上限）
            if(clientInfo->jsonBuffer.size() > 10 * 1024 * 1024) {
                qDebug() << "[错误] JSON数据过大，超过10MB限制，丢弃数据";
                clientInfo->jsonBuffer.clear();
            }
            return;
        }
    } else {
        // 图片数据处理（人脸识别请求）- 使用QDataStream协议
        QDataStream stream(socket);
        stream.setVersion(QDataStream::Qt_5_9);

        if(clientInfo->bsize == 0) {
            if(socket->bytesAvailable() < (qint64)sizeof(clientInfo->bsize)) {
                return;
            }
            stream >> clientInfo->bsize;
            qDebug() << "\n[接收] ===== 开始接收图片数据 =====";
            qDebug() << "[接收] 客户端: " << clientInfo->address << ":" << clientInfo->port;
            qDebug() << "[接收] 数据大小: " << clientInfo->bsize << "字节";
        }

        if(socket->bytesAvailable() < clientInfo->bsize) {
            return;
        }

        QByteArray data;
        stream >> data;

        qDebug() << "[接收] ✓ 图片数据接收完成";
        qDebug() << "  实际接收大小: " << data.size() << "字节";

        clientInfo->bsize = 0;
        clientInfo->is_processing = true;
        clientInfo->process_start_time = std::chrono::steady_clock::now();

        processFaceData(data, clientInfo);
    }
}

// 检测数据是否为JSON格式
bool MainWidget::isJsonData(const QByteArray& data)
{
    if(data.isEmpty()) {
        return false;
    }
    
    // 跳过前导空白字符
    int i = 0;
    while(i < data.size() && (data[i] == ' ' || data[i] == '\t' || data[i] == '\n' || data[i] == '\r')) {
        i++;
    }
    
    // JSON数据以 '{' 或 '[' 开头
    if(i < data.size() && (data[i] == '{' || data[i] == '[')) {
        return true;
    }
    
    return false;
}

// 处理人脸图片数据
void MainWidget::processFaceData(const QByteArray& data, ClientInfo* clientInfo)
{
    auto receive_start = std::chrono::steady_clock::now();

    if(data.size() == 0) {
        qDebug() << "[错误] 接收到的数据为空";
        clientInfo->is_processing = false;
        return;
    }

    // 解码图片 - 优化：直接使用QByteArray数据，避免额外拷贝
    auto decode_start = std::chrono::steady_clock::now();
    cv::Mat faceImage;
    // 直接使用QByteArray的data，避免memcpy拷贝
    faceImage = cv::imdecode(cv::Mat(1, data.size(), CV_8UC1, (void*)data.constData()), cv::IMREAD_COLOR);
    auto decode_end = std::chrono::steady_clock::now();
    auto decode_time = std::chrono::duration_cast<std::chrono::milliseconds>(decode_end - decode_start);

    if(faceImage.empty()) {
        qDebug() << "[错误] OpenCV图片解码失败";
        clientInfo->is_processing = false;
        return;
    }

    qDebug() << "[解码] ✓ 图片解码成功";
    qDebug() << "  OpenCV Mat尺寸: " << faceImage.cols << "x" << faceImage.rows;
    qDebug() << "  解码耗时: " << decode_time.count() << "ms";

    // 显示图片 - 优化：只在需要时才缩放，避免不必要的计算
    QPixmap mmp;
    bool loadSuccess = mmp.loadFromData(data, "jpg");
    if(loadSuccess) {
        QSize labelSize = ui->piclb->size();
        if(mmp.size() != labelSize && !labelSize.isEmpty()) {
            mmp = mmp.scaled(labelSize, Qt::KeepAspectRatio, Qt::SmoothTransformation);
        }
        ui->piclb->setPixmap(mmp);
    }

    qDebug() << "[处理] 开始人脸识别处理...";
    qDebug() << "[计时] ===== 服务器端开始计时 =====";

    // 创建识别任务
    RecognitionTask* task = new RecognitionTask();
    task->setTaskData(faceImage, clientInfo, faceObject);  // 传入FaceEngine对象
    
    // 连接信号槽
    connect(task, &RecognitionTask::taskCompleted, this, &MainWidget::onRecognitionCompleted, Qt::QueuedConnection);
    connect(task, &RecognitionTask::coordReady, this, &MainWidget::onCoordReady, Qt::QueuedConnection);
    
    // 添加到任务列表（管理生命周期）
    {
        QMutexLocker locker(&tasksMutex);
        tasks.append(task);
    }
    
    // 提交到线程池
    qDebug() << "[线程池] 提交识别任务到线程池，客户端: " << clientInfo->address << ":" << clientInfo->port;
    qDebug() << "[线程池] 当前活跃线程数: " << threadPool->activeThreadCount() 
             << "/" << threadPool->maxThreadCount();
    threadPool->start(task);
}

// 处理密码验证请求
void MainWidget::processPasswordVerify(const QString& password, ClientInfo* clientInfo)
{
    qDebug() << "\n[密码验证] ===== 开始密码验证 =====";
    qDebug() << "[密码验证] 客户端: " << clientInfo->address << ":" << clientInfo->port;
    
    // 检查密码是否为空
    if(password.isEmpty()) {
        qDebug() << "[密码验证] 密码为空";
        sendPasswordResult(false, "", "", "密码不能为空", clientInfo);
        clientInfo->is_processing = false;
        return;
    }
    
    // 查询数据库验证密码
    QSqlQuery query;
    query.prepare("SELECT employeeID, name FROM employee WHERE password = ?");
    query.bindValue(0, password);
    
    bool querySuccess = query.exec();
    
    if(!querySuccess) {
        qDebug() << "[错误] 数据库查询失败: " << query.lastError().text();
        sendPasswordResult(false, "", "", "服务器内部错误", clientInfo);
        clientInfo->is_processing = false;
        return;
    }
    
    if(query.next()) {
        // 密码验证成功
        QString employeeID = query.value("employeeID").toString();
        QString name = query.value("name").toString();
        
        qDebug() << "[密码验证] ✓ 验证成功";
        qDebug() << "  工号: " << employeeID;
        qDebug() << "  姓名: " << name;
        
        // 记录考勤 - 密码验证成功
        QSqlQuery attendanceQuery;
        attendanceQuery.prepare("INSERT INTO attendance(employeeID, unlock_type, result) VALUES(?, 'password', 'success')");
        attendanceQuery.bindValue(0, employeeID);
        if(attendanceQuery.exec()) {
            qDebug() << "[数据库] 密码验证考勤记录插入成功 (success)";
        } else {
            qDebug() << "[错误] 密码验证考勤记录插入失败: " << attendanceQuery.lastError().text();
        }
        
        sendPasswordResult(true, employeeID, name, "验证成功", clientInfo);
    } else {
        // 密码验证失败
        qDebug() << "[密码验证] ✗ 验证失败 - 密码错误";
        
        // 记录考勤 - 密码验证失败（employeeID为NULL表示未知用户）
        QSqlQuery attendanceQuery;
        attendanceQuery.prepare("INSERT INTO attendance(employeeID, unlock_type, result) VALUES(NULL, 'password', 'fail')");
        if(attendanceQuery.exec()) {
            qDebug() << "[数据库] 密码验证考勤记录插入成功 (fail)";
        } else {
            qDebug() << "[错误] 密码验证考勤记录插入失败: " << attendanceQuery.lastError().text();
        }
        
        sendPasswordResult(false, "", "", "密码错误，请重新输入", clientInfo);
    }
    
    // 计算处理时间
    auto process_end = std::chrono::steady_clock::now();
    auto process_time = std::chrono::duration_cast<std::chrono::milliseconds>(
        process_end - clientInfo->process_start_time).count();
    
    qDebug() << "[密码验证] 处理耗时: " << process_time << "ms";
    qDebug() << "[密码验证] ===== 密码验证结束 =====\n";
    
    clientInfo->is_processing = false;
}

// 处理指纹验证请求
void MainWidget::processFingerprintVerify(int userId, ClientInfo* clientInfo)
{
    qDebug() << "\n[指纹验证] ===== 开始指纹验证 =====";
    qDebug() << "[指纹验证] 客户端: " << clientInfo->address << ":" << clientInfo->port;
    qDebug() << "[指纹验证] 用户ID: " << userId;
    qDebug() << "[DEBUG] 传入的fingerprint_id = " << userId;
    
    // 使用指纹管理器进行验证，传入userId作为fingerprintId
    FingerprintManager::VerificationResult result = fingerprintManager->verifyFingerprint(userId, userId);
    
    // 发送验证结果
    sendFingerprintResult(result, clientInfo);
    
    // 计算处理时间
    auto process_end = std::chrono::steady_clock::now();
    auto process_time = std::chrono::duration_cast<std::chrono::milliseconds>(
        process_end - clientInfo->process_start_time).count();
    
    qDebug() << "[指纹验证] 处理耗时: " << process_time << "ms";
    qDebug() << "[指纹验证] ===== 指纹验证结束 =====\n";
    
    clientInfo->is_processing = false;
}

// 处理指纹注册请求
void MainWidget::processFingerprintRegister(const QString& employeeId, const QString& fingerprintData, ClientInfo* clientInfo)
{
    qDebug() << "\n[指纹注册] ===== 开始指纹注册 =====";
    qDebug() << "[指纹注册] 客户端: " << clientInfo->address << ":" << clientInfo->port;
    qDebug() << "[指纹注册] 员工ID: " << employeeId;
    
    // 使用指纹管理器进行注册
    bool result = fingerprintManager->registerFingerprintFromNetwork(employeeId, fingerprintData);
    
    // 发送注册结果（注意：实际结果会通过信号异步返回）
    if (!result) {
        sendFingerprintRegisterResult(false, "指纹注册请求失败", clientInfo);
    }
    // 成功的结果会通过FingerprintManager的信号返回
    
    // 计算处理时间
    auto process_end = std::chrono::steady_clock::now();
    auto process_time = std::chrono::duration_cast<std::chrono::milliseconds>(
        process_end - clientInfo->process_start_time).count();
    
    qDebug() << "[指纹注册] 处理耗时: " << process_time << "ms";
    qDebug() << "[指纹注册] ===== 指纹注册结束 =====\n";
    
    clientInfo->is_processing = false;
}

// 处理注册密钥验证请求
void MainWidget::processKeyVerify(const QString& keyValue, ClientInfo* clientInfo)
{
    qDebug() << "\n[密钥验证] ===== 开始注册密钥验证 =====";
    qDebug() << "[密钥验证] 客户端: " << clientInfo->address << ":" << clientInfo->port;
    qDebug() << "[密钥验证] 密钥: " << keyValue;
    
    // 检查密钥是否为空
    if(keyValue.isEmpty()) {
        qDebug() << "[密钥验证] 密钥为空";
        sendKeyVerifyResult(false, "注册密钥不能为空", clientInfo);
        clientInfo->is_processing = false;
        return;
    }
    
    // 使用注册密钥管理器验证密钥
    bool isValid = registrationKeyManager->verifyRegistrationKey(keyValue);
    
    if(isValid) {
        qDebug() << "[密钥验证] ✓ 验证成功";
        sendKeyVerifyResult(true, "注册密钥验证成功", clientInfo);
    } else {
        qDebug() << "[密钥验证] ✗ 验证失败";
        
        // 获取密钥详细信息以提供更具体的错误信息
        RegistrationKey keyInfo = registrationKeyManager->getRegistrationKeyInfo(keyValue);
        QString errorMessage = "注册密钥无效";
        
        if (!keyInfo.keyValue.isEmpty()) {
            if (!keyInfo.isActive) {
                errorMessage = "注册密钥已被禁用";
            } else if (keyInfo.isExpired()) {
                errorMessage = "注册密钥已过期";
            } else if (keyInfo.usedCount >= keyInfo.maxUses) {
                errorMessage = "注册密钥使用次数已达上限";
            }
        } else {
            errorMessage = "注册密钥不存在";
        }
        
        sendKeyVerifyResult(false, errorMessage, clientInfo);
    }
    
    // 计算处理时间
    auto process_end = std::chrono::steady_clock::now();
    auto process_time = std::chrono::duration_cast<std::chrono::milliseconds>(
        process_end - clientInfo->process_start_time).count();
    
    qDebug() << "[密钥验证] 处理耗时: " << process_time << "ms";
    qDebug() << "[密钥验证] ===== 注册密钥验证结束 =====\n";
    
    clientInfo->is_processing = false;
}

// 发送注册密钥验证结果到客户端
void MainWidget::sendKeyVerifyResult(bool success, const QString& message, ClientInfo* clientInfo)
{
    // 检查客户端连接状态
    if(!clientInfo->socket || clientInfo->socket->state() != QTcpSocket::ConnectedState) {
        qDebug() << "[警告] 客户端未连接，无法发送注册密钥验证结果";
        return;
    }
    
    // 构建JSON响应
    QJsonObject responseObj;
    responseObj["type"] = "key_verify_result";
    responseObj["success"] = success;
    responseObj["message"] = message;
    
    QJsonDocument doc(responseObj);
    QByteArray responseData = doc.toJson(QJsonDocument::Compact);
    
    qDebug() << "[发送] 发送注册密钥验证结果给客户端: " << clientInfo->address << ":" << clientInfo->port;
    qDebug() << "[发送] 响应内容: " << responseData;
    
    qint64 written = clientInfo->socket->write(responseData);
    
    if(written == -1) {
        qDebug() << "[错误] 发送注册密钥验证结果失败: " << clientInfo->socket->errorString();
    } else {
        qDebug() << "[发送] ✓ 发送成功: " << written << "字节";
    }
}

// 发送指纹注册结果到客户端
void MainWidget::sendFingerprintRegisterResult(bool success, const QString& message, ClientInfo* clientInfo)
{
    // 检查客户端连接状态
    if(!clientInfo->socket || clientInfo->socket->state() != QTcpSocket::ConnectedState) {
        qDebug() << "[警告] 客户端未连接，无法发送指纹注册结果";
        return;
    }
    
    // 构建JSON响应
    QJsonObject responseObj;
    responseObj["type"] = "fingerprint_register_result";
    responseObj["success"] = success;
    responseObj["message"] = message;
    
    QJsonDocument doc(responseObj);
    QByteArray responseData = doc.toJson(QJsonDocument::Compact);
    
    qDebug() << "[发送] 发送指纹注册结果给客户端: " << clientInfo->address << ":" << clientInfo->port;
    qDebug() << "[发送] 响应内容: " << responseData;
    
    qint64 written = clientInfo->socket->write(responseData);
    
    if(written == -1) {
        qDebug() << "[错误] 发送指纹注册结果失败: " << clientInfo->socket->errorString();
    } else {
        qDebug() << "[发送] ✓ 发送成功: " << written << "字节";
    }
}
void MainWidget::sendFingerprintResult(const FingerprintManager::VerificationResult& result, ClientInfo* clientInfo)
{
    // 检查客户端连接状态
    if(!clientInfo->socket || clientInfo->socket->state() != QTcpSocket::ConnectedState) {
        qDebug() << "[警告] 客户端未连接，无法发送指纹验证结果";
        return;
    }
    
    // 构建JSON响应
    QJsonObject responseObj;
    responseObj["type"] = "fingerprint_result";
    responseObj["success"] = result.success;
    responseObj["user_id"] = result.userId;
    responseObj["user_name"] = result.userName;
    responseObj["department"] = result.department;
    responseObj["message"] = result.message;
    
    QJsonDocument doc(responseObj);
    QByteArray responseData = doc.toJson(QJsonDocument::Compact);
    
    qDebug() << "[发送] 发送指纹验证结果给客户端: " << clientInfo->address << ":" << clientInfo->port;
    qDebug() << "[发送] 响应内容: " << responseData;
    
    qint64 written = clientInfo->socket->write(responseData);
    
    if(written == -1) {
        qDebug() << "[错误] 发送指纹验证结果失败: " << clientInfo->socket->errorString();
    } else {
        qDebug() << "[发送] ✓ 发送成功: " << written << "字节";
    }
}

// 发送密码验证结果到客户端
void MainWidget::sendPasswordResult(bool success, const QString& userId, const QString& userName,
                                    const QString& message, ClientInfo* clientInfo)
{
    // 检查客户端连接状态
    if(!clientInfo->socket || clientInfo->socket->state() != QTcpSocket::ConnectedState) {
        qDebug() << "[警告] 客户端未连接，无法发送密码验证结果";
        return;
    }
    
    // 构建JSON响应
    QJsonObject responseObj;
    responseObj["type"] = "password_result";
    responseObj["success"] = success;
    responseObj["user_id"] = userId;
    responseObj["user_name"] = userName;
    responseObj["message"] = message;
    
    QJsonDocument doc(responseObj);
    QByteArray responseData = doc.toJson(QJsonDocument::Compact);
    
    qDebug() << "[发送] 发送密码验证结果给客户端: " << clientInfo->address << ":" << clientInfo->port;
    qDebug() << "[发送] 响应内容: " << responseData;
    
    qint64 written = clientInfo->socket->write(responseData);
    
    if(written == -1) {
        qDebug() << "[错误] 发送密码验证结果失败: " << clientInfo->socket->errorString();
    } else {
        qDebug() << "[发送] ✓ 发送成功: " << written << "字节";
    }
}

void MainWidget::onRecognitionCompleted(int64_t faceid, ClientInfo* clientInfo)
{
    processRecognitionResult(faceid, clientInfo);

    // 从任务列表中移除
    RecognitionTask* task = qobject_cast<RecognitionTask*>(sender());
    if(task) {
        QMutexLocker locker(&tasksMutex);
        tasks.removeAll(task);
        task->deleteLater();
}
}

void MainWidget::onCoordReady(QString coord, ClientInfo* clientInfo)
{
    // 检查客户端连接状态
    if(!clientInfo->socket || clientInfo->socket->state() != QTcpSocket::ConnectedState) {
        qDebug() << "[警告] 客户端未连接，无法发送坐标";
        return;
    }

    qDebug() << "[发送] 发送人脸坐标给客户端: " << clientInfo->address << ":" << clientInfo->port;
    QByteArray sendData = coord.toUtf8();
    qint64 written = clientInfo->socket->write(sendData);

    if(written == -1) {
        qDebug() << "[错误] 发送人脸坐标失败: " << clientInfo->socket->errorString();
    } else {
        qDebug() << "  发送成功: " << written << "字节";
    }
}

void MainWidget::processRecognitionResult(int64_t seetaFaceID, ClientInfo* clientInfo)
{
    // 检查客户端是否还存在
    if(!clientInfo || !findClientInfo(clientInfo->socket)) {
        qDebug() << "[警告] 客户端已断开，无法处理识别结果";
        return;
    }

    // 计算总处理时间
    auto recognition_end = std::chrono::steady_clock::now();
    auto total_process_time = std::chrono::duration_cast<std::chrono::milliseconds>(
        recognition_end - clientInfo->process_start_time).count();

    qDebug() << "\n[识别] ===== 识别结果返回 =====";
    qDebug() << "[识别] 客户端: " << clientInfo->address << ":" << clientInfo->port;
    qDebug() << "[识别] 接收到SeetaFace内部ID: " << seetaFaceID;
    qDebug() << "[识别] 总处理时间: " << total_process_time << "ms";

    // 检查客户端连接状态
    bool client_connected = (clientInfo->socket != nullptr && 
                             clientInfo->socket->state() == QTcpSocket::ConnectedState);

    if(!client_connected) {
        qDebug() << "[警告] 客户端已断开连接，无法发送识别结果";
        clientInfo->is_processing = false;
        return;
    }

    if(seetaFaceID < 0) {
        qDebug() << "[识别] 未识别到人脸";
        qDebug() << "[DEBUG] seeta_face_id = " << seetaFaceID << " (未检测到人脸)";
        QString sdmsg = QString("{\"employeeID\":\" \",\"name\":\" \",\"department\":\" \",\"time\":\" \",\"rect_x\":\"200\",\"rect_y\":\"200\"}");

        if(clientInfo->socket && clientInfo->socket->state() == QTcpSocket::ConnectedState) {
            qint64 written = clientInfo->socket->write(sdmsg.toUtf8());
        if(written == -1) {
                qDebug() << "[错误] 发送识别结果失败: " << clientInfo->socket->errorString();
        } else {
            qDebug() << "  发送未识别结果: " << written << "字节";
            }
        }

        // 记录考勤 - 人脸识别失败（未检测到人脸）
        QSqlQuery attendanceQuery;
        attendanceQuery.prepare("INSERT INTO attendance(employeeID, unlock_type, result, face_id) VALUES(NULL, 'face', 'fail', NULL)");
        if(attendanceQuery.exec()) {
            qDebug() << "[数据库] 人脸识别考勤记录插入成功 (fail - 未检测到人脸, face_id=NULL)";
        } else {
            qDebug() << "[错误] 人脸识别考勤记录插入失败: " << attendanceQuery.lastError().text();
        }

        clientInfo->is_processing = false;
        return;
    }

    // 步骤1: 通过SeetaFace ID查询映射表获取employeeID
    qDebug() << "[识别] 步骤1: 查询SeetaFace ID映射...";
    QSqlQuery mappingQuery;
    mappingQuery.prepare("SELECT employee_id FROM seeta_face_mapping WHERE seeta_face_id = ?");
    mappingQuery.bindValue(0, QVariant::fromValue<qint64>(seetaFaceID));
    
    if(!mappingQuery.exec()) {
        qDebug() << "[错误] 映射表查询失败: " << mappingQuery.lastError().text();
        qDebug() << "[DEBUG] seeta_face_id = " << seetaFaceID << " (映射表查询失败)";
        QString sdmsg = QString("{\"employeeID\":\" \",\"name\":\" \",\"department\":\" \",\"time\":\" \",\"rect_x\":\"200\",\"rect_y\":\"200\"}");
        
        if(clientInfo->socket && clientInfo->socket->state() == QTcpSocket::ConnectedState) {
            clientInfo->socket->write(sdmsg.toUtf8());
        }
        
        // 记录考勤 - 人脸识别失败（映射表查询失败）
        QSqlQuery attendanceQuery;
        attendanceQuery.prepare("INSERT INTO attendance(employeeID, unlock_type, result, face_id) VALUES(NULL, 'face', 'fail', ?)");
        attendanceQuery.bindValue(0, QVariant::fromValue<qint64>(seetaFaceID));
        if(attendanceQuery.exec()) {
            qDebug() << "[数据库] 人脸识别考勤记录插入成功 (fail - 映射表查询失败, face_id=" << seetaFaceID << ")";
        } else {
            qDebug() << "[错误] 人脸识别考勤记录插入失败: " << attendanceQuery.lastError().text();
        }
        
        clientInfo->is_processing = false;
        return;
    }

    if(!mappingQuery.next()) {
        qDebug() << "[识别] 未在映射表中找到对应记录";
        qDebug() << "[DEBUG] seeta_face_id = " << seetaFaceID << " (映射表中无此ID)";
        qDebug() << "[提示] 该用户可能是在旧系统中注册的，需要重新注册";
        QString sdmsg = QString("{\"employeeID\":\" \",\"name\":\" \",\"department\":\" \",\"time\":\" \",\"rect_x\":\"200\",\"rect_y\":\"200\"}");
        
        if(clientInfo->socket && clientInfo->socket->state() == QTcpSocket::ConnectedState) {
            clientInfo->socket->write(sdmsg.toUtf8());
        }
        
        // 记录考勤 - 人脸识别失败（映射表中无此ID）
        QSqlQuery attendanceQuery;
        attendanceQuery.prepare("INSERT INTO attendance(employeeID, unlock_type, result, face_id) VALUES(NULL, 'face', 'fail', ?)");
        attendanceQuery.bindValue(0, QVariant::fromValue<qint64>(seetaFaceID));
        if(attendanceQuery.exec()) {
            qDebug() << "[数据库] 人脸识别考勤记录插入成功 (fail - 映射表中无此ID, face_id=" << seetaFaceID << ")";
        } else {
            qDebug() << "[错误] 人脸识别考勤记录插入失败: " << attendanceQuery.lastError().text();
        }
        
        clientInfo->is_processing = false;
        return;
    }

    int employeeID = mappingQuery.value("employee_id").toInt();
    qDebug() << "[识别] ✓ 映射查询成功: SeetaFace ID " << seetaFaceID << " → employeeID " << employeeID;

    // 步骤2: 通过employeeID查询用户信息
    qDebug() << "[识别] 步骤2: 查询用户信息...";
    QSqlQuery userQuery;
    userQuery.prepare("SELECT employeeID, name FROM employee WHERE employeeID = ?");
    userQuery.bindValue(0, employeeID);
    
    if(!userQuery.exec()) {
        qDebug() << "[错误] 用户信息查询失败: " << userQuery.lastError().text();
        qDebug() << "[DEBUG] employee_id = " << employeeID << " (用户信息查询失败)";
        QString sdmsg = QString("{\"employeeID\":\" \",\"name\":\" \",\"department\":\" \",\"time\":\" \",\"rect_x\":\"200\",\"rect_y\":\"200\"}");
        
        if(clientInfo->socket && clientInfo->socket->state() == QTcpSocket::ConnectedState) {
            clientInfo->socket->write(sdmsg.toUtf8());
        }
        
        // 记录考勤 - 人脸识别失败（用户信息查询失败）
        QSqlQuery attendanceQuery;
        attendanceQuery.prepare("INSERT INTO attendance(employeeID, unlock_type, result, face_id) VALUES(?, 'face', 'fail', ?)");
        attendanceQuery.bindValue(0, employeeID);
        attendanceQuery.bindValue(1, QVariant::fromValue<qint64>(seetaFaceID));
        if(attendanceQuery.exec()) {
            qDebug() << "[数据库] 人脸识别考勤记录插入成功 (fail - 用户信息查询失败, face_id=" << seetaFaceID << ")";
        } else {
            qDebug() << "[错误] 人脸识别考勤记录插入失败: " << attendanceQuery.lastError().text();
        }
        
        clientInfo->is_processing = false;
        return;
    }

    // 判断是否查询到数据
    if(userQuery.next()) {
        QString employeeIDStr = userQuery.value("employeeID").toString();
        QString name = userQuery.value("name").toString();
        QString timeStr = QDateTime::currentDateTime().toString("yyyy-MM-dd HH:mm:ss");

        qDebug() << "[识别] ✓ 识别成功";
        qDebug() << "  ID号: " << employeeIDStr;
        qDebug() << "  姓名: " << name;
        qDebug() << "  时间: " << timeStr;
        qDebug() << "[DEBUG] seeta_face_id = " << seetaFaceID << " → employee_id = " << employeeID << " (识别成功)";

        QString sdmsg = QString("{\"employeeID\":\"%1\",\"name\":\"%2\",\"department\":\"软件\",\"time\":\"%3\",\"rect_x\":\"200\",\"rect_y\":\"200\"}")
                        .arg(employeeIDStr)
                        .arg(name)
                        .arg(timeStr);

        // 发送数据给客户端
        if(clientInfo->socket && clientInfo->socket->state() == QTcpSocket::ConnectedState) {
            qint64 written = clientInfo->socket->write(sdmsg.toUtf8());
        if(written == -1) {
                qDebug() << "[错误] 发送识别结果失败: " << clientInfo->socket->errorString();
            } else {
                qDebug() << "  发送识别结果: " << written << "字节";
            }
        } else {
            qDebug() << "[警告] 客户端未连接，无法发送识别结果";
        }

        // 把数据写进数据库--考勤表 - 同一人5秒内只记录一次
        QSqlQuery insertQuery;
        insertQuery.prepare("SELECT COUNT(*) FROM attendance WHERE employeeID = ? AND unlock_type = 'face' AND result = 'success' AND attendanceTime >= datetime('now', 'localtime', '-5 seconds')");
        insertQuery.bindValue(0, employeeIDStr);
        bool recentRecord = false;
        if (insertQuery.exec() && insertQuery.next()) {
            recentRecord = insertQuery.value(0).toInt() > 0;
        }
        if (!recentRecord) {
            insertQuery.prepare("INSERT INTO attendance(employeeID, unlock_type, result, face_id) VALUES(?, 'face', 'success', ?)");
            insertQuery.bindValue(0, employeeIDStr);
            insertQuery.bindValue(1, QVariant::fromValue<qint64>(seetaFaceID));
            if(insertQuery.exec()) {
                qDebug() << "[数据库] 人脸识别记录插入成功 (success, face_id=" << seetaFaceID << ")";
            } else {
                qDebug() << "[错误] 人脸识别考勤记录插入失败: " << insertQuery.lastError().text();
            }
        } else {
            qDebug() << "[数据库] 5秒内已有识别记录，跳过重复插入 (employeeID=" << employeeIDStr << ")";
        }
    } else {
        qDebug() << "[识别] 未在数据库中找到对应用户";
        qDebug() << "[DEBUG] employee_id = " << employeeID << " (用户不存在)";
        QString sdmsg = QString("{\"employeeID\":\" \",\"name\":\" \",\"department\":\" \",\"time\":\" \",\"rect_x\":\"200\",\"rect_y\":\"200\"}");
        
        if(clientInfo->socket && clientInfo->socket->state() == QTcpSocket::ConnectedState) {
            clientInfo->socket->write(sdmsg.toUtf8());
        }
        
        // 记录考勤 - 人脸识别失败（用户不存在）
        QSqlQuery attendanceQuery;
        attendanceQuery.prepare("INSERT INTO attendance(employeeID, unlock_type, result, face_id) VALUES(?, 'face', 'fail', ?)");
        attendanceQuery.bindValue(0, employeeID);
        attendanceQuery.bindValue(1, QVariant::fromValue<qint64>(seetaFaceID));
        if(attendanceQuery.exec()) {
            qDebug() << "[数据库] 人脸识别考勤记录插入成功 (fail - 用户不存在, face_id=" << seetaFaceID << ")";
        } else {
            qDebug() << "[错误] 人脸识别考勤记录插入失败: " << attendanceQuery.lastError().text();
        }
    }

    // 处理完成
    auto response_end = std::chrono::steady_clock::now();
    auto total_server_time = std::chrono::duration_cast<std::chrono::milliseconds>(
        response_end - clientInfo->process_start_time).count();
    
    clientInfo->is_processing = false;
    qDebug() << "[处理] ✓ 处理完成，准备接收下一张人脸...";
    qDebug() << "\n[计时] ===== 服务器端识别总耗时 =====";
    qDebug() << "[计时] 从接收图片到返回结果: " << total_server_time << "ms";
    qDebug() << "[计时] ============================\n";
    qDebug() << "[处理] ===== 本次处理流程结束 =====\n";
}

void MainWidget::client_disconnected()
{
    QTcpSocket* socket = qobject_cast<QTcpSocket*>(sender());
    if(!socket) {
        return;
    }

    ClientInfo* clientInfo = findClientInfo(socket);
    if(clientInfo) {
        qDebug() << "\n=== 客户端断开连接 ===";
        qDebug() << "客户端 " << clientInfo->address << ":" << clientInfo->port << " 已断开";
        removeClient(clientInfo);
    }
}

ClientInfo* MainWidget::findClientInfo(QTcpSocket* socket)
{
    QMutexLocker locker(&clientsMutex);
    return clients.value(socket, nullptr);
}

void MainWidget::removeClient(ClientInfo* clientInfo)
{
    if(!clientInfo) {
        return;
    }

    QMutexLocker locker(&clientsMutex);
    if(clientInfo->socket) {
        clients.remove(clientInfo->socket);
        clientInfo->socket->deleteLater();
    }
    delete clientInfo;
    
    qDebug() << "  当前客户端数: " << clients.size();
}

// 注册专用：Haar 检测 → 裁剪 → preprocessForSeetaFace，与认证流程完全一致
// 独立函数避免依赖 RecognitionTask 实例（后者会 emit 信号需要 clientInfo）
static cv::Mat preprocessFaceForRegister(const cv::Mat& image)
{
    // 1. 缩小到 320x240 做 Haar 检测（与认证 detectFace 逻辑一致）
    cv::Mat resized_gray;
    if (image.cols > 320 || image.rows > 240) {
        cv::Mat resized_bgr;
        cv::resize(image, resized_bgr, cv::Size(320, 240), 0, 0, cv::INTER_LINEAR);
        cv::cvtColor(resized_bgr, resized_gray, cv::COLOR_BGR2GRAY);
    } else {
        cv::cvtColor(image, resized_gray, cv::COLOR_BGR2GRAY);
    }

    // CLAHE 增强（与认证一致）
    try {
        cv::Mat clahe_out;
        cv::Ptr<cv::CLAHE> clahe = cv::createCLAHE(2.0, cv::Size(8, 8));
        clahe->apply(resized_gray, clahe_out);
        resized_gray = clahe_out;
    } catch (...) {}

    // 2. Haar 检测
    static cv::CascadeClassifier regCascade;
    static bool regCascadeLoaded = false;
    if (!regCascadeLoaded) {
        regCascadeLoaded = regCascade.load("./haarcascade_frontalface_alt2.xml");
        if (!regCascadeLoaded)
            regCascadeLoaded = regCascade.load("/home/hhs/face_server/haarcascade_frontalface_alt2.xml");
    }

    cv::Mat processedFace;
    std::vector<cv::Rect> faceRects;
    if (!regCascade.empty()) {
        regCascade.detectMultiScale(resized_gray, faceRects, 1.1, 3, 0,
                                    cv::Size(40, 40), cv::Size(200, 200));
    }

    if (!faceRects.empty()) {
        cv::Rect rect = faceRects[0];
        // 坐标反算回原图
        if (resized_gray.cols != image.cols || resized_gray.rows != image.rows) {
            float sx = (float)image.cols / resized_gray.cols;
            float sy = (float)image.rows / resized_gray.rows;
            rect.x = (int)(rect.x * sx);
            rect.y = (int)(rect.y * sy);
            rect.width  = (int)(rect.width  * sx);
            rect.height = (int)(rect.height * sy);
        }

        // 3. 裁剪（与 extractFaceRegion 逻辑一致：padding = min(face/3, 40)，正方形）
        int x = std::max(0, rect.x);
        int y = std::max(0, rect.y);
        int w = std::min(rect.width,  image.cols - x);
        int h = std::min(rect.height, image.rows - y);
        int padding = std::min(40, std::min(w, h) / 3);
        x = std::max(0, x - padding);
        y = std::max(0, y - padding);
        w = std::min(w + 2 * padding, image.cols - x);
        h = std::min(h + 2 * padding, image.rows - y);
        if (w != h) {
            int size = std::max(w, h);
            int cx = x + w / 2, cy = y + h / 2;
            x = std::max(0, cx - size / 2);
            y = std::max(0, cy - size / 2);
            w = std::min(size, image.cols - x);
            h = std::min(size, image.rows - y);
        }
        cv::Mat faceRegion = image(cv::Rect(x, y, w, h));
        processedFace = RecognitionTask::preprocessForSeetaFace(faceRegion);
        qDebug() << "[注册预处理] Haar 检测到人脸，裁剪区域:" << w << "x" << h
                 << "→ 预处理后:" << processedFace.cols << "x" << processedFace.rows;
    } else {
        // 未检测到人脸框，对全图做预处理（兜底）
        processedFace = RecognitionTask::preprocessForSeetaFace(image);
        qDebug() << "[注册预处理] 未检测到人脸框，对全图预处理";
    }

    return processedFace;
}

// 处理用户注册请求
void MainWidget::processUserRegister(const QJsonObject& registerData, ClientInfo* clientInfo)
{
    qDebug() << "\n[用户注册] ===== 开始用户注册 =====";
    qDebug() << "[用户注册] 客户端: " << clientInfo->address << ":" << clientInfo->port;
    
    // 1. 验证注册密钥
    QString registrationKey = registerData.value("registration_key").toString();
    if (registrationKey.isEmpty()) {
        qDebug() << "[用户注册] 注册密钥为空";
        sendUserRegisterResult(false, -1, "注册密钥不能为空", clientInfo);
        clientInfo->is_processing = false;
        return;
    }
    
    // 验证注册密钥（不消耗使用次数，只验证有效性）
    RegistrationKey keyInfo = registrationKeyManager->getRegistrationKeyInfo(registrationKey);
    if (keyInfo.keyValue.isEmpty() || !keyInfo.isUsable()) {
        QString errorMessage = "注册密钥无效";
        if (!keyInfo.keyValue.isEmpty()) {
            if (!keyInfo.isActive) {
                errorMessage = "注册密钥已被禁用";
            } else if (keyInfo.isExpired()) {
                errorMessage = "注册密钥已过期";
            } else if (keyInfo.usedCount >= keyInfo.maxUses) {
                errorMessage = "注册密钥使用次数已达上限";
            }
        } else {
            errorMessage = "注册密钥不存在";
        }
        
        qDebug() << "[用户注册] 注册密钥验证失败:" << errorMessage;
        sendUserRegisterResult(false, -1, errorMessage, clientInfo);
        clientInfo->is_processing = false;
        return;
    }
    
    // 2. 提取用户信息
    QJsonObject userInfo = registerData.value("user_info").toObject();
    QString name = userInfo.value("name").toString().trimmed();
    QString phone = userInfo.value("phone").toString().trimmed();
    QString address = userInfo.value("address").toString().trimmed();
    QString sex = userInfo.value("sex").toString().trimmed();
    QString password = registerData.value("password").toString();
    
    // 验证用户信息完整性
    if (name.isEmpty() || phone.isEmpty() || address.isEmpty() || sex.isEmpty() || password.isEmpty()) {
        qDebug() << "[用户注册] 用户信息不完整";
        sendUserRegisterResult(false, -1, "用户信息不完整，请填写所有必需字段", clientInfo);
        clientInfo->is_processing = false;
        return;
    }
    
    qDebug() << "[用户注册] 用户信息:";
    qDebug() << "  姓名:" << name;
    qDebug() << "  性别:" << sex;
    qDebug() << "  电话:" << phone;
    qDebug() << "  地址:" << address;
    
    // 3. 检查用户是否已存在（通过电话号码）
    QSqlQuery checkQuery;
    checkQuery.prepare("SELECT employeeID FROM employee WHERE phone = ?");
    checkQuery.bindValue(0, phone);
    
    if (!checkQuery.exec()) {
        qDebug() << "[错误] 检查用户是否存在失败:" << checkQuery.lastError().text();
        sendUserRegisterResult(false, -1, "服务器内部错误", clientInfo);
        clientInfo->is_processing = false;
        return;
    }
    
    if (checkQuery.next()) {
        qDebug() << "[用户注册] 用户已存在，电话:" << phone;
        sendUserRegisterResult(false, -1, "该电话号码已被注册", clientInfo);
        clientInfo->is_processing = false;
        return;
    }
    
    // 4. 处理人脸数据
    QString faceDataBase64 = registerData.value("face_data").toString();
    int64_t seetaFaceID = -1;  // SeetaFace内部ID
    
    if (!faceDataBase64.isEmpty()) {
        QByteArray faceImageData = QByteArray::fromBase64(faceDataBase64.toUtf8());
        
        // 解码人脸图像（客户端发来的是 640x480 彩色原图）
        cv::Mat faceImage = cv::imdecode(
            cv::Mat(1, faceImageData.size(), CV_8UC1, (void*)faceImageData.constData()),
            cv::IMREAD_COLOR);
        
        if (!faceImage.empty()) {
            qDebug() << "[用户注册] 解码人脸图像成功:" << faceImage.cols << "x" << faceImage.rows;

            // 与认证流程完全一致：Haar 检测 → 裁剪 → preprocessForSeetaFace
            cv::Mat processedFace = preprocessFaceForRegister(faceImage);

            // 使用SeetaFace注册人脸，获取SeetaFace内部ID
            seetaFaceID = faceObject->face_register(processedFace);
            if (seetaFaceID >= 0) {
                qDebug() << "[用户注册] 人脸注册成功，SeetaFace内部ID:" << seetaFaceID;
            } else {
                qDebug() << "[用户注册] 人脸注册失败（SeetaFace未检测到人脸）";
            }
        } else {
            qDebug() << "[用户注册] 人脸图像解码失败";
        }
    }
    
    // 5. 处理指纹数据 - 从客户端获取硬件模块返回的指纹ID
    QString fingerprintDataBase64 = registerData.value("fingerprint_data").toString();
    int hardwareFingerprintId = registerData.value("fingerprint_id").toInt(-1);  // 从客户端获取硬件指纹ID
    QByteArray fingerprintData;
    
    if (!fingerprintDataBase64.isEmpty()) {
        fingerprintData = QByteArray::fromBase64(fingerprintDataBase64.toUtf8());
        qDebug() << "[用户注册] 指纹数据大小:" << fingerprintData.size() << "字节";
        qDebug() << "[用户注册] 硬件指纹ID:" << hardwareFingerprintId;
    }
    
    // 6. 插入用户数据到数据库 - 先不设置faceID，等获取employeeID后再设置
    QSqlQuery insertQuery;
    insertQuery.prepare("INSERT INTO employee (name, sex, phone, address, password, headfile) "
                       "VALUES (?, ?, ?, ?, ?, ?)");
    insertQuery.bindValue(0, name);
    insertQuery.bindValue(1, sex);
    insertQuery.bindValue(2, phone);
    insertQuery.bindValue(3, address);
    insertQuery.bindValue(4, password);  // 注意：实际应用中应该加密存储
    insertQuery.bindValue(5, QString());  // headfile暂时为空
    
    if (!insertQuery.exec()) {
        qDebug() << "[错误] 用户信息插入数据库失败:" << insertQuery.lastError().text();
        sendUserRegisterResult(false, -1, "用户注册失败，请稍后重试", clientInfo);
        clientInfo->is_processing = false;
        return;
    }
    
    // 获取新插入用户的ID
    int newUserId = insertQuery.lastInsertId().toInt();
    qDebug() << "[用户注册] 用户信息插入成功，用户ID:" << newUserId;
    
    // 7. 创建SeetaFace ID映射并更新faceID
    if (seetaFaceID >= 0) {
        // 7.1 插入SeetaFace ID映射表
        QSqlQuery mappingQuery;
        mappingQuery.prepare("INSERT INTO seeta_face_mapping (seeta_face_id, employee_id) VALUES (?, ?)");
        mappingQuery.bindValue(0, QVariant::fromValue<qint64>(seetaFaceID));
        mappingQuery.bindValue(1, newUserId);
        
        if (mappingQuery.exec()) {
            qDebug() << "[用户注册] ✓ SeetaFace ID映射创建成功";
            qDebug() << "  SeetaFace内部ID:" << seetaFaceID;
            qDebug() << "  映射到employeeID:" << newUserId;
        } else {
            qDebug() << "[错误] SeetaFace ID映射创建失败:" << mappingQuery.lastError().text();
        }
        
        // 7.2 更新employee表的faceID字段（存储SeetaFace内部ID用于参考）
        QSqlQuery updateFaceIdQuery;
        updateFaceIdQuery.prepare("UPDATE employee SET faceID = ? WHERE employeeID = ?");
        updateFaceIdQuery.bindValue(0, QVariant::fromValue<qint64>(seetaFaceID));
        updateFaceIdQuery.bindValue(1, newUserId);
        
        if (updateFaceIdQuery.exec()) {
            qDebug() << "[用户注册] ✓ 人脸ID已更新为SeetaFace内部ID:" << seetaFaceID;
        } else {
            qDebug() << "[错误] 更新faceID失败:" << updateFaceIdQuery.lastError().text();
        }
    }
    
    // 8. 存储指纹数据（使用硬件模块分配的指纹ID）
    int assignedFingerprintId = -1;  // 最终使用的指纹ID
    
    if (!fingerprintData.isEmpty() && hardwareFingerprintId > 0) {
        // 直接使用硬件模块分配的指纹ID，不再从计数器分配
        assignedFingerprintId = hardwareFingerprintId;
        qDebug() << "[用户注册] 使用硬件模块分配的指纹ID:" << assignedFingerprintId;
        
        // 8.1 检查该fingerprint_id是否已被使用
        QSqlQuery checkQuery;
        checkQuery.prepare("SELECT employee_id FROM fingerprints WHERE fingerprint_id = ?");
        checkQuery.bindValue(0, assignedFingerprintId);
        
        if (checkQuery.exec() && checkQuery.next()) {
            int oldEmployeeId = checkQuery.value(0).toInt();
            qDebug() << "[警告] 指纹ID" << assignedFingerprintId << "已被员工" << oldEmployeeId << "使用";
            qDebug() << "[清理] 删除旧的指纹记录...";
            
            // 删除旧的指纹记录
            QSqlQuery deleteQuery;
            deleteQuery.prepare("DELETE FROM fingerprints WHERE fingerprint_id = ?");
            deleteQuery.bindValue(0, assignedFingerprintId);
            
            if (deleteQuery.exec()) {
                qDebug() << "[清理] ✓ 旧指纹记录已删除";
            } else {
                qDebug() << "[错误] 删除旧指纹记录失败:" << deleteQuery.lastError().text();
            }
        }
        
        // 8.2 插入新的指纹数据
        QSqlQuery fingerprintQuery;
        fingerprintQuery.prepare("INSERT INTO fingerprints (employee_id, fingerprint_id, fingerprint_data) "
                                "VALUES (?, ?, ?)");
        fingerprintQuery.bindValue(0, newUserId);
        fingerprintQuery.bindValue(1, assignedFingerprintId);  // 使用硬件分配的指纹ID
        fingerprintQuery.bindValue(2, fingerprintData);
        
        if (fingerprintQuery.exec()) {
            qDebug() << "[用户注册] 指纹数据存储成功，指纹ID:" << assignedFingerprintId;
        } else {
            qDebug() << "[错误] 指纹数据存储失败:" << fingerprintQuery.lastError().text();
            assignedFingerprintId = -1;  // 标记失败
        }
    } else if (!fingerprintData.isEmpty() && hardwareFingerprintId <= 0) {
        qDebug() << "[警告] 指纹数据存在但硬件指纹ID无效，跳过指纹存储";
    }
    
    // 9. 消耗注册密钥使用次数
    registrationKeyManager->verifyRegistrationKey(registrationKey);
    
    // 10. 记录注册成功
    qDebug() << "[用户注册] ✓ 用户注册成功";
    qDebug() << "  用户ID:" << newUserId;
    qDebug() << "  姓名:" << name;
    qDebug() << "  电话:" << phone;
    qDebug() << "  SeetaFace内部ID:" << (seetaFaceID >= 0 ? QString::number(seetaFaceID) : "无");
    qDebug() << "  人脸ID映射:" << (seetaFaceID >= 0 ? QString("SeetaFace ID %1 → employeeID %2").arg(seetaFaceID).arg(newUserId) : "无");
    qDebug() << "  指纹ID（硬件分配）:" << (assignedFingerprintId > 0 ? QString::number(assignedFingerprintId) : "无");
    qDebug() << "  指纹:" << (fingerprintData.isEmpty() ? "无" : "已存储");

    
    sendUserRegisterResult(true, newUserId, "用户注册成功", clientInfo, assignedFingerprintId);
    
    // 计算处理时间
    auto process_end = std::chrono::steady_clock::now();
    auto process_time = std::chrono::duration_cast<std::chrono::milliseconds>(
        process_end - clientInfo->process_start_time).count();
    
    qDebug() << "[用户注册] 处理耗时: " << process_time << "ms";
    qDebug() << "[用户注册] ===== 用户注册结束 =====\n";
    
    clientInfo->is_processing = false;
}

// 发送用户注册结果到客户端
void MainWidget::sendUserRegisterResult(bool success, int userId, const QString& message, ClientInfo* clientInfo, int fingerprintId)
{
    // 检查客户端连接状态
    if(!clientInfo->socket || clientInfo->socket->state() != QTcpSocket::ConnectedState) {
        qDebug() << "[警告] 客户端未连接，无法发送用户注册结果";
        return;
    }
    
    // 构建JSON响应
    QJsonObject responseObj;
    responseObj["type"] = "user_register_result";
    responseObj["success"] = success;
    responseObj["user_id"] = userId;
    responseObj["message"] = message;
    
    // 如果有指纹ID，添加到响应中（返回硬件模块分配的ID）
    if (fingerprintId > 0) {
        responseObj["fingerprint_id"] = fingerprintId;
        qDebug() << "[发送] 返回硬件指纹ID:" << fingerprintId << "（与硬件模块ID一致）";
    }
    
    QJsonDocument doc(responseObj);
    QByteArray responseData = doc.toJson(QJsonDocument::Compact);
    
    qDebug() << "[发送] 发送用户注册结果给客户端: " << clientInfo->address << ":" << clientInfo->port;
    qDebug() << "[发送] 响应内容: " << responseData;
    
    qint64 written = clientInfo->socket->write(responseData);
    
    if(written == -1) {
        qDebug() << "[错误] 发送用户注册结果失败: " << clientInfo->socket->errorString();
    } else {
        qDebug() << "[发送] ✓ 发送成功: " << written << "字节";
    }
}

// 注册密钥管理槽函数实现

void MainWidget::on_generateKeyBtn_clicked()
{
    qDebug() << "[密钥管理] 生成新的注册密钥";
    
    QString newKey = registrationKeyManager->generateRegistrationKey();
    if (!newKey.isEmpty()) {
        qDebug() << "[密钥管理] ✓ 密钥生成成功:" << newKey;
        
        // 显示生成的密钥给管理员
        QMessageBox::information(this, "密钥生成成功", 
                                QString("新的注册密钥已生成：\n\n%1\n\n"
                                       "有效期：24小时\n"
                                       "可用次数：5次\n\n"
                                       "请将此密钥提供给需要注册的用户。").arg(newKey));
        
        // 刷新密钥表格
        refreshKeyTable();
    } else {
        qDebug() << "[密钥管理] ✗ 密钥生成失败";
        QMessageBox::warning(this, "密钥生成失败", "无法生成注册密钥，请检查数据库连接。");
    }
}

void MainWidget::on_refreshKeyBtn_clicked()
{
    qDebug() << "[密钥管理] 刷新密钥列表";
    refreshKeyTable();
}

void MainWidget::on_deactivateKeyBtn_clicked()
{
    // 获取选中的密钥
    QModelIndexList selectedIndexes = ui->keyTableView->selectionModel()->selectedRows();
    if (selectedIndexes.isEmpty()) {
        QMessageBox::information(this, "请选择密钥", "请先选择要禁用的注册密钥。");
        return;
    }
    
    // 获取选中行的密钥值
    int row = selectedIndexes.first().row();
    QString keyValue = keyModel->data(keyModel->index(row, 1)).toString(); // 假设密钥值在第2列
    
    if (keyValue.isEmpty()) {
        QMessageBox::warning(this, "操作失败", "无法获取选中的密钥信息。");
        return;
    }
    
    // 确认禁用
    int ret = QMessageBox::question(this, "确认禁用", 
                                   QString("确定要禁用注册密钥：\n\n%1\n\n"
                                          "禁用后此密钥将无法使用。").arg(keyValue),
                                   QMessageBox::Yes | QMessageBox::No);
    
    if (ret == QMessageBox::Yes) {
        qDebug() << "[密钥管理] 禁用密钥:" << keyValue;
        
        if (registrationKeyManager->deactivateRegistrationKey(keyValue)) {
            qDebug() << "[密钥管理] ✓ 密钥禁用成功";
            QMessageBox::information(this, "禁用成功", "注册密钥已成功禁用。");
            refreshKeyTable();
        } else {
            qDebug() << "[密钥管理] ✗ 密钥禁用失败";
            QMessageBox::warning(this, "禁用失败", "无法禁用注册密钥，请重试。");
        }
    }
}

void MainWidget::on_cleanupKeysBtn_clicked()
{
    // 确认清理
    int ret = QMessageBox::question(this, "确认清理", 
                                   "确定要清理所有过期和已用完的注册密钥吗？\n\n"
                                   "此操作将永久删除这些密钥记录。",
                                   QMessageBox::Yes | QMessageBox::No);
    
    if (ret == QMessageBox::Yes) {
        qDebug() << "[密钥管理] 开始清理过期密钥";
        
        int cleanedCount = registrationKeyManager->cleanupExpiredKeys();
        
        qDebug() << "[密钥管理] ✓ 清理完成，删除了" << cleanedCount << "个密钥";
        
        if (cleanedCount > 0) {
            QMessageBox::information(this, "清理完成", 
                                   QString("成功清理了 %1 个过期或已用完的注册密钥。").arg(cleanedCount));
        } else {
            QMessageBox::information(this, "清理完成", "没有找到需要清理的过期密钥。");
        }
        
        refreshKeyTable();
    }
}

void MainWidget::refreshKeyTable()
{
    qDebug() << "[密钥管理] 刷新密钥表格";
    
    // 查询所有注册密钥
    QString queryStr = "SELECT "
                      "id AS '编号', "
                      "key_value AS '密钥', "
                      "max_uses AS '最大使用次数', "
                      "used_count AS '已使用次数', "
                      "CASE WHEN is_active = 1 THEN '激活' ELSE '禁用' END AS '状态', "
                      "datetime(expires_at, 'localtime') AS '过期时间', "
                      "datetime(created_at, 'localtime') AS '创建时间' "
                      "FROM registration_keys "
                      "ORDER BY created_at DESC";
    
    keyModel->setQuery(queryStr);
    
    if (keyModel->lastError().isValid()) {
        qDebug() << "[错误] 查询注册密钥失败:" << keyModel->lastError().text();
        QMessageBox::warning(this, "查询失败", "无法加载注册密钥列表，请检查数据库连接。");
        return;
    }
    
    // 设置表格列宽
    ui->keyTableView->resizeColumnsToContents();
    ui->keyTableView->horizontalHeader()->setStretchLastSection(true);
    
    qDebug() << "[密钥管理] ✓ 密钥表格刷新完成，共" << keyModel->rowCount() << "条记录";
}


// 初始化数据库结构（创建映射表等）
void MainWidget::initializeDatabaseSchema()
{
    qDebug() << "\n[数据库] ===== 初始化数据库结构 =====";
    
    // 检查映射表是否存在
    QSqlQuery checkTableQuery;
    checkTableQuery.exec("SELECT name FROM sqlite_master WHERE type='table' AND name='seeta_face_mapping'");
    
    if(checkTableQuery.next()) {
        qDebug() << "[数据库] ✓ seeta_face_mapping表已存在";
    } else {
        qDebug() << "[数据库] seeta_face_mapping表不存在，开始创建...";
        
        // 创建映射表
        QSqlQuery createTableQuery;
        QString createTableSql = 
            "CREATE TABLE IF NOT EXISTS seeta_face_mapping ("
            "    seeta_face_id INTEGER PRIMARY KEY,"
            "    employee_id INTEGER NOT NULL,"
            "    create_time TEXT DEFAULT(datetime('now','localtime')),"
            "    FOREIGN KEY (employee_id) REFERENCES employee(employeeID) ON DELETE CASCADE,"
            "    UNIQUE(employee_id)"
            ")";
        
        if(createTableQuery.exec(createTableSql)) {
            qDebug() << "[数据库] ✓ seeta_face_mapping表创建成功";
            
            // 创建索引
            QSqlQuery createIndexQuery;
            createIndexQuery.exec("CREATE INDEX IF NOT EXISTS idx_seeta_employee ON seeta_face_mapping(employee_id)");
            qDebug() << "[数据库] ✓ 索引创建成功";
        } else {
            qDebug() << "[数据库] ✗ seeta_face_mapping表创建失败: " << createTableQuery.lastError().text();
        }
    }
    
    qDebug() << "[数据库] ===== 数据库结构初始化完成 =====\n";
}

// 清理孤立的人脸数据（数据库中不存在的用户）
void MainWidget::cleanupOrphanedFaceData()
{
    qDebug() << "\n[清理] ===== 开始清理孤立的人脸数据 =====";
    
    // 检查映射表是否存在
    QSqlQuery checkTableQuery;
    checkTableQuery.exec("SELECT name FROM sqlite_master WHERE type='table' AND name='seeta_face_mapping'");
    
    if(!checkTableQuery.next()) {
        qDebug() << "[清理] ✗ 映射表不存在，跳过清理";
        qDebug() << "[清理] 这不应该发生，因为initializeDatabaseSchema()应该已经创建了表";
        return;
    }
    
    // 获取映射表中的所有SeetaFace ID
    QSqlQuery mappingQuery;
    mappingQuery.exec("SELECT seeta_face_id FROM seeta_face_mapping ORDER BY seeta_face_id");
    
    QSet<int64_t> validFaceIds;
    int64_t maxValidId = -1;
    
    while(mappingQuery.next()) {
        int64_t faceId = mappingQuery.value(0).toLongLong();
        validFaceIds.insert(faceId);
        if(faceId > maxValidId) {
            maxValidId = faceId;
        }
    }
    
    qDebug() << "[清理] 映射表中的有效人脸ID数量: " << validFaceIds.size();
    
    if(validFaceIds.isEmpty()) {
        qDebug() << "[清理] 映射表为空，将清理所有人脸数据";
        qDebug() << "[清理] 扫描范围: 0 到 99";
    } else {
        qDebug() << "[清理] 有效人脸ID范围: 0 到 " << maxValidId;
    }
    
    // 遍历可能的SeetaFace ID范围
    // 由于无法直接获取总数，我们遍历0到max(maxValidId+20, 100)
    int64_t scanRange = qMax(maxValidId + 20, (int64_t)100);
    
    qDebug() << "[清理] 将扫描人脸ID范围: 0 到 " << scanRange;
    
    int deletedCount = 0;
    int attemptCount = 0;
    
    for(int64_t faceId = 0; faceId <= scanRange; faceId++) {
        // 如果这个ID不在映射表中，说明是孤立数据
        if(!validFaceIds.contains(faceId)) {
            attemptCount++;
            
            // 尝试删除（如果ID不存在，Delete会返回false，这是正常的）
            bool deleted = faceObject->face_delete(faceId);
            if(deleted) {
                deletedCount++;
                qDebug() << "[清理] ✓ 已删除孤立人脸ID: " << faceId;
            }
        }
    }
    
    qDebug() << "[清理] 扫描范围: 0 到 " << scanRange;
    qDebug() << "[清理] 尝试删除: " << attemptCount << " 个ID";
    qDebug() << "[清理] 成功删除: " << deletedCount << " 个孤立人脸数据";
    
    if(deletedCount > 0) {
        qDebug() << "[清理] ✓ 清理完成，已删除 " << deletedCount << " 个孤立人脸数据";
    } else {
        qDebug() << "[清理] 未发现孤立人脸数据（或所有孤立数据已不存在）";
    }
    
    qDebug() << "[清理] ===== 清理完成 =====\n";
}


// 删除指定的SeetaFace人脸ID
bool MainWidget::deleteFaceById(qint64 seetaFaceId)
{
    if (!faceObject) {
        qDebug() << "[MainWidget] FaceObject未初始化，无法删除人脸";
        return false;
    }
    
    qDebug() << "[MainWidget] 尝试删除SeetaFace人脸ID:" << seetaFaceId;
    
    // 调用QFaceObject的删除方法
    bool success = faceObject->face_delete(seetaFaceId);
    
    if (success) {
        qDebug() << "[MainWidget] ✓ SeetaFace人脸ID" << seetaFaceId << "已成功删除";
    } else {
        qDebug() << "[MainWidget] ✗ SeetaFace人脸ID" << seetaFaceId << "删除失败或不存在";
    }
    
    return success;
}


// 处理获取有效指纹ID列表请求
void MainWidget::processGetValidFingerprintIds(ClientInfo* clientInfo)
{
    qDebug() << "\n[指纹同步] ===== 开始获取有效指纹ID列表 =====";
    qDebug() << "[指纹同步] 客户端: " << clientInfo->address << ":" << clientInfo->port;
    
    // 查询数据库中所有有效的指纹ID
    QSqlQuery query;
    query.exec("SELECT fingerprint_id FROM fingerprints ORDER BY fingerprint_id ASC");
    
    QJsonArray idArray;
    int count = 0;
    
    while (query.next()) {
        int fingerprintId = query.value(0).toInt();
        idArray.append(fingerprintId);
        count++;
    }
    
    qDebug() << "[指纹同步] 查询到" << count << "个有效指纹ID";
    
    // 构建JSON响应
    QJsonObject responseObj;
    responseObj["type"] = "valid_fingerprint_ids";
    responseObj["success"] = true;
    responseObj["ids"] = idArray;
    responseObj["count"] = count;
    
    QJsonDocument doc(responseObj);
    QByteArray responseData = doc.toJson(QJsonDocument::Compact);
    
    // 检查客户端连接状态
    if(!clientInfo->socket || clientInfo->socket->state() != QTcpSocket::ConnectedState) {
        qDebug() << "[警告] 客户端未连接，无法发送有效指纹ID列表";
        clientInfo->is_processing = false;
        return;
    }
    
    qDebug() << "[发送] 发送有效指纹ID列表给客户端: " << clientInfo->address << ":" << clientInfo->port;
    qDebug() << "[发送] 响应内容: " << responseData;
    
    qint64 written = clientInfo->socket->write(responseData);
    
    if(written == -1) {
        qDebug() << "[错误] 发送有效指纹ID列表失败: " << clientInfo->socket->errorString();
    } else {
        qDebug() << "[发送] ✓ 发送成功: " << written << "字节";
    }
    
    // 计算处理时间
    auto process_end = std::chrono::steady_clock::now();
    auto process_time = std::chrono::duration_cast<std::chrono::milliseconds>(
        process_end - clientInfo->process_start_time).count();
    
    qDebug() << "[指纹同步] 处理耗时: " << process_time << "ms";
    qDebug() << "[指纹同步] ===== 获取有效指纹ID列表结束 =====\n";
    
    clientInfo->is_processing = false;
}
