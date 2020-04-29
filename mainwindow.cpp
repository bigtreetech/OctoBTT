#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QTimer>
#include <QTime>

#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>

#include <QNetworkReply>

#include <QMessageBox>

//MY Dialog
#include "controlpanel.h"
#include "filedialog.h"
#include "materialctrlpanel.h"
#include "url_camera.h"

#include <QDebug>

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
    m_timer->start(1000);
    //定时器触发信号槽
    connect(m_timer, SIGNAL(timeout()), this, SLOT(TimerTimeOut()));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::TimerTimeOut()
{
    if(QSysInfo::productType() != "raspbian"){qDebug()<<seq<<":"<<"In"<<":"<<QDateTime::currentDateTime().time().toString("hh:mm::ss.zzz");}
    //uodate UI
    ui->Btn_ConnectState->setText(octonetwork.ConnectState);
    ui->Btn_CP->setText((materialctrlpanel->Hotend < 0 ? "-" : QString::number(materialctrlpanel->Hotend))+materialctrlpanel->Symb_Temp);
    ui->Btn_CP_1->setText((materialctrlpanel->Hotend < 0 ? "-" : QString::number(materialctrlpanel->Hotend))+materialctrlpanel->Symb_Temp);
    ui->Btn_CP_2->setText((materialctrlpanel->Heatbed < 0 ? "-" : QString::number(materialctrlpanel->Heatbed))+materialctrlpanel->Symb_Temp);
    if(octonetwork.ConnectState != "Printing" && octonetwork.ConnectState != "Printing from SD")
    {
        ui->Btn_CP_3->setIcon(QIcon(":/assets/fan.svg"));
        ui->Btn_CP_3->setText(QString::number(materialctrlpanel->FanSpeed/materialctrlpanel->FanSpeed_Max)+materialctrlpanel->Symb_Per);
    }

//    qDebug()<<QString::number(materialctrlpanel->FanSpeed/materialctrlpanel->FanSpeed_Max)+materialctrlpanel->Symb_Per;
    //ui->Btn_ConnectState->repaint();
    if(QSysInfo::productType() != "raspbian"){qDebug()<<"\t"<<octonetwork.ConnectState<<":"<<ui->Btn_ConnectState->text();}
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
        else if(!USB_Port.isEmpty() && octonetwork.ConnectState != "Printing" && octonetwork.ConnectState != "Printing from SD")//Close Offline Error
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
                qDebug()<<"Connect"<<":"<<_ConnectJson.constFind("port").value().toString();
            }

            octonetwork.networkAccessManager->post(octonetwork.SetConnectRequest,ConnectJson.toJson());
        }
        else if(octonetwork.ConnectState != "Printing" && octonetwork.ConnectState != "Printing from SD")
        {
            octonetwork.ConnectFlat = true;
            QObject::connect(octonetwork.networkAccessManager, SIGNAL(finished(QNetworkReply*)), this, SLOT(ConnectReply(QNetworkReply*)),Qt::UniqueConnection);
            octonetwork.networkAccessManager->get(octonetwork.GetConnectRequest);
        }

        //Button State
        if(octonetwork.ConnectState == "Printing" && octonetwork.ConnectState != "Printing from SD")
        {
            octonetwork.ConnectFlat = true;
            QObject::connect(octonetwork.networkAccessManager, SIGNAL(finished(QNetworkReply*)), this, SLOT(StateReply(QNetworkReply*)),Qt::UniqueConnection);
            octonetwork.networkAccessManager->get(octonetwork.GetStateRequest);

            ui->Btn_Filament->setIcon(QIcon(":/assets/cancel.svg"));
            ui->Btn_Filament_B->setText(octonetwork.ConnectState);
//            ui->Btn_Filament->setEnabled(true);
        }
        else if(octonetwork.ConnectState == "Cancelling" || octonetwork.ConnectState == "Connecting" || octonetwork.ConnectState =="Detecting baudrate" || octonetwork.ConnectState =="Detecting serial port" || octonetwork.ConnectState =="Closed" || octonetwork.ConnectState =="Offline" || octonetwork.ConnectState =="Error" || octonetwork.ConnectState == "Starting print from SD" || octonetwork.ConnectState == "Starting")
        {
            ui->Btn_Filament->setIcon(QIcon(":/assets/emoji.svg"));
            ui->Btn_Filament_B->setText("Ban");
//            ui->Btn_Filament->setEnabled(false);
        }
        else
        {
            ui->Btn_Filament->setIcon(QIcon(":/assets/haocaiguanli.svg"));
            ui->Btn_Filament_B->setText("Print");
//            ui->Btn_Filament->setEnabled(true);
        }
    }
    if(QSysInfo::productType() != "raspbian")
    {
        qDebug()<<seq++<<":"<<"Out"<<":"<<QDateTime::currentDateTime().time().toString("hh:mm::ss.zzz");
        qDebug()<<"***************************************";
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

        if(QSysInfo::productType() != "raspbian"){qDebug()<<replyArray;}
    }
    else
    {
        octonetwork.ConnectState = "Error";
            //qDebug() << "=========";
    }
    if(QSysInfo::productType() != "raspbian")
    {
        qDebug() << "statusCode:" << statusCode;
        qDebug() << "***************************************";
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

        if(QSysInfo::productType() != "raspbian"){qDebug()<<replyArray;}
    }
    else
    {
        octonetwork.ConnectState = "Error";
        //qDebug() << "=========";
    }
    if(QSysInfo::productType() != "raspbian")
    {
        qDebug() << "statusCode:" << statusCode;
        qDebug() << "***************************************";
    }
    reply->deleteLater();
    octonetwork.ConnectFlat = !octonetwork.ConnectFlat;
    octonetwork.networkAccessManager->disconnect(SIGNAL(finished(QNetworkReply*)));//disconnect
}

//Debug Btn
void MainWindow::on_Btn_setting_clicked()
{

}

void MainWindow::on_Btn_Ctrl_clicked()
{
    controlpanel->resize(this->width(),this->height());
    controlpanel->setFixedSize(this->width(),this->height());
    controlpanel->setWindowState(this->windowState());
    controlpanel->show();
}

void MainWindow::on_UILogo_clicked()
{
    if(QMessageBox::information(NULL, "Warning", "Do you want to quit desktop ?", QMessageBox::Yes  | QMessageBox::No , QMessageBox::No) == QMessageBox::Yes)
    {
        qApp->exit(Base_OnlyExitApp);
    }
}

void MainWindow::on_Btn_FS_clicked()
{
    filedialog->resize(this->width(),this->height());
    filedialog->setFixedSize(this->width(),this->height());
    filedialog->setWindowState(this->windowState());
    filedialog->RefreshFileList();
    filedialog->show();
}

void MainWindow::on_Btn_CP_clicked()
{
    materialctrlpanel->resize(this->width(),this->height());
    materialctrlpanel->setFixedSize(this->width(),this->height());
    materialctrlpanel->setWindowState(this->windowState());
    materialctrlpanel->show();
}

void MainWindow::on_Btn_Camera_clicked()
{
//    cameradialog->resize(this->width(),this->height());
//    cameradialog->setWindowState(this->windowState());
//    cameradialog->show();

    url_camera->resize(this->width(),this->height());
    url_camera->setFixedSize(this->width(),this->height());
    url_camera->setWindowState(this->windowState());
    url_camera->show();
}
//Detail For Print
void MainWindow::on_Btn_Filament_clicked()
{
    octonetwork.JobSwitch(filedialog->SelectURL);
}
