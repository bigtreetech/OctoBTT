#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QTimer>
#include <QTime>
//#include <QSizeF>

#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>

#include <QNetworkReply>
//MY Dialog
#include "controlpanel.h"
#include "filedialog.h"
#include "materialctrlpanel.h"

#include <QMovie>
#include <QPoint>
#include <QCursor>
#include <QDir>
#include <QFile>
#include <QTextBrowser>
#include <QDesktopWidget>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{

    ui->setupUi(this);

    //network wake up
    octonetwork.ConnectFlat = true;
    QObject::connect(octonetwork.networkAccessManager, SIGNAL(finished(QNetworkReply*)), this, SLOT(ConnectReply(QNetworkReply*)),Qt::UniqueConnection);
    octonetwork.networkAccessManager->get(octonetwork.GetConnectRequest);

    //SysTimer
    QTimer *m_timer= new QTimer(this);
    //启动或重启定时器, 并设置定时器时间：毫秒
    //系统状态扫描触发计时器
    m_timer->start(1000);
    //定时器触发信号槽
    connect(m_timer, SIGNAL(timeout()), this, SLOT(TimerTimeOut()));

    this->resize((int)(SizePercent.width()*800),(int)(SizePercent.height()*480));
    this->setMaximumSize((int)(SizePercent.width()*800),(int)(SizePercent.height()*480));
    this->setFixedSize((int)(SizePercent.width()*800),(int)(SizePercent.height()*480));
//    ui->Btn_Camera->setVisible(false);
    filedialog->InitializeUSBDrive();

    //移动光标位置
    QPoint pt(0,0);
    QPoint center = this->mapToGlobal(pt);
    QCursor::setPos(center);

    //设置各个按键的样式表
    ui->Btn_CP->setStyleSheet("QPushButton{border:none;font: 20pt Gill Sans MT; MT;color: rgb(245, 246, 250);}"
                              "QPushButton:pressed{border:none;background-color: rgb(128, 128, 128, 30%); padding-left:4px; padding-top:4px;}"
                              "QPushButton:hover  {border:none;background-color: rgb(128, 128, 128, 30%);}");
}

MainWindow::~MainWindow()
{
    if(!(gcodedialog->_pdataRecvWS->state() == QAbstractSocket::UnconnectedState))
        gcodedialog->_pdataRecvWS->abort();//Close WS
    delete ui;
}

void MainWindow::TimerTimeOut()
{
    if(QSysInfo::productType() != "raspbian")
    {
        LibLog::LogRec(QString("%1 : In :___%2___%3").arg(seq).arg(__FILE__).arg(__LINE__), "connect");
    }
    //uodate UI
    ui->Btn_ConnectState->setText(octonetwork.ConnectState.contains("Printing") ? "Printing : " + filedialog->DisplayName :octonetwork.ConnectState);
    ui->Btn_CP->setText((materialctrlpanel->Hotend < 0 ? "-" : QString::number(materialctrlpanel->Hotend))+materialctrlpanel->Symb_Temp);
    ui->Btn_CP_1->setText((materialctrlpanel->Hotend < 0 ? "-" : QString::number(materialctrlpanel->Hotend))+materialctrlpanel->Symb_Temp);
    ui->Btn_CP_2->setText((materialctrlpanel->Heatbed < 0 ? "-" : QString::number(materialctrlpanel->Heatbed))+materialctrlpanel->Symb_Temp);
    if(octonetwork.ConnectState != "Printing" && octonetwork.ConnectState != "Printing from SD")
    {
        ui->Btn_CP_3->setIcon(QIcon(":/assets/fan.svg"));
        ui->Btn_CP_3->setText(QString::number(materialctrlpanel->FanSpeed/materialctrlpanel->FanSpeed_Max)+materialctrlpanel->Symb_Per);
    }

    //ui->Btn_ConnectState->repaint();
    if(QSysInfo::productType() != "raspbian")
    {
        LibLog::LogRec(QString("%1 : %2___%3___%4").arg(octonetwork.ConnectState).arg(ui->Btn_ConnectState->text()).arg(__FILE__).arg(__LINE__), "connect");

    }
    if(!octonetwork.ConnectFlat)
    {
        if(octonetwork.ConnectState == "Operational" || octonetwork.ConnectState == "Pausing" || octonetwork.ConnectState == "Starting" /*|| octonetwork.ConnectState == "Printing"*/ || octonetwork.ConnectState == "Cancelling" || octonetwork.ConnectState == "Starting print from SD")
        {
            octonetwork.ConnectFlat = true;
            QObject::connect(octonetwork.networkAccessManager, SIGNAL(finished(QNetworkReply*)), this, SLOT(ConnectReply(QNetworkReply*)),Qt::UniqueConnection);
            octonetwork.networkAccessManager->get(octonetwork.GetConnectRequest);
        }
        else if(octonetwork.ConnectState == "Connecting" || octonetwork.ConnectState =="Detecting baudrate"|| octonetwork.ConnectState =="Detecting serial port")
        {
            octonetwork.ConnectFlat = true;
            QObject::connect(octonetwork.networkAccessManager, SIGNAL(finished(QNetworkReply*)), this, SLOT(ConnectReply(QNetworkReply*)),Qt::UniqueConnection);
            octonetwork.networkAccessManager->get(octonetwork.GetConnectRequest);
        }
        else if(!USB_Port.isEmpty() && !octonetwork.ConnectState.contains("Printing"))//Close Offline Error
        {
            //Post
            materialctrlpanel->ReplyFlag = false;
            octonetwork.ConnectState = "Connecting";

            QJsonDocument ConnectJson;
            QJsonObject _ConnectJson;
            _ConnectJson.insert("command","connect");
            _ConnectJson.insert("port",USB_Port.takeFirst());
            _ConnectJson.insert("baudrate",115200);
            _ConnectJson.insert("printerProfile","_default");
            _ConnectJson.insert("save",false);
            _ConnectJson.insert("autoconnect",false);
            ConnectJson.setObject(_ConnectJson);

            if(QSysInfo::productType() != "raspbian")
            {
                LibLog::LogRec(QString("Connect : %1 : In :___%2___%3").arg(_ConnectJson.constFind("port").value().toString()).arg(__FILE__).arg(__LINE__), "connect");
            }

            octonetwork.networkAccessManager->post(octonetwork.SetConnectRequest,ConnectJson.toJson());
        }
        else if(!octonetwork.ConnectState.contains("Printing"))
        {
            octonetwork.ConnectFlat = true;
            QObject::connect(octonetwork.networkAccessManager, SIGNAL(finished(QNetworkReply*)), this, SLOT(ConnectReply(QNetworkReply*)),Qt::UniqueConnection);
            octonetwork.networkAccessManager->get(octonetwork.GetConnectRequest);
        }

        //Button State
        if(octonetwork.ConnectState.contains("Printing"))
        {
            octonetwork.ConnectFlat = true;
            QObject::connect(octonetwork.networkAccessManager, SIGNAL(finished(QNetworkReply*)), this, SLOT(StateReply(QNetworkReply*)),Qt::UniqueConnection);
            octonetwork.networkAccessManager->get(octonetwork.GetStateRequest);

            ui->Btn_Filament->setIcon(QIcon(":/assets/cancel.svg"));
//            ui->Btn_Filament_B->setText(octonetwork.ConnectState);
//            ui->Btn_Filament->setEnabled(true);
        }
        else if(octonetwork.ConnectState == "Connecting" || octonetwork.ConnectState =="Detecting baudrate" || octonetwork.ConnectState =="Detecting serial port")
        {
            ui->Btn_Filament->setIcon(QIcon(":/assets/emoji.svg"));
//            ui->Btn_Filament_B->setText("Connecting");
            //            ui->Btn_Filament->setEnabled(false);
        }
        else if(octonetwork.ConnectState == "Cancelling" || octonetwork.ConnectState =="Error" || octonetwork.ConnectState == "Starting print from SD" || octonetwork.ConnectState == "Starting")
        {
            ui->Btn_Filament->setIcon(QIcon(":/assets/emoji.svg"));
//            ui->Btn_Filament_B->setText("Busy");
//            ui->Btn_Filament->setEnabled(false);
        }
        else if(octonetwork.ConnectState =="Closed" || octonetwork.ConnectState =="Offline")
        {
            ui->Btn_Filament->setIcon(QIcon(":/assets/emoji.svg"));
//            ui->Btn_Filament_B->setText("Lost connection");
        }
        else
        {
            ui->Btn_Filament->setIcon(QIcon(":/assets/haocaiguanli.svg"));
//            ui->Btn_Filament_B->setText("Print " + (filedialog->SelectURL.url() == "" ? "" : filedialog->DisplayName));
//            ui->Btn_Filament->setEnabled(true);
        }
    }
    if(QSysInfo::productType() != "raspbian")
    {
        LibLog::LogRec(QString("%1 : Out :___%2___%3").arg(seq++).arg(__FILE__).arg(__LINE__), "connect");
        LibLog::LogRec("***************************************", "connect");
    }
}

void MainWindow::ConnectReply(QNetworkReply *reply)
{
    int statusCode = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();
    if(reply->error() == QNetworkReply::NoError)
    {
        QByteArray replyArray = reply->readAll();
        QJsonDocument loadDoc(QJsonDocument::fromJson(replyArray));
        QJsonObject QObj = loadDoc.object();

        QList<QJsonValue> ReplyResult = octonetwork.SearchJsonValue(octonetwork.ConnectLink , QObj);
        octonetwork.ConnectState = ReplyResult[0].toString();
//        ReplyResult[0].toArray().isEmpty()//判断是否为空
        if(!USB_Port.isEmpty()) USB_Port.clear();
        foreach(QJsonValue item , ReplyResult[1].toArray())//遍历可连接非串口设备
        {
            if(octonetwork.ConnectSerialLink.contains(item.toString())) continue;//属于串口
            USB_Port.append(item.toString());
        }

        if(QSysInfo::productType() != "raspbian")
        {
            LibLog::LogRec(QString("%1___%2___%3").arg(QString(replyArray)).arg(__FILE__).arg(__LINE__), "connect");
        }
    }
    else
    {
        octonetwork.ConnectState = "Error";
    }
    if(QSysInfo::productType() != "raspbian")
    {
        LibLog::LogRec(QString("statusCode : %1___%2___%3").arg(statusCode).arg(__FILE__).arg(__LINE__), "connect");
        LibLog::LogRec("***************************************", "connect");
    }
    reply->deleteLater();
    octonetwork.ConnectFlat = !octonetwork.ConnectFlat;
    octonetwork.networkAccessManager->disconnect(SIGNAL(finished(QNetworkReply*)));//disconnect
}

void MainWindow::StateReply(QNetworkReply *reply)
{
    int statusCode = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();
    if(reply->error() == QNetworkReply::NoError)
    {
        QByteArray replyArray = reply->readAll();
        QJsonDocument loadDoc(QJsonDocument::fromJson(replyArray));
        QJsonObject QObj = loadDoc.object();

        QList<QJsonValue> ReplyResult = octonetwork.SearchJsonValue(octonetwork.StateLink , QObj);
        octonetwork.ConnectState = ReplyResult[0].toString();
//        ui->Btn_Filament_B->setText(ui->Btn_Filament_B->text() + " " + QString::number((int)ReplyResult[1].Double) + "%");
        ui->Btn_CP_3->setIcon(QIcon(":/assets/filament.svg"));
        ui->Btn_CP_3->setText(QString::number(ReplyResult[1].toDouble(),'f',2) + "%");

        if(QSysInfo::productType() != "raspbian")
        {
            LibLog::LogRec(QString("%1___%2___%3").arg(QString(replyArray)).arg(__FILE__).arg(__LINE__), "connect");
        }
    }
    else
    {
        octonetwork.ConnectState = "Error";
    }
    if(QSysInfo::productType() != "raspbian")
    {
        LibLog::LogRec(QString("statusCode : %1___%2___%3").arg(statusCode).arg(__FILE__).arg(__LINE__), "connect");
        LibLog::LogRec("***************************************", "connect");
    }
    reply->deleteLater();
    octonetwork.ConnectFlat = !octonetwork.ConnectFlat;
    octonetwork.networkAccessManager->disconnect(SIGNAL(finished(QNetworkReply*)));//disconnect
}

//Debug Btn
void MainWindow::on_Btn_setting_clicked()
{
//    terminaldialog->resize(this->width(),this->height());
//    terminaldialog->setFixedSize(this->width(),this->height());
//    terminaldialog->setWindowState(this->windowState());
//    terminaldialog->show();
    configuration->show();
//    if(DebugFlat == 1)
//    {
//        connect(terminaldialog,&TerminalDialog::CMD_Reply,this,[=](QStringList CommandLine){
//            disconnect(terminaldialog,&TerminalDialog::CMD_Reply,0,0);
//        });
//    }

}

void MainWindow::on_Btn_Ctrl_clicked()
{
//    controlpanel->resize(this->width(),this->height());
//    controlpanel->setFixedSize(this->width(),this->height());
//    controlpanel->setWindowState(this->windowState());
    controlpanel->show();
}

void MainWindow::on_UILogo_clicked()
{
    if(DebugFlat > 0)
    {
        QStringList btnName = {"Yes", "No",} ;
        CustomDialog *newDialog = new CustomDialog();
        QObject::connect(newDialog, &CustomDialog::OutputEvent, newDialog,[=](QString instruct)
        {
            if(instruct == "Yes")
            {
                qApp->exit(Base_OnlyExitApp);
            }
        });
        newDialog->showCustomDialog("Quit desktop",":/assets/info.svg",btnName,"Do you want to quit desktop ?",this);
    }
}

void MainWindow::on_Btn_FS_clicked()
{
//    filedialog->resize(this->width(),this->height());
//    filedialog->setFixedSize(this->width(),this->height());
//    filedialog->setWindowState(this->windowState());
//    filedialog->RefreshFileList();
    filedialog->show();
}

void MainWindow::on_Btn_CP_clicked()
{
//    materialctrlpanel->resize(this->width(),this->height());
//    materialctrlpanel->setFixedSize(this->width(),this->height());
//    materialctrlpanel->setWindowState(this->windowState());
    materialctrlpanel->show();
}

void MainWindow::on_Btn_Camera_clicked()
{
//    cameradialog->resize(this->width(),this->height());
//    cameradialog->setWindowState(this->windowState());
//    cameradialog->show();

//    url_camera->resize(this->width(),this->height());
//    url_camera->setFixedSize(this->width(),this->height());
//    url_camera->setWindowState(this->windowState());
    cameradialog->show();
}
//Detail For Print
void MainWindow::on_Btn_Filament_clicked()
{
    if(filedialog->SelectURL.url().trimmed() == "")
        filedialog->show();
    else
        octonetwork.JobSwitch(filedialog->SelectURL);
}
