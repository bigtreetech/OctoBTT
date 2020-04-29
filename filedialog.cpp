#include "filedialog.h"
#include "ui_filedialog.h"
//MY Dialog
#include "selectfile.h"
#include <mainwindow.h>
#include <QNetworkReply>
#include <QJsonArray>
#include <QTreeWidgetItem>
#include <QScrollBar>

FileDialog::FileDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::FileDialog)
{
    ui->setupUi(this);

    FUI = (MainWindow*)parent;

    SelectURL = QUrl(((MainWindow*)FUI)->octonetwork.MainUrl + "files/local/Demo.gcode");

    //Init File
//    QObject::connect(((MainWindow*)FUI)->octonetwork.FSnetworkAccessManager, SIGNAL(finished(QNetworkReply*)), this, SLOT(FSReply(QNetworkReply*)),Qt::UniqueConnection);
//    ((MainWindow*)FUI)->octonetwork.FSnetworkAccessManager->get(((MainWindow*)FUI)->octonetwork.FileRequest);

    selectfile->resize(this->width(),this->height());
    selectfile->setWindowState(this->windowState());

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
}

FileDialog::~FileDialog()
{
    delete ui;
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
    //ReFresh Json
    ((MainWindow*)FUI)->octonetwork.SD_ReLoad();
    //Reload FileList
    //    if(((MainWindow*)FUI)->octonetwork.ConnectState == "Operational" || ((MainWindow*)FUI)->octonetwork.ConnectState == "Pausing" || ((MainWindow*)FUI)->octonetwork.ConnectState == "Starting" || ((MainWindow*)FUI)->octonetwork.ConnectState == "Printing" || ((MainWindow*)FUI)->octonetwork.ConnectState == "Cancelling")
    //    {}
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
    if(ext.toLower() == "gcode" || ext.toLower() == "gco")
        SelectURL = QUrl(item->data(1,0).toString());
}

//quick print
void FileDialog::on_treeWidget_itemDoubleClicked(QTreeWidgetItem *item, int column)
{
    Q_UNUSED(column);
    QString ext = item->data(1,0).toString().split('.').back();
    if(ext.toLower() == "gcode" || ext.toLower() == "gco")
    {
        ((MainWindow*)FUI)->octonetwork.JobSwitch(SelectURL);
    }
}
