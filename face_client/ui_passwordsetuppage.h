/********************************************************************************
** Form generated from reading UI file 'passwordsetuppage.ui'
**
** Created by: Qt User Interface Compiler version 5.12.9
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_PASSWORDSETUPPAGE_H
#define UI_PASSWORDSETUPPAGE_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_PasswordSetupPage
{
public:
    QWidget *headerWidget;
    QPushButton *backBtn;
    QLabel *titleLabel;
    QLabel *userNameLabel;
    QWidget *leftContainer;
    QLabel *progressLabel;
    QLabel *instructionLabel;
    QWidget *inputContainer;
    QLabel *passwordLabel;
    QLabel *passwordDisplay;
    QLabel *confirmPasswordLabel;
    QLabel *confirmPasswordDisplay;
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
    QPushButton *btnDelete;
    QLabel *statusLabel;
    QPushButton *confirmBtn;

    void setupUi(QWidget *PasswordSetupPage)
    {
        if (PasswordSetupPage->objectName().isEmpty())
            PasswordSetupPage->setObjectName(QString::fromUtf8("PasswordSetupPage"));
        PasswordSetupPage->resize(1024, 600);
        PasswordSetupPage->setStyleSheet(QString::fromUtf8("QWidget#PasswordSetupPage {\n"
"    background-color: rgb(245, 247, 250);\n"
"}"));
        headerWidget = new QWidget(PasswordSetupPage);
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
        userNameLabel = new QLabel(headerWidget);
        userNameLabel->setObjectName(QString::fromUtf8("userNameLabel"));
        userNameLabel->setGeometry(QRect(850, 15, 150, 30));
        userNameLabel->setStyleSheet(QString::fromUtf8("font-size: 14px;\n"
"color: rgb(80, 80, 80);\n"
"background: transparent;"));
        userNameLabel->setAlignment(Qt::AlignRight|Qt::AlignVCenter);
        leftContainer = new QWidget(PasswordSetupPage);
        leftContainer->setObjectName(QString::fromUtf8("leftContainer"));
        leftContainer->setGeometry(QRect(0, 60, 424, 540));
        leftContainer->setStyleSheet(QString::fromUtf8("background-color: rgb(245, 247, 250);"));
        progressLabel = new QLabel(leftContainer);
        progressLabel->setObjectName(QString::fromUtf8("progressLabel"));
        progressLabel->setGeometry(QRect(40, 40, 344, 30));
        progressLabel->setStyleSheet(QString::fromUtf8("font-size: 16px;\n"
"font-weight: bold;\n"
"color: rgb(24, 144, 255);\n"
"background: transparent;"));
        instructionLabel = new QLabel(leftContainer);
        instructionLabel->setObjectName(QString::fromUtf8("instructionLabel"));
        instructionLabel->setGeometry(QRect(40, 90, 344, 30));
        instructionLabel->setStyleSheet(QString::fromUtf8("font-size: 14px;\n"
"color: rgb(80, 80, 80);\n"
"background: transparent;"));
        inputContainer = new QWidget(leftContainer);
        inputContainer->setObjectName(QString::fromUtf8("inputContainer"));
        inputContainer->setGeometry(QRect(40, 150, 344, 280));
        inputContainer->setStyleSheet(QString::fromUtf8("background-color: rgb(255, 255, 255);\n"
"border: 1px solid rgb(230, 230, 230);\n"
"border-radius: 12px;"));
        passwordLabel = new QLabel(inputContainer);
        passwordLabel->setObjectName(QString::fromUtf8("passwordLabel"));
        passwordLabel->setGeometry(QRect(20, 20, 304, 25));
        passwordLabel->setStyleSheet(QString::fromUtf8("font-size: 13px;\n"
"color: rgb(80, 80, 80);\n"
"background: transparent;\n"
"border: none;"));
        passwordDisplay = new QLabel(inputContainer);
        passwordDisplay->setObjectName(QString::fromUtf8("passwordDisplay"));
        passwordDisplay->setGeometry(QRect(20, 50, 304, 60));
        passwordDisplay->setStyleSheet(QString::fromUtf8("font-size: 32px;\n"
"font-weight: bold;\n"
"color: rgb(50, 50, 50);\n"
"background-color: rgb(250, 250, 250);\n"
"border: 1px solid rgb(230, 230, 230);\n"
"border-radius: 8px;"));
        passwordDisplay->setAlignment(Qt::AlignCenter);
        passwordDisplay->setCursor(QCursor(Qt::PointingHandCursor));
        confirmPasswordLabel = new QLabel(inputContainer);
        confirmPasswordLabel->setObjectName(QString::fromUtf8("confirmPasswordLabel"));
        confirmPasswordLabel->setGeometry(QRect(20, 130, 304, 25));
        confirmPasswordLabel->setStyleSheet(QString::fromUtf8("font-size: 13px;\n"
"color: rgb(80, 80, 80);\n"
"background: transparent;\n"
"border: none;"));
        confirmPasswordDisplay = new QLabel(inputContainer);
        confirmPasswordDisplay->setObjectName(QString::fromUtf8("confirmPasswordDisplay"));
        confirmPasswordDisplay->setGeometry(QRect(20, 160, 304, 60));
        confirmPasswordDisplay->setStyleSheet(QString::fromUtf8("font-size: 32px;\n"
"font-weight: bold;\n"
"color: rgb(50, 50, 50);\n"
"background-color: rgb(250, 250, 250);\n"
"border: 1px solid rgb(230, 230, 230);\n"
"border-radius: 8px;"));
        confirmPasswordDisplay->setAlignment(Qt::AlignCenter);
        confirmPasswordDisplay->setCursor(QCursor(Qt::PointingHandCursor));
        rightContainer = new QWidget(PasswordSetupPage);
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
        statusLabel = new QLabel(rightContainer);
        statusLabel->setObjectName(QString::fromUtf8("statusLabel"));
        statusLabel->setGeometry(QRect(50, 460, 500, 30));
        statusLabel->setStyleSheet(QString::fromUtf8("font-size: 14px;\n"
"color: rgb(100, 100, 100);\n"
"background: transparent;"));
        statusLabel->setAlignment(Qt::AlignCenter);
        confirmBtn = new QPushButton(rightContainer);
        confirmBtn->setObjectName(QString::fromUtf8("confirmBtn"));
        confirmBtn->setGeometry(QRect(100, 360, 110, 80));
        confirmBtn->setStyleSheet(QString::fromUtf8("QPushButton {\n"
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

        retranslateUi(PasswordSetupPage);

        QMetaObject::connectSlotsByName(PasswordSetupPage);
    } // setupUi

    void retranslateUi(QWidget *PasswordSetupPage)
    {
        PasswordSetupPage->setWindowTitle(QApplication::translate("PasswordSetupPage", "\345\257\206\347\240\201\350\256\276\347\275\256", nullptr));
        backBtn->setText(QApplication::translate("PasswordSetupPage", "\342\206\220 \350\277\224\345\233\236", nullptr));
        titleLabel->setText(QApplication::translate("PasswordSetupPage", "\350\256\276\347\275\256\345\257\206\347\240\201", nullptr));
        userNameLabel->setText(QApplication::translate("PasswordSetupPage", "\347\224\250\346\210\267\357\274\232", nullptr));
        progressLabel->setText(QApplication::translate("PasswordSetupPage", "\346\255\245\351\252\244 3/4 - \350\256\276\347\275\256\345\257\206\347\240\201", nullptr));
        instructionLabel->setText(QApplication::translate("PasswordSetupPage", "\350\257\267\350\256\276\347\275\2566\344\275\215\346\225\260\345\255\227\345\257\206\347\240\201", nullptr));
        passwordLabel->setText(QApplication::translate("PasswordSetupPage", "\350\276\223\345\205\245\345\257\206\347\240\201:", nullptr));
        passwordDisplay->setText(QString());
        confirmPasswordLabel->setText(QApplication::translate("PasswordSetupPage", "\347\241\256\350\256\244\345\257\206\347\240\201:", nullptr));
        confirmPasswordDisplay->setText(QString());
        btn1->setText(QApplication::translate("PasswordSetupPage", "1", nullptr));
        btn2->setText(QApplication::translate("PasswordSetupPage", "2", nullptr));
        btn3->setText(QApplication::translate("PasswordSetupPage", "3", nullptr));
        btn4->setText(QApplication::translate("PasswordSetupPage", "4", nullptr));
        btn5->setText(QApplication::translate("PasswordSetupPage", "5", nullptr));
        btn6->setText(QApplication::translate("PasswordSetupPage", "6", nullptr));
        btn7->setText(QApplication::translate("PasswordSetupPage", "7", nullptr));
        btn8->setText(QApplication::translate("PasswordSetupPage", "8", nullptr));
        btn9->setText(QApplication::translate("PasswordSetupPage", "9", nullptr));
        btn0->setText(QApplication::translate("PasswordSetupPage", "0", nullptr));
        btnDelete->setText(QApplication::translate("PasswordSetupPage", "\345\210\240\351\231\244", nullptr));
        statusLabel->setText(QString());
        confirmBtn->setText(QApplication::translate("PasswordSetupPage", "\347\241\256\350\256\244", nullptr));
    } // retranslateUi

};

namespace Ui {
    class PasswordSetupPage: public Ui_PasswordSetupPage {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_PASSWORDSETUPPAGE_H
