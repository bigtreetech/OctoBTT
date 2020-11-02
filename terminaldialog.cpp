#include "terminaldialog.h"
#include "ui_terminaldialog.h"
#include <QTreeWidgetItem>
#include <QScrollBar>
#include <QProcess>
#include <QDebug>
//#include <QSizeF>
#include <QTimer>
#include <QStringList>
#include <mainwindow.h>
#include <QMessageBox>
#include <QTextCodec>
#include "inputdialog.h"

TerminalDialog::TerminalDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::TerminalDialog)
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

    cmd = new QProcess(this);

    /* 将QProcess的输出错误打印到界面 */
    connect(cmd,&QProcess::readyReadStandardError,this,[=](){
        if(!SendCMD_argu.contains("-e")) ui->Receiver->append("->Receive->->-> Standard Error");
        QString outStr = QString::fromLocal8Bit(cmd->readAllStandardError());
        if(!SendCMD_argu.contains("-e")) ui->Receiver->append("<<StandardError:");
        if(!SendCMD_argu.contains("-e")) ui->Receiver->append(outStr.trimmed());
        if(!SendCMD_argu.contains("-e")) ui->Receiver->horizontalScrollBar()->setValue(ui->Receiver->horizontalScrollBar()->maximum());

//        QStringList _output;
//        _output<<"&&StandardError"<<outStr;
//        emit CMD_Reply(_output);//抛出槽触发
//        if(outStr.trimmed().contains("sudo") && outStr.trimmed().contains("askpass") && command.contains("sudo "))
//        {
//            int startlocal = command.indexOf("sudo ");
//            command = command.replace(startlocal,5,"echo \"" + CMD_Password + "\" | sudo -S ");//片段阔幅代码未处理
//            command = command.remove(0,startlocal);
//            SendCMD(command);
//        }
    });
    /* 将QProcess的输出打印到界面 */
    connect(cmd,&QProcess::readyReadStandardOutput,this,[=](){
        if(!SendCMD_argu.contains("-q")) ui->Receiver->append("->Receive->->-> Standard Output");
        QString outStr = QString::fromLocal8Bit(cmd->readAllStandardOutput());
        if(!SendCMD_argu.contains("-q")) ui->Receiver->append("<<Receiver:");
        //            ui->Receiver->append(outStr.trimmed());
        //            ui->Receiver->horizontalScrollBar()->setValue(ui->Receiver->horizontalScrollBar()->maximum());
//        cmd->disconnect(SIGNAL(readyReadStandardOutput()));
        QStringList _output;
        foreach(QString item,outStr.trimmed().split("\n",QString::SkipEmptyParts))
        {
            if(item.trimmed() != "")
            {
                QString Data = item.trimmed();
                while (Data.indexOf("\\x")>=0)//字符处理
                {
                    int index = Data.indexOf("\\x");
                    QString Block = Data.mid(index,12);
                    Block = Block.replace("\\x", QString());
                    QString Blocks ;
                    Blocks.append(Block.mid(0,2).toUShort(0,16));
                    Blocks.append(Block.mid(2,2).toUShort(0,16));
                    Blocks.append(Block.mid(4,2).toUShort(0,16));
//                    QTextCodec *codec = QTextCodec::codecForName("UTF-8");
//                    QString desStr = codec->fromUnicode(Blocks);
                    Data = Data.replace(index, 12, tr(Blocks.toLatin1().data()));
//                    Data = unicodeToUtf8(Data);
                }
//                while (Data.indexOf("\\u")>=0)
//                {
//                    int index = Data.indexOf("\\u");
//                    QString Block = Data.mid(index,6);
//                    Block = Block.replace("\\u", QString());
//                    int nHex = Block.toInt(0, 16);
//                    Data.replace(index, 6, QChar(nHex));
//                }
                _output.append(Data);//创建序列
                if(!SendCMD_argu.contains("-q")) ui->Receiver->append(Data);
                if(!SendCMD_argu.contains("-q")) ui->Receiver->horizontalScrollBar()->setValue(ui->Receiver->horizontalScrollBar()->maximum());
            }
        }
        emit CMD_Reply(_output);//抛出槽触发
        //            CMD_Reply *CMD_reply = new CMD_Reply(_output);
        //            QCoreApplication::postEvent(this, CMD_reply);//抛出事件
    });
    /* 将QProcess的 "启动/写入/结束" 打印到界面 */
    connect(cmd,&QProcess::started,this,[=](){
        ui->Receiver->append("->Started->->->");
        ui->Receiver->horizontalScrollBar()->setValue(ui->Receiver->horizontalScrollBar()->maximum());
    });
    connect(cmd, QOverload<int, QProcess::ExitStatus>::of(&QProcess::finished),[=](int exitCode, QProcess::ExitStatus exitStatus){
        Q_UNUSED(exitCode);
        Q_UNUSED(exitStatus);
        if(!CloseDialog)
        {
            ui->Receiver->append("->Finished->->->");
            ui->Receiver->horizontalScrollBar()->setValue(ui->Receiver->horizontalScrollBar()->maximum());
            cmd->start("/bin/bash");
        }
    });

//    cmd->setReadChannelMode(QProcess::ForwardedChannels);
    cmd->start("/bin/bash");
    ui->Sender->setFocus();
//    SendCMD("export DISPLAY=\":0.0\"");
    this->resize((int)(SizePercent.width()*800),(int)(SizePercent.height()*480));
    this->setMaximumSize((int)(SizePercent.width()*800),(int)(SizePercent.height()*480));
    this->setFixedSize((int)(SizePercent.width()*800),(int)(SizePercent.height()*480));
}

//QString TerminalDialog::unicodeToUtf8(const QString resStr)
//{
//    QString temp;
//    for(int i=0;i<resStr.length();)
//    {
//        if(resStr.at(i)=='\\' && resStr.at(i+1)=='x')
//        {
//            QString str = resStr.mid(i+2,i+4);
//            temp.append(str.mid(0,2).toUShort(0,16));
//            i=i+4;
//        }
//        else
//        {
//            temp.append(resStr.at(i));
//            ++i;
//        }
//    }
//    QTextCodec *codec = QTextCodec::codecForName("UTF-8");
//    QString desStr = codec->fromUnicode(temp);
//    return tr(desStr.toLatin1().data());
//}

TerminalDialog::~TerminalDialog()
{
    delete ui;

    CloseDialog = true;
    if (cmd->state() == QProcess::Running)
    {
        cmd->terminate();
        cmd->close();
        cmd->waitForFinished();
    }
}

void TerminalDialog::showEvent(QShowEvent *event)
{
    Q_UNUSED(event);
    if(FirstLoad)
    {
//        QMessageBox::information(NULL, "Warning", "Dangerous, careful operation!", QMessageBox::Ok, QMessageBox::Ok);
        FirstLoad = !FirstLoad;
    }
}

void TerminalDialog::on_Btn_Back_clicked()
{
    this->close();
}

void TerminalDialog::on_Btn_Send_clicked()
{
    if(ui->Sender->text().isEmpty())
    {
//        ui->Sender->setFocus();
        return;
    }
    ui->Sender->setText(ui->Sender->text().trimmed());

    SendCMD(ui->Sender->text());
//    ui->Sender->setFocus();
}

void TerminalDialog::on_pushButton_clicked()
{
    ui->Receiver->clear();
}

void TerminalDialog::SendCMD(QString CommandLine , QString argu)
{
    SendCMD_argu = argu;
    command = CommandLine.trimmed();
    if(command.isEmpty() || command == "")
    {
        return;
    }
    if(!cmd->isOpen())
    {
        cmd->open();
    }
    if(command.toLower() == "exit app" || command.toLower() == "exitapp")
        qApp->exit(Base_OnlyExitApp);
    else if(cmd->isOpen() && cmd->waitForStarted())
    {
        if(!SendCMD_argu.contains("-q")) ui->Receiver->append(">>Sender:");
        if(!SendCMD_argu.contains("-q")) ui->Receiver->append(command);
        if(command.contains("sudo "))
        {
            int startlocal = command.indexOf("sudo ");
            command = command.replace(startlocal,5,"echo \"" + CMD_Password + "\" | sudo -S ");//片段阔幅代码未处理
        }
        cmd->write(command.toLocal8Bit()+ "\n");
        if(!SendCMD_argu.contains("-q")) ui->Receiver->horizontalScrollBar()->setValue(ui->Receiver->horizontalScrollBar()->maximum());
    }
    else if(!cmd->isOpen())
    {
        ui->Receiver->append("->System Busy->->->");
    }
}

//QEvent::Type CMD_Reply::m_EventType = QEvent::None;

//CMD_Reply::CMD_Reply(QStringList _value) : QEvent(eventType())
//{
//    m_StringList = _value;
//}

//CMD_Reply::~CMD_Reply()
//{

//}

//QEvent::Type CMD_Reply::eventType()
//{
//    // 创建事件Type
//    if (m_EventType == QEvent::None)
//        m_EventType = (QEvent::Type)QEvent::registerEventType();

//    return m_EventType;
//}

//QStringList CMD_Reply::getValue(void)
//{
//    return m_StringList;
//}

void TerminalDialog::on_Btn_Logo_clicked()
{
    if(QMessageBox::information(NULL, "Warning", "Do you want to change the incoming password from the console?", QMessageBox::Yes | QMessageBox::No,QMessageBox::No) == QMessageBox::No)
        return;
    InputDialog *inputdialog = new InputDialog(this);
    connect(inputdialog,&InputDialog::closedialog,this,[=](QString value,bool SettingMark){
        if(SettingMark)
        {
            CMD_Password = value;
        }
        delete sender();
    });
    inputdialog->setvalue("Please Input Password","",CMD_Password);
}

void TerminalDialog::on_Btn_Kill_clicked()
{
    cmd->terminate();
}
