#include "wlanconfig.h"
#include "ui_wlanconfig.h"
#include <mainwindow.h>
//#include <QSizeF>
#include <QMessageBox>
#include <QScrollBar>
#include "inputdialog.h"

Wlanconfig::Wlanconfig(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Wlanconfig)
{
    ui->setupUi(this);
    FUI = (MainWindow*)parent;//((MainWindow*)FUI)->octonetwork.SendGCode(_GCode_Setting);

    ui->WPA_List->verticalScrollBar()->setStyleSheet("QScrollBar:vertical"
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
    ui->WPA_List->horizontalScrollBar()->setStyleSheet("QScrollBar:horizontal"
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


    QFont font;
    font.setPointSize((int)(ui->DevList->font().pointSize()*(SizePercent.width() < SizePercent.height() ? SizePercent.width():SizePercent.height())));
    ui->DevList->setFont(font);
    font.setPointSize((int)(ui->WPA_List->font().pointSize()*(SizePercent.width() < SizePercent.height() ? SizePercent.width():SizePercent.height())));
    ui->WPA_List->setFont(font);

    ui->WPA_List->setIconSize(QSize(SizePercent.width()*64,SizePercent.height()*64));

    this->resize((int)(SizePercent.width()*800),(int)(SizePercent.height()*480));
    this->setMaximumSize((int)(SizePercent.width()*800),(int)(SizePercent.height()*480));
    this->setFixedSize((int)(SizePercent.width()*800),(int)(SizePercent.height()*480));

//    WPA_Switch = new QMap<QString,bool>;
//    WPA_SSID = new QMap<QString,QString>;

//    ui->DevList->addItem("baby");
//    ui->DevList->addItem("Goodgay");
//    ui->DevList->addItem("HappyGirl");
//    ui->DevList->addItem("baby1");
//    ui->DevList->addItem("Goodgay1");
//    ui->DevList->addItem("HappyGirl1");
//    ui->DevList->addItem("baby2");
//    ui->DevList->addItem("Goodgay2");
//    ui->DevList->addItem("HappyGirl2");
//    ui->DevList->setCurrentIndex(0);
//    QStyle *DL = ui->DevList->style();
//    QMessageBox::information(NULL, "System", DL);
//    ui->Btn_Help->setText(DL->property("on").toString());
//    DL->setProperty("on",50);
//    ui->DevList->setStyle(DL);
}

Wlanconfig::~Wlanconfig()
{
    delete ui;
}

void Wlanconfig::on_Btn_Back_clicked()
{
    this->close();
}

//Load Scan Funtion
//Load List index
void Wlanconfig::showEvent(QShowEvent *event)
{
    Q_UNUSED(event);
    GetDevState();
}

void Wlanconfig::on_Btn_DevRefresh_clicked()
{
    GetDevState();
}

void Wlanconfig::on_Btn_DevSwitch_clicked()
{
    if(!WPA_Switch.contains(ui->DevList->currentText()))
        return;
    ((MainWindow*)FUI)->terminaldialog->SendCMD("sudo ifconfig " + ui->DevList->currentText() + (WPA_Switch[ui->DevList->currentText()] ? " down" : " up"),TeminalState);
    WPA_Switch[ui->DevList->currentText()] = !WPA_Switch[ui->DevList->currentText()];

    ui->Btn_DevSwitch->setIcon(QIcon(WPA_Switch[ui->DevList->currentText()] ? ":/assets/Network.svg":":/assets/shebeiweilianjie-copy.svg"));
    ui->Btn_Connect->setEnabled(WPA_Switch[ui->DevList->currentText()]);
    ui->Btn_Scan->setEnabled(WPA_Switch[ui->DevList->currentText()]);
    ui->WPA_List->setEnabled(WPA_Switch[ui->DevList->currentText()]);
}

void Wlanconfig::on_Btn_Connect_clicked()
{
    if(ui->WPA_List->model() == NULL || ui->WPA_List->model()->rowCount()==0)
        return;
    if(!WPA_Switch.contains(ui->DevList->currentText()))
        return;
    if(!WPA_Switch[ui->DevList->currentText()])
        return;

    QString Current_SSID = ui->WPA_List->model()->data(ui->WPA_List->currentIndex()).toString();

    if(Current_SSID.trimmed() == "")//未选择SSID
        return;

    if(WPA_Scan_SSID_List[Current_SSID].connectflags)//已连接
        return;

    ui->WPA_List->setEnabled(false);

    if(WPA_Scan_SSID_List[Current_SSID].pwdError)//连接无效，重新输入密码 //pwdError为true本身具有前提是属于已有网络ID的
    {
        SetPassword();
        return;
    }

    if(WPA_Scan_SSID_List[Current_SSID].id >= 0)//可执行连接
    {
        ConnectNetwork();
        return;
    }

    CreateNetwork();
}

//双击面板输入密码连接或创建连接点
void Wlanconfig::on_WPA_List_doubleClicked(const QModelIndex &index)
{
    if(WPA_Scan_SSID_List[index.data().toString()].id >= 0)//可执行连接
    {
        SetPassword();
        return;
    }
    CreateNetwork();
}

void Wlanconfig::on_Btn_Scan_clicked()
{
    if(!WPA_Switch.contains(ui->DevList->currentText()))
        return;
    if(!WPA_Switch[ui->DevList->currentText()])
        return;
    ScanWPAList();
}

//点击help查看网络信息
void Wlanconfig::on_Btn_Help_clicked()
{
    GetNetworkInfo();
}

void Wlanconfig::GetNetworkInfo()
{
    QString cmd = "sudo ifconfig | grep -E \"flags|inet|ether\"";
    disconnect(((MainWindow*)FUI)->terminaldialog,&TerminalDialog::CMD_Reply,0,0);
    QObject::connect(((MainWindow*)FUI)->terminaldialog,&TerminalDialog::CMD_Reply,this,[=](QStringList CommandLine)
                     {
                         disconnect(((MainWindow*)FUI)->terminaldialog,&TerminalDialog::CMD_Reply,0,0);
                         GetNetworkInfo(CommandLine);
                     });
    ((MainWindow*)FUI)->terminaldialog->SendCMD(cmd,TeminalState);
}

void Wlanconfig::GetNetworkInfo(QStringList &Responses)
{
//    QStringList data;
    QStringList sending;
    foreach(QString item ,Responses)
    {
        QString replaceitem = item.replace("\t"," ");
//        sending.append(replaceitem);
        QStringList explore = replaceitem.split(" ",QString::SkipEmptyParts);
        if(explore[0].contains(QRegExp("(eth[0-9]+[0-9]*)|(lo)|(wlan[0-9]+[0-9]*)")))
        {
            sending.append(explore[0]);
        }
        else if(explore[0].contains("inet6"))//IPv6
        {
            sending.append(QString("IPv6:%1").arg(explore[1]));
        }
        else if(explore[0].contains("inet"))//IP
        {
            sending.append(QString("IPv4:%1").arg(explore[1]));
            sending.append(QString("IPv4 Netmask:%1").arg(explore[3]));
        }
        else if(explore[0].contains("ether"))//MAC
        {
            sending.append(QString("MAC:%1").arg(explore[1]));
        }
//        bool read = false;
//        data.append(replaceitem).split(" ",QString::SkipEmptyParts));
    }
    QMessageBox::information(NULL,"",sending.join("\n"));
}

void Wlanconfig::CreateNetwork()
{
    QString cmd = QString("sudo wpa_cli -i %1 add_network").arg(ui->DevList->currentText());
    disconnect(((MainWindow*)FUI)->terminaldialog,&TerminalDialog::CMD_Reply,0,0);
    QObject::connect(((MainWindow*)FUI)->terminaldialog,&TerminalDialog::CMD_Reply,this,[=](QStringList CommandLine)
                     {
                         disconnect(((MainWindow*)FUI)->terminaldialog,&TerminalDialog::CMD_Reply,0,0);
                         QString Current_SSID = ui->WPA_List->model()->data(ui->WPA_List->currentIndex()).toString();
                         bool chkID=false;
                         int id = CommandLine[0].toInt(&chkID);
                         if(chkID)
                         {
                             WPA_Scan_SSID_List[Current_SSID].id = id;
                             SetSSID();
                         }
                         else
                             ui->WPA_List->setEnabled(true);
                     });
    ((MainWindow*)FUI)->terminaldialog->SendCMD(cmd,TeminalState);
}

void Wlanconfig::SetSSID()
{
    QString Current_SSID = ui->WPA_List->model()->data(ui->WPA_List->currentIndex()).toString();
    QString cmd = QString("sudo wpa_cli -i %1 set_network %2 ssid '\"%3\"'").arg(ui->DevList->currentText()).arg(QString::number(WPA_Scan_SSID_List[Current_SSID].id)).arg(Current_SSID);
    disconnect(((MainWindow*)FUI)->terminaldialog,&TerminalDialog::CMD_Reply,0,0);
    QObject::connect(((MainWindow*)FUI)->terminaldialog,&TerminalDialog::CMD_Reply,this,[=](QStringList CommandLine)
                     {
                         disconnect(((MainWindow*)FUI)->terminaldialog,&TerminalDialog::CMD_Reply,0,0);
                         SetSSID(CommandLine);
                     });
    ((MainWindow*)FUI)->terminaldialog->SendCMD(cmd,TeminalState);
}

void Wlanconfig::SetSSID(QStringList &Responses)
{
    if(Responses[0].toLower().trimmed() == "ok")
    {
        //设置密码
        SetPassword();
    }
    else
    {
        RemoveNetwork();
    }
}

void Wlanconfig::SetPassword()
{
    QString Current_SSID = ui->WPA_List->model()->data(ui->WPA_List->currentIndex()).toString();
    QString Dev = ui->DevList->currentText();

    InputDialog *inputdialog = new InputDialog(this);
    connect(inputdialog,&InputDialog::closedialog,this,[=](QString value,bool SettingMark){
        if(SettingMark)
        {
//            SetPassword(ui->DevList->currentText() , value , WPA_Scan_SSID_List[Current_SSID].id);
            QString Current_SSID = ui->WPA_List->model()->data(ui->WPA_List->currentIndex()).toString();
            QString Dev = ui->DevList->currentText();

            QString cmd = QString("sudo wpa_cli -i %1 set_network %3 psk '\"%2\"'").arg(Dev).arg(value).arg(QString::number(WPA_Scan_SSID_List[Current_SSID].id));
            disconnect(((MainWindow*)FUI)->terminaldialog,&TerminalDialog::CMD_Reply,0,0);
            QObject::connect(((MainWindow*)FUI)->terminaldialog,&TerminalDialog::CMD_Reply,this,[=](QStringList CommandLine)
                             {
                                 disconnect(((MainWindow*)FUI)->terminaldialog,&TerminalDialog::CMD_Reply,0,0);
                                 SetPassword(CommandLine);
                             });
            ((MainWindow*)FUI)->terminaldialog->SendCMD(cmd,TeminalState);

        }
        else
        {
            if(value.trimmed() == "")
            {
                if(QMessageBox::warning(NULL,"",QString("Do you want to forget \"%1\" ?").arg(ui->WPA_List->model()->data(ui->WPA_List->currentIndex()).toString()),QMessageBox::Yes|QMessageBox::No,QMessageBox::No) == QMessageBox::Yes)
                {
                    RemoveNetwork();
                }
                else
                    ui->WPA_List->setEnabled(true);
            }
            else
                ui->WPA_List->setEnabled(true);
        }
        delete sender();
    });
    inputdialog->setvalue("Please Input "+ Current_SSID +" Password","");
}

void Wlanconfig::SetPassword(QStringList &Responses)
{
    if(Responses[0].toLower().trimmed() == "ok")
    {
        QString Current_SSID = ui->WPA_List->model()->data(ui->WPA_List->currentIndex()).toString();
        //设置图标
//        if(WPA_Scan_SSID_List[((QStandardItemModel)ui->WPA_List->model()).item(0)->text()].connectflags)
//            ((QStandardItemModel)ui->WPA_List->model()).item(0)->setIcon(QIcon(":/assets/Wifi_Unlock.svg"));
        //执行启用并连接
        ConnectNetwork();
    }
    else
    {
        ui->WPA_List->setEnabled(true);
    }
}

void Wlanconfig::RemoveNetwork()
{
    QString Current_SSID = ui->WPA_List->model()->data(ui->WPA_List->currentIndex()).toString();
    QString Dev = ui->DevList->currentText();

    QString cmd = QString("sudo wpa_cli -i %1 remove_network %2").arg(Dev).arg(QString::number(WPA_Scan_SSID_List[Current_SSID].id));

    disconnect(((MainWindow*)FUI)->terminaldialog,&TerminalDialog::CMD_Reply,0,0);
    QObject::connect(((MainWindow*)FUI)->terminaldialog,&TerminalDialog::CMD_Reply,this,[=](QStringList CommandLine)
                     {
                         disconnect(((MainWindow*)FUI)->terminaldialog,&TerminalDialog::CMD_Reply,0,0);
                         RemoveNetwork(CommandLine);
                     });
    ((MainWindow*)FUI)->terminaldialog->SendCMD(cmd,TeminalState);
}

void Wlanconfig::RemoveNetwork(QStringList &Responses)
{
    QString Current_SSID = ui->WPA_List->model()->data(ui->WPA_List->currentIndex()).toString();
    if(Responses[0].toLower().trimmed() == "ok")
    {
        WPA_Scan_SSID_List[Current_SSID].id = -1;
        WPA_Scan_SSID_List[Current_SSID].pwdError = false;
        WPA_Scan_SSID_List[Current_SSID].connectflags = false;
        ui->WPA_List->setEnabled(true);
    }
    else
    {
        QMessageBox::warning(NULL,"",QString("SSID[%1] Delete failed !"));
        ui->WPA_List->setEnabled(true);
    }
}

void Wlanconfig::ConnectNetwork()
{
    QString Current_SSID = ui->WPA_List->model()->data(ui->WPA_List->currentIndex()).toString();
    QString Dev = ui->DevList->currentText();

    QString cmd = QString("sudo wpa_cli -i %1 enable_network %2 &&  wpa_cli -i %1 select_network %2 && wpa_cli -i %1 save_config").arg(Dev).arg(QString::number(WPA_Scan_SSID_List[Current_SSID].id));
//    QMessageBox::information(NULL,"",cmd);
    disconnect(((MainWindow*)FUI)->terminaldialog,&TerminalDialog::CMD_Reply,0,0);
    QObject::connect(((MainWindow*)FUI)->terminaldialog,&TerminalDialog::CMD_Reply,this,[=](QStringList)
                     {
                         disconnect(((MainWindow*)FUI)->terminaldialog,&TerminalDialog::CMD_Reply,0,0);
                         ui->WPA_List->setEnabled(true);
                     });
    ((MainWindow*)FUI)->terminaldialog->SendCMD(cmd,TeminalState);
}

void Wlanconfig::ScanWPAList()
{
    this->setEnabled(false);

    if(LastDev != ui->DevList->currentText())
    {
        LastDev = ui->DevList->currentText();
        WPAModel->clear();
        if(ui->WPA_List->model() != NULL && ui->WPA_List->model()->rowCount()>0)
            ui->WPA_List->model()->removeRows(0,ui->WPA_List->model()->rowCount());
    }

    disconnect(((MainWindow*)FUI)->terminaldialog,&TerminalDialog::CMD_Reply,0,0);
    QObject::connect(((MainWindow*)FUI)->terminaldialog,&TerminalDialog::CMD_Reply,this,[=](QStringList CommandLine)
                     {
                         disconnect(((MainWindow*)FUI)->terminaldialog,&TerminalDialog::CMD_Reply,0,0);
                         ScanWPAList(CommandLine);
                     });
    ((MainWindow*)FUI)->terminaldialog->SendCMD("sudo wpa_cli -i "+ui->DevList->currentText()+" list_network",TeminalState);
}

void Wlanconfig::ScanWPAList(QStringList &Responses)
{
    bool ReadData = false;
    WPA_Scan_SSID_List.clear();
    foreach(QString item , Responses)
    {
        if(ReadData)
        {
            int WPA_ID = item.replace("\t"," ").split(" ",QString::SkipEmptyParts)[0].toInt();
            QString LastStr = item.replace("\t"," ").remove(0,QString::number(WPA_ID).length()).trimmed();//remove NID

            LastStr = ReverseStr(LastStr);
            bool WPA_Flags = false;
            bool WPA_PWD_Error = false;
            if(LastStr.startsWith("]"))//has flags
            {
                QString FlagsStr = ((ReverseStr(LastStr.split(" ",QString::SkipEmptyParts)[0]))).toUpper() ;

                WPA_Flags = (FlagsStr == "[CURRENT]" && WPA_SSID[ui->DevList->currentText()].trimmed() != "");//连接中状态检测[TEMP-DISABLED]//逻辑不够严谨

                WPA_PWD_Error = (FlagsStr == "[TEMP-DISABLED]" || (FlagsStr == "[CURRENT]" && WPA_SSID[ui->DevList->currentText()].trimmed() == ""));

                LastStr = LastStr.remove(0,FlagsStr.count()).trimmed();//remove state
            }
            else
                WPA_Flags = false;

            QString SSID = ReverseStr(LastStr.remove(0,LastStr.split(" ",QString::SkipEmptyParts)[0].count()).trimmed());
            WPA_Scan_SSID_List.insert(SSID,WPAInfo());
            WPA_Scan_SSID_List[SSID].id = WPA_ID;
//            WPA_Scan_SSID_List[SSID].connectflags = SSID == WPA_SSID[ui->DevList->currentText()];
            WPA_Scan_SSID_List[SSID].connectflags = WPA_Flags;
            WPA_Scan_SSID_List[SSID].pwdError = WPA_PWD_Error;

//            QMessageBox::warning(FUI,"",SSID + " -t- "+ QString::number(WPA_ID) + " -t- " + QString::number(WPA_Flags));
        }
        else if(item.toLower().startsWith("network"))
        {
            ReadData = true;
        }
    }
    ScanWPA();
}

QString Wlanconfig::ReverseStr(QString args)
{
    QString outputdata = "";
    foreach(QChar arg , args)
    {
        outputdata = arg + outputdata;
    }
    return outputdata;
}

void Wlanconfig::ScanWPA()
{
    disconnect(((MainWindow*)FUI)->terminaldialog,&TerminalDialog::CMD_Reply,0,0);
    QObject::connect(((MainWindow*)FUI)->terminaldialog,&TerminalDialog::CMD_Reply,this,[=](QStringList CommandLine)
                     {
                         disconnect(((MainWindow*)FUI)->terminaldialog,&TerminalDialog::CMD_Reply,0,0);
                         ScanWPA(CommandLine);
                     });
    ((MainWindow*)FUI)->terminaldialog->SendCMD("sudo wpa_cli -i "+ui->DevList->currentText()+" scan",TeminalState);
}

void Wlanconfig::ScanWPA(QStringList &Responses)
{
    if(Responses[0].toLower().trimmed() == "ok")
        ScanWPA_Ressult();
    else
    {
        this->setEnabled(true);

        if(WPA_SSID[ui->DevList->currentText()].trimmed() == "")
        {
            GetDevState();
        }

//        WPA_Switch[ui->DevList->currentText()] = false;

//        ui->Btn_DevSwitch->setIcon(QIcon(WPA_Switch[ui->DevList->currentText()] ? ":/assets/Network.svg":":/assets/shebeiweilianjie-copy.svg"));
//        ui->Btn_Connect->setEnabled(WPA_Switch[ui->DevList->currentText()]);
//        ui->Btn_Scan->setEnabled(WPA_Switch[ui->DevList->currentText()]);
//        ui->WPA_List->setEnabled(WPA_Switch[ui->DevList->currentText()]);
    }
}

void Wlanconfig::ScanWPA_Ressult()
{
    disconnect(((MainWindow*)FUI)->terminaldialog,&TerminalDialog::CMD_Reply,0,0);
    QObject::connect(((MainWindow*)FUI)->terminaldialog,&TerminalDialog::CMD_Reply,this,[=](QStringList CommandLine)
                     {
                         disconnect(((MainWindow*)FUI)->terminaldialog,&TerminalDialog::CMD_Reply,0,0);
                         ScanWPA_Ressult(CommandLine);
                     });
    ((MainWindow*)FUI)->terminaldialog->SendCMD("sudo wpa_cli -i "+ui->DevList->currentText()+" scan_results",TeminalState);
}

void Wlanconfig::ScanWPA_Ressult(QStringList &Responses)
{
    bool ReadData = false;
    QStringList effectiveSSID;
    foreach(QString item , Responses)
    {
//        QMessageBox::information(FUI,"",item);
        if(ReadData)
        {
            QStringList ModuleStr = item.replace("\t"," ").split(" ",QString::SkipEmptyParts);

//            QMessageBox::information(FUI,"",ModuleStr.join("{-t-}\n"));

            int WPA_Level = ModuleStr[2].toInt();
            QString LastStr  = item.replace("\t"," ").remove(0,ModuleStr[0].count()).trimmed();//remove bssid
//            QMessageBox::information(FUI,"",LastStr);
            LastStr  = LastStr.remove(0,ModuleStr[1].count()).trimmed();//remove freq
//            QMessageBox::information(FUI,"",LastStr);
            LastStr  = LastStr.remove(0,ModuleStr[2].count()).trimmed();//remove level
//            QMessageBox::information(FUI,"",LastStr);
            QString SSID = LastStr.remove(0,ModuleStr[3].count()).trimmed();//remove SecurityFlags
//            QMessageBox::information(FUI,"",SSID);
            if(SSID.trimmed() == "")
                continue;
            effectiveSSID.append(SSID);
            if(!WPA_Scan_SSID_List.contains(SSID))
            {
                WPA_Scan_SSID_List.insert(SSID,WPAInfo());
            }
            WPA_Scan_SSID_List[SSID].level = WPA_Level;
        }
        else if(item.toLower().startsWith("bssid"))
        {
            ReadData = true;
        }
    }

    WPAModel->clear();
    if(ui->WPA_List->model() != NULL && ui->WPA_List->model()->rowCount()>0)
        ui->WPA_List->model()->removeRows(0,ui->WPA_List->model()->rowCount());

    if(WPA_Scan_SSID_List.count() > 0)
    {
//        if(WPA_Scan_SSID_List.contains(WPA_SSID[ui->DevList->currentText()]))
//        {
//            QStandardItem *wpsFirst = new QStandardItem(static_cast<QString>(WPA_SSID[ui->DevList->currentText()]));
//            wpsFirst->setIcon(QIcon(":/assets/Network.svg"));
//            WPAModel->appendRow(wpsFirst);
//        }
        QMapIterator<QString, WPAInfo> WPAReader(WPA_Scan_SSID_List);
        while (WPAReader.hasNext())
        {
            WPAReader.next();
            if(!effectiveSSID.contains(WPAReader.key()))
                continue;
//            if(WPAReader.key() == WPA_SSID[ui->DevList->currentText()])
//                continue;
            QStandardItem *wpsitem = new QStandardItem(static_cast<QString>(WPAReader.key()));
//            QMessageBox::warning(this,"",WPAReader.key()+":"+QString::number(WPAReader.value().id)+QString::number(WPAReader.value().connectflags)+QString::number(WPAReader.value().level));
            if(WPAReader.value().connectflags)
            {
                wpsitem->setIcon(QIcon(":/assets/Network.svg"));
                if(WPAModel->rowCount() > 0)
                    WPAModel->insertRow(0,wpsitem);
                else
                    WPAModel->appendRow(wpsitem);
            }
            else if(WPAReader.value().pwdError)
            {
                wpsitem->setIcon(QIcon(":/assets/wangluolianjiechaoshi.svg"));
                if(WPAModel->rowCount() > 1)
                {
                    if(WPA_Scan_SSID_List[WPAModel->item(0)->text()].connectflags)
                        WPAModel->insertRow(1,wpsitem);
                    else
                        WPAModel->insertRow(0,wpsitem);
                }
                else
                    WPAModel->appendRow(wpsitem);
            }
            else if(WPAReader.value().id >= 0)
            {
                wpsitem->setIcon(QIcon(":/assets/Wifi_Unlock.svg"));
                if(WPAModel->rowCount() > 1)
                {
//                    QMessageBox::warning(NULL,"",WPAModel->item(0)->text());
//                    QMessageBox::information(NULL,"",QString::number(WPA_Scan_SSID_List[WPAModel->item(0)->data().toString()].connectflags));

                    if(WPA_Scan_SSID_List[WPAModel->item(0)->text()].connectflags || WPA_Scan_SSID_List[WPAModel->item(0)->text()].pwdError)
                        WPAModel->insertRow(1,wpsitem);
                    else
                        WPAModel->insertRow(0,wpsitem);
                }
                else
                    WPAModel->appendRow(wpsitem);
            }
            else
            {
                wpsitem->setIcon(QIcon(":/assets/Wifi_Lock.svg"));
                WPAModel->appendRow(wpsitem);
            }
        }
    }

    this->setEnabled(true);
    for (int i=0;i<WPAModel->rowCount();i++)
    {
        if(i & 1)
            WPAModel->item(i)->setBackground(QBrush(Qt::darkYellow));
        else
            WPAModel->item(i)->setBackground(QBrush(Qt::blue));
    }
    ui->WPA_List->setModel(WPAModel);

    if(WPA_SSID[ui->DevList->currentText()].trimmed() == "")
    {
        GetDevState();
    }
}

void Wlanconfig::GetDevState()
{
    this->setEnabled(false);
    LastDev = ui->DevList->currentText();
    disconnect(((MainWindow*)FUI)->terminaldialog,&TerminalDialog::CMD_Reply,0,0);
    QObject::connect(((MainWindow*)FUI)->terminaldialog,&TerminalDialog::CMD_Reply,this,[=](QStringList CommandLine)
                     {
                         disconnect(((MainWindow*)FUI)->terminaldialog,&TerminalDialog::CMD_Reply,0,0);
                         GetDevState(CommandLine);
                     });
    ((MainWindow*)FUI)->terminaldialog->SendCMD("sudo iwconfig | grep -E \"wlan[0-9]|ESSID|Tx-Power=\"",TeminalState);
}

void Wlanconfig::GetDevState(QStringList &Responses)
{
    ui->Btn_DevSwitch->setIcon(QIcon(":/assets/shebeiweilianjie-copy.svg"));
    WPA_Switch.clear();
    WPA_SSID.clear();
    ui->DevList->clear();

    QString DevKey = "";
    foreach(QString item , Responses)
    {
        QStringList Analytical = item.split(" ",QString::SkipEmptyParts);
        if(Analytical[0].toLower().startsWith("wlan"))
        {
            DevKey = Analytical[0];
            if(Analytical[3].toLower() == "essid:off/any")
            {
                WPA_SSID.insert(DevKey,"");
                WPA_Switch.insert(DevKey,false);
            }
            else
            {
                WPA_SSID.insert(DevKey,(item.remove(0,item.indexOf("ESSID:")+6)).replace(QRegExp("\"([^<]*)\""), "\\1"));
                WPA_Switch.insert(DevKey,true);
            }
        }
        else
        {
            //功率数据可在此处提取
            Analytical = item.split("Tx-Power=",QString::SkipEmptyParts);
            WPA_Switch[DevKey] = (Analytical.count() > 1);
        }
    }

    QStringList Dev;
    foreach(QString item , WPA_Switch.keys())
    {
        Dev.append(item);
    }
    ui->DevList->addItems(Dev);
    //LastDev
    for (int i = 0;i < ui->DevList->count();i++)
    {
        if(ui->DevList->itemText(i) == LastDev)
            ui->DevList->setCurrentIndex(i);
    }
    this->setEnabled(true);
}

void Wlanconfig::on_DevList_currentIndexChanged(const QString &arg1)
{
    if(!WPA_Switch.contains(arg1))
    {
        ui->Btn_DevSwitch->setIcon(QIcon(":/assets/shebeiweilianjie-copy.svg"));
        ui->Btn_Connect->setEnabled(false);
        ui->Btn_Scan->setEnabled(false);
        ui->WPA_List->setEnabled(false);
        return;
    }

    ui->Btn_DevSwitch->setIcon(QIcon(WPA_Switch[arg1] ? ":/assets/Network.svg":":/assets/shebeiweilianjie-copy.svg"));
    ui->Btn_Connect->setEnabled(WPA_Switch[arg1]);
    ui->Btn_Scan->setEnabled(WPA_Switch[arg1]);
    ui->WPA_List->setEnabled(WPA_Switch[arg1]);
}
