QT       += core gui network concurrent

# 尝试添加 virtualkeyboard 模块（如果可用）
qtHaveModule(virtualkeyboard):qtHaveModule(quickwidgets) {
    QT += virtualkeyboard quickwidgets qml quick
    DEFINES += HAVE_VIRTUALKEYBOARD
    message("QtVirtualKeyboard 模块已启用")
} else {
    warning("QtVirtualKeyboard 模块不可用，将使用系统默认输入法")
}

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# The following define makes your compiler emit warnings if you use
# any Qt feature that has been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0


##ubuntu
#INCLUDEPATH += /opt/opencv3-pc/include
#INCLUDEPATH +=/opt/opencv3-pc/include/opencv2
#INCLUDEPATH +=/opt/opencv3-pc/include/opencv
#INCLUDEPATH +=/opt/opencv3-pc/include/seeta
#LIBS += -L/usr/lib/x86_64-linux-gun -L/opt/opencv3-pc/lib -lopencv_world -lSeetaFaceDetector -lSeetaFaceLandmarker -lSeetaFaceRecognizer -lSeetaFaceTracker -lSeetaNet -lSeetaQualityAssessor


#imx6ull
INCLUDEPATH += /opt/opencv3-arm/include
LIBS += -L/opt/opencv3-arm/lib \
        -lopencv_core \
        -lopencv_highgui \
        -lopencv_imgproc \
        -lopencv_videoio \
        -lopencv_imgcodecs \
        -lopencv_objdetect \
        -lopencv_calib3d

# 3. 添加zlib动态库路径+显式链接zlib+配置rpath（关键）
ZLIB_LIB_PATH = /usr/local/arm/gcc-linaro-4.9.4-2017.01-x86_64_arm-linux-gnueabihf/arm-linux-gnueabihf/sysroot/usr/lib
LIBS += -L$$ZLIB_LIB_PATH \
        -lz \  # 显式链接zlib库
        -Wl,-rpath=$$ZLIB_LIB_PATH \  # 配置运行时zlib库路径
        -Wl,-rpath=/opt/opencv3-arm/lib  # 配置运行时OpenCV库路径


SOURCES += \
    main.cpp \
    homepage.cpp \
    maincontroller.cpp \
    passwordpage.cpp \
    facerecognition.cpp \
    doorsimulator.cpp \
    fingerprintpage.cpp \
    fingerprintcontroller.cpp \
    servocontroller.cpp \
    hardwarecontroller.cpp \
    registrationkeypage.cpp \
    userinfopage.cpp \
    biometriccollectionpage.cpp \
    facecapturepage.cpp \
    fingerprintcapturepage.cpp \
    passwordsetuppage.cpp \
    registrationcompletepage.cpp \
    networkmanager.cpp


HEADERS += \
    homepage.h \
    maincontroller.h \
    passwordpage.h \
    facerecognition.h \
    doorsimulator.h \
    fingerprintpage.h \
    fingerprintcontroller.h \
    servocontroller.h \
    hardwarecontroller.h \
    registrationkeypage.h \
    userinfopage.h \
    biometriccollectionpage.h \
    facecapturepage.h \
    fingerprintcapturepage.h \
    passwordsetuppage.h \
    registrationcompletepage.h \
    networkmanager.h


FORMS += \
    homepage.ui \
    passwordpage.ui \
    facerecognition.ui \
    fingerprintpage.ui \
    registrationkeypage.ui \
    userinfopage.ui \
    biometriccollectionpage.ui \
    facecapturepage.ui \
    fingerprintcapturepage.ui \
    passwordsetuppage.ui \
    registrationcompletepage.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    image.qrc \
    keyboard.qrc
