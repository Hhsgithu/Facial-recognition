#ifndef CLIENTINFO_H
#define CLIENTINFO_H

#include <QTcpSocket>
#include <QString>
#include <chrono>
#include <opencv2/opencv.hpp>

// 客户端信息结构
struct ClientInfo {
    QTcpSocket* socket;           // 客户端Socket
    QString address;               // 客户端地址
    quint16 port;                  // 客户端端口
    quint64 bsize;                 // 当前接收的数据大小
    bool is_processing;            // 是否正在处理
    std::chrono::steady_clock::time_point process_start_time;  // 处理开始时间
    QByteArray jsonBuffer;         // JSON数据缓冲区（用于大数据包分包接收）
    
    ClientInfo() : socket(nullptr), port(0), bsize(0), is_processing(false) {}
    
    ClientInfo(QTcpSocket* s, const QString& addr, quint16 p) 
        : socket(s), address(addr), port(p), bsize(0), is_processing(false) {}
};

#endif // CLIENTINFO_H

