#include "gcodedialog.h"
#include "ui_gcodedialog.h"
#include <QTreeWidgetItem>
#include <QScrollBar>
#include <QDebug>
//#include <QSizeF>
#include <QTimer>
#include <QStringList>
#include <mainwindow.h>
#include <QMessageBox>
#include <QTextCodec>
#include "inputdialog.h"

#include <QWebSocket>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>

GcodeDialog::GcodeDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::GcodeDialog)
{
    ui->setupUi(this);

    FUI = (MainWindow*)parent;//((MainWindow*)FUI)->octonetwork.SendGCode(_GCode_Setting);

    ui->Receiver->verticalScrollBar()->setStyleSheet("QScrollBar:vertical"
                                                       "{"
                                                       "width:40px;"
                                                       "background:rgba(0,128,255,0%);"
                                                       "margin:0px,0px,0px,0px;"
                                                       "padding-top:40px;"
                                                       "padding-bottom:40px;"
                                                       "}"
                                                       "QScrollBar::handle:vertical"
                                                       "{"
                                                       "width:40px;"
                                                       "background:rgba(0,128,255,25%);"
                                                       " border-radius:10px;"
                                                       "min-height:40;"
                                                       "}"
                                                       "QScrollBar::handle:vertical:hover"
                                                       "{"
                                                       "width:40px;"
                                                       "background:rgba(0,128,255,50%);"
                                                       " border-radius:10px;"
                                                       "min-height:40;"
                                                       "}"
                                                       "QScrollBar::add-page:vertical,QScrollBar::sub-page:vertical"
                                                       "{"
                                                       "background:rgba(0,128,255,10%);"
                                                       "border-radius:0px;"
                                                       "}");
    ui->Receiver->horizontalScrollBar()->setStyleSheet("QScrollBar:horizontal"
                                                         "{"
                                                         "height:40px;"
                                                         "background:rgba(0,128,255,0%);"
                                                         "margin:0px,0px,0px,0px;"
                                                         "padding-left:40px;"
                                                         "padding-right:40px;"
                                                         "}"
                                                         "QScrollBar::handle:horizontal"
                                                         "{"
                                                         "height:40x;"
                                                         "background:rgba(0,128,255,25%);"
                                                         " border-radius:10px;"
                                                         "min-width:40;"
                                                         "}"
                                                         "QScrollBar::handle:horizontal:hover"
                                                         "{"
                                                         "height:40px;"
                                                         "background:rgba(0,128,255,50%);"
                                                         " border-radius:10px;"
                                                         "min-width:40;"
                                                         "}"
                                                         "QScrollBar::add-page:horizontal,QScrollBar::sub-page:horizontal"
                                                         "{"
                                                         "background:rgba(0,128,255,10%);"
                                                         "border-radius:0px;"
                                                         "}");

    ui->Sender->setFocus();

    this->resize((int)(SizePercent.width()*800),(int)(SizePercent.height()*480));
    this->setMaximumSize((int)(SizePercent.width()*800),(int)(SizePercent.height()*480));
    this->setFixedSize((int)(SizePercent.width()*800),(int)(SizePercent.height()*480));

    _ptimer = new QTimer();
    connect(_pdataRecvWS,SIGNAL(disconnected()),this,SLOT(onWSDisconnected()),Qt::AutoConnection);
    connect(_pdataRecvWS,SIGNAL(textMessageReceived(QString)),this,SLOT(onWSTextReceived(QString)),Qt::AutoConnection);
    connect(_pdataRecvWS,SIGNAL(connected()),this,SLOT(onWSConnected()),Qt::AutoConnection);
    connect(_ptimer,SIGNAL(timeout()),this,SLOT(onWSReconnect()),Qt::AutoConnection);
    _pdataRecvWS->open(QUrl("ws://"+((MainWindow*)FUI)->octonetwork.MainIP+"/sockjs/websocket"));
//    _pdataRecvWS->open(QUrl("ws://192.168.100.111/sockjs/websocket"));
}

GcodeDialog::~GcodeDialog()
{
    delete ui;
}

void GcodeDialog::showEvent(QShowEvent *event)
{
    Q_UNUSED(event);
}

void GcodeDialog::on_Btn_Back_clicked()
{
    this->close();
}

void GcodeDialog::on_Btn_Send_clicked()
{
    if(ui->Sender->text().isEmpty())
    {
//        ui->Sender->setFocus();
        return;
    }
    ui->Sender->setText(ui->Sender->text().trimmed());

//    SendCMD(ui->Sender->text());
    ((MainWindow*)FUI)->octonetwork.SendGCode(ui->Sender->text());

    ui->Sender->setFocus();
}

void GcodeDialog::on_pushButton_clicked()
{
    ui->Receiver->clear();
}

void GcodeDialog::on_Btn_Logo_clicked()
{

}


//断开连接会触发这个槽函数
void GcodeDialog::onWSDisconnected()
{
    _ptimer->start();
}
//连接成功会触发这个槽函数
void GcodeDialog::onWSConnected()
{
    _ptimer->stop();
}
//收到服务发来的消息会触发这个槽函数
void GcodeDialog::onWSTextReceived(QString msg)
{
//    msg = msg.replace("\\","\"");
    if(!msg.trimmed().startsWith("{\"current\":") || msg.trimmed().length() == 0)
        return;
    QJsonDocument loadDoc(QJsonDocument::fromJson(msg.toUtf8()));
    QJsonObject QObj = loadDoc.object();
    QList<QString> SearchLink ;
    SearchLink << "current" << "logs";
    foreach(QJsonValue _value, SearchWSJsonValue(SearchLink , QObj).toArray())
    {
        ui->Receiver->append(_value.toString());
        //        recveData(_value.toString());
    }
    //    recveData(SearchWSJsonValue(SearchLink , QObj)[0].toString());
    //    recvData(SearchJsonValue(SearchLink , QObj).toString());
}
//断开连接会启动定时器，触发这个槽函数重新连接
void GcodeDialog::onWSReconnect()
{
    _pdataRecvWS->abort();
    _pdataRecvWS->open(QUrl("ws://"+((MainWindow*)FUI)->octonetwork.MainIP+"/sockjs/websocket"));
}


//Find_List_Name : Retrieved List Sequence --> [Grandma,Mother,Me]
//_QObj:Retrieved Retrieved Json Objecet
QJsonValue GcodeDialog::SearchWSJsonValue(QList<QString> Find_List_Name , QJsonObject _QObj)
{
    QJsonValue returnValue;
    QList<QString> SearchLink ;
    SearchLink << Find_List_Name;
    if(SearchLink.count() > 1)
    {
        QString MotherKey = SearchLink[0];
        SearchLink.removeFirst();
        if(MotherKey.indexOf("*") >= 0)//wildcard
        {
            QJsonArray ArrayValue;
            for(int i = 0 ; i <= 9 ; i++)
            {
                QString TempMotherKey = MotherKey;
                if(_QObj.find(TempMotherKey.replace("*",QString::number(i))).value().isUndefined())continue;
                //                if(QSysInfo::productType() != "raspbian"){qDebug()<<_QObj.find(TempMotherKey.replace("*",QString::number(i))).value().isUndefined();};
                QJsonValue data = SearchWSJsonValue(SearchLink , _QObj.constFind(TempMotherKey.replace("*",QString::number(i))).value().toObject());
                ArrayValue.append(data);
            }
            returnValue = ArrayValue;
        }
        else
            returnValue = SearchWSJsonValue(SearchLink , _QObj.constFind(MotherKey).value().toObject());
    }
    else
    {
        if(SearchLink[0].indexOf("*") >= 0)//wildcard
        {

        }
        else
            returnValue = _QObj.constFind(SearchLink[0]).value();
    }
    return returnValue;
}
QList<QJsonValue> GcodeDialog::SearchWSJsonValue(QList<QList<QString>> Find_List_Names , QJsonObject _QObj)
{
    QList<QJsonValue> _returnValue ;
    foreach(QList<QString> item , Find_List_Names)
    {
        _returnValue.append(SearchWSJsonValue(item,_QObj));
    }
    return _returnValue;
}
