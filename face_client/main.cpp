#include "maincontroller.h"

#include <QApplication>
#include <QDebug>
#include <signal.h>
#include <unistd.h>

// 全局指针，用于信号处理
static QApplication* g_app = nullptr;

// 信号处理函数
void signalHandler(int sig)
{
    qDebug() << "\n[系统] 收到信号:" << sig << "，正在安全退出...";
    
    if(g_app) {
        // 使用 Qt 的事件循环安全退出
        QMetaObject::invokeMethod(g_app, "quit", Qt::QueuedConnection);
    }
}

// 清理函数，程序退出时调用
void cleanup()
{
    qDebug() << "[系统] 程序退出，清理资源...";
    
    // 同步文件系统，确保数据写入
    sync();
    
    qDebug() << "[系统] 清理完成";
}

int main(int argc, char *argv[])
{
    // 初始化 QtVirtualKeyboard - 必须在创建 QApplication 之前设置
    qputenv("QT_IM_MODULE", QByteArray("qtvirtualkeyboard"));

    // 设置虚拟键盘默认语言为简体中文
    qputenv("QT_VIRTUALKEYBOARD_LOCALE", QByteArray("zh_CN"));

    // 启用手写识别（如不需要可关闭）
    qputenv("QT_VIRTUALKEYBOARD_HANDWRITING_ENABLED", "1");
    
    QApplication a(argc, argv);
    g_app = &a;

    // 全局基础样式（尽量不覆盖各页面已写死的 styleSheet，只做兜底统一）
    a.setStyleSheet(
        "QWidget {"
        "  font-family: \"Microsoft YaHei\", \"Noto Sans CJK SC\", \"SimSun\", sans-serif;"
        "  color: rgb(50, 50, 50);"
        "}"
        "QLineEdit {"
        "  border: 2px solid rgb(230, 230, 230);"
        "  border-radius: 8px;"
        "  padding: 8px 12px;"
        "  font-size: 16px;"
        "  background-color: rgb(250, 250, 250);"
        "}"
        "QLineEdit:focus {"
        "  border-color: rgb(24, 144, 255);"
        "  background-color: white;"
        "}"
        "QPushButton {"
        "  border: none;"
        "  border-radius: 8px;"
        "  font-size: 16px;"
        "}"
        "QPushButton:disabled {"
        "  background-color: rgb(200, 200, 200);"
        "  color: rgb(150, 150, 150);"
        "}"
    );
    
    // 注册退出时的清理函数
    atexit(cleanup);
    
    // 注册信号处理，确保程序能正常退出
    signal(SIGINT, signalHandler);   // Ctrl+C
    signal(SIGTERM, signalHandler);  // kill 命令
    signal(SIGHUP, signalHandler);   // 终端断开
    
    qDebug() << "[系统] 客户端启动...";
    qDebug() << "[系统] 按 Ctrl+C 可安全退出程序";
    
    MainController w;
    w.show();
    
    int ret = a.exec();
    
    qDebug() << "[系统] 程序正常退出，返回码:" << ret;
    
    return ret;
}
