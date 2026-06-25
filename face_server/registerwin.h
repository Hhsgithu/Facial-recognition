#ifndef REGISTERWIN_H
#define REGISTERWIN_H

#include <QWidget>
#include <opencv.hpp>

class QLineEdit;

namespace Ui {
class RegisterWin;
}

class RegisterWin : public QWidget
{
    Q_OBJECT

public:
    explicit RegisterWin(QWidget *parent = nullptr);
    ~RegisterWin();
    //定时器事件
    void timerEvent(QTimerEvent *e);

private slots:
    void on_resetBt_clicked();

    void on_addpicBt_clicked();

    void on_registerBt_clicked();

    void on_videoswitchBt_clicked();

    void on_cameraBt_clicked();
    
    // 指纹录入按钮槽函数
    void on_fingerprintBt_clicked();

private:
    // 输入验证方法
    bool validateInput();
    void setFieldError(QLineEdit* field, const QString& errorMsg);
    void clearFieldError(QLineEdit* field);
    bool validatePhoneNumber(const QString& phone);
    
    // 指纹相关方法（硬件API待实现）
    // bool initFingerprintSensor();      // TODO: 初始化指纹传感器
    // bool captureFingerprintData();     // TODO: 采集指纹数据
    // int registerFingerprint();         // TODO: 注册指纹到数据库
    
    Ui::RegisterWin *ui;
    int timerid;
    cv::VideoCapture cap;
    cv::Mat image;
    
    // 指纹相关成员
    int m_fingerprintID;                   // 当前录入的指纹ID（-1表示未录入）
    QByteArray m_fingerprintData;          // 指纹模板数据（待硬件API实现）
};

#endif // REGISTERWIN_H
