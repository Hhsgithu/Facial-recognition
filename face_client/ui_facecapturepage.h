/********************************************************************************
** Form generated from reading UI file 'facecapturepage.ui'
**
** Created by: Qt User Interface Compiler version 5.12.9
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_FACECAPTUREPAGE_H
#define UI_FACECAPTUREPAGE_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_FaceCapturePage
{
public:
    QWidget *headerWidget;
    QLabel *titleLabel;
    QPushButton *backBtn;
    QLabel *userNameLabel;
    QLabel *progressLabel;
    QWidget *contentWidget;
    QWidget *previewContainer;
    QLabel *previewTitleLabel;
    QLabel *previewPlaceholder;
    QWidget *controlContainer;
    QLabel *faceIconLabel;
    QLabel *descLabel;
    QPushButton *startCaptureBtn;
    QLabel *statusLabel;
    QLabel *tipLabel;

    void setupUi(QWidget *FaceCapturePage)
    {
        if (FaceCapturePage->objectName().isEmpty())
            FaceCapturePage->setObjectName(QString::fromUtf8("FaceCapturePage"));
        FaceCapturePage->resize(1024, 600);
        FaceCapturePage->setStyleSheet(QString::fromUtf8("QWidget#FaceCapturePage {\n"
"    background-color: rgb(245, 247, 250);\n"
"}"));
        headerWidget = new QWidget(FaceCapturePage);
        headerWidget->setObjectName(QString::fromUtf8("headerWidget"));
        headerWidget->setGeometry(QRect(0, 0, 1024, 60));
        headerWidget->setStyleSheet(QString::fromUtf8("background-color: rgb(255, 255, 255);\n"
"border-bottom: 1px solid rgb(230, 230, 230);"));
        titleLabel = new QLabel(headerWidget);
        titleLabel->setObjectName(QString::fromUtf8("titleLabel"));
        titleLabel->setGeometry(QRect(0, 0, 1024, 60));
        titleLabel->setStyleSheet(QString::fromUtf8("font-size: 22px;\n"
"font-weight: bold;\n"
"color: rgb(50, 50, 50);\n"
"background: transparent;"));
        titleLabel->setAlignment(Qt::AlignCenter);
        backBtn = new QPushButton(headerWidget);
        backBtn->setObjectName(QString::fromUtf8("backBtn"));
        backBtn->setGeometry(QRect(20, 15, 80, 30));
        backBtn->setStyleSheet(QString::fromUtf8("QPushButton {\n"
"    background-color: transparent;\n"
"    border: 1px solid rgb(200, 200, 200);\n"
"    border-radius: 4px;\n"
"    font-size: 14px;\n"
"    color: rgb(100, 100, 100);\n"
"}\n"
"QPushButton:pressed {\n"
"    background-color: rgb(240, 240, 240);\n"
"}"));
        userNameLabel = new QLabel(headerWidget);
        userNameLabel->setObjectName(QString::fromUtf8("userNameLabel"));
        userNameLabel->setGeometry(QRect(800, 15, 200, 30));
        userNameLabel->setStyleSheet(QString::fromUtf8("font-size: 14px;\n"
"color: rgb(100, 100, 100);\n"
"background: transparent;"));
        userNameLabel->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        progressLabel = new QLabel(FaceCapturePage);
        progressLabel->setObjectName(QString::fromUtf8("progressLabel"));
        progressLabel->setGeometry(QRect(0, 70, 1024, 30));
        progressLabel->setStyleSheet(QString::fromUtf8("font-size: 16px;\n"
"color: rgb(24, 144, 255);\n"
"background: transparent;"));
        progressLabel->setAlignment(Qt::AlignCenter);
        contentWidget = new QWidget(FaceCapturePage);
        contentWidget->setObjectName(QString::fromUtf8("contentWidget"));
        contentWidget->setGeometry(QRect(50, 120, 924, 450));
        contentWidget->setStyleSheet(QString::fromUtf8("background-color: rgb(255, 255, 255);\n"
"border-radius: 16px;"));
        previewContainer = new QWidget(contentWidget);
        previewContainer->setObjectName(QString::fromUtf8("previewContainer"));
        previewContainer->setGeometry(QRect(30, 30, 500, 390));
        previewContainer->setStyleSheet(QString::fromUtf8("background: transparent;"));
        previewTitleLabel = new QLabel(previewContainer);
        previewTitleLabel->setObjectName(QString::fromUtf8("previewTitleLabel"));
        previewTitleLabel->setGeometry(QRect(0, 0, 500, 30));
        previewTitleLabel->setStyleSheet(QString::fromUtf8("font-size: 16px;\n"
"font-weight: bold;\n"
"color: rgb(50, 50, 50);\n"
"background: transparent;"));
        previewTitleLabel->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignVCenter);
        previewPlaceholder = new QLabel(previewContainer);
        previewPlaceholder->setObjectName(QString::fromUtf8("previewPlaceholder"));
        previewPlaceholder->setGeometry(QRect(30, 40, 440, 330));
        previewPlaceholder->setStyleSheet(QString::fromUtf8("background-color: rgb(50, 50, 50);\n"
"border: 2px solid rgb(200, 200, 200);\n"
"border-radius: 8px;\n"
"color: rgb(150, 150, 150);\n"
"font-size: 14px;"));
        previewPlaceholder->setAlignment(Qt::AlignCenter);
        controlContainer = new QWidget(contentWidget);
        controlContainer->setObjectName(QString::fromUtf8("controlContainer"));
        controlContainer->setGeometry(QRect(550, 30, 344, 390));
        controlContainer->setStyleSheet(QString::fromUtf8("background: transparent;"));
        faceIconLabel = new QLabel(controlContainer);
        faceIconLabel->setObjectName(QString::fromUtf8("faceIconLabel"));
        faceIconLabel->setGeometry(QRect(0, 20, 344, 80));
        faceIconLabel->setStyleSheet(QString::fromUtf8("font-size: 64px;\n"
"color: rgb(24, 144, 255);\n"
"background: transparent;"));
        faceIconLabel->setAlignment(Qt::AlignCenter);
        descLabel = new QLabel(controlContainer);
        descLabel->setObjectName(QString::fromUtf8("descLabel"));
        descLabel->setGeometry(QRect(20, 110, 304, 80));
        descLabel->setStyleSheet(QString::fromUtf8("font-size: 15px;\n"
"color: rgb(100, 100, 100);\n"
"background: transparent;"));
        descLabel->setAlignment(Qt::AlignCenter);
        descLabel->setWordWrap(true);
        startCaptureBtn = new QPushButton(controlContainer);
        startCaptureBtn->setObjectName(QString::fromUtf8("startCaptureBtn"));
        startCaptureBtn->setGeometry(QRect(72, 210, 200, 50));
        startCaptureBtn->setStyleSheet(QString::fromUtf8("QPushButton {\n"
"    background-color: rgb(24, 144, 255);\n"
"    color: white;\n"
"    border: none;\n"
"    border-radius: 8px;\n"
"    font-size: 18px;\n"
"    font-weight: bold;\n"
"}\n"
"QPushButton:pressed {\n"
"    background-color: rgb(16, 120, 220);\n"
"}\n"
"QPushButton:disabled {\n"
"    background-color: rgb(200, 200, 200);\n"
"    color: rgb(150, 150, 150);\n"
"}"));
        statusLabel = new QLabel(controlContainer);
        statusLabel->setObjectName(QString::fromUtf8("statusLabel"));
        statusLabel->setGeometry(QRect(20, 280, 304, 40));
        statusLabel->setStyleSheet(QString::fromUtf8("font-size: 14px;\n"
"color: rgb(100, 100, 100);\n"
"background: transparent;"));
        statusLabel->setAlignment(Qt::AlignCenter);
        statusLabel->setWordWrap(true);
        tipLabel = new QLabel(controlContainer);
        tipLabel->setObjectName(QString::fromUtf8("tipLabel"));
        tipLabel->setGeometry(QRect(20, 330, 304, 50));
        tipLabel->setStyleSheet(QString::fromUtf8("font-size: 12px;\n"
"color: rgb(150, 150, 150);\n"
"background: transparent;"));
        tipLabel->setAlignment(Qt::AlignCenter);
        tipLabel->setWordWrap(true);

        retranslateUi(FaceCapturePage);

        QMetaObject::connectSlotsByName(FaceCapturePage);
    } // setupUi

    void retranslateUi(QWidget *FaceCapturePage)
    {
        FaceCapturePage->setWindowTitle(QApplication::translate("FaceCapturePage", "\344\272\272\350\204\270\351\207\207\351\233\206", nullptr));
        titleLabel->setText(QApplication::translate("FaceCapturePage", "\347\224\250\346\210\267\346\263\250\345\206\214 - \344\272\272\350\204\270\351\207\207\351\233\206", nullptr));
        backBtn->setText(QApplication::translate("FaceCapturePage", "\350\277\224\345\233\236", nullptr));
        userNameLabel->setText(QApplication::translate("FaceCapturePage", "\347\224\250\346\210\267\357\274\232", nullptr));
        progressLabel->setText(QApplication::translate("FaceCapturePage", "\346\255\245\351\252\244 1/4: \344\272\272\350\204\270\351\207\207\351\233\206", nullptr));
        previewTitleLabel->setText(QApplication::translate("FaceCapturePage", "\345\256\236\346\227\266\347\224\273\351\235\242", nullptr));
        previewPlaceholder->setText(QApplication::translate("FaceCapturePage", "\347\202\271\345\207\273\\\"\345\274\200\345\247\213\351\207\207\351\233\206\\\"\345\220\257\345\212\250\346\221\204\345\203\217\345\244\264", nullptr));
        faceIconLabel->setText(QString());
        descLabel->setText(QApplication::translate("FaceCapturePage", "\350\257\267\346\255\243\350\247\206\346\221\204\345\203\217\345\244\264\n"
"\344\277\235\346\214\201\351\235\242\351\203\250\346\270\205\346\231\260\345\217\257\350\247\201\n"
"\347\202\271\345\207\273\344\270\213\346\226\271\346\214\211\351\222\256\345\274\200\345\247\213\351\207\207\351\233\206", nullptr));
        startCaptureBtn->setText(QApplication::translate("FaceCapturePage", "\345\274\200\345\247\213\351\207\207\351\233\206", nullptr));
        statusLabel->setText(QString());
        tipLabel->setText(QApplication::translate("FaceCapturePage", "\351\207\207\351\233\206\345\256\214\346\210\220\345\220\216\345\260\206\350\207\252\345\212\250\350\277\233\345\205\245\344\270\213\344\270\200\346\255\245", nullptr));
    } // retranslateUi

};

namespace Ui {
    class FaceCapturePage: public Ui_FaceCapturePage {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_FACECAPTUREPAGE_H
