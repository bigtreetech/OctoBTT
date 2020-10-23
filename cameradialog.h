#ifndef CAMERADIALOG_H
#define CAMERADIALOG_H

#include <QDialog>
#include <QSizeF>
#include <QNetworkReply>

extern int DebugFlat ;
extern QSizeF SizePercent;
//#include <QCamera>
//#include <QCameraImageCapture>
//#include <QMediaRecorder>

namespace Ui {
    class CameraDialog;
}

//class QCamera;
//class QCameraViewfinder;
//class QCameraImageCapture;
class CameraDialog : public QDialog
{
        Q_OBJECT

    public:
        explicit CameraDialog(QWidget *parent = nullptr);
        ~CameraDialog();

    private slots:
        virtual void on_Btn_Back_clicked();

        void mjpeg_streamer_reply(QNetworkReply *reply);

        void on_Btn_Sensor_toggled(bool checked);

    private:
        Ui::CameraDialog *ui;
        QNetworkAccessManager *manager = new QNetworkAccessManager();
        QNetworkRequest *request;
        QWidget *FUI;
    protected:
        void showEvent(QShowEvent *event);
};

#endif // CAMERADIALOG_H
