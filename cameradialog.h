#ifndef CAMERADIALOG_H
#define CAMERADIALOG_H

#include <QDialog>
#include <QSizeF>

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

//        void on_Box_CameraId_currentIndexChanged(int index);

    private:
        Ui::CameraDialog *ui;

//        QCamera *camera;
//        QCameraViewfinder *viewfinder;
//        QCameraImageCapture *imageCapture;
};

#endif // CAMERADIALOG_H
