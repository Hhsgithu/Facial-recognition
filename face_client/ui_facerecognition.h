/********************************************************************************
** Form generated from reading UI file 'facerecognition.ui'
**
** Created by: Qt User Interface Compiler version 5.12.9
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_FACERECOGNITION_H
#define UI_FACERECOGNITION_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_FaceRecognition
{
public:
    QWidget *headerWidget;
    QPushButton *backBtn;
    QLabel *titleLabel;
    QLabel *statusIndicator;
    QWidget *contentWidget;
    QWidget *cameraSection;
    QLabel *cameraTitle;
    QLabel *videoLabel;
    QLabel *faceStatusLabel;
    QLabel *tipLabel;
    QWidget *infoSection;
    QLabel *avatarLabel;
    QLabel *resultStatusLabel;
    QWidget *infoCard;
    QLabel *idTitleLabel;
    QLabel *idValueLabel;
    QLabel *nameTitleLabel;
    QLabel *nameValueLabel;
    QLabel *deptTitleLabel;
    QLabel *deptValueLabel;
    QLabel *timeTitleLabel;
    QLabel *timeValueLabel;

    void setupUi(QWidget *FaceRecognition)
    {
        if (FaceRecognition->objectName().isEmpty())
            FaceRecognition->setObjectName(QString::fromUtf8("FaceRecognition"));
        FaceRecognition->resize(1024, 600);
        FaceRecognition->setAutoFillBackground(true);
        FaceRecognition->setStyleSheet(QString::fromUtf8("QWidget#FaceRecognition {\n"
"    background-color: rgb(245, 247, 250);\n"
"}"));
        headerWidget = new QWidget(FaceRecognition);
        headerWidget->setObjectName(QString::fromUtf8("headerWidget"));
        headerWidget->setGeometry(QRect(0, 0, 1024, 60));
        headerWidget->setAutoFillBackground(true);
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
        statusIndicator = new QLabel(headerWidget);
        statusIndicator->setObjectName(QString::fromUtf8("statusIndicator"));
        statusIndicator->setGeometry(QRect(900, 18, 100, 24));
        statusIndicator->setStyleSheet(QString::fromUtf8("font-size: 12px;\n"
"color: rgb(82, 196, 26);\n"
"background: transparent;"));
        statusIndicator->setAlignment(Qt::AlignRight|Qt::AlignVCenter);
        contentWidget = new QWidget(FaceRecognition);
        contentWidget->setObjectName(QString::fromUtf8("contentWidget"));
        contentWidget->setGeometry(QRect(0, 60, 1024, 540));
        contentWidget->setAutoFillBackground(true);
        contentWidget->setStyleSheet(QString::fromUtf8("background-color: rgb(245, 247, 250);"));
        cameraSection = new QWidget(contentWidget);
        cameraSection->setObjectName(QString::fromUtf8("cameraSection"));
        cameraSection->setGeometry(QRect(30, 20, 480, 500));
        cameraSection->setAutoFillBackground(true);
        cameraSection->setStyleSheet(QString::fromUtf8("background-color: rgb(255, 255, 255);\n"
"border-radius: 16px;"));
        cameraTitle = new QLabel(cameraSection);
        cameraTitle->setObjectName(QString::fromUtf8("cameraTitle"));
        cameraTitle->setGeometry(QRect(20, 15, 200, 30));
        cameraTitle->setStyleSheet(QString::fromUtf8("font-size: 16px;\n"
"font-weight: bold;\n"
"color: rgb(50, 50, 50);\n"
"background: transparent;"));
        videoLabel = new QLabel(cameraSection);
        videoLabel->setObjectName(QString::fromUtf8("videoLabel"));
        videoLabel->setGeometry(QRect(20, 55, 440, 330));
        videoLabel->setAutoFillBackground(true);
        videoLabel->setStyleSheet(QString::fromUtf8("background-color: rgb(30, 30, 30);\n"
"border-radius: 12px;"));
        videoLabel->setAlignment(Qt::AlignCenter);
        faceStatusLabel = new QLabel(cameraSection);
        faceStatusLabel->setObjectName(QString::fromUtf8("faceStatusLabel"));
        faceStatusLabel->setGeometry(QRect(20, 400, 440, 40));
        faceStatusLabel->setStyleSheet(QString::fromUtf8("font-size: 16px;\n"
"color: rgb(250, 173, 20);\n"
"background: transparent;"));
        faceStatusLabel->setAlignment(Qt::AlignCenter);
        tipLabel = new QLabel(cameraSection);
        tipLabel->setObjectName(QString::fromUtf8("tipLabel"));
        tipLabel->setGeometry(QRect(20, 450, 440, 30));
        tipLabel->setStyleSheet(QString::fromUtf8("font-size: 12px;\n"
"color: rgb(150, 150, 150);\n"
"background: transparent;"));
        tipLabel->setAlignment(Qt::AlignCenter);
        infoSection = new QWidget(contentWidget);
        infoSection->setObjectName(QString::fromUtf8("infoSection"));
        infoSection->setGeometry(QRect(530, 20, 464, 500));
        infoSection->setAutoFillBackground(true);
        infoSection->setStyleSheet(QString::fromUtf8("background-color: rgb(255, 255, 255);\n"
"border-radius: 16px;"));
        avatarLabel = new QLabel(infoSection);
        avatarLabel->setObjectName(QString::fromUtf8("avatarLabel"));
        avatarLabel->setGeometry(QRect(172, 30, 120, 120));
        avatarLabel->setStyleSheet(QString::fromUtf8("background-color: rgb(240, 240, 240);\n"
"border-radius: 60px;"));
        avatarLabel->setAlignment(Qt::AlignCenter);
        avatarLabel->setScaledContents(true);
        resultStatusLabel = new QLabel(infoSection);
        resultStatusLabel->setObjectName(QString::fromUtf8("resultStatusLabel"));
        resultStatusLabel->setGeometry(QRect(0, 165, 464, 35));
        resultStatusLabel->setStyleSheet(QString::fromUtf8("font-size: 18px;\n"
"font-weight: bold;\n"
"color: rgb(150, 150, 150);\n"
"background: transparent;"));
        resultStatusLabel->setAlignment(Qt::AlignCenter);
        infoCard = new QWidget(infoSection);
        infoCard->setObjectName(QString::fromUtf8("infoCard"));
        infoCard->setGeometry(QRect(25, 215, 414, 260));
        infoCard->setStyleSheet(QString::fromUtf8("background-color: rgb(250, 250, 250);\n"
"border-radius: 12px;"));
        idTitleLabel = new QLabel(infoCard);
        idTitleLabel->setObjectName(QString::fromUtf8("idTitleLabel"));
        idTitleLabel->setGeometry(QRect(30, 25, 60, 30));
        idTitleLabel->setStyleSheet(QString::fromUtf8("font-size: 14px;\n"
"color: rgb(120, 120, 120);\n"
"background: transparent;"));
        idValueLabel = new QLabel(infoCard);
        idValueLabel->setObjectName(QString::fromUtf8("idValueLabel"));
        idValueLabel->setGeometry(QRect(120, 20, 270, 40));
        idValueLabel->setStyleSheet(QString::fromUtf8("font-size: 18px;\n"
"font-weight: bold;\n"
"color: rgb(50, 50, 50);\n"
"background-color: rgb(255, 255, 255);\n"
"border-radius: 8px;\n"
"padding-left: 15px;"));
        idValueLabel->setAlignment(Qt::AlignLeft|Qt::AlignVCenter);
        nameTitleLabel = new QLabel(infoCard);
        nameTitleLabel->setObjectName(QString::fromUtf8("nameTitleLabel"));
        nameTitleLabel->setGeometry(QRect(30, 80, 60, 30));
        nameTitleLabel->setStyleSheet(QString::fromUtf8("font-size: 14px;\n"
"color: rgb(120, 120, 120);\n"
"background: transparent;"));
        nameValueLabel = new QLabel(infoCard);
        nameValueLabel->setObjectName(QString::fromUtf8("nameValueLabel"));
        nameValueLabel->setGeometry(QRect(120, 75, 270, 40));
        nameValueLabel->setStyleSheet(QString::fromUtf8("font-size: 18px;\n"
"font-weight: bold;\n"
"color: rgb(50, 50, 50);\n"
"background-color: rgb(255, 255, 255);\n"
"border-radius: 8px;\n"
"padding-left: 15px;"));
        nameValueLabel->setAlignment(Qt::AlignLeft|Qt::AlignVCenter);
        deptTitleLabel = new QLabel(infoCard);
        deptTitleLabel->setObjectName(QString::fromUtf8("deptTitleLabel"));
        deptTitleLabel->setGeometry(QRect(30, 135, 60, 30));
        deptTitleLabel->setStyleSheet(QString::fromUtf8("font-size: 14px;\n"
"color: rgb(120, 120, 120);\n"
"background: transparent;"));
        deptValueLabel = new QLabel(infoCard);
        deptValueLabel->setObjectName(QString::fromUtf8("deptValueLabel"));
        deptValueLabel->setGeometry(QRect(120, 130, 270, 40));
        deptValueLabel->setStyleSheet(QString::fromUtf8("font-size: 18px;\n"
"font-weight: bold;\n"
"color: rgb(50, 50, 50);\n"
"background-color: rgb(255, 255, 255);\n"
"border-radius: 8px;\n"
"padding-left: 15px;"));
        deptValueLabel->setAlignment(Qt::AlignLeft|Qt::AlignVCenter);
        timeTitleLabel = new QLabel(infoCard);
        timeTitleLabel->setObjectName(QString::fromUtf8("timeTitleLabel"));
        timeTitleLabel->setGeometry(QRect(30, 190, 60, 30));
        timeTitleLabel->setStyleSheet(QString::fromUtf8("font-size: 14px;\n"
"color: rgb(120, 120, 120);\n"
"background: transparent;"));
        timeValueLabel = new QLabel(infoCard);
        timeValueLabel->setObjectName(QString::fromUtf8("timeValueLabel"));
        timeValueLabel->setGeometry(QRect(120, 185, 270, 40));
        timeValueLabel->setStyleSheet(QString::fromUtf8("font-size: 18px;\n"
"font-weight: bold;\n"
"color: rgb(50, 50, 50);\n"
"background-color: rgb(255, 255, 255);\n"
"border-radius: 8px;\n"
"padding-left: 15px;"));
        timeValueLabel->setAlignment(Qt::AlignLeft|Qt::AlignVCenter);

        retranslateUi(FaceRecognition);

        QMetaObject::connectSlotsByName(FaceRecognition);
    } // setupUi

    void retranslateUi(QWidget *FaceRecognition)
    {
        FaceRecognition->setWindowTitle(QApplication::translate("FaceRecognition", "\344\272\272\350\204\270\350\257\206\345\210\253", nullptr));
        backBtn->setText(QApplication::translate("FaceRecognition", "\342\206\220 \350\277\224\345\233\236", nullptr));
        titleLabel->setText(QApplication::translate("FaceRecognition", "\344\272\272\350\204\270\350\257\206\345\210\253", nullptr));
        statusIndicator->setText(QApplication::translate("FaceRecognition", "\342\227\217 \345\267\262\350\277\236\346\216\245", nullptr));
        cameraTitle->setText(QApplication::translate("FaceRecognition", "\345\256\236\346\227\266\347\224\273\351\235\242", nullptr));
        videoLabel->setText(QApplication::translate("FaceRecognition", "\346\255\243\345\234\250\345\210\235\345\247\213\345\214\226\346\221\204\345\203\217\345\244\264...", nullptr));
        faceStatusLabel->setText(QApplication::translate("FaceRecognition", "\350\257\267\345\260\206\351\235\242\351\203\250\345\257\271\345\207\206\346\221\204\345\203\217\345\244\264", nullptr));
        tipLabel->setText(QApplication::translate("FaceRecognition", "\346\243\200\346\265\213\345\210\260\344\272\272\350\204\270\345\220\216\345\260\206\350\207\252\345\212\250\350\277\233\350\241\214\350\257\206\345\210\253", nullptr));
        avatarLabel->setText(QString());
        resultStatusLabel->setText(QApplication::translate("FaceRecognition", "\347\255\211\345\276\205\350\257\206\345\210\253...", nullptr));
        idTitleLabel->setText(QApplication::translate("FaceRecognition", "\345\267\245\345\217\267", nullptr));
        idValueLabel->setText(QApplication::translate("FaceRecognition", "--", nullptr));
        nameTitleLabel->setText(QApplication::translate("FaceRecognition", "\345\247\223\345\220\215", nullptr));
        nameValueLabel->setText(QApplication::translate("FaceRecognition", "--", nullptr));
        deptTitleLabel->setText(QApplication::translate("FaceRecognition", "\351\203\250\351\227\250", nullptr));
        deptValueLabel->setText(QApplication::translate("FaceRecognition", "--", nullptr));
        timeTitleLabel->setText(QApplication::translate("FaceRecognition", "\346\227\266\351\227\264", nullptr));
        timeValueLabel->setText(QApplication::translate("FaceRecognition", "--:--:--", nullptr));
    } // retranslateUi

};

namespace Ui {
    class FaceRecognition: public Ui_FaceRecognition {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_FACERECOGNITION_H
