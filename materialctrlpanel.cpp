#include "materialctrlpanel.h"
#include "ui_materialctrlpanel.h"

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
    QObject::connect(((MainWindow*)FUI)->octonetwork.MCPnetworkAccessManager, SIGNAL(finished(QNetworkReply*)), this, SLOT(Reply(QNetworkReply*)),Qt::UniqueConnection);

    //SysTimer
    QTimer *m_timer= new QTimer(this);
    //启动或重启定时器, 并设置定时器时间：毫秒
    m_timer->start(10);
    //定时器触发信号槽
    connect(m_timer, SIGNAL(timeout()), this, SLOT(TimerTimeOut()));

    //屏幕自适应大小
    this->resize((int)(SizePercent.width()*800),(int)(SizePercent.height()*480));
    this->setMaximumSize((int)(SizePercent.width()*800),(int)(SizePercent.height()*480));
    this->setFixedSize((int)(SizePercent.width()*800),(int)(SizePercent.height()*480));

    //设置Save按键的边框样式表
    ui->Btn_Save->setStyleSheet("border-color: rgb(85, 255, 0);");

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
    else if((((MainWindow*)FUI)->octonetwork.ConnectState == "Closed" || ((MainWindow*)FUI)->octonetwork.ConnectState.contains("Error")) || ((MainWindow*)FUI)->octonetwork.ConnectState == "Connecting" || ((MainWindow*)FUI)->octonetwork.ConnectState =="Detecting baudrate" || ((MainWindow*)FUI)->octonetwork.ConnectState =="Detecting serial port")
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
                    if(QSysInfo::productType() != "raspbian"){
                        LibLog::LogRec(QString("%1___%2___%3").arg(item.toDouble()).arg(__FILE__).arg(__LINE__), "connect");
                    }
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

         if(QSysInfo::productType() != "raspbian")
         {
             LibLog::LogRec(QString("%1___%2___%3").arg(QString(replyArray)).arg(__FILE__).arg(__LINE__), "connect");
         }
    }
    else
    {
        ((MainWindow*)FUI)->octonetwork.ConnectState = "Error";
    }
    if(QSysInfo::productType() != "raspbian")
    {
        LibLog::LogRec(QString("statusCode : %1___%2___%3").arg(statusCode).arg(__FILE__).arg(__LINE__), "connect");
        LibLog::LogRec("***************************************", "connect");
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
//    ui->Btn_Save->setStyleSheet(StyleSheet_Temp);
}

void MaterialCtrlPanel::on_Btn_Hotend_A_pressed()
{
    SettingFlag = true;
    StyleSheet_Temp = ui->Btn_Hotend_A->styleSheet();
    ui->Btn_Hotend_A->setStyleSheet(StyleSheet_Temp+"\nbackground-color:rgb(128,128,128,128);");
    timer = new mTimer(&Hotend_Target,qobject_cast<QPushButton *>(sender()),1);
    MaterialCtrlPanel::longPress(&Hotend_Max);


}

void MaterialCtrlPanel::on_Btn_Hotend_A_released()
{
    timer->stop();
    timer->disconnect();
    timerStartFlag = false;
    firstClickedFlag = false;
    ui->Btn_Hotend_A->setStyleSheet(StyleSheet_Temp);
}

void MaterialCtrlPanel::on_Btn_Hotend_D_pressed()
{
    SettingFlag = true;
    StyleSheet_Temp = ui->Btn_Hotend_D->styleSheet();
    ui->Btn_Hotend_D->setStyleSheet(StyleSheet_Temp+"\nbackground-color:rgb(128,128,128,128);");
    timer = new mTimer(&Hotend_Target,qobject_cast<QPushButton *>(sender()),-1);
    MaterialCtrlPanel::longPress(&Hotend_Max);

}

void MaterialCtrlPanel::on_Btn_Hotend_D_released()
{
    timer->stop();
    timer->disconnect();
    timerStartFlag = false;
    firstClickedFlag = false;
    ui->Btn_Hotend_D->setStyleSheet(StyleSheet_Temp);
}

void MaterialCtrlPanel::on_Btn_Heatbed_A_pressed()
{
    SettingFlag = true;
    StyleSheet_Temp = ui->Btn_Heatbed_A->styleSheet();
    ui->Btn_Heatbed_A->setStyleSheet(StyleSheet_Temp+"\nbackground-color:rgb(128,128,128,128);");
    timer = new mTimer(&Heatbed_Target,qobject_cast<QPushButton *>(sender()),1);
    MaterialCtrlPanel::longPress(&Heatbed_Max);
}

void MaterialCtrlPanel::on_Btn_Heatbed_A_released()
{
    timer->stop();
    timer->disconnect();
    timerStartFlag = false;
    firstClickedFlag = false;

    ui->Btn_Heatbed_A->setStyleSheet(StyleSheet_Temp);
}

void MaterialCtrlPanel::on_Btn_Heatbed_D_pressed()
{
    SettingFlag = true;

    StyleSheet_Temp = ui->Btn_Heatbed_D->styleSheet();
    ui->Btn_Heatbed_D->setStyleSheet(StyleSheet_Temp+"\nbackground-color:rgb(128,128,128,128);");

    timer = new mTimer(&Heatbed_Target,qobject_cast<QPushButton *>(sender()),-1);

    MaterialCtrlPanel::longPress(&Heatbed_Max);
}

void MaterialCtrlPanel::on_Btn_Heatbed_D_released()
{
    timer->stop();
    timer->disconnect();
    timerStartFlag = false;
    firstClickedFlag = false;
    ui->Btn_Heatbed_D->setStyleSheet(StyleSheet_Temp);
}

void MaterialCtrlPanel::on_Btn_Feedrate_A_pressed()
{
    SettingFlag = true;
    StyleSheet_Temp = ui->Btn_Feedrate_A->styleSheet();
    ui->Btn_Feedrate_A->setStyleSheet(StyleSheet_Temp+"\nbackground-color:rgb(128,128,128,128);");
    timer = new mTimer(&Feedrate_Target,qobject_cast<QPushButton *>(sender()),1);

    MaterialCtrlPanel::longPress(&Feedrate_Max);
}

void MaterialCtrlPanel::on_Btn_Feedrate_A_released()
{
    timer->stop();
    timer->disconnect();
    timerStartFlag = false;
    firstClickedFlag = false;
    ui->Btn_Feedrate_A->setStyleSheet(StyleSheet_Temp);
}

void MaterialCtrlPanel::on_Btn_Feedrate_D_pressed()
{
    SettingFlag = true;
    StyleSheet_Temp = ui->Btn_Feedrate_D->styleSheet();
    ui->Btn_Feedrate_D->setStyleSheet(StyleSheet_Temp+"\nbackground-color:rgb(128,128,128,128);");
    timer = new mTimer(&Feedrate_Target,qobject_cast<QPushButton *>(sender()),-1);
    MaterialCtrlPanel::longPress(&Feedrate_Max);
}

void MaterialCtrlPanel::on_Btn_Feedrate_D_released()
{
    timer->stop();
    timer->disconnect();
    timerStartFlag = false;
    firstClickedFlag = false;
    ui->Btn_Feedrate_D->setStyleSheet(StyleSheet_Temp);
}

void MaterialCtrlPanel::on_Btn_Flowrate_A_pressed()
{
    SettingFlag = true;
    StyleSheet_Temp = ui->Btn_Flowrate_A->styleSheet();
    ui->Btn_Flowrate_A->setStyleSheet(StyleSheet_Temp+"\nbackground-color:rgb(128,128,128,128);");
    timer = new mTimer(&Flowrate_Target,qobject_cast<QPushButton *>(sender()),1);

    MaterialCtrlPanel::longPress(&Flowrate_Max);
}

void MaterialCtrlPanel::on_Btn_Flowrate_A_released()
{
    timer->stop();
    timer->disconnect();
    timerStartFlag = false;
    firstClickedFlag = false;
    ui->Btn_Flowrate_A->setStyleSheet(StyleSheet_Temp);
}

void MaterialCtrlPanel::on_Btn_Flowrate_D_pressed()
{
    SettingFlag = true;

    StyleSheet_Temp = ui->Btn_Flowrate_D->styleSheet();
    ui->Btn_Flowrate_D->setStyleSheet(StyleSheet_Temp+"\nbackground-color:rgb(128,128,128,128);");


    timer = new mTimer(&Flowrate_Target,qobject_cast<QPushButton *>(sender()),-1);

    MaterialCtrlPanel::longPress(&Flowrate_Max);
}

void MaterialCtrlPanel::on_Btn_Flowrate_D_released()
{
    timer->stop();
    timer->disconnect();
    timerStartFlag = false;
    firstClickedFlag = false;
    ui->Btn_Flowrate_D->setStyleSheet(StyleSheet_Temp);
}

void MaterialCtrlPanel::longPress(const double *maximumCount)
{
    //设置长按效果--12.25
    timerStartFlag = true;
    if(timerStartFlag)
    {
        connect(timer, &mTimer::mtimeout, this, [=](){
            if(timer->addval > 0)
            {
                if(*(timer->value) + timer->addval < *maximumCount)
                    *(timer->value) += timer->addval;
                else
                    *(timer->value) = *maximumCount;
            }
            else
            {
                if(*(timer->value) + timer->addval > 0)
                    *(timer->value) += timer->addval;
                else
                    *(timer->value) = 0;
            }
            *timer->value = (int)*(timer->value);
        });
    }

    //第一次点击
    if(firstClickedFlag == false)
    {
        if(timer->addval > 0)
        {
            if(*(timer->value) + timer->addval < *maximumCount)
            {
                *(timer->value) += timer->addval;
            }
            else
                *(timer->value) = *maximumCount;
        }
        else
        {
            if(*(timer->value) + timer->addval > 0)
                *(timer->value) += timer->addval;
            else
                *(timer->value) = 0;
        }
        *(timer->value) = (int)*(timer->value);

        firstClickedFlag = true;

        timer->start(100);
    }
}

