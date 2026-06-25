/********************************************************************************
** Form generated from reading UI file 'fingerprintpage.ui'
**
** Created by: Qt User Interface Compiler version 5.12.9
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_FINGERPRINTPAGE_H
#define UI_FINGERPRINTPAGE_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_FingerprintPage
{
public:
    QWidget *headerWidget;
    QPushButton *backBtn;
    QLabel *titleLabel;
    QWidget *contentWidget;
    QWidget *fingerprintWidget;
    QLabel *fingerprintIcon;
    QLabel *fingerprintTip;
    QLabel *statusLabel;
    QLabel *connectionLabel;
    QWidget *buttonWidget;
    QPushButton *startBtn;
    QPushButton *stopBtn;

    void setupUi(QWidget *FingerprintPage)
    {
        if (FingerprintPage->objectName().isEmpty())
            FingerprintPage->setObjectName(QString::fromUtf8("FingerprintPage"));
        FingerprintPage->resize(1024, 600);
        FingerprintPage->setAutoFillBackground(false);
        FingerprintPage->setStyleSheet(QString::fromUtf8("QWidget#FingerprintPage {\n"
"    background-color: rgb(245, 247, 250);\n"
"}"));
        headerWidget = new QWidget(FingerprintPage);
        headerWidget->setObjectName(QString::fromUtf8("headerWidget"));
        headerWidget->setGeometry(QRect(0, 0, 1024, 60));
        headerWidget->setAutoFillBackground(false);
        headerWidget->setStyleSheet(QString::fromUtf8("background-color: rgb(255, 255, 255);\n"
"border-bottom: 1px solid rgb(230, 230, 230);"));
        backBtn = new QPushButton(headerWidget);
        backBtn->setObjectName(QString::fromUtf8("backBtn"));
        backBtn->setGeometry(QRect(20, 12, 80, 36));
        backBtn->setStyleSheet(QString::fromUtf8("QPushButton {\n"
"    background-color: rgb(240, 240, 240);\n"
"    border: none;\n"
"    border-radius: 18px;\n"
"    font-size: 14px;\n"
"    color: rgb(80, 80, 80);\n"
"}\n"
"QPushButton:pressed {\n"
"    background-color: rgb(220, 220, 220);\n"
"}"));
        titleLabel = new QLabel(headerWidget);
        titleLabel->setObjectName(QString::fromUtf8("titleLabel"));
        titleLabel->setGeometry(QRect(200, 0, 624, 60));
        titleLabel->setStyleSheet(QString::fromUtf8("font-size: 22px;\n"
"font-weight: bold;\n"
"color: rgb(50, 50, 50);\n"
"background: transparent;"));
        titleLabel->setAlignment(Qt::AlignCenter);
        contentWidget = new QWidget(FingerprintPage);
        contentWidget->setObjectName(QString::fromUtf8("contentWidget"));
        contentWidget->setGeometry(QRect(0, 60, 1024, 540));
        contentWidget->setAutoFillBackground(false);
        contentWidget->setStyleSheet(QString::fromUtf8("background-color: rgb(245, 247, 250);"));
        fingerprintWidget = new QWidget(contentWidget);
        fingerprintWidget->setObjectName(QString::fromUtf8("fingerprintWidget"));
        fingerprintWidget->setGeometry(QRect(312, 80, 400, 200));
        fingerprintWidget->setAutoFillBackground(false);
        fingerprintWidget->setStyleSheet(QString::fromUtf8("background-color: rgb(255, 255, 255);\n"
"border-radius: 20px;"));
        fingerprintIcon = new QLabel(fingerprintWidget);
        fingerprintIcon->setObjectName(QString::fromUtf8("fingerprintIcon"));
        fingerprintIcon->setGeometry(QRect(150, 40, 100, 100));
        fingerprintIcon->setStyleSheet(QString::fromUtf8("font-size: 72px;\n"
"color: rgb(82, 196, 26);\n"
"background: transparent;"));
        fingerprintIcon->setAlignment(Qt::AlignCenter);
        fingerprintTip = new QLabel(fingerprintWidget);
        fingerprintTip->setObjectName(QString::fromUtf8("fingerprintTip"));
        fingerprintTip->setGeometry(QRect(0, 150, 400, 30));
        fingerprintTip->setStyleSheet(QString::fromUtf8("font-size: 16px;\n"
"color: rgb(100, 100, 100);\n"
"background: transparent;"));
        fingerprintTip->setAlignment(Qt::AlignCenter);
        statusLabel = new QLabel(contentWidget);
        statusLabel->setObjectName(QString::fromUtf8("statusLabel"));
        statusLabel->setGeometry(QRect(0, 320, 1024, 40));
        statusLabel->setStyleSheet(QString::fromUtf8("font-size: 18px;\n"
"font-weight: bold;\n"
"color: rgb(100, 100, 100);\n"
"background: transparent;"));
        statusLabel->setAlignment(Qt::AlignCenter);
        connectionLabel = new QLabel(contentWidget);
        connectionLabel->setObjectName(QString::fromUtf8("connectionLabel"));
        connectionLabel->setGeometry(QRect(0, 380, 1024, 30));
        connectionLabel->setStyleSheet(QString::fromUtf8("font-size: 14px;\n"
"color: rgb(150, 150, 150);\n"
"background: transparent;"));
        connectionLabel->setAlignment(Qt::AlignCenter);
        buttonWidget = new QWidget(contentWidget);
        buttonWidget->setObjectName(QString::fromUtf8("buttonWidget"));
        buttonWidget->setGeometry(QRect(0, 440, 1024, 80));
        buttonWidget->setAutoFillBackground(false);
        buttonWidget->setStyleSheet(QString::fromUtf8("background: transparent;"));
        startBtn = new QPushButton(buttonWidget);
        startBtn->setObjectName(QString::fromUtf8("startBtn"));
        startBtn->setGeometry(QRect(312, 20, 180, 40));
        startBtn->setStyleSheet(QString::fromUtf8("QPushButton {\n"
"    background-color: rgb(82, 196, 26);\n"
"    border: none;\n"
"    border-radius: 20px;\n"
"    font-size: 16px;\n"
"    font-weight: bold;\n"
"    color: white;\n"
"}\n"
"QPushButton:pressed {\n"
"    background-color: rgb(60, 170, 10);\n"
"}\n"
"QPushButton:disabled {\n"
"    background-color: rgb(200, 200, 200);\n"
"    color: rgb(150, 150, 150);\n"
"}"));
        stopBtn = new QPushButton(buttonWidget);
        stopBtn->setObjectName(QString::fromUtf8("stopBtn"));
        stopBtn->setGeometry(QRect(532, 20, 180, 40));
        stopBtn->setStyleSheet(QString::fromUtf8("QPushButton {\n"
"    background-color: rgb(255, 77, 79);\n"
"    border: none;\n"
"    border-radius: 20px;\n"
"    font-size: 16px;\n"
"    font-weight: bold;\n"
"    color: white;\n"
"}\n"
"QPushButton:pressed {\n"
"    background-color: rgb(230, 50, 50);\n"
"}\n"
"QPushButton:disabled {\n"
"    background-color: rgb(200, 200, 200);\n"
"    color: rgb(150, 150, 150);\n"
"}"));

        retranslateUi(FingerprintPage);

        QMetaObject::connectSlotsByName(FingerprintPage);
    } // setupUi

    void retranslateUi(QWidget *FingerprintPage)
    {
        FingerprintPage->setWindowTitle(QApplication::translate("FingerprintPage", "\346\214\207\347\272\271\350\247\243\351\224\201", nullptr));
        backBtn->setText(QApplication::translate("FingerprintPage", "\342\206\220 \350\277\224\345\233\236", nullptr));
        titleLabel->setText(QApplication::translate("FingerprintPage", "\346\214\207\347\272\271\350\247\243\351\224\201", nullptr));
        fingerprintIcon->setText(QString());
        fingerprintTip->setText(QApplication::translate("FingerprintPage", "\350\257\267\345\260\206\346\211\213\346\214\207\346\224\276\345\234\250\346\214\207\347\272\271\344\274\240\346\204\237\345\231\250\344\270\212", nullptr));
        statusLabel->setText(QApplication::translate("FingerprintPage", "\347\255\211\345\276\205\346\214\207\347\272\271\350\257\206\345\210\253...", nullptr));
        connectionLabel->setText(QApplication::translate("FingerprintPage", "\346\234\215\345\212\241\345\231\250\350\277\236\346\216\245\347\212\266\346\200\201: \346\234\252\350\277\236\346\216\245", nullptr));
        startBtn->setText(QApplication::translate("FingerprintPage", "\345\274\200\345\247\213\350\257\206\345\210\253", nullptr));
        stopBtn->setText(QApplication::translate("FingerprintPage", "\345\201\234\346\255\242\350\257\206\345\210\253", nullptr));
    } // retranslateUi

};

namespace Ui {
    class FingerprintPage: public Ui_FingerprintPage {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_FINGERPRINTPAGE_H
