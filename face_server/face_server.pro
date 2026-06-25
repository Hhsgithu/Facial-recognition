QT       += core gui network sql

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# The following define makes your compiler emit warnings if you use
# any Qt feature that has been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

#ubuntu
INCLUDEPATH += /opt/opencv3-pc-youhua/include
INCLUDEPATH +=/opt/opencv3-pc-youhua/include/opencv2
INCLUDEPATH +=/opt/opencv3-pc-youhua/include/opencv
INCLUDEPATH +=/opt/opencv3-pc-youhua/include/seeta
LIBS += -L/usr/lib/x86_64-linux-gun -L/opt/opencv3-pc-youhua/lib -lopencv_world -lSeetaFaceDetector -lSeetaFaceLandmarker -lSeetaFaceRecognizer -lSeetaFaceTracker -lSeetaNet


# You can also make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    main.cpp \
    mainwidget.cpp \
    qfaceobject.cpp \
    registerwin.cpp \
    selectwin.cpp \
    recognitiontask.cpp \
    managewin.cpp \
    loginwindow.cpp \
    fingerprintmanager.cpp \
    registrationkeymanager.cpp

HEADERS += \
    mainwidget.h \
    qfaceobject.h \
    registerwin.h \
    selectwin.h \
    clientinfo.h \
    recognitiontask.h \
    managewin.h \
    loginwindow.h \
    fingerprintmanager.h \
    registrationkeymanager.h

FORMS += \
    mainwidget.ui \
    registerwin.ui \
    selectwin.ui \
    managewin.ui \
    loginwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
