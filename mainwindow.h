#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSizeF>


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
#include "terminaldialog.h"
#include "configuration.h"
#include "wlanconfig.h"
#include "gcodedialog.h"

extern int DebugFlat ;
extern QSizeF SizePercent;

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
        OctoNetwork octonetwork = OctoNetwork(this);
//        OctoNetwork octonetwork;

    private slots:
        virtual void on_Btn_setting_clicked();

        virtual void on_Btn_Ctrl_clicked();

        virtual void on_UILogo_clicked();

        virtual void on_Btn_FS_clicked();

        virtual void on_Btn_CP_clicked();

        virtual void on_Btn_Camera_clicked();

        virtual void TimerTimeOut();

        virtual void ConnectReply(QNetworkReply *reply);

        virtual void StateReply(QNetworkReply *reply);

        virtual void on_Btn_Filament_clicked();

    private:
        Ui::MainWindow *ui;
//        QString SearchJsonValue(QList<QString> Find_List_Name , QJsonObject _QObj);

    public://Frm
        ControlPanel *controlpanel = new ControlPanel(this);
        FileDialog *filedialog = new FileDialog(this);
        MaterialCtrlPanel *materialctrlpanel = new MaterialCtrlPanel(this);
        CameraDialog *cameradialog = new CameraDialog(this);
        URL_Camera *url_camera = new URL_Camera(this);
        TerminalDialog *terminaldialog = new TerminalDialog(this);
        GcodeDialog *gcodedialog = new GcodeDialog(this);
        Configuration *configuration = new Configuration(this);
        Wlanconfig *wlanconfig = new Wlanconfig(this);

    private:
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
