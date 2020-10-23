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
#include <QDir>
#include <QHttpMultiPart>
#include <QDirIterator>
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

    QFont _font = ui->Btn_Upload->font();
    _font.setPointSize(this->maximumWidth() > this->maximumHeight() ? (int)this->maximumHeight()/30 : (int)this->maximumWidth()/30);
    ui->Btn_Upload->setFont(_font);
    ui->Btn_Print->setFont(_font);
    ui->Btn_Delete->setFont(_font);

    /* 创建一个用于应用程序发送网络请求和接收网络应答的"管理员" */
    manager = new QNetworkAccessManager(this);
    /* 创建一个要发送的请求实例 */
    request = new QNetworkRequest(QUrl(((MainWindow*)FUI)->octonetwork.MainUrl +"/files"));
    request->setRawHeader("X_Api_Key",((MainWindow*)FUI)->octonetwork.X_API_Key);

//    InitializeUSBDrive();
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
    ((MainWindow*)FUI)->terminaldialog->SendCMD("cd /home/pi/USB && find [s,h]d[a-z]* -maxdepth 0 -type d -regex \"[s,h]d[a-z][0-9]+$\"; cd /home/pi",TeminalState);

    QObject::connect(terminal_timer,SIGNAL(timeout()),this,SLOT(GetDirlistTimeout()));
    terminal_timer->start(500);
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

//        QTreeWidgetItem *USBItem = new QTreeWidgetItem(ui->treeWidget,QStringList(QString("USB")));
//        USBItem->setIcon(0,QIcon(":/assets/usb.svg"));
//        QList<QTreeWidgetItem *> *USBItem;

        SDcardItem = new QTreeWidgetItem(ui->treeWidget,QStringList(QString("Board")));
        SDcardItem->setIcon(0,QIcon(":/assets/wenjian.svg"));

        FilesPraser(RootItem , QObj.constFind("files").value().toArray());
        QCoreApplication::processEvents();//update Dir
        QList<QTreeWidgetItem *> USBDev = LoadUSBPath("/home/pi/USB");

        QList<QTreeWidgetItem *> AllItems;
        AllItems.append(RootItem);
        AllItems.append(USBDev);
        AllItems.append(SDcardItem);
        if(SDcardItem->childCount() <= 0)
        {
            SDcardItem->setHidden(true);
        }

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
    ui->Btn_Delete->setEnabled(false);
    ui->Btn_Print->setEnabled(false);
    ui->Btn_Upload->setEnabled(false);
    ((MainWindow*)FUI)->octonetwork.FSnetworkAccessManager->disconnect(SIGNAL(finished(QNetworkReply*)));//disconnect
}
QList<QTreeWidgetItem *> FileDialog::LoadUSBPath(QString USBPath)
{
    QList<QTreeWidgetItem *> USBDev;
    QDir RootDir = QDir(USBPath);
//    QMessageBox::information(NULL, "item.dir().path()", USBPath, QMessageBox::Ok, QMessageBox::Ok);
    if(RootDir.exists() && RootDir.entryInfoList(QDir::Dirs | QDir::NoDotAndDotDot).count() > 0)//文件夹存在，并且包含内容
    {
        QDirIterator dir_iterator(USBPath, QDir::Dirs | QDir::NoDotAndDotDot/*,QDirIterator::Subdirectories*/);
        while(dir_iterator.hasNext())
        {
            dir_iterator.next();
//            QMessageBox::information(NULL, "item.dir().path()", "LoadUSBPath", QMessageBox::Ok, QMessageBox::Ok);
//            QMessageBox::information(NULL, "item.dir().path()", dir_iterator.fileInfo().absoluteFilePath(), QMessageBox::Ok, QMessageBox::Ok);
//            dir_iterator.fileInfo().dir().path();
            QTreeWidgetItem *USBItem = new QTreeWidgetItem(ui->treeWidget,QStringList(QString(dir_iterator.fileName().toUpper())));
            USBItem->setIcon(0,QIcon(":/assets/usb.svg"));

//            QStringList chilData;
//            chilData.append(dir_iterator.fileName().toUpper());
//            QTreeWidgetItem *chilItem = new QTreeWidgetItem(MotherItem,chilData);
//            chilItem->setIcon(0,QIcon(":/assets/usb.svg"));
            LoadFiles(USBItem , dir_iterator.filePath());
            USBDev.append(USBItem);
//            MotherItem->addChild(chilItem);
        }
    }
    return USBDev;
}

void FileDialog::LoadFiles(QTreeWidgetItem *MotherItem , QString DirPath)
{

//    QMessageBox::information(NULL, "item.dir().path()", DirPath, QMessageBox::Ok, QMessageBox::Ok);
//    QMessageBox::information(NULL, "item.dir().path()", "LoadFiles", QMessageBox::Ok, QMessageBox::Ok);
    QDirIterator dir_iterator(DirPath,QDir::Files | QDir::Dirs | QDir::NoDotAndDotDot/*,QDirIterator::Subdirectories*/);
    while(dir_iterator.hasNext())
    {
        dir_iterator.next();
//        QFileInfo file_info = dir_iterator.fileInfo();
//        QString files = file_info.absoluteFilePath();
//        QMessageBox::information(NULL, "item.dir().path()", "LoadFiles", QMessageBox::Ok, QMessageBox::Ok);
//        QMessageBox::information(NULL, "item.dir().path()", dir_iterator.fileInfo().absoluteFilePath(), QMessageBox::Ok, QMessageBox::Ok);
//        QMessageBox::information(NULL, "item.dir().path()", dir_iterator.path(), QMessageBox::Ok, QMessageBox::Ok);
//        QMessageBox::information(NULL, "item.dir().path()", dir_iterator.fileName(), QMessageBox::Ok, QMessageBox::Ok);
//        QMessageBox::information(NULL, "item.dir().path()", dir_iterator.filePath(), QMessageBox::Ok, QMessageBox::Ok);
        if(dir_iterator.fileInfo().isDir())
        {
            if(dir_iterator.fileInfo().fileName() == "System Volume Information")
                continue;
            QStringList chilData;
            chilData.append(dir_iterator.fileName());
            QTreeWidgetItem *chilItem = new QTreeWidgetItem(MotherItem,chilData);
            chilItem->setIcon(0,QIcon(":/assets/wenjian.svg"));
            LoadFiles(chilItem , dir_iterator.filePath());
            MotherItem->addChild(chilItem);
        }
        else if(dir_iterator.fileInfo().isFile())
        {
            QStringList chilData;
            chilData.append(dir_iterator.fileName());
            chilData.append(dir_iterator.filePath());
            QTreeWidgetItem *chilItem = new QTreeWidgetItem(MotherItem,chilData);

            QString ext = dir_iterator.fileInfo().suffix().toLower();
            if(ext == "gco" || ext == "gcode" || ext == "g")//machinecode
            {
                chilItem->setIcon(0,QIcon(":/assets/3ddayin.svg"));
            }
            else if(ext == "stl" || ext == "obj" )//model object
            {
                chilItem->setIcon(0,QIcon(":/assets/object.svg"));
            }
            else
            {
                chilItem->setIcon(0,QIcon(":/assets/rizhi.svg"));
            }
            MotherItem->addChild(chilItem);
        }
    }
}

void FileDialog::FilesPraser(QTreeWidgetItem *MotherItem , QJsonArray Data)
{
    //QList<QTreeWidgetItem *> _return;
    foreach(QJsonValue item,Data)
    {
        if(item.toObject().constFind("origin").value().toString() == "local")
        {
            QStringList chilData;
            chilData.append(Hex2QString(item.toObject().constFind("display").value().toString()));
            chilData.append(item.toObject().constFind("refs").value().toObject().constFind("resource").value().toString());
            QTreeWidgetItem *chilItem = new QTreeWidgetItem(MotherItem,chilData);
            chilItem->setIcon(0,
                              item.toObject().constFind("type").value().toString() == "machinecode" ? QIcon(":/assets/3ddayin.svg") :
                              item.toObject().constFind("type").value().toString() == "model" ? QIcon(":/assets/object.svg") : QIcon(":/assets/rizhi.svg"));
            MotherItem->addChild(chilItem);
        }
        else if(item.toObject().constFind("origin").value().toString() == "sdcard")
        {
            QStringList chilData;
            chilData.append(Hex2QString(item.toObject().constFind("display").value().toString()));
            chilData.append(item.toObject().constFind("refs").value().toObject().constFind("resource").value().toString());
            QTreeWidgetItem *chilItem = new QTreeWidgetItem(SDcardItem,chilData);
            chilItem->setIcon(0,
                              item.toObject().constFind("type").value().toString() == "machinecode" ? QIcon(":/assets/3ddayin.svg") :
                              item.toObject().constFind("type").value().toString() == "model" ? QIcon(":/assets/object.svg") : QIcon(":/assets/rizhi.svg"));
            SDcardItem->addChild(chilItem);
        }
        else if(item.toObject().constFind("type").value().toString() == "folder")
        {
            QStringList chilData;
            chilData.append(item.toObject().constFind("display").value().toString());
            QTreeWidgetItem *chilItem = new QTreeWidgetItem(MotherItem,chilData);
            chilItem->setIcon(0,QIcon(":/assets/wenjian.svg"));
//            if(item.toObject().constFind("display").value().toString().contains(QRegExp("[s,h]d[a-z][0-9]+")))
//                chilItem->setIcon(0,QIcon(":/assets/usb.svg"));
//            else
//                chilItem->setIcon(0,QIcon(":/assets/wenjian.svg"));
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

    if(((MainWindow*)FUI)->octonetwork.ConnectState.contains("Printing") ||((MainWindow*)FUI)->octonetwork.ConnectState == "Starting print from SD" || ((MainWindow*)FUI)->octonetwork.ConnectState == "Starting")
        return;
    QString ext = item->data(1,0).toString().split('.').last();
    if(ext.toLower() == "gcode" || ext.toLower() == "gco" || ext.toLower() == "g")
    {
        SelectURL = QUrl(item->data(1,0).toString());
        DisplayName = item->data(0,0).toString();
        ui->Btn_Delete->setEnabled(true);
        ui->Btn_Print->setEnabled(true);
        if(!SelectURL.url().startsWith("http://"))
            ui->Btn_Upload->setEnabled(true);
        else
            ui->Btn_Upload->setEnabled(false);
    }
    else
    {
        ui->Btn_Delete->setEnabled(false);
        ui->Btn_Print->setEnabled(false);
        ui->Btn_Upload->setEnabled(false);
    }
}

//quick print
void FileDialog::on_treeWidget_itemDoubleClicked(QTreeWidgetItem *item, int column)
{
    Q_UNUSED(column);
    QString ext = item->data(1,0).toString().split('.').last();
    if(ext.toLower() == "gcode"  || ext.toLower() == "gco" || ext.toLower() == "g")
    {
        ((MainWindow*)FUI)->octonetwork.JobSwitch(SelectURL);
        this->close();
    }
}

QUrl FileDialog::GetRealURL(QUrl URL)
{
    QUrl _URL;
    if(!URL.url().startsWith("http://"))
    {
//        QString newFileName = QString2Hex(URL.url() , false);
//        //构建新文件名称
//        if(!QDir("/home/pi").exists("temp"))
//            QDir("/home/pi").mkdir("temp");
//        if(QFile::exists("/home/pi/temp/"+newFileName))
//            QFile::remove("/home/pi/temp/"+newFileName);
//        //检查同名堆积
//        QFile::copy(URL.url(),"/home/pi/temp/"+newFileName);
//        //复制外部路径文件到临时文件夹
//        _URL = UploadFile("/home/pi/temp/"+newFileName);
//        //上传临时文件至Octoprint并索取真实URL
//        QFile::remove("/home/pi/temp/"+newFileName);
//        //删除临时文件

        _URL = UploadFile(URL.url());
    }
    else
    {
        _URL = URL;
    }
    return _URL;
}

void FileDialog::DeleteURL(QUrl URL)
{
    QString FileName = Hex2QString(URL.url(),false);
    if(QMessageBox::information(NULL, "Warning", "Do you want to Delete "+FileName+"?", QMessageBox::Yes  | QMessageBox::No , QMessageBox::No) == QMessageBox::Yes)
    {
        if(URL.url().toLower().startsWith("http://"))
        {
            request->setUrl(URL);
            if(manager->deleteResource(*request)->error() == QNetworkReply::NoError)
                RefreshFileList();
        }
        else
        {
            DeleteFile(URL.url());
        }
        SelectURL.setUrl("");
    }
}

void FileDialog::DeleteFile(QString FilePath)
{
    if(QFile::exists(FilePath))
    {
        QFile::remove(FilePath);
        RefreshFileList();
    }
}

QUrl FileDialog::UploadFile(QString FilePath)
{
//    QMessageBox::information(NULL, "FilePath", "FilePath:" +FilePath, QMessageBox::Ok, QMessageBox::Ok);//Debug
    QUrl _return;
    //设置发送的数据
    QHttpMultiPart *multiPart = new QHttpMultiPart(QHttpMultiPart::FormDataType);

    QFile *file = new QFile(FilePath);      //由于是异步，所以记得一定要new
    file->open(QIODevice::ReadOnly);

    QHttpPart contentPart;
    contentPart.setHeader(QNetworkRequest::ContentDispositionHeader, QVariant(QString("form-data; name=\"file\"; filename=\"%1\"").arg(QFileInfo(FilePath).fileName())));
    contentPart.setBodyDevice(file);
    file->setParent(multiPart);
    multiPart->append(contentPart);

    request->setUrl(QUrl(((MainWindow*)FUI)->octonetwork.MainUrl +"/files/local"));

    QNetworkReply *reply = manager->post(*request,multiPart);

    while(!reply->isFinished())
        QCoreApplication::processEvents();//update http post

//    QMessageBox::information(NULL, "item.dir().path()", "reply statusCode:" + QString::number(reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt()), QMessageBox::Ok, QMessageBox::Ok);//Debug
    file->close();
    if(reply->error() == QNetworkReply::NoError)
    {
        QByteArray replyArray = reply->readAll();
        QJsonDocument loadDoc(QJsonDocument::fromJson(replyArray));
        QJsonObject QObj = loadDoc.object();

        QList<QString> ResourcesLocation = {"files","local","refs","resource"};
        _return = QUrl(((MainWindow*)FUI)->octonetwork.SearchJsonValue( ResourcesLocation, QObj).toString());
    }
    reply->deleteLater();
    return _return;
}

QUrl FileDialog::GetRealURL()
{
    return GetRealURL(SelectURL);
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

//        QObject::connect(((MainWindow*)FUI)->terminaldialog,&TerminalDialog::CMD_Reply,this,SLOT(GetDevlist(QStringList)));
    QObject::connect(((MainWindow*)FUI)->terminaldialog,&TerminalDialog::CMD_Reply,this,[=](QStringList CommandLine)
    {
        GetDevlist(CommandLine);
    });
    ((MainWindow*)FUI)->terminaldialog->SendCMD("cd /dev/ && find [s,h]d[a-z]* -maxdepth 0 -type b -regex \"[s,h]d[a-z][0-9]+$\"; cd /home/pi",TeminalState);

    QObject::connect(terminal_timer,SIGNAL(timeout()),this,SLOT(GetDevlistTimeout()));
    terminal_timer->start(500);
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
    terminal_timer->start(500);
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
        QString RmCMD = "cd /home/pi/USB ; sudo umount ";
//        RmCMD = RmCMD + RmList.join(" ") + " ; rm -rf " + RmList.join(" ") + " ; cd /home/pi";
        RmCMD = RmCMD + RmList.join(" ")  + " ; cd /home/pi";
//        QMessageBox::information(NULL, "item.dir().path()", "GetDevlist:" + RmCMD, QMessageBox::Ok, QMessageBox::Ok);//Debug
        ((MainWindow*)FUI)->terminaldialog->SendCMD(RmCMD,TeminalState);
        QCoreApplication::processEvents();
        QThread::msleep(500);
        QCoreApplication::processEvents();

        RmCMD = "cd /home/pi/USB ; rm -rf " + RmList.join(" ") + " ; cd /home/pi";
        ((MainWindow*)FUI)->terminaldialog->SendCMD(RmCMD,TeminalState);
//        QMessageBox::information(NULL, "item.dir().path()", "GetDevlistTimeout:" + RmCMD, QMessageBox::Ok, QMessageBox::Ok);//Debug
        QCoreApplication::processEvents();
        QThread::msleep(500);
        QCoreApplication::processEvents();

        if(!SelectURL.url().startsWith("http://"))
        {
            SelectURL = QUrl();
        }
    }

    if(!NewList.isEmpty())//sudo mount -o uid=pi,gid=pi /dev/sda1 /mnt/1GB_USB_flash
    {
        QString NewCMD = "";
        QDir dir("/home/pi/USB");//Load USB Path
        if(QSysInfo::productType() == "raspbian" && !dir.exists())
        {
            NewCMD = "mkdir /home/pi/USB ; ";
        }
        NewCMD = NewCMD + "cd /home/pi/USB ; mkdir " + NewList.join(" ");
        foreach(QString Newitem , NewList)
        {
            NewCMD = NewCMD + " ; sudo mount -o uid=pi,gid=pi,utf8=true,iocharset=utf8 /dev/" + Newitem + " " + Newitem;//字符集编码异常Debug文件夹被破坏
        }
        NewCMD = NewCMD + " ; cd /home/pi";

//        QMessageBox::information(NULL, "item.dir().path()", "GetDevlist:" + NewCMD, QMessageBox::Ok, QMessageBox::Ok);//Debug
        ((MainWindow*)FUI)->terminaldialog->SendCMD(NewCMD,TeminalState);
        QCoreApplication::processEvents();
        QThread::msleep(500);
        QCoreApplication::processEvents();
//        QMessageBox::information(NULL, "Tip", "Mount the USB drive, please refresh it later!", QMessageBox::Ok, QMessageBox::Ok);
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
        QString RmCMD = "cd /home/pi/USB ; sudo umount ";
//        RmCMD = RmCMD + RmList.join(" ") + " ; rm -rf " + RmList.join(" ") + " ; cd /home/pi";
        RmCMD = RmCMD + RmList.join(" ")  + " ; cd /home/pi";
//        QMessageBox::information(NULL, "item.dir().path()", "GetDevlistTimeout:" + RmCMD, QMessageBox::Ok, QMessageBox::Ok);//Debug
        ((MainWindow*)FUI)->terminaldialog->SendCMD(RmCMD,TeminalState);
        QCoreApplication::processEvents();
        QThread::msleep(500);
        QCoreApplication::processEvents();

        RmCMD = "cd /home/pi/USB ; rm -rf " + RmList.join(" ") + " ; cd /home/pi";
//        QMessageBox::information(NULL, "item.dir().path()", "GetDevlistTimeout:" + RmCMD, QMessageBox::Ok, QMessageBox::Ok);//Debug
        ((MainWindow*)FUI)->terminaldialog->SendCMD(RmCMD,TeminalState);
        QCoreApplication::processEvents();
        QThread::msleep(500);
        QCoreApplication::processEvents();

        if(!SelectURL.url().startsWith("http://"))
        {
            SelectURL = QUrl();
        }
    }

    if(!NewList.isEmpty())
    {
        QString NewCMD = "";
        QDir dir("/home/pi/USB");//Load USB Path
        if(QSysInfo::productType() == "raspbian" && !dir.exists())
        {
            NewCMD = "mkdir /home/pi/USB ; ";
        }
        NewCMD = NewCMD + "cd /home/pi/.octoprint/uploads  ;  mkdir " + NewList.join(" ");
        foreach(QString Newitem , NewList)
        {
            NewCMD = NewCMD + " ; sudo mount -o uid=pi,gid=pi,utf8=true,iocharset=utf8 /dev/" + Newitem + " " + Newitem;
            //字符集编码异常（iocharset=cp936,codepage=cp936） posix=true utf8=true uid=pi,gid=pi nls=utf8
        }
        NewCMD = NewCMD + " ; cd /home/pi";
//        QMessageBox::information(NULL, "item.dir().path()", "GetDevlistTimeout:" + NewCMD, QMessageBox::Ok, QMessageBox::Ok);//Debug
        ((MainWindow*)FUI)->terminaldialog->SendCMD(NewCMD,TeminalState);
        QCoreApplication::processEvents();
        QThread::msleep(500);
        QCoreApplication::processEvents();
//        QMessageBox::information(NULL, "Tip", "Mount the USB drive, please refresh it later!", QMessageBox::Ok, QMessageBox::Ok);
    }

    if(!RmList.isEmpty() || !NewList.isEmpty())
    {
//        QThread::msleep(500);
        ReFreshJson();
    }
}

void FileDialog::InitializeUSBDrive()
{
    QDir dir("/home/pi/USB");//Load USB Path
    if(QSysInfo::productType() == "raspbian" && dir.exists())
    {
        QStringList DirList = dir.entryList(QDir::Dirs | QDir::NoDotAndDotDot);
        QString RmCMD = "cd /home/pi/USB ; ";
        RmCMD = RmCMD + "sudo umount " + DirList.join(" ") + " ; cd /home/pi";
//        QMessageBox::information(NULL, "item.dir().path()", "GetDevlistTimeout:" + RmCMD, QMessageBox::Ok, QMessageBox::Ok);//Debug
        ((MainWindow*)FUI)->terminaldialog->SendCMD(RmCMD,TeminalState);

//        QStringList DirList = dir.entryList(QDir::Dirs | QDir::NoDotAndDotDot);
//        QString RmCMD = "cd /home/pi/USB ; ";
//        RmCMD = RmCMD + "sudo umount " + DirList.join(" ") + " ;  rm -rf " + DirList.join(" ") + " ; cd /home/pi";
//        QMessageBox::information(NULL, "item.dir().path()", "GetDevlistTimeout:" + RmCMD, QMessageBox::Ok, QMessageBox::Ok);//Debug
//        ((MainWindow*)FUI)->terminaldialog->SendCMD(RmCMD,TeminalState);

        QCoreApplication::processEvents();
        QThread::msleep(500);
        QCoreApplication::processEvents();

        RmCMD = "cd /home/pi/USB ; ";
        RmCMD = RmCMD + "  rm -rf " + DirList.join(" ") + " ; cd /home/pi";
//        QMessageBox::information(NULL, "item.dir().path()", "GetDevlistTimeout:" + RmCMD, QMessageBox::Ok, QMessageBox::Ok);//Debug
        ((MainWindow*)FUI)->terminaldialog->SendCMD(RmCMD,TeminalState);
        QCoreApplication::processEvents();
        QThread::msleep(500);
        QCoreApplication::processEvents();
    }
    RefreshFileList();
}

QString FileDialog::QString2Hex(QString str ,bool FullPath)
{
    QString _returnData = "--BTT--";
    QStringList strList = str.split('.');
    if(str.contains(QRegExp("([a-z]|[A-Z])+:\\\\")))//windows path mode
    {
        if(FullPath)
            _returnData = str.left(str.length() - str.split("\\").last().length()) + _returnData;
        str = str.split("\\").last();
    }
    else if(str.startsWith("/"))//unix path mode
    {
        if(FullPath)
            _returnData = str.left(str.length() - str.split("/").last().length()) + _returnData;
        str = str.split("/").last();
    }
    QByteArray hexData = str.left(str.length() - strList.last().length() - 1).toUtf8();
    _returnData = _returnData + hexData.toHex('-').toUpper()  + (strList.length() >= 2 ? ("." + strList.last()) : "");
    return _returnData;
}

QString FileDialog::Hex2QString(QString hx,bool FullPath)
{
    QString _returnData = "";
    if(hx.contains(("--BTT--")))
    {
        QString fileNameWExt = hx.split("--BTT--").last();
        QStringList strList = fileNameWExt.split('.');
        QByteArray hexData = QByteArray::fromHex(fileNameWExt.left(fileNameWExt.length() - strList.last().length() - 1).replace("-","").toLocal8Bit());
        _returnData = FullPath ? hx.left(hx.length()-fileNameWExt.length() - 7) : "" + QString::fromUtf8(hexData.data(),hexData.size()) + (strList.length() >= 2 ? ("." + strList.last()) : "");
    }
    else
    {
        if(FullPath)
            _returnData=hx;
        else
        {

            if(hx.contains(QRegExp("([a-z]|[A-Z])+:\\\\")))//windows path mode
            {
                _returnData = hx.split("\\").last();
            }
            else if(hx.startsWith("/"))//unix path mode
            {
                _returnData = hx.split("/").last();
            }
            else if(hx.startsWith("http://"))//url mode
            {
                _returnData = hx.split("/").last();
            }
            else
                _returnData=hx;
        }

    }
    return _returnData;
}

void FileDialog::on_Btn_Upload_clicked()
{
    if(!SelectURL.url().startsWith("http://") && !SelectURL.url().isEmpty())
    {
        SelectURL=UploadFile(SelectURL.url());
        RefreshFileList();
    }
}

void FileDialog::on_Btn_Print_clicked()
{
    if(!SelectURL.url().isEmpty())
    {
        ((MainWindow*)FUI)->octonetwork.JobSwitch(SelectURL);
        this->close();
    }
}

void FileDialog::on_Btn_Delete_clicked()
{
    if(!SelectURL.url().isEmpty())
    {
        DeleteURL(SelectURL);
    }
}
