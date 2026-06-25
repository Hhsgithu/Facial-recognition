#include "registerwin.h"
#include "ui_registerwin.h"
#include <QFileDialog>
#include "qfaceobject.h"
#include "recognitiontask.h"
#include <QSqlTableModel>
#include <QSqlRecord>
#include <QMessageBox>
#include <QDebug>
#include <QDir>
#include <QRegExp>
#include <QTimer>
#include <QStyle>

RegisterWin::RegisterWin(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::RegisterWin),
    m_fingerprintID(-1)
{
    ui->setupUi(this);
    
    // 设置注册页面样式 - 与客户端统一的浅色风格
    this->setStyleSheet(
        // 主窗口背景 - 与客户端一致
        "RegisterWin {"
        "    background-color: rgb(245, 247, 250);"
        "}"
        
        // 标签样式 - 简洁浅色
        "QLabel {"
        "    background-color: transparent;"
        "    color: rgb(120, 120, 120);"
        "    padding: 8px 0px;"
        "    font-size: 14px;"
        "    min-width: 60px;"
        "}"
        
        // 输入框样式 - 与客户端密码输入框一致
        "QLineEdit {"
        "    background-color: rgb(250, 250, 250);"
        "    border: none;"
        "    border-radius: 12px;"
        "    padding: 12px 15px;"
        "    font-size: 14px;"
        "    color: rgb(50, 50, 50);"
        "    selection-background-color: rgb(82, 196, 26);"
        "    selection-color: white;"
        "}"
        "QLineEdit:focus {"
        "    background-color: rgb(255, 255, 255);"
        "    border: 2px solid rgb(82, 196, 26);"
        "}"
        "QLineEdit:hover {"
        "    background-color: rgb(245, 245, 245);"
        "}"
        // 错误状态样式
        "QLineEdit.error {"
        "    border: 2px solid rgb(245, 34, 45);"
        "    background-color: rgb(255, 240, 240);"
        "}"
        
        // QDateEdit样式 - 与输入框一致
        "QDateEdit {"
        "    background-color: rgb(250, 250, 250);"
        "    border: none;"
        "    border-radius: 12px;"
        "    padding: 12px 15px;"
        "    font-size: 14px;"
        "    color: rgb(50, 50, 50);"
        "}"
        "QDateEdit:focus {"
        "    background-color: rgb(255, 255, 255);"
        "    border: 2px solid rgb(82, 196, 26);"
        "}"
        "QDateEdit::drop-down {"
        "    subcontrol-origin: padding;"
        "    subcontrol-position: top right;"
        "    width: 30px;"
        "    border: none;"
        "    border-top-right-radius: 12px;"
        "    border-bottom-right-radius: 12px;"
        "    background-color: rgb(240, 240, 240);"
        "}"
        "QDateEdit::drop-down:hover {"
        "    background-color: rgb(230, 230, 230);"
        "}"
        "QDateEdit::up-arrow {"
        "    image: none;"
        "    border-left: 5px solid transparent;"
        "    border-right: 5px solid transparent;"
        "    border-bottom: 6px solid rgb(82, 196, 26);"
        "    width: 0px;"
        "    height: 0px;"
        "}"
        "QDateEdit::down-arrow {"
        "    image: none;"
        "    border-left: 5px solid transparent;"
        "    border-right: 5px solid transparent;"
        "    border-top: 6px solid rgb(82, 196, 26);"
        "    width: 0px;"
        "    height: 0px;"
        "}"
        
        // 单选按钮样式 - 简洁
        "QRadioButton {"
        "    font-size: 14px;"
        "    color: rgb(50, 50, 50);"
        "    spacing: 8px;"
        "}"
        "QRadioButton::indicator {"
        "    width: 18px;"
        "    height: 18px;"
        "    border-radius: 9px;"
        "    border: 2px solid rgb(200, 200, 200);"
        "    background-color: white;"
        "}"
        "QRadioButton::indicator:checked {"
        "    background-color: rgb(82, 196, 26);"
        "    border: 2px solid rgb(82, 196, 26);"
        "}"
        
        // 按钮样式 - 与客户端一致
        "QPushButton {"
        "    background-color: rgb(250, 250, 250);"
        "    color: rgb(50, 50, 50);"
        "    border: none;"
        "    border-radius: 12px;"
        "    padding: 12px 25px;"
        "    font-size: 14px;"
        "    font-weight: bold;"
        "    min-height: 20px;"
        "}"
        "QPushButton:hover {"
        "    background-color: rgb(240, 240, 240);"
        "}"
        "QPushButton:pressed {"
        "    background-color: rgb(230, 230, 230);"
        "}"
        
        // 注册按钮特殊样式 - 绿色主题
        "QPushButton#registerBt {"
        "    background-color: rgb(82, 196, 26);"
        "    color: white;"
        "}"
        "QPushButton#registerBt:hover {"
        "    background-color: rgb(60, 170, 10);"
        "}"
        "QPushButton#registerBt:pressed {"
        "    background-color: rgb(50, 150, 5);"
        "}"
        
        // 头像标签样式 - 与客户端视频区域一致
        "QLabel#headpiclb {"
        "    background-color: rgb(30, 30, 30);"
        "    border: none;"
        "    border-radius: 12px;"
        "    min-height: 200px;"
        "}"
    );
    
    // 初始化：创建data目录（避免保存图片时路径不存在）
    QDir dir;
    if(!dir.exists("./data")) {
        dir.mkdir("./data");
    }
    // 初始化摄像头设备号（根据实际环境修改，虚拟机注释，开发板改2）
    cap.open("/dev/video0"); // 开发板用/dev/video2，虚拟机注释这行
    
    // 设置输入框的placeholder提示
    ui->nameEdit->setPlaceholderText("请输入姓名（必填）");
    ui->addressEdit->setPlaceholderText("请输入地址（必填）");
    ui->phoneEdit->setPlaceholderText("请输入电话号码（必填，11位数字）");
    ui->picFileEdit->setPlaceholderText("请选择或拍摄人脸图片（必填）");
    ui->passwordEdit->setPlaceholderText("请输入密码（选填，用于密码解锁）");
    ui->fingerprintEdit->setPlaceholderText("点击录入指纹按钮（选填）");
    
    // 连接输入框的textChanged信号，清除错误状态
    connect(ui->nameEdit, &QLineEdit::textChanged, this, [this]() {
        clearFieldError(ui->nameEdit);
    });
    connect(ui->addressEdit, &QLineEdit::textChanged, this, [this]() {
        clearFieldError(ui->addressEdit);
    });
    connect(ui->phoneEdit, &QLineEdit::textChanged, this, [this]() {
        clearFieldError(ui->phoneEdit);
    });
    connect(ui->picFileEdit, &QLineEdit::textChanged, this, [this]() {
        clearFieldError(ui->picFileEdit);
    });
}

RegisterWin::~RegisterWin()
{
    delete ui;
}

void RegisterWin::timerEvent(QTimerEvent *e)
{
    // 获取摄像头数据显示
    if(cap.isOpened())
    {
         cap>>image;
         if(image.data == nullptr) return;
    }
    cv::Mat rgbImage;
    cv::cvtColor(image, rgbImage, cv::COLOR_BGR2RGB);
    QImage qImg(rgbImage.data, rgbImage.cols, rgbImage.rows, rgbImage.step1(),QImage::Format_RGB888);

    // 再qt界面上显示
    QPixmap mmp = QPixmap::fromImage(qImg);
    mmp = mmp.scaledToWidth(ui->headpiclb->width());
    ui->headpiclb->setPixmap(mmp);
}

void RegisterWin::on_resetBt_clicked()
{
    // 清空数据
    ui->nameEdit->clear();
    ui->addressEdit->clear();
    ui->phoneEdit->clear();
    ui->picFileEdit->clear();
    ui->passwordEdit->clear();
    ui->fingerprintEdit->clear();
    
    // 重置指纹状态
    m_fingerprintID = -1;
    m_fingerprintData.clear();
}

void RegisterWin::on_addpicBt_clicked()
{
    // 通过文件对话框 选中图片路径
    QString filepath = QFileDialog::getOpenFileName(this, "选择人脸图片", "./", "图片文件 (*.jpg *.png *.bmp)");
    if(filepath.isEmpty()) { // 新增：取消选择时返回
        return;
    }
    ui->picFileEdit->setText(filepath);

    // 显示图片
    QPixmap mmp(filepath);
    if(mmp.isNull()) { // 新增：检查图片是否加载成功
        QMessageBox::warning(this, "错误", "选择的图片文件损坏或格式不支持！");
        ui->picFileEdit->clear();
        return;
    }
    mmp = mmp.scaledToWidth(ui->headpiclb->width());
    ui->headpiclb->setPixmap(mmp);
}

bool RegisterWin::validatePhoneNumber(const QString& phone)
{
    // 检查是否为空
    if(phone.trimmed().isEmpty()) {
        return false;
    }
    
    // 检查是否全是数字
    QRegExp digitRegExp("^\\d+$");
    if(!digitRegExp.exactMatch(phone.trimmed())) {
        return false;
    }
    
    // 检查长度是否不超过11位
    if(phone.trimmed().length() > 11) {
        return false;
    }
    
    return true;
}

void RegisterWin::setFieldError(QLineEdit* field, const QString& errorMsg)
{
    if(!field) return;
    
    // 保存原始样式
    if(!field->property("originalStyle").isValid()) {
        field->setProperty("originalStyle", field->styleSheet());
    }
    
    // 设置错误样式（红色边框）- 与客户端风格一致
    field->setStyleSheet(
        "QLineEdit {"
        "    background-color: rgb(255, 240, 240);"
        "    border: 2px solid rgb(245, 34, 45);"
        "    border-radius: 12px;"
        "    padding: 12px 15px;"
        "    font-size: 14px;"
        "    color: rgb(50, 50, 50);"
        "}"
        "QLineEdit:focus {"
        "    border: 2px solid rgb(245, 34, 45);"
        "    background-color: rgb(255, 230, 230);"
        "}"
    );
    
    // 设置tooltip显示错误信息
    field->setToolTip(errorMsg);
    
    // 设置placeholder显示错误（如果当前为空）
    if(field->text().isEmpty()) {
        QString originalPlaceholder = field->placeholderText();
        field->setPlaceholderText(errorMsg);
        // 保存原始placeholder，在清除错误时恢复
        field->setProperty("originalPlaceholder", originalPlaceholder);
    }
}

void RegisterWin::clearFieldError(QLineEdit* field)
{
    if(!field) return;
    
    // 恢复原始样式
    QString originalStyle = field->property("originalStyle").toString();
    if(!originalStyle.isEmpty()) {
        field->setStyleSheet(originalStyle);
    } else {
        // 如果没有保存的样式，使用默认样式 - 与客户端风格一致
        field->setStyleSheet(
            "QLineEdit {"
            "    background-color: rgb(250, 250, 250);"
            "    border: none;"
            "    border-radius: 12px;"
            "    padding: 12px 15px;"
            "    font-size: 14px;"
            "    color: rgb(50, 50, 50);"
            "    selection-background-color: rgb(82, 196, 26);"
            "    selection-color: white;"
            "}"
            "QLineEdit:focus {"
            "    background-color: rgb(255, 255, 255);"
            "    border: 2px solid rgb(82, 196, 26);"
            "}"
            "QLineEdit:hover {"
            "    background-color: rgb(245, 245, 245);"
            "}"
        );
    }
    
    // 清除tooltip
    field->setToolTip("");
    
    // 恢复原始placeholder
    QString originalPlaceholder = field->property("originalPlaceholder").toString();
    if(!originalPlaceholder.isEmpty()) {
        field->setPlaceholderText(originalPlaceholder);
    }
}

bool RegisterWin::validateInput()
{
    bool isValid = true;
    
    // 验证姓名
    if(ui->nameEdit->text().trimmed().isEmpty()) {
        setFieldError(ui->nameEdit, "姓名不能为空");
        isValid = false;
    } else {
        clearFieldError(ui->nameEdit);
    }
    
    // 验证地址
    if(ui->addressEdit->text().trimmed().isEmpty()) {
        setFieldError(ui->addressEdit, "地址不能为空");
        isValid = false;
    } else {
        clearFieldError(ui->addressEdit);
    }
    
    // 验证电话号码
    QString phone = ui->phoneEdit->text().trimmed();
    if(phone.isEmpty()) {
        setFieldError(ui->phoneEdit, "电话号码不能为空");
        isValid = false;
    } else if(!validatePhoneNumber(phone)) {
        // 检查是否是纯数字
        QRegExp digitRegExp("^\\d+$");
        if(!digitRegExp.exactMatch(phone)) {
            setFieldError(ui->phoneEdit, "电话号码必须为纯数字");
        } else if(phone.length() > 11) {
            setFieldError(ui->phoneEdit, "电话号码不能超过11位");
        }
        isValid = false;
    } else {
        clearFieldError(ui->phoneEdit);
    }
    
    // 验证图片路径
    if(ui->picFileEdit->text().trimmed().isEmpty()) {
        setFieldError(ui->picFileEdit, "请选择或拍摄人脸图片");
        isValid = false;
    } else {
        clearFieldError(ui->picFileEdit);
    }
    
    return isValid;
}

void RegisterWin::on_registerBt_clicked()
{
    // 1. 验证所有输入字段
    if(!validateInput()) {
        return;
    }
    
    QString picPath = ui->picFileEdit->text().trimmed();

    // 2. 加载图片并检查是否成功
    cv::Mat image = cv::imread(picPath.toUtf8().data());
    if(image.empty()) {
        setFieldError(ui->picFileEdit, "图片加载失败，请检查路径或文件格式");
        return;
    }

    // 3. 与认证流程保持一致的预处理（格式统一 + resize 256x256 + 温和CLAHE）
    //    注册和认证使用相同的预处理，确保特征空间一致，提高识别准确率
    cv::Mat processedImage = RecognitionTask::preprocessForSeetaFace(image);
    if(processedImage.empty()) {
        setFieldError(ui->picFileEdit, "图片预处理失败");
        return;
    }

    // 4. 人脸注册并检查结果
    QFaceObject faceobj;
    int faceID = faceobj.face_register(processedImage);
    qDebug() << faceID;
    if(faceID < 0) {
        setFieldError(ui->picFileEdit, "人脸注册失败，请确保图片中有人脸");
        return;
    }

    // 5. 保存头像（保存预处理后的图片，与注册特征一致）
    QString headfile = QString("./data/%1.jpg").arg(QString(ui->nameEdit->text().toUtf8().toBase64()));
    bool saveOk = cv::imwrite(headfile.toUtf8().data(), processedImage);
    if(!saveOk) {
        setFieldError(ui->picFileEdit, "头像保存失败，请检查权限");
        return;
    }

    // 6. 把个人信息存储到数据库
    QSqlTableModel model;
    model.setTable("employee");
    QSqlRecord record = model.record();
    // 设置数据
    record.setValue("name", ui->nameEdit->text());
    record.setValue("sex", ui->mrb->isChecked() ? "男" : "女");
    record.setValue("address", ui->addressEdit->text());
    record.setValue("phone", ui->phoneEdit->text());
    record.setValue("faceID", faceID);
    // 头像路径
    record.setValue("headfile", headfile);
    // 密码（如果输入了）
    QString password = ui->passwordEdit->text().trimmed();
    if(!password.isEmpty()) {
        record.setValue("password", password);
    }
    // 指纹ID（如果录入了）
    if(m_fingerprintID >= 0) {
        record.setValue("fingerprintID", m_fingerprintID);
    }
    // 把记录插入到数据库表格
    bool ret = model.insertRecord(0, record);

    // 7. 提示注册成功
    if(ret)
    {
        // 提交
        model.submitAll();
        // 注册成功，清空所有字段
        on_resetBt_clicked();
        // 在注册按钮上显示成功提示（临时）
        QString originalText = ui->registerBt->text();
        ui->registerBt->setText("注册成功！");
        ui->registerBt->setStyleSheet(
            "QPushButton#registerBt {"
            "    background-color: rgb(82, 196, 26);"
            "    color: white;"
            "    border: none;"
            "    border-radius: 12px;"
            "}"
        );
        // 2秒后恢复
        QTimer::singleShot(2000, this, [this, originalText]() {
            ui->registerBt->setText(originalText);
            ui->registerBt->setStyleSheet(
                "QPushButton#registerBt {"
                "    background-color: rgb(82, 196, 26);"
                "    color: white;"
                "    border: none;"
                "    border-radius: 12px;"
                "}"
                "QPushButton#registerBt:hover {"
                "    background-color: rgb(60, 170, 10);"
                "}"
                "QPushButton#registerBt:pressed {"
                "    background-color: rgb(50, 150, 5);"
                "}"
            );
        });
    }else{
        setFieldError(ui->nameEdit, "数据库插入失败，请重试");
    }
}

void RegisterWin::on_videoswitchBt_clicked()
{
    if(ui->videoswitchBt->text() == "打开摄像头")
    {
        // 修复：根据环境修改摄像头设备号（开发板用2，虚拟机注释）
        // 虚拟机注意：需要先挂载物理摄像头到虚拟机
        if(cap.open(0)) { // 开发板/dev/video2，虚拟机改0（若有摄像头）
             ui->videoswitchBt->setText("关闭摄像头");
             timerid = startTimer(100);
        } else {
            QMessageBox::critical(this, "错误", "摄像头打开失败！请检查设备号或权限。");
        }
    }
    else
    {
        killTimer(timerid); // 关闭定时器
        ui->videoswitchBt->setText("打开摄像头");
        // 关闭摄像头
        cap.release();
    }
}

void RegisterWin::on_cameraBt_clicked()
{
    // 核心修复1：检查姓名是否输入
    if(ui->nameEdit->text().trimmed().isEmpty()) {
        QMessageBox::warning(this, "错误", "请先输入姓名！");
        return;
    }
    // 核心修复2：检查摄像头是否打开、图像是否为空
    if(!cap.isOpened() || image.empty()) {
        QMessageBox::critical(this, "错误", "摄像头未打开或无有效图像！");
        return;
    }

    // 保存数据
    QString headfile = QString("./data/%1.jpg").arg(QString(ui->nameEdit->text().toUtf8().toBase64()));
    ui->picFileEdit->setText(headfile);

    // 预处理后再保存，与注册特征一致
    cv::Mat processedImage = RecognitionTask::preprocessForSeetaFace(image);
    bool saveOk = cv::imwrite(headfile.toUtf8().data(), processedImage.empty() ? image : processedImage);
    if(!saveOk) {
        QMessageBox::warning(this, "错误", "拍照保存失败！请检查权限或路径。");
        return;
    }

    killTimer(timerid);
    ui->videoswitchBt->setText("打开摄像头");
    cap.release();
    QMessageBox::information(this, "提示", "拍照成功！");
}


void RegisterWin::on_fingerprintBt_clicked()
{
    // TODO: 指纹硬件API待实现
    // 以下是指纹录入的框架代码，硬件API接入后取消注释
    
    /*
    // 1. 初始化指纹传感器
    if(!initFingerprintSensor()) {
        QMessageBox::critical(this, "错误", "指纹传感器初始化失败！");
        return;
    }
    
    // 2. 提示用户放置手指
    QMessageBox::information(this, "提示", "请将手指放在指纹传感器上...");
    
    // 3. 采集指纹数据
    if(!captureFingerprintData()) {
        QMessageBox::warning(this, "错误", "指纹采集失败，请重试！");
        return;
    }
    
    // 4. 注册指纹到数据库
    m_fingerprintID = registerFingerprint();
    if(m_fingerprintID < 0) {
        QMessageBox::warning(this, "错误", "指纹注册失败！");
        return;
    }
    
    // 5. 更新UI显示
    ui->fingerprintEdit->setText(QString("已录入 (ID: %1)").arg(m_fingerprintID));
    QMessageBox::information(this, "成功", "指纹录入成功！");
    */
    
    // 临时提示：硬件API未实现
    QMessageBox::information(this, "提示", 
        "指纹录入功能暂未实现\n\n"
        "需要接入指纹传感器硬件API后才能使用此功能。\n"
        "当前仅创建了数据库表结构和UI界面。");
}
