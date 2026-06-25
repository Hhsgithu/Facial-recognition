/********************************************************************************
** Form generated from reading UI file 'registrationcompletepage.ui'
**
** Created by: Qt User Interface Compiler version 5.12.9
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_REGISTRATIONCOMPLETEPAGE_H
#define UI_REGISTRATIONCOMPLETEPAGE_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_RegistrationCompletePage
{
public:
    QVBoxLayout *verticalLayout;
    QWidget *headerWidget;
    QHBoxLayout *horizontalLayout;
    QPushButton *backBtn;
    QSpacerItem *horizontalSpacer;
    QLabel *pageTitleLabel;
    QSpacerItem *horizontalSpacer_2;
    QLabel *userNameLabel;
    QWidget *progressWidget;
    QHBoxLayout *horizontalLayout_2;
    QLabel *progressLabel;
    QWidget *contentWidget;
    QVBoxLayout *verticalLayout_2;
    QSpacerItem *verticalSpacer;
    QLabel *iconLabel;
    QLabel *titleLabel;
    QLabel *statusLabel;
    QSpacerItem *verticalSpacer_2;
    QHBoxLayout *horizontalLayout_3;
    QSpacerItem *horizontalSpacer_3;
    QPushButton *completeBtn;
    QPushButton *retryBtn;
    QSpacerItem *horizontalSpacer_4;
    QSpacerItem *verticalSpacer_3;

    void setupUi(QWidget *RegistrationCompletePage)
    {
        if (RegistrationCompletePage->objectName().isEmpty())
            RegistrationCompletePage->setObjectName(QString::fromUtf8("RegistrationCompletePage"));
        RegistrationCompletePage->resize(1024, 600);
        RegistrationCompletePage->setStyleSheet(QString::fromUtf8("QWidget {\n"
"    background-color: rgb(245, 247, 250);\n"
"}\n"
"\n"
"QPushButton {\n"
"    background-color: rgb(24, 144, 255);\n"
"    color: white;\n"
"    border: none;\n"
"    border-radius: 4px;\n"
"    font-size: 16px;\n"
"    padding: 10px 20px;\n"
"}\n"
"\n"
"QPushButton:hover {\n"
"    background-color: rgb(64, 169, 255);\n"
"}\n"
"\n"
"QPushButton:pressed {\n"
"    background-color: rgb(9, 109, 217);\n"
"}\n"
"\n"
"QPushButton:disabled {\n"
"    background-color: rgb(200, 200, 200);\n"
"    color: rgb(150, 150, 150);\n"
"}\n"
"\n"
"QPushButton#retryBtn {\n"
"    background-color: rgb(255, 193, 7);\n"
"}\n"
"\n"
"QPushButton#retryBtn:hover {\n"
"    background-color: rgb(255, 213, 79);\n"
"}\n"
"\n"
"QPushButton#retryBtn:pressed {\n"
"    background-color: rgb(255, 160, 0);\n"
"}\n"
"\n"
"QLabel {\n"
"    color: rgb(50, 50, 50);\n"
"}"));
        verticalLayout = new QVBoxLayout(RegistrationCompletePage);
        verticalLayout->setSpacing(0);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        verticalLayout->setContentsMargins(0, 0, 0, 0);
        headerWidget = new QWidget(RegistrationCompletePage);
        headerWidget->setObjectName(QString::fromUtf8("headerWidget"));
        headerWidget->setMinimumSize(QSize(0, 60));
        headerWidget->setMaximumSize(QSize(16777215, 60));
        headerWidget->setStyleSheet(QString::fromUtf8("QWidget#headerWidget {\n"
"    background-color: white;\n"
"    border-bottom: 1px solid rgb(230, 230, 230);\n"
"}"));
        horizontalLayout = new QHBoxLayout(headerWidget);
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        horizontalLayout->setContentsMargins(20, 10, 20, 10);
        backBtn = new QPushButton(headerWidget);
        backBtn->setObjectName(QString::fromUtf8("backBtn"));
        backBtn->setMinimumSize(QSize(80, 40));
        backBtn->setMaximumSize(QSize(80, 40));

        horizontalLayout->addWidget(backBtn);

        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer);

        pageTitleLabel = new QLabel(headerWidget);
        pageTitleLabel->setObjectName(QString::fromUtf8("pageTitleLabel"));
        QFont font;
        font.setPointSize(18);
        font.setBold(true);
        font.setWeight(75);
        pageTitleLabel->setFont(font);
        pageTitleLabel->setAlignment(Qt::AlignCenter);

        horizontalLayout->addWidget(pageTitleLabel);

        horizontalSpacer_2 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer_2);

        userNameLabel = new QLabel(headerWidget);
        userNameLabel->setObjectName(QString::fromUtf8("userNameLabel"));
        QFont font1;
        font1.setPointSize(12);
        userNameLabel->setFont(font1);

        horizontalLayout->addWidget(userNameLabel);


        verticalLayout->addWidget(headerWidget);

        progressWidget = new QWidget(RegistrationCompletePage);
        progressWidget->setObjectName(QString::fromUtf8("progressWidget"));
        progressWidget->setMinimumSize(QSize(0, 30));
        progressWidget->setMaximumSize(QSize(16777215, 30));
        horizontalLayout_2 = new QHBoxLayout(progressWidget);
        horizontalLayout_2->setObjectName(QString::fromUtf8("horizontalLayout_2"));
        progressLabel = new QLabel(progressWidget);
        progressLabel->setObjectName(QString::fromUtf8("progressLabel"));
        QFont font2;
        font2.setPointSize(11);
        progressLabel->setFont(font2);
        progressLabel->setStyleSheet(QString::fromUtf8("color: rgb(100, 100, 100);"));
        progressLabel->setAlignment(Qt::AlignCenter);

        horizontalLayout_2->addWidget(progressLabel);


        verticalLayout->addWidget(progressWidget);

        contentWidget = new QWidget(RegistrationCompletePage);
        contentWidget->setObjectName(QString::fromUtf8("contentWidget"));
        verticalLayout_2 = new QVBoxLayout(contentWidget);
        verticalLayout_2->setSpacing(30);
        verticalLayout_2->setObjectName(QString::fromUtf8("verticalLayout_2"));
        verticalLayout_2->setContentsMargins(100, 60, 100, 60);
        verticalSpacer = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout_2->addItem(verticalSpacer);

        iconLabel = new QLabel(contentWidget);
        iconLabel->setObjectName(QString::fromUtf8("iconLabel"));
        QFont font3;
        font3.setPointSize(80);
        iconLabel->setFont(font3);
        iconLabel->setStyleSheet(QString::fromUtf8("color: rgb(24, 144, 255);"));
        iconLabel->setAlignment(Qt::AlignCenter);

        verticalLayout_2->addWidget(iconLabel);

        titleLabel = new QLabel(contentWidget);
        titleLabel->setObjectName(QString::fromUtf8("titleLabel"));
        QFont font4;
        font4.setPointSize(20);
        font4.setBold(true);
        font4.setWeight(75);
        titleLabel->setFont(font4);
        titleLabel->setAlignment(Qt::AlignCenter);

        verticalLayout_2->addWidget(titleLabel);

        statusLabel = new QLabel(contentWidget);
        statusLabel->setObjectName(QString::fromUtf8("statusLabel"));
        QFont font5;
        font5.setPointSize(14);
        statusLabel->setFont(font5);
        statusLabel->setStyleSheet(QString::fromUtf8("color: rgb(100, 100, 100);"));
        statusLabel->setAlignment(Qt::AlignCenter);
        statusLabel->setWordWrap(true);

        verticalLayout_2->addWidget(statusLabel);

        verticalSpacer_2 = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout_2->addItem(verticalSpacer_2);

        horizontalLayout_3 = new QHBoxLayout();
        horizontalLayout_3->setObjectName(QString::fromUtf8("horizontalLayout_3"));
        horizontalSpacer_3 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_3->addItem(horizontalSpacer_3);

        completeBtn = new QPushButton(contentWidget);
        completeBtn->setObjectName(QString::fromUtf8("completeBtn"));
        completeBtn->setMinimumSize(QSize(200, 50));
        completeBtn->setFont(font5);

        horizontalLayout_3->addWidget(completeBtn);

        retryBtn = new QPushButton(contentWidget);
        retryBtn->setObjectName(QString::fromUtf8("retryBtn"));
        retryBtn->setMinimumSize(QSize(200, 50));
        retryBtn->setFont(font5);

        horizontalLayout_3->addWidget(retryBtn);

        horizontalSpacer_4 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_3->addItem(horizontalSpacer_4);


        verticalLayout_2->addLayout(horizontalLayout_3);

        verticalSpacer_3 = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout_2->addItem(verticalSpacer_3);


        verticalLayout->addWidget(contentWidget);


        retranslateUi(RegistrationCompletePage);

        QMetaObject::connectSlotsByName(RegistrationCompletePage);
    } // setupUi

    void retranslateUi(QWidget *RegistrationCompletePage)
    {
        RegistrationCompletePage->setWindowTitle(QApplication::translate("RegistrationCompletePage", "\346\263\250\345\206\214\345\256\214\346\210\220", nullptr));
        backBtn->setText(QApplication::translate("RegistrationCompletePage", "\350\277\224\345\233\236", nullptr));
        pageTitleLabel->setText(QApplication::translate("RegistrationCompletePage", "\346\263\250\345\206\214\345\256\214\346\210\220", nullptr));
        userNameLabel->setText(QApplication::translate("RegistrationCompletePage", "\347\224\250\346\210\267\357\274\232", nullptr));
        progressLabel->setText(QApplication::translate("RegistrationCompletePage", "\346\255\245\351\252\244 4/4: \345\256\214\346\210\220\346\263\250\345\206\214", nullptr));
        iconLabel->setText(QApplication::translate("RegistrationCompletePage", "\342\234\223", nullptr));
        titleLabel->setText(QApplication::translate("RegistrationCompletePage", "\344\277\241\346\201\257\351\207\207\351\233\206\345\256\214\346\210\220", nullptr));
        statusLabel->setText(QApplication::translate("RegistrationCompletePage", "\350\257\267\347\202\271\345\207\273\344\270\213\346\226\271\346\214\211\351\222\256\345\256\214\346\210\220\346\263\250\345\206\214", nullptr));
        completeBtn->setText(QApplication::translate("RegistrationCompletePage", "\345\256\214\346\210\220\346\263\250\345\206\214", nullptr));
        retryBtn->setText(QApplication::translate("RegistrationCompletePage", "\351\207\215\350\257\225", nullptr));
    } // retranslateUi

};

namespace Ui {
    class RegistrationCompletePage: public Ui_RegistrationCompletePage {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_REGISTRATIONCOMPLETEPAGE_H
