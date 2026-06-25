/********************************************************************************
** Form generated from reading UI file 'homepage.ui'
**
** Created by: Qt User Interface Compiler version 5.12.9
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_HOMEPAGE_H
#define UI_HOMEPAGE_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_HomePage
{
public:
    QWidget *headerWidget;
    QLabel *titleLabel;
    QWidget *dateTimeWidget;
    QLabel *timeLabel;
    QLabel *dateLabel;
    QLabel *weekLabel;
    QWidget *buttonWidget;
    QPushButton *faceRecognitionBtn;
    QPushButton *passwordUnlockBtn;
    QPushButton *fingerprintUnlockBtn;
    QPushButton *userRegistrationBtn;
    QLabel *tipLabel;

    void setupUi(QWidget *HomePage)
    {
        if (HomePage->objectName().isEmpty())
            HomePage->setObjectName(QString::fromUtf8("HomePage"));
        HomePage->resize(1024, 600);
        HomePage->setAutoFillBackground(true);
        HomePage->setStyleSheet(QString::fromUtf8("QWidget#HomePage {\n"
"    background-color: rgb(245, 247, 250);\n"
"}"));
        headerWidget = new QWidget(HomePage);
        headerWidget->setObjectName(QString::fromUtf8("headerWidget"));
        headerWidget->setGeometry(QRect(0, 0, 1024, 60));
        headerWidget->setAutoFillBackground(true);
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
        dateTimeWidget = new QWidget(HomePage);
        dateTimeWidget->setObjectName(QString::fromUtf8("dateTimeWidget"));
        dateTimeWidget->setGeometry(QRect(0, 60, 1024, 200));
        dateTimeWidget->setAutoFillBackground(true);
        dateTimeWidget->setStyleSheet(QString::fromUtf8("background-color: rgb(245, 247, 250);"));
        timeLabel = new QLabel(dateTimeWidget);
        timeLabel->setObjectName(QString::fromUtf8("timeLabel"));
        timeLabel->setGeometry(QRect(0, 30, 1024, 80));
        timeLabel->setStyleSheet(QString::fromUtf8("font-size: 72px;\n"
"font-weight: bold;\n"
"color: rgb(50, 50, 50);\n"
"background: transparent;"));
        timeLabel->setAlignment(Qt::AlignCenter);
        dateLabel = new QLabel(dateTimeWidget);
        dateLabel->setObjectName(QString::fromUtf8("dateLabel"));
        dateLabel->setGeometry(QRect(0, 120, 512, 40));
        dateLabel->setStyleSheet(QString::fromUtf8("font-size: 18px;\n"
"color: rgb(120, 120, 120);\n"
"background: transparent;"));
        dateLabel->setAlignment(Qt::AlignRight|Qt::AlignVCenter);
        weekLabel = new QLabel(dateTimeWidget);
        weekLabel->setObjectName(QString::fromUtf8("weekLabel"));
        weekLabel->setGeometry(QRect(522, 120, 200, 40));
        weekLabel->setStyleSheet(QString::fromUtf8("font-size: 18px;\n"
"color: rgb(120, 120, 120);\n"
"background: transparent;"));
        weekLabel->setAlignment(Qt::AlignLeft|Qt::AlignVCenter);
        buttonWidget = new QWidget(HomePage);
        buttonWidget->setObjectName(QString::fromUtf8("buttonWidget"));
        buttonWidget->setGeometry(QRect(0, 260, 1024, 340));
        buttonWidget->setAutoFillBackground(true);
        buttonWidget->setStyleSheet(QString::fromUtf8("background-color: rgb(245, 247, 250);"));
        faceRecognitionBtn = new QPushButton(buttonWidget);
        faceRecognitionBtn->setObjectName(QString::fromUtf8("faceRecognitionBtn"));
        faceRecognitionBtn->setGeometry(QRect(80, 30, 200, 120));
        faceRecognitionBtn->setStyleSheet(QString::fromUtf8("QPushButton {\n"
"    background-color: rgb(255, 255, 255);\n"
"    border: none;\n"
"    border-radius: 16px;\n"
"    font-size: 20px;\n"
"    font-weight: bold;\n"
"    color: rgb(50, 50, 50);\n"
"}\n"
"QPushButton:pressed {\n"
"    background-color: rgb(240, 240, 240);\n"
"}"));
        passwordUnlockBtn = new QPushButton(buttonWidget);
        passwordUnlockBtn->setObjectName(QString::fromUtf8("passwordUnlockBtn"));
        passwordUnlockBtn->setGeometry(QRect(300, 30, 200, 120));
        passwordUnlockBtn->setStyleSheet(QString::fromUtf8("QPushButton {\n"
"    background-color: rgb(255, 255, 255);\n"
"    border: none;\n"
"    border-radius: 16px;\n"
"    font-size: 20px;\n"
"    font-weight: bold;\n"
"    color: rgb(50, 50, 50);\n"
"}\n"
"QPushButton:pressed {\n"
"    background-color: rgb(240, 240, 240);\n"
"}"));
        fingerprintUnlockBtn = new QPushButton(buttonWidget);
        fingerprintUnlockBtn->setObjectName(QString::fromUtf8("fingerprintUnlockBtn"));
        fingerprintUnlockBtn->setGeometry(QRect(520, 30, 200, 120));
        fingerprintUnlockBtn->setStyleSheet(QString::fromUtf8("QPushButton {\n"
"    background-color: rgb(255, 255, 255);\n"
"    border: none;\n"
"    border-radius: 16px;\n"
"    font-size: 20px;\n"
"    font-weight: bold;\n"
"    color: rgb(50, 50, 50);\n"
"}\n"
"QPushButton:pressed {\n"
"    background-color: rgb(240, 240, 240);\n"
"}"));
        userRegistrationBtn = new QPushButton(buttonWidget);
        userRegistrationBtn->setObjectName(QString::fromUtf8("userRegistrationBtn"));
        userRegistrationBtn->setGeometry(QRect(740, 30, 200, 120));
        userRegistrationBtn->setStyleSheet(QString::fromUtf8("QPushButton {\n"
"    background-color: rgb(24, 144, 255);\n"
"    border: none;\n"
"    border-radius: 16px;\n"
"    font-size: 20px;\n"
"    font-weight: bold;\n"
"    color: white;\n"
"}\n"
"QPushButton:pressed {\n"
"    background-color: rgb(16, 120, 220);\n"
"}"));
        tipLabel = new QLabel(buttonWidget);
        tipLabel->setObjectName(QString::fromUtf8("tipLabel"));
        tipLabel->setGeometry(QRect(0, 170, 1024, 30));
        tipLabel->setStyleSheet(QString::fromUtf8("font-size: 14px;\n"
"color: rgb(180, 180, 180);\n"
"background: transparent;"));
        tipLabel->setAlignment(Qt::AlignCenter);

        retranslateUi(HomePage);

        QMetaObject::connectSlotsByName(HomePage);
    } // setupUi

    void retranslateUi(QWidget *HomePage)
    {
        HomePage->setWindowTitle(QApplication::translate("HomePage", "\351\246\226\351\241\265", nullptr));
        titleLabel->setText(QApplication::translate("HomePage", "\346\231\272\350\203\275\351\227\250\347\246\201\347\263\273\347\273\237", nullptr));
        timeLabel->setText(QApplication::translate("HomePage", "12:00:00", nullptr));
        dateLabel->setText(QApplication::translate("HomePage", "2024\345\271\26401\346\234\21001\346\227\245", nullptr));
        weekLabel->setText(QApplication::translate("HomePage", "\346\230\237\346\234\237\344\270\200", nullptr));
        faceRecognitionBtn->setText(QApplication::translate("HomePage", "\344\272\272\350\204\270\350\257\206\345\210\253", nullptr));
        passwordUnlockBtn->setText(QApplication::translate("HomePage", "\345\257\206\347\240\201\350\247\243\351\224\201", nullptr));
        fingerprintUnlockBtn->setText(QApplication::translate("HomePage", "\346\214\207\347\272\271\350\247\243\351\224\201", nullptr));
        userRegistrationBtn->setText(QApplication::translate("HomePage", "\347\224\250\346\210\267\346\263\250\345\206\214", nullptr));
        tipLabel->setText(QApplication::translate("HomePage", "\350\257\267\351\200\211\346\213\251\350\247\243\351\224\201\346\226\271\345\274\217\346\210\226\346\263\250\345\206\214\346\226\260\347\224\250\346\210\267", nullptr));
    } // retranslateUi

};

namespace Ui {
    class HomePage: public Ui_HomePage {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_HOMEPAGE_H
