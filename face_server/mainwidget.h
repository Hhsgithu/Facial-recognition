#ifndef MAINWIDGET_H
#define MAINWIDGET_H

#include <QWidget>
#include <QTcpSocket>
#include <QTcpServer>
#include <QDebug>
#include <QSqlTableModel>
#include <QSqlRecord>
#include <QThreadPool>
#include <QRunnable>
#include <QMutex>
#include <QMap>
#include <chrono>
#include <QJsonDocument>
#include <QJsonObject>
#include "clientinfo.h"
#include "recognitiontask.h"
#include "qfaceobject.h"
#include "fingerprintmanager.h"
#include "registrationkeymanager.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWidget; }
QT_END_NAMESPACE

class MainWidget : public QWidget
{
    Q_OBJECT

public:
    MainWidget(QWidget *parent = nullptr);
    ~MainWidget();
    
    // 公共方法：删除指定的SeetaFace人脸ID
    bool deleteFaceById(qint64 seetaFaceId);

protected slots:
    void accept_client();
    void read_data();
    void client_disconnected();

    void onRecognitionCompleted(int64_t faceid, ClientInfo* clientInfo);
    void onCoordReady(QString coord, ClientInfo* clientInfo);
    
    // 注册密钥管理槽函数
    void on_generateKeyBtn_clicked();
    void on_refreshKeyBtn_clicked();
    void on_deactivateKeyBtn_clicked();
    void on_cleanupKeysBtn_clicked();

private:
    void processRecognitionResult(int64_t seetaFaceID, ClientInfo* clientInfo);
    void processPasswordVerify(const QString& password, ClientInfo* clientInfo);
    void processFingerprintVerify(int userId, ClientInfo* clientInfo);
    void processFingerprintRegister(const QString& employeeId, const QString& fingerprintData, ClientInfo* clientInfo);
    void processKeyVerify(const QString& keyValue, ClientInfo* clientInfo);
    void processUserRegister(const QJsonObject& registerData, ClientInfo* clientInfo);
    void processGetValidFingerprintIds(ClientInfo* clientInfo);
    void processFaceData(const QByteArray& data, ClientInfo* clientInfo);
    void sendPasswordResult(bool success, const QString& userId, const QString& userName, 
                           const QString& message, ClientInfo* clientInfo);
    void sendFingerprintResult(const FingerprintManager::VerificationResult& result, ClientInfo* clientInfo);
    void sendFingerprintRegisterResult(bool success, const QString& message, ClientInfo* clientInfo);
    void sendKeyVerifyResult(bool success, const QString& message, ClientInfo* clientInfo);
    void sendUserRegisterResult(bool success, int userId, const QString& message, ClientInfo* clientInfo, int fingerprintId = -1);
    bool isJsonData(const QByteArray& data);
    ClientInfo* findClientInfo(QTcpSocket* socket);
    void removeClient(ClientInfo* clientInfo);
    void refreshKeyTable();
    void initializeDatabaseSchema();  // 初始化数据库结构（创建映射表等）
    void cleanupOrphanedFaceData();  // 清理孤立的人脸数据（数据库中不存在的用户）

    Ui::MainWidget *ui;
    QTcpServer mserver;

    // 多客户端管理
    QMap<QTcpSocket*, ClientInfo*> clients;  // Socket到客户端信息的映射
    QMutex clientsMutex;  // 保护clients的互斥锁

    QSqlTableModel model;

    // 线程池
    QThreadPool* threadPool;
    
    // SeetaFace引擎（使用qfaceobject的方式初始化，避免ModelSetting依赖）
    QFaceObject* faceObject;
    
    // 指纹管理器
    FingerprintManager* fingerprintManager;
    
    // 注册密钥管理器
    RegistrationKeyManager* registrationKeyManager;
    
    // 注册密钥表格模型
    QSqlQueryModel* keyModel;
    
    // 识别任务列表（用于管理任务生命周期）
    QList<RecognitionTask*> tasks;
    QMutex tasksMutex;
};
#endif // MAINWIDGET_H
