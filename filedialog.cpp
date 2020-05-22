#include "filedialog.h"
#include "ui_filedialog.h"
//MY Dialog
#include <mainwindow.h>
#include <QNetworkReply>
#include <QJsonArray>
#include <QTreeWidgetItem>
#include <QScrollBar>
#include <QMessageBox>
#include <QThread>
//#include <QSizeF>

FileDialog::FileDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::FileDialog)
{
    ui->setupUi(this);

    FUI = (MainWindow*)parent;

//    SelectURL = QUrl(((MainWindow*)FUI)->octonetwork.MainUrl + "files/local/Demo.gcode");

    //Init File
//    QObject::connect(((MainWindow*)FUI)->octonetwork.FSnetworkAccessManager, SIGNAL(finished(QNetworkReply*)), this, SLOT(FSReply(QNetworkReply*)),Qt::UniqueConnection);
//    ((MainWindow*)FUI)->octonetwork.FSnetworkAccessManager->get(((MainWindow*)FUI)->octonetwork.FileRequest);

    ui->treeWidget->verticalScrollBar()->setStyleSheet("QScrollBar:vertical"
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
    ui->treeWidget->horizontalScrollBar()->setStyleSheet("QScrollBar:horizontal"
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
    terminal_timer = new QTimer(this);

    terminal_timer->setSingleShot(true);

    this->resize((int)(SizePercent.width()*800),(int)(SizePercent.height()*480));
    this->setMaximumSize((int)(SizePercent.width()*800),(int)(SizePercent.height()*480));
    this->setFixedSize((int)(SizePercent.width()*800),(int)(SizePercent.height()*480));
}

FileDialog::~FileDialog()
{
    delete ui;
}

void FileDialog::showEvent(QShowEvent *event)
{
    Q_UNUSED(event);
    RefreshFileList();
}

void FileDialog::on_Btn_back_clicked()
{
    this->close();
}

void FileDialog::on_Btn_Refresh_pressed()
{
    StyleSheet_Temp = ui->Btn_Refresh->styleSheet();
    ui->Btn_Refresh->setStyleSheet(StyleSheet_Temp+"\nbackground-color:rgb(128,128,128,128);");
}

void FileDialog::on_Btn_Refresh_released()
{
    ui->Btn_Refresh->setStyleSheet(StyleSheet_Temp);
}

void FileDialog::RefreshFileList()
{
    ReFreshJson();

    disconnect(((MainWindow*)FUI)->terminaldialog,&TerminalDialog::CMD_Reply,0,0);
//    QObject::connect(((MainWindow*)FUI)->terminaldialog,&TerminalDialog::CMD_Reply,this,SLOT(GetDirlist(QStringList)));
    QObject::connect(((MainWindow*)FUI)->terminaldialog,&TerminalDialog::CMD_Reply,this,[=](QStringList CommandLine)
    {
        GetDirlist(CommandLine);
//        disconnect(terminal_timer,SIGNAL(timeout()),0,0);
//        terminal_timer->stop();
//        DirList.clear();
//        foreach(QString Dir , CommandLine)
//        {
//         DirList.append(Dir.split("\t", QString::SkipEmptyParts));
//        }
//        disconnect(((MainWindow*)FUI)->terminaldialog,&TerminalDialog::CMD_Reply,0,0);
//        //Test
////        QMessageBox::information(NULL, "Warning", "GetDirlist", QMessageBox::Ok, QMessageBox::Ok);

////        QObject::connect(((MainWindow*)FUI)->terminaldialog,&TerminalDialog::CMD_Reply,this,SLOT(GetDevlist(QStringList)));
//        QObject::connect(((MainWindow*)FUI)->terminaldialog,&TerminalDialog::CMD_Reply,this,[=](QStringList CommandLine)
//        {
//            disconnect(terminal_timer,SIGNAL(timeout()),0,0);
//            terminal_timer->stop();

//            DevList.clear();
//            foreach(QString Dev , CommandLine)
//            {
//                DevList.append(Dev.split("\t", QString::SkipEmptyParts));
//            }

//            disconnect(((MainWindow*)FUI)->terminaldialog,&TerminalDialog::CMD_Reply,0,0);

//            RmList.clear();
//            foreach(QString Dir , DirList)
//            {
//                if(!DevList.contains(Dir))
//                    RmList.append(Dir);
//            }
//            NewList.clear();
//            foreach(QString Dev , DevList)
//            {
//                if(!DirList.contains(Dev))
//                    NewList.append(Dev);
//            }

//            if(!RmList.isEmpty())
//            {
//                QString RmCMD = "cd /home/pi/.octoprint/uploads ; sudo umount ";
//                RmCMD = RmCMD + RmList.join(" ") + " ; rm -rf " + RmList.join(" ") + " ; cd /home/pi";
//                ((MainWindow*)FUI)->terminaldialog->SendCMD(RmCMD,TeminalState);
//            }

//            if(!NewList.isEmpty())
//            {
//                QString NewCMD = "cd /home/pi/.octoprint/uploads  ;  mkdir " + NewList.join(" ");
//                foreach(QString Newitem , NewList)
//                {
//                    NewCMD = NewCMD + " ; sudo mount /dev/" + Newitem + " " + Newitem;
//                }
//                NewCMD = NewCMD + " ; cd /home/pi";
//                ((MainWindow*)FUI)->terminaldialog->SendCMD(NewCMD,TeminalState);
//                QMessageBox::information(NULL, "Tip", "Parsing the USB file, please refresh it later!", QMessageBox::Ok, QMessageBox::Ok);
//            }

//            if(!RmList.isEmpty() || !NewList.isEmpty())
//            {
//                QThread::msleep(500);
//                ReFreshJson();
//            }
//        });
//        ((MainWindow*)FUI)->terminaldialog->SendCMD("cd /dev/ && find [s,h]d[a-z][0-9] -type b ; cd /home/pi",TeminalState);

//        QObject::connect(terminal_timer,SIGNAL(timeout()),this,SLOT(GetDevlistTimeout()));
//        terminal_timer->start(1000);
    });
    ((MainWindow*)FUI)->terminaldialog->SendCMD("cd /home/pi/.octoprint/uploads && find [s,h]d[a-z]* -maxdepth 0 -type d -regex \"[s,h]d[a-z][0-9]+$\"; cd /home/pi",TeminalState);

    QObject::connect(terminal_timer,SIGNAL(timeout()),this,SLOT(GetDirlistTimeout()));
    terminal_timer->start(1000);
}

void FileDialog::ReFreshJson()
{
    //ReFresh Json
    ((MainWindow*)FUI)->octonetwork.SD_ReLoad();
    //Reload FileList
    //    if(((MainWindow*)FUI)->octonetwork.ConnectState == "Operational" || ((MainWindow*)FUI)->octonetwork.ConnectState == "Pausing" || ((MainWindow*)FUI)->octonetwork.ConnectState == "Starting" || ((MainWindow*)FUI)->octonetwork.ConnectState == "Printing" || ((MainWindow*)FUI)->octonetwork.ConnectState == "Cancelling")
    //    {}
    ((MainWindow*)FUI)->octonetwork.FSnetworkAccessManager->disconnect(SIGNAL(finished(QNetworkReply*)));//disconnect
    QObject::connect(((MainWindow*)FUI)->octonetwork.FSnetworkAccessManager, SIGNAL(finished(QNetworkReply*)), this, SLOT(FSReply(QNetworkReply*)),Qt::UniqueConnection);
    ((MainWindow*)FUI)->octonetwork.FSnetworkAccessManager->get(((MainWindow*)FUI)->octonetwork.FileRequest);
}

void FileDialog::on_Btn_Refresh_clicked()
{
    RefreshFileList();
}

void FileDialog::FSReply(QNetworkReply *reply)
{
    int statusCode = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();
    if(reply->error() == QNetworkReply::NoError)
    {
        QByteArray replyArray = reply->readAll();
        QJsonDocument loadDoc(QJsonDocument::fromJson(replyArray));
        QJsonObject QObj = loadDoc.object();
        //建立列表
        ui->treeWidget->clear();
        QTreeWidgetItem *RootItem = new QTreeWidgetItem(ui->treeWidget,QStringList(QString("Local")));
        RootItem->setIcon(0,QIcon(":/assets/wenjian.svg"));
        SDcardItem = new QTreeWidgetItem(ui->treeWidget,QStringList(QString("Board")));
        SDcardItem->setIcon(0,QIcon(":/assets/wenjian.svg"));
        FilesPraser(RootItem , QObj.constFind("files").value().toArray());

        QList<QTreeWidgetItem *> AllItems;
        AllItems.append(RootItem);
        AllItems.append(SDcardItem);
        ui->treeWidget->insertTopLevelItems(0,AllItems);
        ui->treeWidget->setItemsExpandable(true);
        ui->treeWidget->expandAll();

        if(QSysInfo::productType() != "raspbian"){qDebug()<<replyArray;}
    }
    if(QSysInfo::productType() != "raspbian")
    {
        qDebug() << "statusCode:" << statusCode;
        qDebug() << "***************************************";
    }
    reply->deleteLater();
    ((MainWindow*)FUI)->octonetwork.FSnetworkAccessManager->disconnect(SIGNAL(finished(QNetworkReply*)));//disconnect
}

void FileDialog::FilesPraser(QTreeWidgetItem *MotherItem , QJsonArray Data)
{
    //QList<QTreeWidgetItem *> _return;
    foreach(QJsonValue item,Data)
    {
        if(item.toObject().constFind("type").value().toString() == "machinecode" && item.toObject().constFind("origin").value().toString() == "local")
        {
            QStringList chilData;
            chilData.append(item.toObject().constFind("display").value().toString());
            chilData.append(item.toObject().constFind("refs").value().toObject().constFind("resource").value().toString());
            QTreeWidgetItem *chilItem = new QTreeWidgetItem(MotherItem,chilData);
            chilItem->setIcon(0,QIcon(":/assets/3ddayin.svg"));
            MotherItem->addChild(chilItem);
        }
        else if(item.toObject().constFind("type").value().toString() == "machinecode" && item.toObject().constFind("origin").value().toString() == "sdcard")
        {
            QStringList chilData;
            chilData.append(item.toObject().constFind("display").value().toString());
            chilData.append(item.toObject().constFind("refs").value().toObject().constFind("resource").value().toString());
            QTreeWidgetItem *chilItem = new QTreeWidgetItem(SDcardItem,chilData);
            chilItem->setIcon(0,QIcon(":/assets/3ddayin.svg"));
            SDcardItem->addChild(chilItem);
        }
        else if(item.toObject().constFind("type").value().toString() == "folder")
        {
            QStringList chilData;
            chilData.append(item.toObject().constFind("display").value().toString());
            QTreeWidgetItem *chilItem = new QTreeWidgetItem(MotherItem,chilData);
            if(item.toObject().constFind("display").value().toString().contains(QRegExp("[s,h]d[a-z][0-9]+")))
                chilItem->setIcon(0,QIcon(":/assets/usb.svg"));
            else
                chilItem->setIcon(0,QIcon(":/assets/wenjian.svg"));
            FilesPraser(chilItem , item.toObject().constFind("children").value().toArray());
            MotherItem->addChild(chilItem);
        }
    }
}

void FileDialog::on_treeWidget_itemClicked(QTreeWidgetItem *item, int column)
{
    if(QSysInfo::productType() != "raspbian")
    {
        qDebug()<<column<<":"<<item;
        qDebug()<<item->data(1,0).toString();
    }
    QString ext = item->data(1,0).toString().split('.').back();
    if(ext.toLower() == "gcode" || ext.toLower() == "gco" || ext.toLower() == "g")
        SelectURL = QUrl(item->data(1,0).toString());
}

//quick print
void FileDialog::on_treeWidget_itemDoubleClicked(QTreeWidgetItem *item, int column)
{
    Q_UNUSED(column);
    QString ext = item->data(1,0).toString().split('.').back();
    if(ext.toLower() == "gcode"  || ext.toLower() == "gco" || ext.toLower() == "g")
    {
        ((MainWindow*)FUI)->octonetwork.JobSwitch(SelectURL);
    }
}

void FileDialog::GetDirlist(QStringList CommandLine)
{
    disconnect(terminal_timer,SIGNAL(timeout()),0,0);
    terminal_timer->stop();
    DirList.clear();
    foreach(QString Dir , CommandLine)
    {
        DirList.append(Dir.split("\t", QString::SkipEmptyParts));
    }
    disconnect(((MainWindow*)FUI)->terminaldialog,&TerminalDialog::CMD_Reply,0,0);
//Test
//        QMessageBox::information(NULL, "Warning", "GetDirlist", QMessageBox::Ok, QMessageBox::Ok);

//        QObject::connect(((MainWindow*)FUI)->terminaldialog,&TerminalDialog::CMD_Reply,this,SLOT(GetDevlist(QStringList)));
    QObject::connect(((MainWindow*)FUI)->terminaldialog,&TerminalDialog::CMD_Reply,this,[=](QStringList CommandLine)
    {
        GetDevlist(CommandLine);
    });
    ((MainWindow*)FUI)->terminaldialog->SendCMD("cd /dev/ && find [s,h]d[a-z]* -maxdepth 0 -type b -regex \"[s,h]d[a-z][0-9]+$\"; cd /home/pi",TeminalState);

    QObject::connect(terminal_timer,SIGNAL(timeout()),this,SLOT(GetDevlistTimeout()));
    terminal_timer->start(1000);
}

void FileDialog::GetDirlistTimeout()
{
    DirList.clear();
    disconnect(((MainWindow*)FUI)->terminaldialog,&TerminalDialog::CMD_Reply,0,0);

//    QObject::connect(((MainWindow*)FUI)->terminaldialog,&TerminalDialog::CMD_Reply,this,SLOT(GetDevlist(QStringList)));
    QObject::connect(((MainWindow*)FUI)->terminaldialog,&TerminalDialog::CMD_Reply,this,[=](QStringList CommandLine)
    {
        GetDevlist(CommandLine);
    });
    ((MainWindow*)FUI)->terminaldialog->SendCMD("cd /dev/ && find [s,h]d[a-z]* -maxdepth 0 -type b -regex \"[s,h]d[a-z][0-9]+$\"; cd /home/pi",TeminalState);

    disconnect(terminal_timer,SIGNAL(timeout()),0,0);
    QObject::connect(terminal_timer,SIGNAL(timeout()),this,SLOT(GetDevlistTimeout()));
    terminal_timer->start(1000);
}

void FileDialog::GetDevlist(QStringList CommandLine)
{
    disconnect(terminal_timer,SIGNAL(timeout()),0,0);
    terminal_timer->stop();

    DevList.clear();
    foreach(QString Dev , CommandLine)
    {
        DevList.append(Dev.split("\t", QString::SkipEmptyParts));
    }

    disconnect(((MainWindow*)FUI)->terminaldialog,&TerminalDialog::CMD_Reply,0,0);

    RmList.clear();
    foreach(QString Dir , DirList)
    {
        if(!DevList.contains(Dir))
            RmList.append(Dir);
    }
    NewList.clear();
    foreach(QString Dev , DevList)
    {
        if(!DirList.contains(Dev))
            NewList.append(Dev);
    }

    if(!RmList.isEmpty())
    {
        QString RmCMD = "cd /home/pi/.octoprint/uploads ; sudo umount ";
        RmCMD = RmCMD + RmList.join(" ") + " ; rm -rf " + RmList.join(" ") + " ; cd /home/pi";
        ((MainWindow*)FUI)->terminaldialog->SendCMD(RmCMD,TeminalState);
    }

    if(!NewList.isEmpty())
    {
        QString NewCMD = "cd /home/pi/.octoprint/uploads  ;  mkdir " + NewList.join(" ");
        foreach(QString Newitem , NewList)
        {
            NewCMD = NewCMD + " ; sudo mount /dev/" + Newitem + " " + Newitem;
        }
        NewCMD = NewCMD + " ; cd /home/pi";
        ((MainWindow*)FUI)->terminaldialog->SendCMD(NewCMD,TeminalState);
        QMessageBox::information(NULL, "Tip", "Parsing the USB file, please refresh it later!", QMessageBox::Ok, QMessageBox::Ok);
    }

    if(!RmList.isEmpty() || !NewList.isEmpty())
    {
        QThread::msleep(500);
        ReFreshJson();
    }
}

void FileDialog::GetDevlistTimeout()
{
    DevList.clear();
    disconnect(((MainWindow*)FUI)->terminaldialog,&TerminalDialog::CMD_Reply,0,0);
    disconnect(terminal_timer,SIGNAL(timeout()),0,0);

    RmList.clear();
    foreach(QString Dir , DirList)
    {
        if(!DevList.contains(Dir))
            RmList.append(Dir);
    }
    NewList.clear();
    foreach(QString Dev , DevList)
    {
        if(!DirList.contains(Dev))
            NewList.append(Dev);
    }

    if(!RmList.isEmpty())
    {
        QString RmCMD = "cd /home/pi/.octoprint/uploads ; sudo umount ";
        RmCMD = RmCMD + RmList.join(" ") + " ; rm -rf " + RmList.join(" ") + " ; cd /home/pi";
        ((MainWindow*)FUI)->terminaldialog->SendCMD(RmCMD,TeminalState);
    }

    if(!NewList.isEmpty())
    {
        QString NewCMD = "cd /home/pi/.octoprint/uploads  ;  mkdir " + NewList.join(" ");
        foreach(QString Newitem , NewList)
        {
            NewCMD = NewCMD + " ; sudo mount /dev/" + Newitem + " " + Newitem;
        }
        NewCMD = NewCMD + " ; cd /home/pi";
        ((MainWindow*)FUI)->terminaldialog->SendCMD(NewCMD,TeminalState);
        QMessageBox::information(NULL, "Tip", "Parsing the USB file, please refresh it later!", QMessageBox::Ok, QMessageBox::Ok);
    }

    if(!RmList.isEmpty() || !NewList.isEmpty())
    {
        QThread::msleep(500);
        ReFreshJson();
    }
}
