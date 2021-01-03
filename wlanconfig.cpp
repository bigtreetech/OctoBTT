#include "wlanconfig.h"
#include "ui_wlanconfig.h"
#include <mainwindow.h>
//#include <QSizeF>
#include <QScrollBar>
#include "inputdialog.h"
#include <QTextBrowser>
#include <QDesktopWidget>
#include <QDialog>

Wlanconfig::Wlanconfig(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Wlanconfig)
{
    ui->setupUi(this);

    FUI = (MainWindow*)parent;//((MainWindow*)FUI)->octonetwork.SendGCode(_GCode_Setting);

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
    ((MainWindow*)FUI)->terminaldialog->SendCMD("ifconfig " + ui->DevList->currentText() + (WPA_Switch[ui->DevList->currentText()] ? " down" : " up"),TeminalState);
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
    QString cmd = "ifconfig | grep -E \"flags|inet|ether\"";
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
    //=======================================================================12.23
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

    //创建QDialog
    QDialog dialog(this);
    //设置倒角?和透明度
    dialog.setWindowOpacity(0.9);
    dialog.setStyleSheet("background-color: rgba(128, 128, 128, 25%)");

    //设置窗口的标题和标识
    dialog.setWindowTitle(tr("Wlan Information"));
    dialog.setWindowIcon(QIcon(":/assets/WiFi2.svg"));
    //适应屏幕大小
    dialog.resize(SizePercent.width()*800*0.9,SizePercent.height()*480*0.9);
    dialog.setFixedSize(SizePercent.width()*800*0.9,SizePercent.height()*480*0.8);

    //使弹窗居中显示
    dialog.move(this->pos().x() + (this->width() - dialog.width()) / 2,this->pos().y() + (this->height() - dialog.height()) / 2);

    //设置文本显示
    QTextBrowser textBrowser(&dialog);
    textBrowser.setStyleSheet("border: none; color: rgb(245, 246, 255);");
    textBrowser.setText(sending.join("\n"));
    textBrowser.setFixedSize(SizePercent.width()*800*0.877,SizePercent.height()*480*0.5);
    textBrowser.setGeometry(50,50,SizePercent.width()*800*0.8,SizePercent.height()*480*0.5);

    //设置按键ok
    QPushButton OKBT("OK",&dialog);

    OKBT.setWhatsThis("BTStyleSheet");

    //按键连接信号
    QObject::connect(&OKBT, &QPushButton::clicked, &dialog, &QDialog::reject);

    //字体适应屏幕大小
    QFont _font = OKBT.font();
    _font.setPointSize(this->maximumWidth() > this->maximumHeight() ? (int)this->maximumHeight()/30 : (int)this->maximumWidth()/30);
    OKBT.setFont(_font);
    textBrowser.setFont(_font);

    //调整文本和按键的位置
    QVBoxLayout VLayout(&dialog);
    VLayout.addWidget(&textBrowser);
    VLayout.addWidget(&OKBT);

    int ret = dialog.exec();
    if(ret == QDialog::Rejected)
    {
        dialog.close();
    }
}

void Wlanconfig::CreateNetwork()
{
    QString cmd = QString("wpa_cli -i %1 add_network").arg(ui->DevList->currentText());
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
    QString cmd = QString("wpa_cli -i %1 set_network %2 ssid '\"%3\"'").arg(ui->DevList->currentText()).arg(QString::number(WPA_Scan_SSID_List[Current_SSID].id)).arg(Current_SSID);
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

            QString cmd = QString("wpa_cli -i %1 set_network %3 psk '\"%2\"'").arg(Dev).arg(value).arg(QString::number(WPA_Scan_SSID_List[Current_SSID].id));
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
                QStringList btnName = {"Yes", "No"} ;
                CustomDialog *newDialog = new CustomDialog();
                QObject::connect(newDialog, &CustomDialog::OutputEvent, newDialog,[=](QString instruct)
                {
                    if(instruct == "Yes")
                    {
                        RemoveNetwork();
                    }
                    else if(instruct == "No")
                    {
                        ui->WPA_List->setEnabled(true);

                    }

                });
                newDialog->showCustomDialog("Warning",":/assets/info.svg",btnName,QString("Do you want to forget \"%1\" ?").arg(ui->WPA_List->model()->data(ui->WPA_List->currentIndex()).toString()),this);
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

    QString cmd = QString("wpa_cli -i %1 remove_network %2").arg(Dev).arg(QString::number(WPA_Scan_SSID_List[Current_SSID].id));

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
        CustomDialog *newDialog = new CustomDialog();
        newDialog->showCustomDialog("Warning",":/assets/WiFi2.svg",QString("SSID[%1] Delete failed !"),this);
        ui->WPA_List->setEnabled(true);
    }
}

void Wlanconfig::ConnectNetwork()
{
    QString Current_SSID = ui->WPA_List->model()->data(ui->WPA_List->currentIndex()).toString();
    QString Dev = ui->DevList->currentText();

    QString cmd = QString("wpa_cli -i %1 enable_network %2 &&  wpa_cli -i %1 select_network %2 && wpa_cli -i %1 save_config").arg(Dev).arg(QString::number(WPA_Scan_SSID_List[Current_SSID].id));
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
//    this->setEnabled(false);

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
    ((MainWindow*)FUI)->terminaldialog->SendCMD("wpa_cli -i "+ui->DevList->currentText()+" list_network",TeminalState);
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
    ((MainWindow*)FUI)->terminaldialog->SendCMD("wpa_cli -i "+ui->DevList->currentText()+" scan",TeminalState);
}

void Wlanconfig::ScanWPA(QStringList &Responses)
{
    if(Responses[0].toLower().trimmed() == "ok")
        ScanWPA_Ressult();
    else
    {
//        this->setEnabled(true);

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
    ((MainWindow*)FUI)->terminaldialog->SendCMD("wpa_cli -i "+ui->DevList->currentText()+" scan_results",TeminalState);
}

void Wlanconfig::ScanWPA_Ressult(QStringList &Responses)
{
    bool ReadData = false;
    QStringList effectiveSSID;
    foreach(QString item , Responses)
    {
        if(ReadData)
        {
            QStringList ModuleStr = item.replace("\t"," ").split(" ",QString::SkipEmptyParts);


            int WPA_Level = ModuleStr[2].toInt();
            QString LastStr  = item.replace("\t"," ").remove(0,ModuleStr[0].count()).trimmed();//remove bssid
            LastStr  = LastStr.remove(0,ModuleStr[1].count()).trimmed();//remove freq
            LastStr  = LastStr.remove(0,ModuleStr[2].count()).trimmed();//remove level
            QString SSID = LastStr.remove(0,ModuleStr[3].count()).trimmed();//remove SecurityFlags
            if(SSID.trimmed() == "")
                continue;
            effectiveSSID.append(SSID);
            if(!WPA_Scan_SSID_List.contains(SSID))
            {
                WPA_Scan_SSID_List.insert(SSID,WPAInfo());
            }
            WPA_Scan_SSID_List[SSID].level = WPA_Level;
//            WPA_Scan_SSID_List[SSID].Quality_value ;///信号强度排序
            if(WPA_Scan_SSID_List[SSID].level <= -100)
                WPA_Scan_SSID_List[SSID].Quality_value = 0;
            else if(WPA_Scan_SSID_List[SSID].level >= -50)
                WPA_Scan_SSID_List[SSID].Quality_value = 100;
            else
                WPA_Scan_SSID_List[SSID].Quality_value = 2 * (WPA_Scan_SSID_List[SSID].level + 100);
//            // dBm to Quality:
//            if(dBm <= -100)
//                quality = 0;
//            else if(dBm >= -50)
//                quality = 100;
//            else
//                quality = 2 * (dBm + 100);
//            // Quality to dBm:
//            if(quality <= 0)
//                dBm = -100;
//            else if(quality >= 100)
//                dBm = -50;
//            else
//                dBm = (quality / 2) - 100;
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
            wpsitem->setForeground(QColor(255,255,255));

            if(WPAReader.value().Quality_value < 95)
            {
                wpsitem->setIcon(QIcon(":/assets/jinggao.svg"));
                WPAModel->appendRow(wpsitem);
            }
            else if(WPAReader.value().connectflags)
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

//    this->setEnabled(true);

//    for (int i=0;i<WPAModel->rowCount();i++)
//    {
//        if(i & 1)
//            WPAModel->item(i)->setBackground(QBrush(Qt::gray));
//        else
//            WPAModel->item(i)->setBackground(QBrush(Qt::darkGray));
//    }
    ui->WPA_List->setModel(WPAModel);

    if(WPA_SSID[ui->DevList->currentText()].trimmed() == "")
    {
        GetDevState();
    }
}

void Wlanconfig::GetDevState()
{
//    this->setEnabled(false);
    LastDev = ui->DevList->currentText();
    disconnect(((MainWindow*)FUI)->terminaldialog,&TerminalDialog::CMD_Reply,0,0);
    QObject::connect(((MainWindow*)FUI)->terminaldialog,&TerminalDialog::CMD_Reply,this,[=](QStringList CommandLine)
                     {
                         disconnect(((MainWindow*)FUI)->terminaldialog,&TerminalDialog::CMD_Reply,0,0);
                         GetDevState(CommandLine);
                     });
    ((MainWindow*)FUI)->terminaldialog->SendCMD("iwconfig | grep -E \"wlan[0-9]|ESSID|Tx-Power=\"",TeminalState);
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
//    this->setEnabled(true);
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
