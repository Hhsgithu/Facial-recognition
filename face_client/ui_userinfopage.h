/********************************************************************************
** Form generated from reading UI file 'userinfopage.ui'
**
** Created by: Qt User Interface Compiler version 5.12.9
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_USERINFOPAGE_H
#define UI_USERINFOPAGE_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QRadioButton>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_UserInfoPage
{
public:
    QWidget *headerWidget;
    QPushButton *backBtn;
    QLabel *titleLabel;
    QWidget *leftContainer;
    QWidget *inputCard;
    QLabel *iconLabel;
    QLabel *descLabel;
    QLabel *nameLabel;
    QLineEdit *nameLineEdit;
    QLabel *sexLabel;
    QRadioButton *maleRadioBtn;
    QRadioButton *femaleRadioBtn;
    QLabel *phoneLabel;
    QLineEdit *phoneLineEdit;
    QLabel *addressLabel;
    QLineEdit *addressLineEdit;
    QPushButton *nextBtn;
    QLabel *statusLabel;
    QWidget *rightContainer;

    void setupUi(QWidget *UserInfoPage)
    {
        if (UserInfoPage->objectName().isEmpty())
            UserInfoPage->setObjectName(QString::fromUtf8("UserInfoPage"));
        UserInfoPage->resize(1024, 600);
        UserInfoPage->setStyleSheet(QString::fromUtf8("QWidget#UserInfoPage {\n"
"    background-color: rgb(245, 247, 250);\n"
"}"));
        headerWidget = new QWidget(UserInfoPage);
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
        leftContainer = new QWidget(UserInfoPage);
        leftContainer->setObjectName(QString::fromUtf8("leftContainer"));
        leftContainer->setGeometry(QRect(0, 60, 411, 540));
        leftContainer->setStyleSheet(QString::fromUtf8("background-color: rgb(245, 247, 250);"));
        inputCard = new QWidget(leftContainer);
        inputCard->setObjectName(QString::fromUtf8("inputCard"));
        inputCard->setGeometry(QRect(1, 0, 411, 541));
        inputCard->setStyleSheet(QString::fromUtf8("background-color: rgb(255, 255, 255);\n"
"border: 1px solid rgb(230, 230, 230);\n"
"border-radius: 12px;"));
        iconLabel = new QLabel(inputCard);
        iconLabel->setObjectName(QString::fromUtf8("iconLabel"));
        iconLabel->setGeometry(QRect(0, 30, 432, 60));
        iconLabel->setStyleSheet(QString::fromUtf8("font-size: 50px;\n"
"color: rgb(24, 144, 255);\n"
"background: transparent;\n"
"border: none;"));
        iconLabel->setAlignment(Qt::AlignCenter);
        descLabel = new QLabel(inputCard);
        descLabel->setObjectName(QString::fromUtf8("descLabel"));
        descLabel->setGeometry(QRect(20, 100, 392, 30));
        descLabel->setStyleSheet(QString::fromUtf8("font-size: 16px;\n"
"color: rgb(100, 100, 100);\n"
"background: transparent;\n"
"border: none;"));
        descLabel->setAlignment(Qt::AlignCenter);
        nameLabel = new QLabel(inputCard);
        nameLabel->setObjectName(QString::fromUtf8("nameLabel"));
        nameLabel->setGeometry(QRect(40, 160, 80, 40));
        nameLabel->setStyleSheet(QString::fromUtf8("font-size: 16px;\n"
"color: rgb(80, 80, 80);\n"
"background: transparent;\n"
"border: none;"));
        nameLabel->setAlignment(Qt::AlignVCenter);
        nameLineEdit = new QLineEdit(inputCard);
        nameLineEdit->setObjectName(QString::fromUtf8("nameLineEdit"));
        nameLineEdit->setGeometry(QRect(120, 160, 272, 40));
        nameLineEdit->setStyleSheet(QString::fromUtf8("QLineEdit {\n"
"    border: 2px solid rgb(230, 230, 230);\n"
"    border-radius: 8px;\n"
"    padding: 8px 12px;\n"
"    font-size: 16px;\n"
"    background-color: rgb(250, 250, 250);\n"
"}\n"
"QLineEdit:focus {\n"
"    border-color: rgb(24, 144, 255);\n"
"    background-color: white;\n"
"}"));
        nameLineEdit->setMaxLength(20);
        sexLabel = new QLabel(inputCard);
        sexLabel->setObjectName(QString::fromUtf8("sexLabel"));
        sexLabel->setGeometry(QRect(40, 220, 80, 40));
        sexLabel->setStyleSheet(QString::fromUtf8("font-size: 16px;\n"
"color: rgb(80, 80, 80);\n"
"background: transparent;\n"
"border: none;"));
        sexLabel->setAlignment(Qt::AlignVCenter);
        maleRadioBtn = new QRadioButton(inputCard);
        maleRadioBtn->setObjectName(QString::fromUtf8("maleRadioBtn"));
        maleRadioBtn->setGeometry(QRect(120, 220, 60, 40));
        maleRadioBtn->setChecked(true);
        femaleRadioBtn = new QRadioButton(inputCard);
        femaleRadioBtn->setObjectName(QString::fromUtf8("femaleRadioBtn"));
        femaleRadioBtn->setGeometry(QRect(190, 220, 60, 40));
        phoneLabel = new QLabel(inputCard);
        phoneLabel->setObjectName(QString::fromUtf8("phoneLabel"));
        phoneLabel->setGeometry(QRect(40, 280, 80, 40));
        phoneLabel->setStyleSheet(QString::fromUtf8("font-size: 16px;\n"
"color: rgb(80, 80, 80);\n"
"background: transparent;\n"
"border: none;"));
        phoneLabel->setAlignment(Qt::AlignVCenter);
        phoneLineEdit = new QLineEdit(inputCard);
        phoneLineEdit->setObjectName(QString::fromUtf8("phoneLineEdit"));
        phoneLineEdit->setGeometry(QRect(120, 280, 272, 40));
        phoneLineEdit->setStyleSheet(QString::fromUtf8("QLineEdit {\n"
"    border: 2px solid rgb(230, 230, 230);\n"
"    border-radius: 8px;\n"
"    padding: 8px 12px;\n"
"    font-size: 16px;\n"
"    background-color: rgb(250, 250, 250);\n"
"}\n"
"QLineEdit:focus {\n"
"    border-color: rgb(24, 144, 255);\n"
"    background-color: white;\n"
"}"));
        phoneLineEdit->setMaxLength(11);
        addressLabel = new QLabel(inputCard);
        addressLabel->setObjectName(QString::fromUtf8("addressLabel"));
        addressLabel->setGeometry(QRect(40, 340, 80, 40));
        addressLabel->setStyleSheet(QString::fromUtf8("font-size: 16px;\n"
"color: rgb(80, 80, 80);\n"
"background: transparent;\n"
"border: none;"));
        addressLabel->setAlignment(Qt::AlignVCenter);
        addressLineEdit = new QLineEdit(inputCard);
        addressLineEdit->setObjectName(QString::fromUtf8("addressLineEdit"));
        addressLineEdit->setGeometry(QRect(120, 340, 272, 40));
        addressLineEdit->setStyleSheet(QString::fromUtf8("QLineEdit {\n"
"    border: 2px solid rgb(230, 230, 230);\n"
"    border-radius: 8px;\n"
"    padding: 8px 12px;\n"
"    font-size: 16px;\n"
"    background-color: rgb(250, 250, 250);\n"
"}\n"
"QLineEdit:focus {\n"
"    border-color: rgb(24, 144, 255);\n"
"    background-color: white;\n"
"}"));
        addressLineEdit->setMaxLength(100);
        nextBtn = new QPushButton(inputCard);
        nextBtn->setObjectName(QString::fromUtf8("nextBtn"));
        nextBtn->setGeometry(QRect(141, 410, 150, 50));
        nextBtn->setStyleSheet(QString::fromUtf8("QPushButton {\n"
"    background-color: rgb(24, 144, 255);\n"
"    border: none;\n"
"    border-radius: 8px;\n"
"    font-size: 18px;\n"
"    font-weight: bold;\n"
"    color: white;\n"
"}\n"
"QPushButton:pressed {\n"
"    background-color: rgb(9, 109, 217);\n"
"}\n"
"QPushButton:disabled {\n"
"    background-color: rgb(200, 200, 200);\n"
"    color: rgb(150, 150, 150);\n"
"}"));
        statusLabel = new QLabel(leftContainer);
        statusLabel->setObjectName(QString::fromUtf8("statusLabel"));
        statusLabel->setGeometry(QRect(40, 490, 432, 30));
        statusLabel->setStyleSheet(QString::fromUtf8("font-size: 15px;\n"
"color: rgb(100, 100, 100);\n"
"background: transparent;"));
        statusLabel->setAlignment(Qt::AlignCenter);
        rightContainer = new QWidget(UserInfoPage);
        rightContainer->setObjectName(QString::fromUtf8("rightContainer"));
        rightContainer->setGeometry(QRect(403, 60, 621, 540));
        rightContainer->setStyleSheet(QString::fromUtf8("background-color: rgb(245, 247, 250);"));

        retranslateUi(UserInfoPage);

        QMetaObject::connectSlotsByName(UserInfoPage);
    } // setupUi

    void retranslateUi(QWidget *UserInfoPage)
    {
        UserInfoPage->setWindowTitle(QApplication::translate("UserInfoPage", "\347\224\250\346\210\267\344\277\241\346\201\257\346\263\250\345\206\214", nullptr));
        backBtn->setText(QApplication::translate("UserInfoPage", "\342\206\220 \350\277\224\345\233\236", nullptr));
        titleLabel->setText(QApplication::translate("UserInfoPage", "\347\224\250\346\210\267\346\263\250\345\206\214 - \345\237\272\346\234\254\344\277\241\346\201\257", nullptr));
        iconLabel->setText(QString());
        descLabel->setText(QApplication::translate("UserInfoPage", "\350\257\267\345\241\253\345\206\231\346\202\250\347\232\204\345\237\272\346\234\254\344\277\241\346\201\257", nullptr));
        nameLabel->setText(QApplication::translate("UserInfoPage", "\345\247\223\345\220\215\357\274\232", nullptr));
        nameLineEdit->setPlaceholderText(QApplication::translate("UserInfoPage", "\350\257\267\350\276\223\345\205\245\346\202\250\347\232\204\345\247\223\345\220\215", nullptr));
        sexLabel->setText(QApplication::translate("UserInfoPage", "\346\200\247\345\210\253\357\274\232", nullptr));
        maleRadioBtn->setText(QApplication::translate("UserInfoPage", "\347\224\267", nullptr));
        femaleRadioBtn->setText(QApplication::translate("UserInfoPage", "\345\245\263", nullptr));
        phoneLabel->setText(QApplication::translate("UserInfoPage", "\346\211\213\346\234\272\345\217\267\357\274\232", nullptr));
        phoneLineEdit->setPlaceholderText(QApplication::translate("UserInfoPage", "\350\257\267\350\276\223\345\205\24511\344\275\215\346\211\213\346\234\272\345\217\267\347\240\201", nullptr));
        addressLabel->setText(QApplication::translate("UserInfoPage", "\345\234\260\345\235\200\357\274\232", nullptr));
        addressLineEdit->setPlaceholderText(QApplication::translate("UserInfoPage", "\350\257\267\350\276\223\345\205\245\346\202\250\347\232\204\345\234\260\345\235\200", nullptr));
        nextBtn->setText(QApplication::translate("UserInfoPage", "\344\270\213\344\270\200\346\255\245", nullptr));
        statusLabel->setText(QString());
    } // retranslateUi

};

namespace Ui {
    class UserInfoPage: public Ui_UserInfoPage {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_USERINFOPAGE_H
