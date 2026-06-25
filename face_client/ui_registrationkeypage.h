/********************************************************************************
** Form generated from reading UI file 'registrationkeypage.ui'
**
** Created by: Qt User Interface Compiler version 5.12.9
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_REGISTRATIONKEYPAGE_H
#define UI_REGISTRATIONKEYPAGE_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_RegistrationKeyPage
{
public:
    QWidget *headerWidget;
    QPushButton *backBtn;
    QLabel *titleLabel;
    QWidget *leftContainer;
    QWidget *inputCard;
    QLabel *iconLabel;
    QLabel *descLabel;
    QLabel *keyDisplay;
    QPushButton *verifyBtn;
    QLabel *statusLabel;
    QWidget *rightContainer;
    QPushButton *btn1;
    QPushButton *btn2;
    QPushButton *btn3;
    QPushButton *btn4;
    QPushButton *btn5;
    QPushButton *btn6;
    QPushButton *btn7;
    QPushButton *btn8;
    QPushButton *btn9;
    QPushButton *btn0;
    QPushButton *btnConfirm;
    QPushButton *btnDelete;

    void setupUi(QWidget *RegistrationKeyPage)
    {
        if (RegistrationKeyPage->objectName().isEmpty())
            RegistrationKeyPage->setObjectName(QString::fromUtf8("RegistrationKeyPage"));
        RegistrationKeyPage->resize(1024, 600);
        RegistrationKeyPage->setStyleSheet(QString::fromUtf8("QWidget#RegistrationKeyPage {\n"
"    background-color: rgb(245, 247, 250);\n"
"}"));
        headerWidget = new QWidget(RegistrationKeyPage);
        headerWidget->setObjectName(QString::fromUtf8("headerWidget"));
        headerWidget->setGeometry(QRect(0, 0, 1024, 60));
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
        leftContainer = new QWidget(RegistrationKeyPage);
        leftContainer->setObjectName(QString::fromUtf8("leftContainer"));
        leftContainer->setGeometry(QRect(0, 60, 424, 540));
        leftContainer->setStyleSheet(QString::fromUtf8("background-color: rgb(245, 247, 250);"));
        inputCard = new QWidget(leftContainer);
        inputCard->setObjectName(QString::fromUtf8("inputCard"));
        inputCard->setGeometry(QRect(40, 80, 344, 320));
        inputCard->setStyleSheet(QString::fromUtf8("background-color: rgb(255, 255, 255);\n"
"border: 1px solid rgb(230, 230, 230);\n"
"border-radius: 12px;"));
        iconLabel = new QLabel(inputCard);
        iconLabel->setObjectName(QString::fromUtf8("iconLabel"));
        iconLabel->setGeometry(QRect(0, 30, 344, 50));
        iconLabel->setStyleSheet(QString::fromUtf8("font-size: 48px;\n"
"color: rgb(24, 144, 255);\n"
"background: transparent;\n"
"border: none;"));
        iconLabel->setAlignment(Qt::AlignCenter);
        descLabel = new QLabel(inputCard);
        descLabel->setObjectName(QString::fromUtf8("descLabel"));
        descLabel->setGeometry(QRect(20, 95, 304, 30));
        descLabel->setStyleSheet(QString::fromUtf8("font-size: 14px;\n"
"color: rgb(100, 100, 100);\n"
"background: transparent;\n"
"border: none;"));
        descLabel->setAlignment(Qt::AlignCenter);
        keyDisplay = new QLabel(inputCard);
        keyDisplay->setObjectName(QString::fromUtf8("keyDisplay"));
        keyDisplay->setGeometry(QRect(20, 140, 304, 70));
        keyDisplay->setStyleSheet(QString::fromUtf8("font-size: 32px;\n"
"font-weight: bold;\n"
"color: rgb(50, 50, 50);\n"
"background-color: rgb(250, 250, 250);\n"
"border: 2px solid rgb(24, 144, 255);\n"
"border-radius: 8px;\n"
"letter-spacing: 8px;"));
        keyDisplay->setAlignment(Qt::AlignCenter);
        verifyBtn = new QPushButton(inputCard);
        verifyBtn->setObjectName(QString::fromUtf8("verifyBtn"));
        verifyBtn->setGeometry(QRect(72, 240, 200, 50));
        verifyBtn->setStyleSheet(QString::fromUtf8("QPushButton {\n"
"    background-color: rgb(82, 196, 26);\n"
"    border: none;\n"
"    border-radius: 8px;\n"
"    font-size: 18px;\n"
"    font-weight: bold;\n"
"    color: white;\n"
"}\n"
"QPushButton:pressed {\n"
"    background-color: rgb(56, 158, 13);\n"
"}\n"
"QPushButton:disabled {\n"
"    background-color: rgb(200, 200, 200);\n"
"    color: rgb(150, 150, 150);\n"
"}"));
        statusLabel = new QLabel(leftContainer);
        statusLabel->setObjectName(QString::fromUtf8("statusLabel"));
        statusLabel->setGeometry(QRect(40, 420, 344, 30));
        statusLabel->setStyleSheet(QString::fromUtf8("font-size: 14px;\n"
"color: rgb(100, 100, 100);\n"
"background: transparent;"));
        statusLabel->setAlignment(Qt::AlignCenter);
        rightContainer = new QWidget(RegistrationKeyPage);
        rightContainer->setObjectName(QString::fromUtf8("rightContainer"));
        rightContainer->setGeometry(QRect(424, 60, 600, 540));
        rightContainer->setStyleSheet(QString::fromUtf8("background-color: rgb(245, 247, 250);"));
        btn1 = new QPushButton(rightContainer);
        btn1->setObjectName(QString::fromUtf8("btn1"));
        btn1->setGeometry(QRect(100, 60, 110, 80));
        btn1->setStyleSheet(QString::fromUtf8("QPushButton {\n"
"    background-color: rgb(24, 144, 255);\n"
"    border: none;\n"
"    border-radius: 12px;\n"
"    font-size: 28px;\n"
"    font-weight: bold;\n"
"    color: white;\n"
"}\n"
"QPushButton:pressed {\n"
"    background-color: rgb(9, 109, 217);\n"
"}"));
        btn2 = new QPushButton(rightContainer);
        btn2->setObjectName(QString::fromUtf8("btn2"));
        btn2->setGeometry(QRect(230, 60, 110, 80));
        btn2->setStyleSheet(QString::fromUtf8("QPushButton {\n"
"    background-color: rgb(24, 144, 255);\n"
"    border: none;\n"
"    border-radius: 12px;\n"
"    font-size: 28px;\n"
"    font-weight: bold;\n"
"    color: white;\n"
"}\n"
"QPushButton:pressed {\n"
"    background-color: rgb(9, 109, 217);\n"
"}"));
        btn3 = new QPushButton(rightContainer);
        btn3->setObjectName(QString::fromUtf8("btn3"));
        btn3->setGeometry(QRect(360, 60, 110, 80));
        btn3->setStyleSheet(QString::fromUtf8("QPushButton {\n"
"    background-color: rgb(24, 144, 255);\n"
"    border: none;\n"
"    border-radius: 12px;\n"
"    font-size: 28px;\n"
"    font-weight: bold;\n"
"    color: white;\n"
"}\n"
"QPushButton:pressed {\n"
"    background-color: rgb(9, 109, 217);\n"
"}"));
        btn4 = new QPushButton(rightContainer);
        btn4->setObjectName(QString::fromUtf8("btn4"));
        btn4->setGeometry(QRect(100, 160, 110, 80));
        btn4->setStyleSheet(QString::fromUtf8("QPushButton {\n"
"    background-color: rgb(24, 144, 255);\n"
"    border: none;\n"
"    border-radius: 12px;\n"
"    font-size: 28px;\n"
"    font-weight: bold;\n"
"    color: white;\n"
"}\n"
"QPushButton:pressed {\n"
"    background-color: rgb(9, 109, 217);\n"
"}"));
        btn5 = new QPushButton(rightContainer);
        btn5->setObjectName(QString::fromUtf8("btn5"));
        btn5->setGeometry(QRect(230, 160, 110, 80));
        btn5->setStyleSheet(QString::fromUtf8("QPushButton {\n"
"    background-color: rgb(24, 144, 255);\n"
"    border: none;\n"
"    border-radius: 12px;\n"
"    font-size: 28px;\n"
"    font-weight: bold;\n"
"    color: white;\n"
"}\n"
"QPushButton:pressed {\n"
"    background-color: rgb(9, 109, 217);\n"
"}"));
        btn6 = new QPushButton(rightContainer);
        btn6->setObjectName(QString::fromUtf8("btn6"));
        btn6->setGeometry(QRect(360, 160, 110, 80));
        btn6->setStyleSheet(QString::fromUtf8("QPushButton {\n"
"    background-color: rgb(24, 144, 255);\n"
"    border: none;\n"
"    border-radius: 12px;\n"
"    font-size: 28px;\n"
"    font-weight: bold;\n"
"    color: white;\n"
"}\n"
"QPushButton:pressed {\n"
"    background-color: rgb(9, 109, 217);\n"
"}"));
        btn7 = new QPushButton(rightContainer);
        btn7->setObjectName(QString::fromUtf8("btn7"));
        btn7->setGeometry(QRect(100, 260, 110, 80));
        btn7->setStyleSheet(QString::fromUtf8("QPushButton {\n"
"    background-color: rgb(24, 144, 255);\n"
"    border: none;\n"
"    border-radius: 12px;\n"
"    font-size: 28px;\n"
"    font-weight: bold;\n"
"    color: white;\n"
"}\n"
"QPushButton:pressed {\n"
"    background-color: rgb(9, 109, 217);\n"
"}"));
        btn8 = new QPushButton(rightContainer);
        btn8->setObjectName(QString::fromUtf8("btn8"));
        btn8->setGeometry(QRect(230, 260, 110, 80));
        btn8->setStyleSheet(QString::fromUtf8("QPushButton {\n"
"    background-color: rgb(24, 144, 255);\n"
"    border: none;\n"
"    border-radius: 12px;\n"
"    font-size: 28px;\n"
"    font-weight: bold;\n"
"    color: white;\n"
"}\n"
"QPushButton:pressed {\n"
"    background-color: rgb(9, 109, 217);\n"
"}"));
        btn9 = new QPushButton(rightContainer);
        btn9->setObjectName(QString::fromUtf8("btn9"));
        btn9->setGeometry(QRect(360, 260, 110, 80));
        btn9->setStyleSheet(QString::fromUtf8("QPushButton {\n"
"    background-color: rgb(24, 144, 255);\n"
"    border: none;\n"
"    border-radius: 12px;\n"
"    font-size: 28px;\n"
"    font-weight: bold;\n"
"    color: white;\n"
"}\n"
"QPushButton:pressed {\n"
"    background-color: rgb(9, 109, 217);\n"
"}"));
        btn0 = new QPushButton(rightContainer);
        btn0->setObjectName(QString::fromUtf8("btn0"));
        btn0->setGeometry(QRect(230, 360, 110, 80));
        btn0->setStyleSheet(QString::fromUtf8("QPushButton {\n"
"    background-color: rgb(24, 144, 255);\n"
"    border: none;\n"
"    border-radius: 12px;\n"
"    font-size: 28px;\n"
"    font-weight: bold;\n"
"    color: white;\n"
"}\n"
"QPushButton:pressed {\n"
"    background-color: rgb(9, 109, 217);\n"
"}"));
        btnConfirm = new QPushButton(rightContainer);
        btnConfirm->setObjectName(QString::fromUtf8("btnConfirm"));
        btnConfirm->setGeometry(QRect(100, 360, 110, 80));
        btnConfirm->setStyleSheet(QString::fromUtf8("QPushButton {\n"
"    background-color: rgb(82, 196, 26);\n"
"    border: none;\n"
"    border-radius: 12px;\n"
"    font-size: 16px;\n"
"    font-weight: bold;\n"
"    color: white;\n"
"}\n"
"QPushButton:pressed {\n"
"    background-color: rgb(56, 158, 13);\n"
"}\n"
"QPushButton:disabled {\n"
"    background-color: rgb(200, 200, 200);\n"
"    color: rgb(150, 150, 150);\n"
"}"));
        btnDelete = new QPushButton(rightContainer);
        btnDelete->setObjectName(QString::fromUtf8("btnDelete"));
        btnDelete->setGeometry(QRect(360, 360, 110, 80));
        btnDelete->setStyleSheet(QString::fromUtf8("QPushButton {\n"
"    background-color: rgb(255, 193, 7);\n"
"    border: none;\n"
"    border-radius: 12px;\n"
"    font-size: 16px;\n"
"    font-weight: bold;\n"
"    color: white;\n"
"}\n"
"QPushButton:pressed {\n"
"    background-color: rgb(255, 160, 0);\n"
"}"));

        retranslateUi(RegistrationKeyPage);

        QMetaObject::connectSlotsByName(RegistrationKeyPage);
    } // setupUi

    void retranslateUi(QWidget *RegistrationKeyPage)
    {
        RegistrationKeyPage->setWindowTitle(QApplication::translate("RegistrationKeyPage", "\346\263\250\345\206\214\345\257\206\351\222\245\351\252\214\350\257\201", nullptr));
        backBtn->setText(QApplication::translate("RegistrationKeyPage", "\342\206\220 \350\277\224\345\233\236", nullptr));
        titleLabel->setText(QApplication::translate("RegistrationKeyPage", "\347\224\250\346\210\267\346\263\250\345\206\214 - \345\257\206\351\222\245\351\252\214\350\257\201", nullptr));
        iconLabel->setText(QString());
        descLabel->setText(QApplication::translate("RegistrationKeyPage", "\350\257\267\350\276\223\345\205\245\347\256\241\347\220\206\345\221\230\346\217\220\344\276\233\347\232\204\346\263\250\345\206\214\345\257\206\351\222\245\357\274\2108\344\275\215\357\274\211", nullptr));
        keyDisplay->setText(QString());
        verifyBtn->setText(QApplication::translate("RegistrationKeyPage", "\351\252\214\350\257\201\345\257\206\351\222\245", nullptr));
        statusLabel->setText(QString());
        btn1->setText(QApplication::translate("RegistrationKeyPage", "1", nullptr));
        btn2->setText(QApplication::translate("RegistrationKeyPage", "2", nullptr));
        btn3->setText(QApplication::translate("RegistrationKeyPage", "3", nullptr));
        btn4->setText(QApplication::translate("RegistrationKeyPage", "4", nullptr));
        btn5->setText(QApplication::translate("RegistrationKeyPage", "5", nullptr));
        btn6->setText(QApplication::translate("RegistrationKeyPage", "6", nullptr));
        btn7->setText(QApplication::translate("RegistrationKeyPage", "7", nullptr));
        btn8->setText(QApplication::translate("RegistrationKeyPage", "8", nullptr));
        btn9->setText(QApplication::translate("RegistrationKeyPage", "9", nullptr));
        btn0->setText(QApplication::translate("RegistrationKeyPage", "0", nullptr));
        btnConfirm->setText(QApplication::translate("RegistrationKeyPage", "\347\241\256\350\256\244", nullptr));
        btnDelete->setText(QApplication::translate("RegistrationKeyPage", "\345\210\240\351\231\244", nullptr));
    } // retranslateUi

};

namespace Ui {
    class RegistrationKeyPage: public Ui_RegistrationKeyPage {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_REGISTRATIONKEYPAGE_H
