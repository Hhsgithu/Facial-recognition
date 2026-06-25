/********************************************************************************
** Form generated from reading UI file 'fingerprintcapturepage.ui'
**
** Created by: Qt User Interface Compiler version 5.12.9
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_FINGERPRINTCAPTUREPAGE_H
#define UI_FINGERPRINTCAPTUREPAGE_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_FingerprintCapturePage
{
public:
    QWidget *headerWidget;
    QLabel *titleLabel;
    QPushButton *backBtn;
    QLabel *userNameLabel;
    QLabel *progressLabel;
    QWidget *contentWidget;
    QLabel *iconLabel;
    QLabel *descLabel;
    QPushButton *startCaptureBtn;
    QLabel *statusLabel;
    QLabel *tipLabel;

    void setupUi(QWidget *FingerprintCapturePage)
    {
        if (FingerprintCapturePage->objectName().isEmpty())
            FingerprintCapturePage->setObjectName(QString::fromUtf8("FingerprintCapturePage"));
        FingerprintCapturePage->resize(1024, 600);
        FingerprintCapturePage->setStyleSheet(QString::fromUtf8("QWidget#FingerprintCapturePage {\n"
"    background-color: rgb(245, 247, 250);\n"
"}"));
        headerWidget = new QWidget(FingerprintCapturePage);
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
        progressLabel = new QLabel(FingerprintCapturePage);
        progressLabel->setObjectName(QString::fromUtf8("progressLabel"));
        progressLabel->setGeometry(QRect(0, 70, 1024, 30));
        progressLabel->setStyleSheet(QString::fromUtf8("font-size: 16px;\n"
"color: rgb(24, 144, 255);\n"
"background: transparent;"));
        progressLabel->setAlignment(Qt::AlignCenter);
        contentWidget = new QWidget(FingerprintCapturePage);
        contentWidget->setObjectName(QString::fromUtf8("contentWidget"));
        contentWidget->setGeometry(QRect(262, 120, 500, 450));
        contentWidget->setStyleSheet(QString::fromUtf8("background-color: rgb(255, 255, 255);\n"
"border-radius: 16px;"));
        iconLabel = new QLabel(contentWidget);
        iconLabel->setObjectName(QString::fromUtf8("iconLabel"));
        iconLabel->setGeometry(QRect(0, 60, 500, 80));
        iconLabel->setStyleSheet(QString::fromUtf8("font-size: 64px;\n"
"color: rgb(24, 144, 255);\n"
"background: transparent;"));
        iconLabel->setAlignment(Qt::AlignCenter);
        descLabel = new QLabel(contentWidget);
        descLabel->setObjectName(QString::fromUtf8("descLabel"));
        descLabel->setGeometry(QRect(50, 150, 400, 80));
        descLabel->setStyleSheet(QString::fromUtf8("font-size: 16px;\n"
"color: rgb(100, 100, 100);\n"
"background: transparent;\n"
"line-height: 1.6;"));
        descLabel->setAlignment(Qt::AlignCenter);
        descLabel->setWordWrap(true);
        startCaptureBtn = new QPushButton(contentWidget);
        startCaptureBtn->setObjectName(QString::fromUtf8("startCaptureBtn"));
        startCaptureBtn->setGeometry(QRect(175, 250, 150, 50));
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
        statusLabel = new QLabel(contentWidget);
        statusLabel->setObjectName(QString::fromUtf8("statusLabel"));
        statusLabel->setGeometry(QRect(50, 320, 400, 40));
        statusLabel->setStyleSheet(QString::fromUtf8("font-size: 14px;\n"
"color: rgb(100, 100, 100);\n"
"background: transparent;"));
        statusLabel->setAlignment(Qt::AlignCenter);
        statusLabel->setWordWrap(true);
        tipLabel = new QLabel(contentWidget);
        tipLabel->setObjectName(QString::fromUtf8("tipLabel"));
        tipLabel->setGeometry(QRect(50, 370, 400, 50));
        tipLabel->setStyleSheet(QString::fromUtf8("font-size: 12px;\n"
"color: rgb(150, 150, 150);\n"
"background: transparent;"));
        tipLabel->setAlignment(Qt::AlignCenter);
        tipLabel->setWordWrap(true);

        retranslateUi(FingerprintCapturePage);

        QMetaObject::connectSlotsByName(FingerprintCapturePage);
    } // setupUi

    void retranslateUi(QWidget *FingerprintCapturePage)
    {
        FingerprintCapturePage->setWindowTitle(QApplication::translate("FingerprintCapturePage", "\346\214\207\347\272\271\351\207\207\351\233\206", nullptr));
        titleLabel->setText(QApplication::translate("FingerprintCapturePage", "\347\224\250\346\210\267\346\263\250\345\206\214 - \346\214\207\347\272\271\351\207\207\351\233\206", nullptr));
        backBtn->setText(QApplication::translate("FingerprintCapturePage", "\350\277\224\345\233\236", nullptr));
        userNameLabel->setText(QApplication::translate("FingerprintCapturePage", "\347\224\250\346\210\267\357\274\232", nullptr));
        progressLabel->setText(QApplication::translate("FingerprintCapturePage", "\346\255\245\351\252\244 2/4: \346\214\207\347\272\271\351\207\207\351\233\206", nullptr));
        iconLabel->setText(QString());
        descLabel->setText(QApplication::translate("FingerprintCapturePage", "\350\257\267\345\260\206\346\211\213\346\214\207\350\275\273\346\224\276\345\234\250\346\214\207\347\272\271\344\274\240\346\204\237\345\231\250\344\270\212\n"
"\344\277\235\346\214\201\346\211\213\346\214\207\345\271\262\347\207\245\346\270\205\346\264\201\357\274\214\346\214\211\345\216\213\351\200\202\344\270\255", nullptr));
        startCaptureBtn->setText(QApplication::translate("FingerprintCapturePage", "\345\274\200\345\247\213\351\207\207\351\233\206", nullptr));
        statusLabel->setText(QString());
        tipLabel->setText(QApplication::translate("FingerprintCapturePage", "\351\207\207\351\233\206\346\210\220\345\212\237\345\220\216\345\260\206\350\207\252\345\212\250\350\277\233\345\205\245\344\270\213\344\270\200\346\255\245", nullptr));
    } // retranslateUi

};

namespace Ui {
    class FingerprintCapturePage: public Ui_FingerprintCapturePage {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_FINGERPRINTCAPTUREPAGE_H
