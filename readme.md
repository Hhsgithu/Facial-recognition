# 简介

在使用本项目时请先认真查看readme

本项目是基于正点原子i.mx6ull阿尔法开发板，进行的开发，人脸识别功能基于OpenCV3.4.11 + Seetaface2（再进行优化后在室内下的光照环境下相识度 > 0.9 符合正常人脸识别门禁系统的精度），指纹识别基于sfm-v1.7（基于这个模块编写的驱动+sg90舵机驱动），QT5.12.9实现的gui设计，虚拟键盘采用QT自带的虚拟键盘，目前运行项目时，屏幕显示的画面帧数低，对显示有要求的可以自行裁剪内核 + 根文件系统，优化性能，项目本身数据库表并不完善对此有要求的可自行优化，对于在编译时遇到的问题请自行将编译报错复制到ai软件，让ai解决编译时遇到的问题

fr_2_10.da文件大小超50M无法上传，请自行下载到/face_server/目录下
通过网盘分享的文件：fr_2_10.dat
链接: https://pan.baidu.com/s/1qw6RgZLEiQSI1frtGkFwrw 提取码: qupp

# 环境准备

## 软件

- OpenCV3.4.11

- Seetaface2

- QT5.12.9

- 正点原子i.mx6ull提供的虚拟机或sdk源码（编译内核源码时需要自行解决编译内核时的问题，没办法解决的话就直接使用正点原子提供的虚拟机）[正点原子i.MX6ULL Linux阿尔法开发板 — 正点原子资料下载中心 1.0.0 文档](http://www.openedv.com/docs/boards/arm-linux/zdyz-i.mx6ull.html)

- 数据库sqlite3

## 硬件

- 开发板：i.mx6ull阿尔法开发板 + 屏幕

- 指纹模块：sfm-v1.7（本项目使用的是sfm-v1.7模块，但是没有这个模块的驱动，所以我自己写了一个，功能可能不太完善，淘宝直接搜索即可购买，有点小贵）

- 舵机模块：sg90舵机（模拟门禁开关，需要的是带限位器的180°舵机，360°舵机无法完成本需求）

- 开发板需要使能gpio：
  
   开发板需要使能pwm
  
  设备树：
  
  
  
   指纹模块使用uart

- 采集画面摄像头：采用usb摄像头（lubancat的ov9726 100万像素 如果内核中没有使能UVC则需要先使能否则无法识别摄像头，项目本身选用的开发板性能不行，有条件的可自行更换开发板，在更换开发板之后可能会造成驱动无法正常加载，需要自行修改舵机、指纹驱动让驱动能在你所更换的开发板中正常运行，由于需要显示画面所以摄像头选用分辨率并不高仅为540p左右），本项目对于usb摄像头没有要求，分辨率在项目中不要开的太高，太高会导致画面卡顿
  
  make menuconfig 使能USB摄像头
  
  ```
  Device Drivers->  
       <*>Multimedia support -->  
        [*] Media USB Adapters -->  
          <*> USB Video Class (UVC)
  <*> platform camera support
  ```

- 网络通信：需要网线连接电脑与开发板，电脑虚拟机做服务器端处理客户端传输过来的数据，服务器端进行数据比对，采用 QTCP/IP 进行通信



在项目初期设计的时候我在没有使用正点原子的电源适配器而是直接使用串口的usb进行供电，导致触摸屏无法触摸和开发板屏幕端显示摄像头画面卡顿，故需要配套的电源适配器



### 连线



开发板本身外设就很丰富，故我使用的是空闲的引脚，在引脚复用时需要检查使用的引脚是否被占用

![](C:\Users\Huafeng\AppData\Roaming\marktext\images\2026-06-24-10-29-36-f03b6c1f1e65c2a6874dc47a68707d2f.jpg)



舵机模块：
GND（橘色）------>gpio4
VCC（红色）------->5v
信号端（棕色）--->gnd



指纹模块：

V_TOUCH（白色）------->3.3v（需要一直供电）
GND（红色）-------------->gnd
VCC（绿色）--------------->3.3v
TX（黄色）----------------->u2-rx
RX（黑色）----------------->u2-tx

TOUCH_OUT（蓝色）--------此引脚我在驱动中没有配置（唤醒引脚）



![](C:\Users\Huafeng\AppData\Roaming\marktext\images\2026-06-24-15-24-01-image.png)



# 编译环境

opencv-3.4.11[下载网站](https://gitee.com/link?target=https%3A%2F%2Fopencv.org%2F)/opencv_contrib-3.4[下载网站](https://gitee.com/link?target=https%3A%2F%2Fgithub.com%2Fopencv%2Fopencv_contrib%2Ftree%2F3.4)/Seetaface2[下载网站](https://gitee.com/link?target=https%3A%2F%2Fgithub.com%2Fseetafaceengine%2FSeetaFace2)）  
Ubuntu中安装gcc,g++,cmake-gui



## 编译OpenCV3.4.11 + Seetaface2到Ubuntu虚拟机中

在开发时可以直接在Ubuntu端运行qt程序查看效果，当到项目开发的后期时可以将交叉编译好的qt程序复制到开发板上运行进行后期调试



在opencv3.4.11源码中建立bulid文件夹，进入文件夹，使用 cmake-gui .. 进入cmake配置界面

1. 默认use default native comiles
2. 勾选BUILD_opencv_word
3. 在编译OpenCV时请将识别人物年龄性别的配置关闭，本项目不需要且这个配置对性能消耗很大（关于这个配置我懒得再编译一遍，在编译时请自行找到并取消勾选）
4. OPENCV_EXTRA_MODULES_PATH 指定到opencv_contrib-3.4/modules/face/
5. CMAKE_INSTALL_PREFIX 指定/opt/opencv3-pc-youhua

Configure/Generate-->make -j8  


编译seetaface2  
在seetaface2源码中建立bulid文件夹，进入文件夹，使用cmake-gui .. 进入cmake配置界面 CMAKE_INSTALL_PREFIX 指定/opt/opencv3-pc-youhua  
Configure/Generate-->make -j8 


## 编译OpenCV3.4.11 + Seetaface2到开发板中

移植opencv到开发板  
下载opencv-3.4.11源码，在opencv3.4.11源码中建立bulid、install文件夹，进入文件夹，使用cmake-gui .. 进入cmake配置界面

1. 选择Specify options for cross-compiling 下一步。
2. Operating System :Linux（有坑：L必须大写，否则后面无法使用摄像头）。
3. 交叉编译器的gcc和g++在sdk中buildroot的目录下的output/host/bin中,下面的Target Root要选择output/host/arm-buildroot-linux-uclibcgnueabihf/sysroot/usr/bin,这里有很多buildroot的库环境,可以大大减少编译问题。
4. CMAKE_EXE_LINKER_FLAGS = -pthread -lrt -ldl
5. CMAKE_INSTALL_PRTEFIX = /opencv/opencv-3.4.11/install（之前建立的install文件夹）
6. 勾选WITH_V4L(否则无法使用摄像头)
7. Configure/Generate-->make -j8 编译可能会出错，请自行将编译日志复制给ai

编译完成后，将lib文件夹下的文件拷贝到开发板根文件系统文件下的opt/opencv3-arm/lib/目录下



## 关于qt虚拟键盘

在正点原子提供的sdk中是默认将qt虚拟键盘编译的，在使用qt虚拟键盘时只需要在客户端的.pro文件写上相关配置即可，但是实际上正点原子提供的是编译了qt官方支持的全部语言的键盘，有需要请自行编译，将语言数量减少

```
qtHaveModule(virtualkeyboard):qtHaveModule(quickwidgets) {
    QT += virtualkeyboard quickwidgets qml quick
    DEFINES += HAVE_VIRTUALKEYBOARD
    message("QtVirtualKeyboard 模块已启用")
} else {
    warning("QtVirtualKeyboard 模块不可用，将使用系统默认输入法")
}
```

# 运行程序

在运行客户端项目时可能会报缺失其他依赖库的报错，需要自行将需要的库下载下来再编译生成依赖库，然后再相关的.pro文件中添加库路径

```
ZLIB_LIB_PATH = /usr/local/arm/gcc-linaro-4.9.4-2017.01-x86_64_arm-linux-gnueabihf/arm-linux-gnueabihf/sysroot/usr/lib
LIBS += -L$$ZLIB_LIB_PATH \
        -lz \  # 显式链接zlib库
        -Wl,-rpath=$$ZLIB_LIB_PATH \  # 配置运行时zlib库路径
        -Wl,-rpath=/opt/opencv3-arm/lib  # 配置运行时OpenCV库路径
```

此时需要将编译出来的zlib依赖复制到开发板中/opt/opencv3-arm/lib



在客户端中的.pro文件中定义了如下平台，当运行在Ubuntu虚拟机时则将imx6ull中的配置注释，反之则注释ubuntu中的配置

```
#ubuntu
#INCLUDEPATH += /opt/opencv3-pc-youhua/include
#INCLUDEPATH +=/opt/opencv3-pc-youhua/include/opencv2
#INCLUDEPATH +=/opt/opencv3-pc-youhua/include/opencv
#INCLUDEPATH +=/opt/opencv3-pc-youhua/include/seeta
#LIBS += -L/usr/lib/x86_64-linux-gun -L/opt/opencv3-pc-youhua/lib -lopencv_world -lSeetaFaceDetector -lSeetaFaceLandmarker -lSeetaFaceRecognizer -lSeetaFaceTracker -lSeetaNet


##imx6ull
INCLUDEPATH += /opt/opencv3-arm/include
LIBS += -L/opt/opencv3-arm/lib \
        -lopencv_core \
        -lopencv_highgui \
        -lopencv_imgproc \
        -lopencv_videoio \
        -lopencv_imgcodecs \
        -lopencv_objdetect \
        -lopencv_calib3d


```



## 交叉编译客户端

客户端是运行在开发板上的所以需要交叉编译才能运行

**注意：需要在客户端的.pro文件中取消注释imx6ull的配置**

方法一：

参考正点原子的文档的第二章交叉编译环境构建

【金山文档 | WPS云文档】 05【正点原子】I.MX6U 出厂系统Qt交叉编译环境搭建V1.7
https://www.kdocs.cn/l/cbd3ostr6jkS



方法二：如果在buildroot目录中有编译了qt的话在这里会有qmake，查看qmake所在的目录，在驱动源码目录下执行以下命令

/home/buildroot-xxx/output/host/usr/bin/qmake  
make -j4 将编译完成后的可执行程序xxx拷贝到开发板，./xxx执行



**注意：在正点原子提供的Ubuntu虚拟机中有专门为arm架构编译过的qt5.12.9**



## 开发板上运行

**注意：需要将编译出来的OpenCV依赖库复制到开发板中的指定位置否则无法运行**



**在此时需要将haarcascade_frontalface_default.xml 或 haarcascade_frontalface_alt2.xml**（具体需要看代码中用了那个级联分类器）**复制到可执行程序的目录下才能运行程序**，在程序运行时需要插入usb摄像头否则程序也不会成功运行，可能会卡在某处亦或者能运行但是没有采集到画面，同时舵机和指纹模块都需要插上否则可能会出现程序卡死或者具体功能没响应（目前我已经很久没运行这个项目了，正常的话在缺失相关硬件的话可能程序会卡死在检测硬件的环节，或者也不会出现这些问题）



在加载两个驱动程序后

./xxx

直接运行程序，此时同目录下需要有级联分类器任意一个

```
    if(!cascade.load("./haarcascade_frontalface_alt2.xml") && 
       !cascade.load("./haarcascade_frontalface_default.xml")) {
        ui->faceStatusLabel->setText("分类器加载失败");
        return;
    }
```



客户端中设置了注册用户密钥，也可自行修改，有效注册密钥好像是六位数，可自行测试

```
   // 生成测试注册密钥（仅用于调试）
    #ifdef QT_DEBUG
    qDebug() << "[调试] 生成测试注册密钥...";
    RegistrationKeyManager testKeyManager;
    testKeyManager.generateCustomRegistrationKey("123456", 24 * 365, 100);  // 有效期1年，可使用100次
    testKeyManager.generateCustomRegistrationKey("12345678", 24 * 365, 100);  // 有效期1年，可使用100次
    #endif
    
```

## 

在客户端注册用户人脸信息以及指纹数据、认证密码（此时我并没有做唯一标识密码，这个可以是改进的地方，这里的唯一标识密码指的是，当两个用户设置密码相同时，输入密码后认证成功，在插入数据库表后并不唯指定是哪个用户通过了密码认证，）注册人脸时会检测人脸是否完整，图片是否清晰，当不清晰时需要重新录入，指纹注册时手指一直按住模块即可，注册成功会在屏幕上提示



此时串口终端会有日志生成，可查看运行日志发现程序在运行时的问题





## 服务器端运行

**注意：服务器端的源码下需要有预训练模型fd_2_00.dat，没有的话也是会运行失败，当运行失败时看qt端的运行日志**

**服务器端是运行在Ubuntu虚拟机上的，此时要保证开发板能ping通虚拟机**否则会提示通信失败，客户端为离线状态

基于开发板已经能和电脑通信，此时在开发板上查看ip地址，将客户端代码中的ip地址设置为自己开发板的ip地址

```
NetworkManager::NetworkManager(QObject *parent)
    : QObject(parent)
    , m_socket(nullptr)
    , m_reconnectTimer(nullptr)
    , m_serverAddress("192.168.138.188")
    , m_serverPort(8888)
    , m_isConnected(false)
    , m_reconnectAttempts(0)
{
    setupConnection();
    qDebug() << "[网络管理器] NetworkManager初始化完成";
}
```

目前我在运行程序时的ip地址为192.168.138.188，可直接修改这个ip，这个ip地址在代码中多次定义m_socket->connectToHost()，请都修改了



在Ubuntu虚拟机中打开qt creater，点击构建即可运行程序，服务器端是有设置登录密码的，且设置了注册密钥

loginwindow.h

```
private:
    void initDatabase();
    
    Ui::LoginWindow *ui;
    const QString REGISTER_KEY = "1234567890";（可自行修改）
};


```

通过注册密钥注册管理员注册了即可登录系统



# 驱动代码编译

这里写一个驱动为例，另一个驱动也是用同一种方法即可编译

## 以sg90驱动为例

驱动编译有两种方法 

### 将驱动编译进内核

将驱动源码放到drivers/pwm/sg90/目录下在这个目录下需要有Kconfig和Makefile

在上级目录下需要修改drivers/pwm目录下的Makefile和Kconfig

参考如下，我手头上没有内核源码，无法写示例

[Linux嵌入式驱动开发02——驱动编译到内核_驱动编译进内核-CSDN博客](https://blog.csdn.net/szm1234/article/details/112883452?spm=1001.2014.3001.5501)



### 单独编译驱动

这时需要成功编译一次内核源码

1. 第一步：
   
   编写Makefile
   
   ```
   # 内核源码路径（替换为你的实际路径）
   KERNELDIR := /home/hhs/duojikaifa/linux-imx-rel_imx_4.1.15_2.1.0_ga_alientek
   CURRENT_PATH := $(shell pwd)
   obj-m := sg90_pwm_driver.o
   
   # 编译规则
   all:
       make -C $(KERNELDIR) M=$(CURRENT_PATH) modules ARCH=arm CROSS_COMPILE=arm-linux-gnueabihf-
   clean:
       make -C $(KERNELDIR) M=$(CURRENT_PATH) clean
   ```

2. 译驱动模块用的内核源码要和板子上的Linux内核源码是同一套，sg90驱动源码

```
# 1. 加载交叉编译环境
source /opt/fsl-imx-x11/4.1.15-2.1.0/environment-setup-cortexa7hf-neon-poky-linux-gnueabi

# 2. 编译驱动（生成 sg90_pwm.ko）
make -j4
```

 编译好后将.ko文件复制到开发板中放置驱动的目录下，同时我提供了一个test程序用来测试驱动加载后的舵机能否正常被驱动

insmod sg90_pwm.ko 加载驱动

rmmod sg90_pwm 卸载驱动



# 项目运行逻辑

数据库设计：

![](C:\Users\Huafeng\AppData\Roaming\marktext\images\2026-06-24-15-26-28-image.png)

### 

## 系统架构总览

```
ARM 开发板（i.MX6ULL）                    PC 服务器
┌─────────────────────────┐              ┌──────────────────────────┐
│  face_client (Qt5)      │  TCP:8888    │  face_server (Qt5)       │
│  ┌──────────────────┐   │◄────JSON────►│  ┌────────────────────┐  │
│  │  MainController  │   │             │  │  MainWidget         │  │
│  │  (页面调度中心)    │   │             │  │  (TCP服务器+线程池)  │  │
│  └──────────────────┘   │             │  └────────────────────┘  │
│  ┌──────────────────┐   │             │  ┌────────────────────┐  │
│  │  NetworkManager  │   │             │  │  RecognitionTask   │  │
│  │  (TCP通信)        │   │             │  │  (SeetaFace2识别)  │  │
│  └──────────────────┘   │             │  └────────────────────┘  │
│  ┌──────────────────┐   │             │  ┌────────────────────┐  │
│  │ HardwareController│  │             │  │  SQLite3 数据库     │  │
│  │ 指纹模块 SFM17    │   │             │  │  employee/attend.   │  │
│  │ 舵机 SG90         │   │             │  └────────────────────┘  │
│  └──────────────────┘   │             └──────────────────────────┘
│  摄像头 OpenCV           │
└─────────────────────────┘

```

## 

## 流程一：用户注册流程

```
管理员（服务器端）                客户端用户
      │                              │
      │ 生成注册密钥                   │
      │ (8位字母数字，24h有效，5次限)    │
      │                              │
      │                    ① 输入注册密钥
      │◄─────── key_verify ──────────│
      │ 验证密钥有效性                 │
      │──── key_verify_result ──────►│
      │                              │
      │                    ② 填写用户信息
      │                    (姓名/手机/地址/性别)
      │                              │
      │                    ③ 人脸采集
      │                    (实时预览，手动拍照确认)
      │                              │
      │                    ④ 指纹采集
      │                    (SFM17模块，按压3次录入)
      │                              │
      │                    ⑤ 设置6位密码
      │                              │
      │◄─── user_register ───────────│
      │ (注册密钥+用户信息+人脸Base64    │
      │  +指纹Base64+密码)             │
      │                              │
      │ 存入SQLite3数据库              │
      │ SeetaFace2注册人脸特征         │
      │ 分配指纹ID                    │
      │──── user_register_result ───►│
      │ (返回user_id + fingerprint_id)│
      │                              │
      │                    ⑥ 显示注册完成页面

```

## 流程二：人脸识别解锁

```
客户端                                    服务器
  │                                          │
  │ 摄像头采集 640×480 @ 15fps                 │
  │ OpenCV Haar级联分类器检测人脸               │
  │ (haarcascade_frontalface_alt2.xml)       │
  │                                          │
  │ 质量过滤：                                 │
  │  - 跳过前3-4帧                            │
  │  - Laplacian方差 > 60.0 才发送            │
  │                                          │
  │ JPEG压缩(75%)→ Base64编码                 │
  │──────── face_verify (JSON) ────────────► │
  │                                          │
  │                          线程池取出任务     │
  │                          OpenCV提取人脸区域(+30%边距)
  │                          预处理流水线:     │
  │                           CLAHE增强       │
  │                           非锐化掩模       │
  │                           缩放200×200     │
  │                          SeetaFace2 1:N查询
  │                          多帧融合(5帧/900ms窗口)
  │                          相似度 > 0.5 → 成功
  │                                          │
  │◄──────── auth_result (JSON) ──────────── │
  │ (success, user_id, name, dept, message)  │
  │                                          │
  │ 成功：舵机SG90旋转90°（开门）                │
  │       显示用户信息5秒                      │
  │       5秒后舵机复位0°（关门）               │
  │ 失败：显示识别失败提示                      │
  │                                         │
  │                          记录考勤日志     │
  │                          (unlock_type=face)
```

---

## 流程三：指纹解锁

```
客户端                                    服务器
  │                                          │
  │ 用户按压SFM17指纹模块                       │
  │ 硬件内部1:N匹配                            │
  │ 返回 user_id (int)                        │
  │                                          │
  │──── fingerprint_verify (JSON) ─────────► │
  │ {type, user_id, fingerprint_id}          │
  │                                          │
  │                          查询fingerprints表
  │                          fingerprint_id → employee_id
  │                          查询employee表获取用户信息
  │                                          │
  │◄──────── auth_result (JSON) ──────────── │
  │                                          │
  │ 成功：舵机开门5秒                           │
  │                          记录考勤日志      │
  │                          (unlock_type=fingerprint)
```

---

## 流程四：密码解锁

```
客户端                                      服务器
  │                                          │
  │ 用户输入6位数字密码                         │
  │ (虚拟键盘，星号显示)                        │
  │                                          │
  │──── password_verify (JSON) ────────────► │
  │ {type, password}                         │
  │                                          │
  │                          查询employee表   │
  │                          密码匹配验证      │
  │                                          │
  │◄──────── auth_result (JSON) ──────────── │
  │                                          │
  │ 成功：舵机开门5秒                           │
  │                          记录考勤日志      │
  │                          (unlock_type=password)
```

---





# 本项目基于以下完成

感谢以下大佬的开源项目，让我得以完成此项目，本项目是基于以下项目进行二次开发，完善系统功能，提高识别精度

[基于imx6ull人脸识别考勤系统: 基于imx6ull人脸识别考勤系统 - Gitee.com](https://gitee.com/aff544/face_recognition/tree/master)


