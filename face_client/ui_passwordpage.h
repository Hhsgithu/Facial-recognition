/********************************************************************************
** Form generated from reading UI file 'passwordpage.ui'
**
** Created by: Qt User Interface Compiler version 5.12.9
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_PASSWORDPAGE_H
#define UI_PASSWORDPAGE_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_PasswordPage
{
public:
    QWidget *headerWidget;
    QPushButton *backBtn;
    QLabel *titleLabel;
    QWidget *contentWidget;
    QLabel *passwordDisplay;
    QPushButton *btn1;
    QPushButton *btn2;
    QPushButton *btn3;
    QPushButton *btn4;
    QPushButton *btn5;
    QPushButton *btn6;
    QPushButton *btn7;
    QPushButton *btn8;
    QPushButton *btn9;
    QPushButton *btnDelete;
    QPushButton *btn0;
    QPushButton *btnConfirm;
    QLabel *statusLabel;
    QLabel *connectionLabel;

    void setupUi(QWidget *PasswordPage)
    {
        if (PasswordPage->objectName().isEmpty())
            PasswordPage->setObjectName(QString::fromUtf8("PasswordPage"));
        PasswordPage->resize(1024, 600);
        PasswordPage->setAutoFillBackground(false);
        PasswordPage->setStyleSheet(QString::fromUtf8("QWidget#PasswordPage {\n"
"    background-color: rgb(245, 247, 250);\n"
"}"));
        headerWidget = new QWidget(PasswordPage);
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
        contentWidget = new QWidget(PasswordPage);
        contentWidget->setObjectName(QString::fromUtf8("contentWidget"));
        contentWidget->setGeometry(QRect(0, 60, 1024, 540));
        contentWidget->setAutoFillBackground(false);
        contentWidget->setStyleSheet(QString::fromUtf8("background-color: rgb(245, 247, 250);"));
        passwordDisplay = new QLabel(contentWidget);
        passwordDisplay->setObjectName(QString::fromUtf8("passwordDisplay"));
        passwordDisplay->setGeometry(QRect(320, 30, 400, 60));
        passwordDisplay->setStyleSheet(QString::fromUtf8("font-size: 36px;\n"
"font-weight: bold;\n"
"color: rgb(50, 50, 50);\n"
"background-color: rgb(250, 250, 250);\n"
"border-radius: 12px;"));
        passwordDisplay->setAlignment(Qt::AlignCenter);
        btn1 = new QPushButton(contentWidget);
        btn1->setObjectName(QString::fromUtf8("btn1"));
        btn1->setGeometry(QRect(50, 110, 90, 70));
        btn1->setStyleSheet(QString::fromUtf8("QPushButton {\n"
"    background-color: rgb(250, 250, 250);\n"
"    border: none;\n"
"    border-radius: 12px;\n"
"    font-size: 28px;\n"
"    font-weight: bold;\n"
"    color: rgb(50, 50, 50);\n"
"}\n"
"QPushButton:pressed {\n"
"    background-color: rgb(230, 230, 230);\n"
"}"));
        btn2 = new QPushButton(contentWidget);
        btn2->setObjectName(QString::fromUtf8("btn2"));
        btn2->setGeometry(QRect(155, 110, 90, 70));
        btn2->setStyleSheet(QString::fromUtf8("QPushButton {\n"
"    background-color: rgb(250, 250, 250);\n"
"    border: none;\n"
"    border-radius: 12px;\n"
"    font-size: 28px;\n"
"    font-weight: bold;\n"
"    color: rgb(50, 50, 50);\n"
"}\n"
"QPushButton:pressed {\n"
"    background-color: rgb(230, 230, 230);\n"
"}"));
        btn3 = new QPushButton(contentWidget);
        btn3->setObjectName(QString::fromUtf8("btn3"));
        btn3->setGeometry(QRect(260, 110, 90, 70));
        btn3->setStyleSheet(QString::fromUtf8("QPushButton {\n"
"    background-color: rgb(250, 250, 250);\n"
"    border: none;\n"
"    border-radius: 12px;\n"
"    font-size: 28px;\n"
"    font-weight: bold;\n"
"    color: rgb(50, 50, 50);\n"
"}\n"
"QPushButton:pressed {\n"
"    background-color: rgb(230, 230, 230);\n"
"}"));
        btn4 = new QPushButton(contentWidget);
        btn4->setObjectName(QString::fromUtf8("btn4"));
        btn4->setGeometry(QRect(50, 190, 90, 70));
        btn4->setStyleSheet(QString::fromUtf8("QPushButton {\n"
"    background-color: rgb(250, 250, 250);\n"
"    border: none;\n"
"    border-radius: 12px;\n"
"    font-size: 28px;\n"
"    font-weight: bold;\n"
"    color: rgb(50, 50, 50);\n"
"}\n"
"QPushButton:pressed {\n"
"    background-color: rgb(230, 230, 230);\n"
"}"));
        btn5 = new QPushButton(contentWidget);
        btn5->setObjectName(QString::fromUtf8("btn5"));
        btn5->setGeometry(QRect(155, 190, 90, 70));
        btn5->setStyleSheet(QString::fromUtf8("QPushButton {\n"
"    background-color: rgb(250, 250, 250);\n"
"    border: none;\n"
"    border-radius: 12px;\n"
"    font-size: 28px;\n"
"    font-weight: bold;\n"
"    color: rgb(50, 50, 50);\n"
"}\n"
"QPushButton:pressed {\n"
"    background-color: rgb(230, 230, 230);\n"
"}"));
        btn6 = new QPushButton(contentWidget);
        btn6->setObjectName(QString::fromUtf8("btn6"));
        btn6->setGeometry(QRect(260, 190, 90, 70));
        btn6->setStyleSheet(QString::fromUtf8("QPushButton {\n"
"    background-color: rgb(250, 250, 250);\n"
"    border: none;\n"
"    border-radius: 12px;\n"
"    font-size: 28px;\n"
"    font-weight: bold;\n"
"    color: rgb(50, 50, 50);\n"
"}\n"
"QPushButton:pressed {\n"
"    background-color: rgb(230, 230, 230);\n"
"}"));
        btn7 = new QPushButton(contentWidget);
        btn7->setObjectName(QString::fromUtf8("btn7"));
        btn7->setGeometry(QRect(50, 270, 90, 70));
        btn7->setStyleSheet(QString::fromUtf8("QPushButton {\n"
"    background-color: rgb(250, 250, 250);\n"
"    border: none;\n"
"    border-radius: 12px;\n"
"    font-size: 28px;\n"
"    font-weight: bold;\n"
"    color: rgb(50, 50, 50);\n"
"}\n"
"QPushButton:pressed {\n"
"    background-color: rgb(230, 230, 230);\n"
"}"));
        btn8 = new QPushButton(contentWidget);
        btn8->setObjectName(QString::fromUtf8("btn8"));
        btn8->setGeometry(QRect(155, 270, 90, 70));
        btn8->setStyleSheet(QString::fromUtf8("QPushButton {\n"
"    background-color: rgb(250, 250, 250);\n"
"    border: none;\n"
"    border-radius: 12px;\n"
"    font-size: 28px;\n"
"    font-weight: bold;\n"
"    color: rgb(50, 50, 50);\n"
"}\n"
"QPushButton:pressed {\n"
"    background-color: rgb(230, 230, 230);\n"
"}"));
        btn9 = new QPushButton(contentWidget);
        btn9->setObjectName(QString::fromUtf8("btn9"));
        btn9->setGeometry(QRect(260, 270, 90, 70));
        btn9->setStyleSheet(QString::fromUtf8("QPushButton {\n"
"    background-color: rgb(250, 250, 250);\n"
"    border: none;\n"
"    border-radius: 12px;\n"
"    font-size: 28px;\n"
"    font-weight: bold;\n"
"    color: rgb(50, 50, 50);\n"
"}\n"
"QPushButton:pressed {\n"
"    background-color: rgb(230, 230, 230);\n"
"}"));
        btnDelete = new QPushButton(contentWidget);
        btnDelete->setObjectName(QString::fromUtf8("btnDelete"));
        btnDelete->setGeometry(QRect(50, 350, 90, 70));
        btnDelete->setStyleSheet(QString::fromUtf8("QPushButton {\n"
"    background-color: rgb(250, 250, 250);\n"
"    border: none;\n"
"    border-radius: 12px;\n"
"    font-size: 18px;\n"
"    color: rgb(100, 100, 100);\n"
"}\n"
"QPushButton:pressed {\n"
"    background-color: rgb(230, 230, 230);\n"
"}"));
        btn0 = new QPushButton(contentWidget);
        btn0->setObjectName(QString::fromUtf8("btn0"));
        btn0->setGeometry(QRect(155, 350, 90, 70));
        btn0->setStyleSheet(QString::fromUtf8("QPushButton {\n"
"    background-color: rgb(250, 250, 250);\n"
"    border: none;\n"
"    border-radius: 12px;\n"
"    font-size: 28px;\n"
"    font-weight: bold;\n"
"    color: rgb(50, 50, 50);\n"
"}\n"
"QPushButton:pressed {\n"
"    background-color: rgb(230, 230, 230);\n"
"}"));
        btnConfirm = new QPushButton(contentWidget);
        btnConfirm->setObjectName(QString::fromUtf8("btnConfirm"));
        btnConfirm->setGeometry(QRect(260, 350, 90, 70));
        btnConfirm->setStyleSheet(QString::fromUtf8("QPushButton {\n"
"    background-color: rgb(82, 196, 26);\n"
"    border: none;\n"
"    border-radius: 12px;\n"
"    font-size: 18px;\n"
"    font-weight: bold;\n"
"    color: white;\n"
"}\n"
"QPushButton:pressed {\n"
"    background-color: rgb(60, 170, 10);\n"
"}"));
        statusLabel = new QLabel(contentWidget);
        statusLabel->setObjectName(QString::fromUtf8("statusLabel"));
        statusLabel->setGeometry(QRect(50, 430, 400, 30));
        statusLabel->setStyleSheet(QString::fromUtf8("font-size: 16px;\n"
"color: rgb(100, 100, 100);\n"
"background: transparent;"));
        statusLabel->setAlignment(Qt::AlignCenter);
        connectionLabel = new QLabel(contentWidget);
        connectionLabel->setObjectName(QString::fromUtf8("connectionLabel"));
        connectionLabel->setGeometry(QRect(50, 460, 400, 24));
        connectionLabel->setStyleSheet(QString::fromUtf8("font-size: 12px;\n"
"color: rgb(180, 180, 180);\n"
"background: transparent;"));
        connectionLabel->setAlignment(Qt::AlignCenter);

        retranslateUi(PasswordPage);

        QMetaObject::connectSlotsByName(PasswordPage);
    } // setupUi

    void retranslateUi(QWidget *PasswordPage)
    {
        PasswordPage->setWindowTitle(QApplication::translate("PasswordPage", "\345\257\206\347\240\201\350\247\243\351\224\201", nullptr));
        backBtn->setText(QApplication::translate("PasswordPage", "\342\206\220 \350\277\224\345\233\236", nullptr));
        titleLabel->setText(QApplication::translate("PasswordPage", "\345\257\206\347\240\201\350\247\243\351\224\201", nullptr));
        passwordDisplay->setText(QString());
        btn1->setText(QApplication::translate("PasswordPage", "1", nullptr));
        btn2->setText(QApplication::translate("PasswordPage", "2", nullptr));
        btn3->setText(QApplication::translate("PasswordPage", "3", nullptr));
        btn4->setText(QApplication::translate("PasswordPage", "4", nullptr));
        btn5->setText(QApplication::translate("PasswordPage", "5", nullptr));
        btn6->setText(QApplication::translate("PasswordPage", "6", nullptr));
        btn7->setText(QApplication::translate("PasswordPage", "7", nullptr));
        btn8->setText(QApplication::translate("PasswordPage", "8", nullptr));
        btn9->setText(QApplication::translate("PasswordPage", "9", nullptr));
        btnDelete->setText(QApplication::translate("PasswordPage", "\345\210\240\351\231\244", nullptr));
        btn0->setText(QApplication::translate("PasswordPage", "0", nullptr));
        btnConfirm->setText(QApplication::translate("PasswordPage", "\347\241\256\350\256\244", nullptr));
        statusLabel->setText(QString());
        connectionLabel->setText(QApplication::translate("PasswordPage", "\347\275\221\347\273\234\347\212\266\346\200\201: \346\234\252\350\277\236\346\216\245", nullptr));
    } // retranslateUi

};

namespace Ui {
    class PasswordPage: public Ui_PasswordPage {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_PASSWORDPAGE_H
