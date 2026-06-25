/********************************************************************************
** Form generated from reading UI file 'biometriccollectionpage.ui'
**
** Created by: Qt User Interface Compiler version 5.12.9
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_BIOMETRICCOLLECTIONPAGE_H
#define UI_BIOMETRICCOLLECTIONPAGE_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_BiometricCollectionPage
{
public:
    QWidget *headerWidget;
    QLabel *titleLabel;
    QPushButton *backBtn;
    QLabel *userNameLabel;
    QLabel *progressLabel;
    QWidget *faceCollectionWidget;
    QWidget *previewContainer;
    QLabel *previewTitleLabel;
    QLabel *previewPlaceholder;
    QWidget *controlContainer;
    QLabel *faceIconLabel;
    QLabel *faceDescLabel;
    QPushButton *startFaceCaptureBtn;
    QLabel *faceStatusLabel;
    QLabel *faceTipLabel;
    QWidget *fingerprintCollectionWidget;
    QLabel *fingerprintIconLabel;
    QLabel *fingerprintDescLabel;
    QPushButton *startFingerprintBtn;
    QLabel *fingerprintStatusLabel;
    QWidget *passwordSettingWidget;
    QLabel *passwordIconLabel;
    QLabel *passwordDescLabel;
    QLineEdit *passwordLineEdit;
    QLineEdit *confirmPasswordLineEdit;
    QPushButton *confirmPasswordBtn;
    QLabel *passwordStatusLabel;
    QWidget *completionWidget;
    QLabel *completeIconLabel;
    QLabel *completeDescLabel;
    QPushButton *completeBtn;

    void setupUi(QWidget *BiometricCollectionPage)
    {
        if (BiometricCollectionPage->objectName().isEmpty())
            BiometricCollectionPage->setObjectName(QString::fromUtf8("BiometricCollectionPage"));
        BiometricCollectionPage->resize(1024, 600);
        BiometricCollectionPage->setAutoFillBackground(false);
        BiometricCollectionPage->setStyleSheet(QString::fromUtf8("QWidget#BiometricCollectionPage {\n"
"    background-color: rgb(245, 247, 250);\n"
"}"));
        headerWidget = new QWidget(BiometricCollectionPage);
        headerWidget->setObjectName(QString::fromUtf8("headerWidget"));
        headerWidget->setGeometry(QRect(0, 0, 1024, 60));
        headerWidget->setAutoFillBackground(false);
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
        progressLabel = new QLabel(BiometricCollectionPage);
        progressLabel->setObjectName(QString::fromUtf8("progressLabel"));
        progressLabel->setGeometry(QRect(0, 70, 1024, 30));
        progressLabel->setStyleSheet(QString::fromUtf8("font-size: 16px;\n"
"color: rgb(24, 144, 255);\n"
"background: transparent;"));
        progressLabel->setAlignment(Qt::AlignCenter);
        faceCollectionWidget = new QWidget(BiometricCollectionPage);
        faceCollectionWidget->setObjectName(QString::fromUtf8("faceCollectionWidget"));
        faceCollectionWidget->setGeometry(QRect(50, 120, 924, 450));
        faceCollectionWidget->setAutoFillBackground(false);
        faceCollectionWidget->setStyleSheet(QString::fromUtf8("background-color: rgb(255, 255, 255);\n"
"border-radius: 16px;"));
        previewContainer = new QWidget(faceCollectionWidget);
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
        controlContainer = new QWidget(faceCollectionWidget);
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
        faceDescLabel = new QLabel(controlContainer);
        faceDescLabel->setObjectName(QString::fromUtf8("faceDescLabel"));
        faceDescLabel->setGeometry(QRect(20, 110, 304, 80));
        faceDescLabel->setStyleSheet(QString::fromUtf8("font-size: 15px;\n"
"color: rgb(100, 100, 100);\n"
"background: transparent;\n"
"line-height: 1.6;"));
        faceDescLabel->setAlignment(Qt::AlignCenter);
        faceDescLabel->setWordWrap(true);
        startFaceCaptureBtn = new QPushButton(controlContainer);
        startFaceCaptureBtn->setObjectName(QString::fromUtf8("startFaceCaptureBtn"));
        startFaceCaptureBtn->setGeometry(QRect(72, 210, 200, 50));
        startFaceCaptureBtn->setStyleSheet(QString::fromUtf8("QPushButton {\n"
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
        faceStatusLabel = new QLabel(controlContainer);
        faceStatusLabel->setObjectName(QString::fromUtf8("faceStatusLabel"));
        faceStatusLabel->setGeometry(QRect(20, 280, 304, 40));
        faceStatusLabel->setStyleSheet(QString::fromUtf8("font-size: 14px;\n"
"color: rgb(100, 100, 100);\n"
"background: transparent;"));
        faceStatusLabel->setAlignment(Qt::AlignCenter);
        faceStatusLabel->setWordWrap(true);
        faceTipLabel = new QLabel(controlContainer);
        faceTipLabel->setObjectName(QString::fromUtf8("faceTipLabel"));
        faceTipLabel->setGeometry(QRect(20, 330, 304, 50));
        faceTipLabel->setStyleSheet(QString::fromUtf8("font-size: 12px;\n"
"color: rgb(150, 150, 150);\n"
"background: transparent;"));
        faceTipLabel->setAlignment(Qt::AlignCenter);
        faceTipLabel->setWordWrap(true);
        fingerprintCollectionWidget = new QWidget(BiometricCollectionPage);
        fingerprintCollectionWidget->setObjectName(QString::fromUtf8("fingerprintCollectionWidget"));
        fingerprintCollectionWidget->setGeometry(QRect(262, 120, 500, 400));
        fingerprintCollectionWidget->setAutoFillBackground(false);
        fingerprintCollectionWidget->setStyleSheet(QString::fromUtf8("background-color: rgb(255, 255, 255);\n"
"border-radius: 16px;"));
        fingerprintIconLabel = new QLabel(fingerprintCollectionWidget);
        fingerprintIconLabel->setObjectName(QString::fromUtf8("fingerprintIconLabel"));
        fingerprintIconLabel->setGeometry(QRect(0, 40, 500, 60));
        fingerprintIconLabel->setStyleSheet(QString::fromUtf8("font-size: 48px;\n"
"color: rgb(24, 144, 255);\n"
"background: transparent;"));
        fingerprintIconLabel->setAlignment(Qt::AlignCenter);
        fingerprintDescLabel = new QLabel(fingerprintCollectionWidget);
        fingerprintDescLabel->setObjectName(QString::fromUtf8("fingerprintDescLabel"));
        fingerprintDescLabel->setGeometry(QRect(50, 110, 400, 60));
        fingerprintDescLabel->setStyleSheet(QString::fromUtf8("font-size: 16px;\n"
"color: rgb(100, 100, 100);\n"
"background: transparent;"));
        fingerprintDescLabel->setAlignment(Qt::AlignCenter);
        startFingerprintBtn = new QPushButton(fingerprintCollectionWidget);
        startFingerprintBtn->setObjectName(QString::fromUtf8("startFingerprintBtn"));
        startFingerprintBtn->setGeometry(QRect(175, 190, 150, 40));
        startFingerprintBtn->setStyleSheet(QString::fromUtf8("QPushButton {\n"
"    background-color: rgb(24, 144, 255);\n"
"    color: white;\n"
"    border: none;\n"
"    border-radius: 8px;\n"
"    font-size: 16px;\n"
"    font-weight: bold;\n"
"}\n"
"QPushButton:pressed {\n"
"    background-color: rgb(16, 120, 220);\n"
"}\n"
"QPushButton:disabled {\n"
"    background-color: rgb(200, 200, 200);\n"
"    color: rgb(150, 150, 150);\n"
"}"));
        fingerprintStatusLabel = new QLabel(fingerprintCollectionWidget);
        fingerprintStatusLabel->setObjectName(QString::fromUtf8("fingerprintStatusLabel"));
        fingerprintStatusLabel->setGeometry(QRect(50, 250, 400, 30));
        fingerprintStatusLabel->setStyleSheet(QString::fromUtf8("font-size: 14px;\n"
"color: rgb(100, 100, 100);\n"
"background: transparent;"));
        fingerprintStatusLabel->setAlignment(Qt::AlignCenter);
        passwordSettingWidget = new QWidget(BiometricCollectionPage);
        passwordSettingWidget->setObjectName(QString::fromUtf8("passwordSettingWidget"));
        passwordSettingWidget->setGeometry(QRect(262, 120, 500, 400));
        passwordSettingWidget->setAutoFillBackground(false);
        passwordSettingWidget->setStyleSheet(QString::fromUtf8("background-color: rgb(255, 255, 255);\n"
"border-radius: 16px;"));
        passwordIconLabel = new QLabel(passwordSettingWidget);
        passwordIconLabel->setObjectName(QString::fromUtf8("passwordIconLabel"));
        passwordIconLabel->setGeometry(QRect(0, 40, 500, 60));
        passwordIconLabel->setStyleSheet(QString::fromUtf8("font-size: 48px;\n"
"color: rgb(24, 144, 255);\n"
"background: transparent;"));
        passwordIconLabel->setAlignment(Qt::AlignCenter);
        passwordDescLabel = new QLabel(passwordSettingWidget);
        passwordDescLabel->setObjectName(QString::fromUtf8("passwordDescLabel"));
        passwordDescLabel->setGeometry(QRect(50, 110, 400, 30));
        passwordDescLabel->setStyleSheet(QString::fromUtf8("font-size: 16px;\n"
"color: rgb(100, 100, 100);\n"
"background: transparent;"));
        passwordDescLabel->setAlignment(Qt::AlignCenter);
        passwordLineEdit = new QLineEdit(passwordSettingWidget);
        passwordLineEdit->setObjectName(QString::fromUtf8("passwordLineEdit"));
        passwordLineEdit->setGeometry(QRect(50, 160, 400, 35));
        passwordLineEdit->setStyleSheet(QString::fromUtf8("QLineEdit {\n"
"    border: 1px solid rgb(230, 230, 230);\n"
"    border-radius: 6px;\n"
"    padding: 8px 12px;\n"
"    font-size: 14px;\n"
"    background-color: rgb(250, 250, 250);\n"
"}\n"
"QLineEdit:focus {\n"
"    border-color: rgb(24, 144, 255);\n"
"    background-color: white;\n"
"}"));
        passwordLineEdit->setMaxLength(20);
        passwordLineEdit->setEchoMode(QLineEdit::Password);
        confirmPasswordLineEdit = new QLineEdit(passwordSettingWidget);
        confirmPasswordLineEdit->setObjectName(QString::fromUtf8("confirmPasswordLineEdit"));
        confirmPasswordLineEdit->setGeometry(QRect(50, 210, 400, 35));
        confirmPasswordLineEdit->setStyleSheet(QString::fromUtf8("QLineEdit {\n"
"    border: 1px solid rgb(230, 230, 230);\n"
"    border-radius: 6px;\n"
"    padding: 8px 12px;\n"
"    font-size: 14px;\n"
"    background-color: rgb(250, 250, 250);\n"
"}\n"
"QLineEdit:focus {\n"
"    border-color: rgb(24, 144, 255);\n"
"    background-color: white;\n"
"}"));
        confirmPasswordLineEdit->setMaxLength(20);
        confirmPasswordLineEdit->setEchoMode(QLineEdit::Password);
        confirmPasswordBtn = new QPushButton(passwordSettingWidget);
        confirmPasswordBtn->setObjectName(QString::fromUtf8("confirmPasswordBtn"));
        confirmPasswordBtn->setGeometry(QRect(175, 270, 150, 40));
        confirmPasswordBtn->setStyleSheet(QString::fromUtf8("QPushButton {\n"
"    background-color: rgb(24, 144, 255);\n"
"    color: white;\n"
"    border: none;\n"
"    border-radius: 8px;\n"
"    font-size: 16px;\n"
"    font-weight: bold;\n"
"}\n"
"QPushButton:pressed {\n"
"    background-color: rgb(16, 120, 220);\n"
"}\n"
"QPushButton:disabled {\n"
"    background-color: rgb(200, 200, 200);\n"
"    color: rgb(150, 150, 150);\n"
"}"));
        passwordStatusLabel = new QLabel(passwordSettingWidget);
        passwordStatusLabel->setObjectName(QString::fromUtf8("passwordStatusLabel"));
        passwordStatusLabel->setGeometry(QRect(50, 320, 400, 30));
        passwordStatusLabel->setStyleSheet(QString::fromUtf8("font-size: 14px;\n"
"color: rgb(100, 100, 100);\n"
"background: transparent;"));
        passwordStatusLabel->setAlignment(Qt::AlignCenter);
        completionWidget = new QWidget(BiometricCollectionPage);
        completionWidget->setObjectName(QString::fromUtf8("completionWidget"));
        completionWidget->setGeometry(QRect(262, 120, 500, 400));
        completionWidget->setAutoFillBackground(false);
        completionWidget->setStyleSheet(QString::fromUtf8("background-color: rgb(255, 255, 255);\n"
"border-radius: 16px;"));
        completeIconLabel = new QLabel(completionWidget);
        completeIconLabel->setObjectName(QString::fromUtf8("completeIconLabel"));
        completeIconLabel->setGeometry(QRect(0, 60, 500, 80));
        completeIconLabel->setStyleSheet(QString::fromUtf8("font-size: 64px;\n"
"color: rgb(82, 196, 26);\n"
"background: transparent;"));
        completeIconLabel->setAlignment(Qt::AlignCenter);
        completeDescLabel = new QLabel(completionWidget);
        completeDescLabel->setObjectName(QString::fromUtf8("completeDescLabel"));
        completeDescLabel->setGeometry(QRect(50, 150, 400, 60));
        completeDescLabel->setStyleSheet(QString::fromUtf8("font-size: 18px;\n"
"color: rgb(50, 50, 50);\n"
"background: transparent;"));
        completeDescLabel->setAlignment(Qt::AlignCenter);
        completeBtn = new QPushButton(completionWidget);
        completeBtn->setObjectName(QString::fromUtf8("completeBtn"));
        completeBtn->setGeometry(QRect(175, 240, 150, 50));
        completeBtn->setStyleSheet(QString::fromUtf8("QPushButton {\n"
"    background-color: rgb(82, 196, 26);\n"
"    color: white;\n"
"    border: none;\n"
"    border-radius: 8px;\n"
"    font-size: 18px;\n"
"    font-weight: bold;\n"
"}\n"
"QPushButton:pressed {\n"
"    background-color: rgb(70, 170, 20);\n"
"}"));

        retranslateUi(BiometricCollectionPage);

        QMetaObject::connectSlotsByName(BiometricCollectionPage);
    } // setupUi

    void retranslateUi(QWidget *BiometricCollectionPage)
    {
        BiometricCollectionPage->setWindowTitle(QApplication::translate("BiometricCollectionPage", "\347\224\237\347\211\251\347\211\271\345\276\201\351\207\207\351\233\206", nullptr));
        titleLabel->setText(QApplication::translate("BiometricCollectionPage", "\347\224\250\346\210\267\346\263\250\345\206\214 - \347\224\237\347\211\251\347\211\271\345\276\201\351\207\207\351\233\206", nullptr));
        backBtn->setText(QApplication::translate("BiometricCollectionPage", "\350\277\224\345\233\236", nullptr));
        userNameLabel->setText(QApplication::translate("BiometricCollectionPage", "\347\224\250\346\210\267\357\274\232", nullptr));
        progressLabel->setText(QApplication::translate("BiometricCollectionPage", "\346\255\245\351\252\244 1/3: \344\272\272\350\204\270\351\207\207\351\233\206", nullptr));
        previewTitleLabel->setText(QApplication::translate("BiometricCollectionPage", "\345\256\236\346\227\266\347\224\273\351\235\242", nullptr));
        previewPlaceholder->setText(QApplication::translate("BiometricCollectionPage", "\346\255\243\345\234\250\345\210\235\345\247\213\345\214\226\346\221\204\345\203\217\345\244\264...", nullptr));
        faceIconLabel->setText(QString());
        faceDescLabel->setText(QApplication::translate("BiometricCollectionPage", "\350\257\267\346\255\243\350\247\206\346\221\204\345\203\217\345\244\264\n"
"\344\277\235\346\214\201\351\235\242\351\203\250\346\270\205\346\231\260\345\217\257\350\247\201\n"
"\347\202\271\345\207\273\344\270\213\346\226\271\346\214\211\351\222\256\346\213\215\347\205\247", nullptr));
        startFaceCaptureBtn->setText(QApplication::translate("BiometricCollectionPage", "\345\274\200\345\247\213\351\207\207\351\233\206", nullptr));
        faceStatusLabel->setText(QString());
        faceTipLabel->setText(QApplication::translate("BiometricCollectionPage", "\346\243\200\346\265\213\345\210\260\344\272\272\350\204\270\345\220\216\344\274\232\350\207\252\345\212\250\345\220\257\347\224\250\346\213\215\347\205\247\346\214\211\351\222\256", nullptr));
        fingerprintIconLabel->setText(QApplication::translate("BiometricCollectionPage", "\360\237\221\206", nullptr));
        fingerprintDescLabel->setText(QApplication::translate("BiometricCollectionPage", "\350\257\267\345\260\206\346\211\213\346\214\207\350\275\273\346\224\276\345\234\250\346\214\207\347\272\271\344\274\240\346\204\237\345\231\250\344\270\212\n"
"\344\277\235\346\214\201\346\211\213\346\214\207\345\271\262\347\207\245\346\270\205\346\264\201\357\274\214\346\214\211\345\216\213\351\200\202\344\270\255", nullptr));
        startFingerprintBtn->setText(QApplication::translate("BiometricCollectionPage", "\345\274\200\345\247\213\351\207\207\351\233\206", nullptr));
        fingerprintStatusLabel->setText(QString());
        passwordIconLabel->setText(QApplication::translate("BiometricCollectionPage", "\360\237\224\222", nullptr));
        passwordDescLabel->setText(QApplication::translate("BiometricCollectionPage", "\350\257\267\350\256\276\347\275\256\347\231\273\345\275\225\345\257\206\347\240\201\357\274\210\350\207\263\345\260\2216\344\275\215\357\274\214\345\214\205\345\220\253\345\255\227\346\257\215\345\222\214\346\225\260\345\255\227\357\274\211", nullptr));
        passwordLineEdit->setPlaceholderText(QApplication::translate("BiometricCollectionPage", "\350\257\267\350\276\223\345\205\245\345\257\206\347\240\201", nullptr));
        confirmPasswordLineEdit->setPlaceholderText(QApplication::translate("BiometricCollectionPage", "\350\257\267\345\206\215\346\254\241\350\276\223\345\205\245\345\257\206\347\240\201", nullptr));
        confirmPasswordBtn->setText(QApplication::translate("BiometricCollectionPage", "\347\241\256\350\256\244\345\257\206\347\240\201", nullptr));
        passwordStatusLabel->setText(QString());
        completeIconLabel->setText(QApplication::translate("BiometricCollectionPage", "\342\234\223", nullptr));
        completeDescLabel->setText(QApplication::translate("BiometricCollectionPage", "\347\224\237\347\211\251\347\211\271\345\276\201\351\207\207\351\233\206\345\256\214\346\210\220\357\274\201\n"
"\347\202\271\345\207\273\344\270\213\346\226\271\346\214\211\351\222\256\345\256\214\346\210\220\346\263\250\345\206\214", nullptr));
        completeBtn->setText(QApplication::translate("BiometricCollectionPage", "\345\256\214\346\210\220\346\263\250\345\206\214", nullptr));
    } // retranslateUi

};

namespace Ui {
    class BiometricCollectionPage: public Ui_BiometricCollectionPage {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_BIOMETRICCOLLECTIONPAGE_H
