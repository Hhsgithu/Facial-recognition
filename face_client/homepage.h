#ifndef HOMEPAGE_H
#define HOMEPAGE_H

#include <QWidget>
#include <QTimer>
#include <QLabel>
#include <QPushButton>

QT_BEGIN_NAMESPACE
namespace Ui { class HomePage; }
QT_END_NAMESPACE

class HomePage : public QWidget
{
    Q_OBJECT

public:
    explicit HomePage(QWidget *parent = nullptr);
    ~HomePage();

    void showRegistrationProgress(const QString& stepText);
    void hideRegistrationProgress();

signals:
    void faceRecognitionClicked();
    void passwordUnlockClicked();
    void fingerprintUnlockClicked();
    void userRegistrationClicked();

private slots:
    void updateDateTime();
    void onFaceRecognitionClicked();
    void onPasswordUnlockClicked();
    void onFingerprintUnlockClicked();
    void onUserRegistrationClicked();

private:
    Ui::HomePage *ui;
    QTimer *dateTimeTimer;
    QLabel *registrationProgressLabel;
};

#endif // HOMEPAGE_H

