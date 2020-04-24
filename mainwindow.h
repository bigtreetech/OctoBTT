#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>


//#include <QJsonDocument>
//#include <QJsonParseError>

#include <QNetworkReply>

//MY Dialog
#include "cameradialog.h"
#include "controlpanel.h"
#include "filedialog.h"
#include "materialctrlpanel.h"
#include "url_camera.h"
#include "octonetwork.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
        Q_OBJECT

    public:
        MainWindow(QWidget *parent = nullptr);
        ~MainWindow();

    public:
        OctoNetwork octonetwork = OctoNetwork();
//        OctoNetwork octonetwork;

    private slots:
        void on_Btn_setting_clicked();

        void on_Btn_Ctrl_clicked();

        void on_UILogo_clicked();

        void on_Btn_FS_clicked();

        void on_Btn_CP_clicked();

        void on_Btn_Camera_clicked();

        void TimerTimeOut();

        void ConnectReply(QNetworkReply *reply);
        void StateReply(QNetworkReply *reply);

        void on_Btn_Filament_clicked();

    private:
        Ui::MainWindow *ui;
//        QString SearchJsonValue(QList<QString> Find_List_Name , QJsonObject _QObj);

    private:
        ControlPanel *controlpanel = new ControlPanel(this);
        FileDialog *filedialog = new FileDialog(this);
        MaterialCtrlPanel *materialctrlpanel = new MaterialCtrlPanel(this);
        CameraDialog *cameradialog = new CameraDialog(this);
        URL_Camera *url_camera = new URL_Camera(this);

        QList<QString> USB_Port;

//        QTimer *m_timer;
//        void InitTimer();

//        QNetworkAccessManager *networkAccessManager = new QNetworkAccessManager();
//        QNetworkRequest request;

//        bool ConnectFlat = false;
//        QString ConnectState = "Close";
//        QByteArray X_API_Key = "C1B64D24AE99475EAA9385B5DBC77820";
//        QString MainUrl = "http://192.168.3.111/api/";
        int seq = 1;
};
#endif // MAINWINDOW_H
