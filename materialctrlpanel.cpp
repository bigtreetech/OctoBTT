#include "materialctrlpanel.h"
#include "ui_materialctrlpanel.h"
#include <QTimer>
#include <QDebug>
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>
//#include <QSizeF>
#include <QNetworkReply>
#include <mainwindow.h>
//#include <QMap>

MaterialCtrlPanel::MaterialCtrlPanel(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::MaterialCtrlPanel)
{
    ui->setupUi(this);

    FUI = (MainWindow*)parent;

    ReLoopFlag = E_Temperature;
//    qDebug() << "E_Bed:" <<E_Bed << ":" << (ReLoop) 0;
//    qDebug() << "E_Tool:" <<E_Tool << ":" << (ReLoop) 1;
    QObject::connect(((MainWindow*)FUI)->octonetwork.MCPnetworkAccessManager, SIGNAL(finished(QNetworkReply*)), this, SLOT(Reply(QNetworkReply*)),Qt::UniqueConnection);

    //SysTimer
    QTimer *m_timer= new QTimer(this);
    //启动或重启定时器, 并设置定时器时间：毫秒
    m_timer->start(500);
    //定时器触发信号槽
    connect(m_timer, SIGNAL(timeout()), this, SLOT(TimerTimeOut()));

    this->resize((int)(SizePercent.width()*800),(int)(SizePercent.height()*480));
    this->setMaximumSize((int)(SizePercent.width()*800),(int)(SizePercent.height()*480));
    this->setFixedSize((int)(SizePercent.width()*800),(int)(SizePercent.height()*480));
}
void MaterialCtrlPanel::TimerTimeOut()
{
    if(!SettingFlag)
    {
        Hotend_Target = Hotend;
        Heatbed_Target = Heatbed;
        Feedrate_Target = Feedrate;
        Flowrate_Target = Flowrate;
        Tool_Target = Tool;
        FanSpeed_Target = FanSpeed;
    }
    //update UI
    ui->Txt_Hotend->setText((Hotend < 0 ? "-" : QString::number(Hotend_Target))+Symb_Temp);
    ui->Btn_Hotend_A->setEnabled(Hotend >= 0);
    ui->Btn_Hotend_D->setEnabled(Hotend >= 0);
    ui->Txt_Heatbed->setText((Heatbed < 0 ? "-" : QString::number(Heatbed_Target))+Symb_Temp);
    ui->Btn_Heatbed_A->setEnabled(Heatbed >= 0);
    ui->Btn_Heatbed_D->setEnabled(Heatbed >= 0);
    ui->Txt_Feedrate->setText((Feedrate < 0 ? "-" : QString::number(Feedrate_Target))+Symb_Per);
    ui->Btn_Feedrate_A->setEnabled(Feedrate >= 0);
    ui->Btn_Feedrate_D->setEnabled(Feedrate >= 0);
    ui->Txt_Flowrate->setText((Flowrate < 0 ? "-" : QString::number(Flowrate_Target))+Symb_Per);
    ui->Btn_Flowrate_A->setEnabled(Flowrate >= 0);
    ui->Btn_Flowrate_D->setEnabled(Flowrate >= 0);

    //    qDebug()<<QString::number(materialctrlpanel->FanSpeed/materialctrlpanel->FanSpeed_Max)+materialctrlpanel->Symb_Per;
    //ui->Btn_ConnectState->repaint();
    if((((MainWindow*)FUI)->octonetwork.ConnectState == "Operational" || ((MainWindow*)FUI)->octonetwork.ConnectState == "Printing" || ((MainWindow*)FUI)->octonetwork.ConnectState == "Cancelling" || ((MainWindow*)FUI)->octonetwork.ConnectState == "Pausing" || ((MainWindow*)FUI)->octonetwork.ConnectState == "Starting" || ((MainWindow*)FUI)->octonetwork.ConnectState == "Printing from SD" || ((MainWindow*)FUI)->octonetwork.ConnectState == "Starting print from SD") && !ReplyFlag)
    {

//            //Post
//            Fui->octonetwork.ConnectState = "Connecting";
        switch(ReLoopFlag)
        {
            case E_Temperature :
                ((MainWindow*)FUI)->octonetwork.MCPnetworkAccessManager->get(((MainWindow*)FUI)->octonetwork.GetTemperatureRequest);
                break;
            default:
                break;
        }
        ReplyFlag = true;
    }
    else
    {
        Hotend = -15;
        Heatbed = -15;
        Feedrate = 0;
        Flowrate = 0;
        Tool = -15;
        FanSpeed = 0;
    }
}
void MaterialCtrlPanel::Reply(QNetworkReply *reply)
{
    QList<QJsonValue> _V_Temperature;

    int statusCode = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();
    if(reply->error() == QNetworkReply::NoError)
    {
        QByteArray replyArray = reply->readAll();
        QJsonDocument loadDoc(QJsonDocument::fromJson(replyArray));
        QJsonObject QObj = loadDoc.object();
        double HotendValue = -15;
        double ToolValue = -15;
        switch(ReLoopFlag)
        {
            case E_Temperature :
                _V_Temperature << ((MainWindow*)FUI)->octonetwork.SearchJsonValue(((MainWindow*)FUI)->octonetwork.TemperatureLink , QObj);
                Heatbed = _V_Temperature[0].toDouble();
                foreach(QJsonValue item,_V_Temperature[1].toArray())
                {
                    if(QSysInfo::productType() != "raspbian"){qDebug()<<item.toDouble();}
                    if(item.toDouble() > HotendValue)
                        HotendValue = item.toDouble();
                    else if(item.toDouble() > ToolValue)
                        ToolValue = item.toDouble();
                }
                Hotend = HotendValue;
                Tool = ToolValue;
                break;
            default:
                break;
         }

         if(QSysInfo::productType() != "raspbian"){qDebug()<<replyArray;}
    }
    else
    {
        ((MainWindow*)FUI)->octonetwork.ConnectState = "Error";
        //qDebug() << "=========";
    }
    if(QSysInfo::productType() != "raspbian")
    {
        qDebug() << "statusCode:" << statusCode;
        qDebug() << "***************************************";
    }
    reply->deleteLater();
    ReLoopFlag ++ ;
    if(ReLoopFlag > E_Temperature)
    {
        ReLoopFlag = E_Temperature;
    }
    ReplyFlag = false;
//    ((MainWindow*)FUI)->octonetwork.MCPnetworkAccessManager->disconnect(SIGNAL(finished(QNetworkReply*)));//disconnect
}

MaterialCtrlPanel::~MaterialCtrlPanel()
{
    delete ui;
}

void MaterialCtrlPanel::on_Btn_back_clicked()
{
    SettingFlag = false;
    this->close();
}

void MaterialCtrlPanel::on_Btn_Save_pressed()
{
    StyleSheet_Temp = ui->Btn_Save->styleSheet();
    ui->Btn_Save->setStyleSheet(StyleSheet_Temp+"\nbackground-color:rgb(128,128,128,128);");

    QList<QString> _GCode_Setting;

    if(Heatbed != Heatbed_Target)
    {
        _GCode_Setting.append("M140 S"+QString::number((int)Heatbed_Target));
    }

    if(Hotend != Hotend_Target)
    {
        _GCode_Setting.append("M104 S"+QString::number((int)Hotend_Target));
    }

    ((MainWindow*)FUI)->octonetwork.SendGCode(_GCode_Setting);

//    qDebug()<<TemperatureJsonObj_SonKey.count();
    SettingFlag = false;
}

void MaterialCtrlPanel::on_Btn_Save_released()
{
    ui->Btn_Save->setStyleSheet(StyleSheet_Temp);
}

void MaterialCtrlPanel::on_Btn_Hotend_A_pressed()
{
    SettingFlag = true;

    StyleSheet_Temp = ui->Btn_Hotend_A->styleSheet();
    ui->Btn_Hotend_A->setStyleSheet(StyleSheet_Temp+"\nbackground-color:rgb(128,128,128,128);");

    if(Hotend_Target + SettingStep < Hotend_Max)
        Hotend_Target += SettingStep;
    else
        Hotend_Target = Hotend_Max;

    Hotend_Target = (int)Hotend_Target;
}

void MaterialCtrlPanel::on_Btn_Hotend_A_released()
{
    ui->Btn_Hotend_A->setStyleSheet(StyleSheet_Temp);
}

void MaterialCtrlPanel::on_Btn_Hotend_D_pressed()
{
    SettingFlag = true;

    StyleSheet_Temp = ui->Btn_Hotend_D->styleSheet();
    ui->Btn_Hotend_D->setStyleSheet(StyleSheet_Temp+"\nbackground-color:rgb(128,128,128,128);");

    if(Hotend_Target - SettingStep > 0)
        Hotend_Target -= SettingStep;
    else
        Hotend_Target = 0;

    Hotend_Target = (int)Hotend_Target;
}

void MaterialCtrlPanel::on_Btn_Hotend_D_released()
{
    ui->Btn_Hotend_D->setStyleSheet(StyleSheet_Temp);
}

void MaterialCtrlPanel::on_Btn_Heatbed_A_pressed()
{
    SettingFlag = true;

    StyleSheet_Temp = ui->Btn_Heatbed_A->styleSheet();
    ui->Btn_Heatbed_A->setStyleSheet(StyleSheet_Temp+"\nbackground-color:rgb(128,128,128,128);");

    if(Heatbed_Target + SettingStep < Heatbed_Max)
        Heatbed_Target += SettingStep;
    else
        Heatbed_Target = Heatbed_Max;

    Heatbed_Target = (int)Heatbed_Target;
}

void MaterialCtrlPanel::on_Btn_Heatbed_A_released()
{
    ui->Btn_Heatbed_A->setStyleSheet(StyleSheet_Temp);
}

void MaterialCtrlPanel::on_Btn_Heatbed_D_pressed()
{
    SettingFlag = true;

    StyleSheet_Temp = ui->Btn_Heatbed_D->styleSheet();
    ui->Btn_Heatbed_D->setStyleSheet(StyleSheet_Temp+"\nbackground-color:rgb(128,128,128,128);");

    if(Heatbed_Target - SettingStep > 0)
        Heatbed_Target -= SettingStep;
    else
        Heatbed_Target = 0;

    Heatbed_Target = (int)Heatbed_Target;
}

void MaterialCtrlPanel::on_Btn_Heatbed_D_released()
{
    ui->Btn_Heatbed_D->setStyleSheet(StyleSheet_Temp);
}

void MaterialCtrlPanel::on_Btn_Feedrate_A_pressed()
{
    SettingFlag = true;

    StyleSheet_Temp = ui->Btn_Feedrate_A->styleSheet();
    ui->Btn_Feedrate_A->setStyleSheet(StyleSheet_Temp+"\nbackground-color:rgb(128,128,128,128);");

    if(Feedrate_Target + SettingStep < Feedrate_Max)
        Feedrate_Target+= SettingStep;
    else
        Feedrate_Target = Feedrate_Max;

    Feedrate_Target = (int)Feedrate_Target;
}

void MaterialCtrlPanel::on_Btn_Feedrate_A_released()
{
    ui->Btn_Feedrate_A->setStyleSheet(StyleSheet_Temp);
}

void MaterialCtrlPanel::on_Btn_Feedrate_D_pressed()
{
    SettingFlag = true;

    StyleSheet_Temp = ui->Btn_Feedrate_D->styleSheet();
    ui->Btn_Feedrate_D->setStyleSheet(StyleSheet_Temp+"\nbackground-color:rgb(128,128,128,128);");

    if(Feedrate_Target - SettingStep > 0)
        Feedrate_Target -= SettingStep;
    else
        Feedrate_Target = 0;

    Feedrate_Target = (int)Feedrate_Target;
}

void MaterialCtrlPanel::on_Btn_Feedrate_D_released()
{
    ui->Btn_Feedrate_D->setStyleSheet(StyleSheet_Temp);
}

void MaterialCtrlPanel::on_Btn_Flowrate_A_pressed()
{
    SettingFlag = true;

    StyleSheet_Temp = ui->Btn_Flowrate_A->styleSheet();
    ui->Btn_Flowrate_A->setStyleSheet(StyleSheet_Temp+"\nbackground-color:rgb(128,128,128,128);");

    if(Flowrate_Target + SettingStep < Flowrate_Max)
        Flowrate_Target += SettingStep;
    else
        Flowrate_Target = Flowrate_Max;

    Flowrate_Target = (int)Flowrate_Target;
}

void MaterialCtrlPanel::on_Btn_Flowrate_A_released()
{
    ui->Btn_Flowrate_A->setStyleSheet(StyleSheet_Temp);
}

void MaterialCtrlPanel::on_Btn_Flowrate_D_pressed()
{
    SettingFlag = true;

    StyleSheet_Temp = ui->Btn_Flowrate_D->styleSheet();
    ui->Btn_Flowrate_D->setStyleSheet(StyleSheet_Temp+"\nbackground-color:rgb(128,128,128,128);");

    if(Flowrate_Target - SettingStep > 0)
        Flowrate_Target -= SettingStep;
    else
        Flowrate_Target = 0;

    Flowrate_Target = (int)Flowrate_Target;
}

void MaterialCtrlPanel::on_Btn_Flowrate_D_released()
{
    ui->Btn_Flowrate_D->setStyleSheet(StyleSheet_Temp);
}

void MaterialCtrlPanel::on_Box_SettingStep_valueChanged(int arg1)
{
    SettingStep = arg1;
}
