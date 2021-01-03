#include "controlpanel.h"
#include "ui_controlpanel.h"
#include <QProcess>
#include <mainwindow.h>
#include <QDialog>
//#include <QSizeF>
#include <QPalette>
#include <QBitmap>
#include <QPainter>
#include <QTextBrowser>
#include <QDesktopWidget>


ControlPanel::ControlPanel(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ControlPanel)
{
    ui->setupUi(this);
    FUI = (MainWindow*)parent;//((MainWindow*)FUI)->octonetwork.SendGCode(_GCode_Setting);

    //使控件适应屏幕的大小
    this->resize((int)(SizePercent.width()*800),(int)(SizePercent.height()*480));   //将控件大小固定为800*480
    this->setMaximumSize((int)(SizePercent.width()*800),(int)(SizePercent.height()*480));
    this->setFixedSize((int)(SizePercent.width()*800),(int)(SizePercent.height()*480));

    //设置冷热按键的样式表
    ui->Btn_Cool->setStyleSheet("QPushButton        {padding: 10px;border-style: solid;border-width: 5px;border-radius: 20px;border-color: rgb(36, 148, 251);color: rgb(245, 246, 250);}"\
                                "QPushButton:hover  {padding: 10px;border-style: solid;border-width: 5px;border-radius: 20px;border-color: rgb(36, 148, 251); background-color: rgb(18, 74, 125);}"\
                                "QPushButton:pressed{padding: 10px;border-style: solid;border-width: 5px;border-radius: 20px;border-color: rgb(36, 148, 251); background-color: rgb(18, 74, 125); padding-right:4px;padding-bottom:4px;}");
    ui->Btn_Heat->setStyleSheet("QPushButton        {padding: 10px;border-style: solid;border-width: 5px;border-radius: 20px;border-color: rgb(216, 30, 6);color: rgb(245, 246, 250);}"\
                                "QPushButton:hover  {padding: 10px;border-style: solid;border-width: 5px;border-radius: 20px;border-color: rgb(216, 30, 6); background-color: rgb(108, 15, 3);}"\
                                "QPushButton:pressed{padding: 10px;border-style: solid;border-width: 5px;border-radius: 20px;border-color: rgb(216, 30, 6); background-color: rgb(108, 15, 3); padding-right:4px;padding-bottom:4px;}");

    ui->Btn_Axle_Home->setStyleSheet("border-color: rgb(230, 230, 230);");
    ui->Btn_Unlock->setStyleSheet("border-color: rgb(43, 166, 56);");
    ui->Btn_Restore->setStyleSheet("border-color: rgb(205, 205, 205);");
    ui->Btn_Shutdown->setStyleSheet("border-color: rgb(216, 30, 6);");
    ui->Btn_Extruder_E->setStyleSheet("border-color: rgb(245, 246, 255);");
    ui->Btn_Extruder_R->setStyleSheet("border-color: rgb(245, 246, 255);");
}

ControlPanel::~ControlPanel()
{
    delete ui;
}

void ControlPanel::on_Btn_Back_clicked()
{
    this->close();//关闭窗口
}

void ControlPanel::on_Btn_R_1_clicked()
{
    MoveSpeed = 1;
    ui->Btn_R_1->setStyleSheet(SelectKey);
    ui->Btn_R_5->setStyleSheet(UnSelectKey);
    ui->Btn_R_10->setStyleSheet(UnSelectKey);
    ui->Btn_R_50->setStyleSheet(UnSelectKey);
    ui->Btn_R_100->setStyleSheet(UnSelectKey);
}

void ControlPanel::on_Btn_R_5_clicked()
{
    MoveSpeed = 5;
    ui->Btn_R_1->setStyleSheet(UnSelectKey);
    ui->Btn_R_5->setStyleSheet(SelectKey);
    ui->Btn_R_10->setStyleSheet(UnSelectKey);
    ui->Btn_R_50->setStyleSheet(UnSelectKey);
    ui->Btn_R_100->setStyleSheet(UnSelectKey);
}

void ControlPanel::on_Btn_R_10_clicked()
{
    MoveSpeed = 10;
    ui->Btn_R_1->setStyleSheet(UnSelectKey);
    ui->Btn_R_5->setStyleSheet(UnSelectKey);
    ui->Btn_R_10->setStyleSheet(SelectKey);
    ui->Btn_R_50->setStyleSheet(UnSelectKey);
    ui->Btn_R_100->setStyleSheet(UnSelectKey);
}

void ControlPanel::on_Btn_R_50_clicked()
{
    MoveSpeed = 50;
    ui->Btn_R_1->setStyleSheet(UnSelectKey);
    ui->Btn_R_5->setStyleSheet(UnSelectKey);
    ui->Btn_R_10->setStyleSheet(UnSelectKey);
    ui->Btn_R_50->setStyleSheet(SelectKey);
    ui->Btn_R_100->setStyleSheet(UnSelectKey);
}

void ControlPanel::on_Btn_R_100_clicked()
{
    MoveSpeed = 100;
    ui->Btn_R_1->setStyleSheet(UnSelectKey);
    ui->Btn_R_5->setStyleSheet(UnSelectKey);
    ui->Btn_R_10->setStyleSheet(UnSelectKey);
    ui->Btn_R_50->setStyleSheet(UnSelectKey);
    ui->Btn_R_100->setStyleSheet(SelectKey);
}

void ControlPanel::on_Btn_Heat_toggled(bool checked)
{

    if(checked)//如果升温按钮被点击，关闭降温按钮
    {
        ui->Btn_Cool->setStyleSheet("QPushButton        {padding: 10px;border-style: solid;border-width: 5px;border-radius: 20px;border-color: rgb(36, 148, 251);}"\
                                    "QPushButton:hover  {padding: 10px;border-style: solid;border-width: 5px;border-radius: 20px;border-color: rgb(36, 148, 251); background-color: rgb(18, 74, 125);}"\
                                    "QPushButton:pressed{padding: 10px;border-style: solid;border-width: 5px;border-radius: 20px;border-color: rgb(36, 148, 251); background-color: rgb(18, 74, 125); padding-right:4px;padding-bottom:4px;}");

        ui->Btn_Heat->setStyleSheet("QPushButton        {padding: 10px;border-style: solid;border-width: 5px;border-radius: 20px;border-color: rgb(216, 30, 6); background-color: rgb(108, 15, 3); padding-right:4px;padding-bottom:4px;}"\
                                    "QPushButton:hover  {padding: 10px;border-style: solid;border-width: 5px;border-radius: 20px;border-color: rgb(216, 30, 6); background-color: rgb(108, 15, 3); padding-right:4px;padding-bottom:4px;}"\
                                    "QPushButton:pressed{padding: 10px;border-style: solid;border-width: 5px;border-radius: 20px;border-color: rgb(216, 30, 6); background-color: rgb(108, 15, 3); padding-right:4px;padding-bottom:4px;}");
        if(ui->Btn_Cool->isChecked())
        {
            ui->Btn_Cool->setChecked(false);
            //发送升温指令
            QList<QString> _GCode_Setting;
            _GCode_Setting.append("M140 S60");
            _GCode_Setting.append("M104 S200");
            ((MainWindow*)FUI)->octonetwork.SendGCode(_GCode_Setting);
        }

    }
}

void ControlPanel::on_Btn_Cool_toggled(bool checked)
{

    if(checked)
    {
        ui->Btn_Heat->setStyleSheet("QPushButton        {padding: 10px;border-style: solid;border-width: 5px;border-radius: 20px;border-color: rgb(216, 30, 6);}"\
                                    "QPushButton:hover  {padding: 10px;border-style: solid;border-width: 5px;border-radius: 20px;border-color: rgb(216, 30, 6); background-color: rgb(108, 15, 3);}"\
                                    "QPushButton:pressed{padding: 10px;border-style: solid;border-width: 5px;border-radius: 20px;border-color: rgb(216, 30, 6); background-color: rgb(108, 15, 3); padding-right:4px;padding-bottom:4px;}");
        ui->Btn_Cool->setStyleSheet("QPushButton        {padding: 10px;border-style: solid;border-width: 5px;border-radius: 20px;border-color: rgb(36, 148, 251); background-color: rgb(18, 74, 125); padding-right:4px;padding-bottom:4px;}"\
                                    "QPushButton:hover  {padding: 10px;border-style: solid;border-width: 5px;border-radius: 20px;border-color: rgb(36, 148, 251); background-color: rgb(18, 74, 125); padding-right:4px;padding-bottom:4px;}"\
                                    "QPushButton:pressed{padding: 10px;border-style: solid;border-width: 5px;border-radius: 20px;border-color: rgb(36, 148, 251); background-color: rgb(18, 74, 125); padding-right:4px;padding-bottom:4px;}");
    }

//    ui->Btn_Cool->setStyleSheet(checked?ui->Btn_Cool->styleSheet()+"background-color:rgb(18, 74, 125);":"padding: 10px;\nborder-style: solid;\nborder-width: 5px;\nborder-radius: 20px;\nborder-color: rgb(36, 148, 251);");


    QList<QString> _GCode_;
    if(checked)//如果降下温按钮被点击，关闭升温按钮
    {
        if(ui->Btn_Heat->isChecked())
            ui->Btn_Heat->setChecked(false);
        //发送降温指令
        _GCode_.append("M106 S255");
        _GCode_.append("M104 S0");
        _GCode_.append("M140 S0");
    }
    else
    {
        _GCode_.append("M106 S0");//?
    }
    ((MainWindow*)FUI)->octonetwork.SendGCode(_GCode_);
}

//坐标归位按键按下所有坐标归位，按键变色
void ControlPanel::on_Btn_Axle_Home_clicked()
{
    //发送归位指令
    QList<QString> _GCode_;
    _GCode_.append("G91");
    _GCode_.append("G28");
    _GCode_.append("G90");
    ((MainWindow*)FUI)->octonetwork.SendGCode(_GCode_);
}

//Unlock Motors 锁定喷头温度和工床温度
void ControlPanel::on_Btn_Unlock_clicked()
{
    QList<QString> _GCode_;
    _GCode_.append("M18");
    ((MainWindow*)FUI)->octonetwork.SendGCode(_GCode_);
}

//重启 OctoBTT、重启OctoPrint以及取消
void ControlPanel::on_Btn_Restore_clicked()
{
//    StyleSheet_Temp = ui->Btn_Restore->styleSheet();//获取按键的样式表用以恢复颜色
//    ui->Btn_Restore->setStyleSheet(StyleSheet_Temp+"\nbackground-color:rgb(128,128,128,128);");

    QStringList btnName = {"Restart OctoBTT" , "Restart OctoPrint", "Cencal"} ;
    CustomDialog *newDialog = new CustomDialog();
    QObject::connect(newDialog, &CustomDialog::OutputEvent, newDialog,[=](QString instruct)
    {
        if(instruct == "Restart OctoBTT")
        {
            //重启OctoBTT
            qApp->exit(Base_Restart);
        }
        else if(instruct == "Restart OctoPrint")
        {
            //重启Octoprint
            QProcess P(0);
            P.start("sudo service octoprint restart");
            P.waitForStarted();
            P.waitForFinished();
            qApp->exit(Base_Restart);
        }
    });
    newDialog->showCustomDialog("what do you want ?",":/assets/zhongzhi.svg",btnName,this);
}

//紧急制停、关机以及取消
void ControlPanel::on_Btn_Shutdown_clicked()
{
    QStringList btnName = {"Emergency Stop" , "Shutdown", "Cencal"} ;
    CustomDialog *newDialog = new CustomDialog();
    QObject::connect(newDialog, &CustomDialog::OutputEvent, newDialog,[=](QString instruct)
    {
        if(instruct == "Emergency Stop")
        {
            //紧急制停
            QList<QString> _GCode_;
            //        _GCode_.append("M110 N0");
            _GCode_.append("M112");
            ((MainWindow*)FUI)->octonetwork.SendGCode(_GCode_);
        }
        else if(instruct == "Shutdown")
        {
            //关机
            QProcess P(0);
            P.start("xset dpms force off");
            P.waitForStarted();
            P.waitForFinished();
            qApp->exit(Base_Shutdown);
        }
    });
    newDialog->showCustomDialog("what do you want ?",":/assets/hazard.svg",btnName,this);
}

void ControlPanel::on_Btn_Axle_X_clicked()
{
    QList<QString> _GCode_;
    _GCode_.append("G91");
    _GCode_.append("G28 X");
    _GCode_.append("G90");
    ((MainWindow*)FUI)->octonetwork.SendGCode(_GCode_);
}

void ControlPanel::on_Btn_Axle_Y_clicked()
{
    QList<QString> _GCode_;
    _GCode_.append("G91");
    _GCode_.append("G28 Y");
    _GCode_.append("G90");
    ((MainWindow*)FUI)->octonetwork.SendGCode(_GCode_);
}

void ControlPanel::on_Btn_Axle_Z_clicked()
{
    QList<QString> _GCode_;
    _GCode_.append("G91");
    _GCode_.append("G28 Z");
    _GCode_.append("G90");
    ((MainWindow*)FUI)->octonetwork.SendGCode(_GCode_);
}

void ControlPanel::on_Btn_XY_Res_clicked()
{
    QList<QString> _GCode_;
    _GCode_.append("G91");
    _GCode_.append("G28 X Y");
    _GCode_.append("G90");
    ((MainWindow*)FUI)->octonetwork.SendGCode(_GCode_);
}

void ControlPanel::on_Btn_Y_Add_clicked()
{
    QList<QString> _GCode_;
    _GCode_.append("G91");
    _GCode_.append("G1 Y"+QString::number(MoveSpeed)+" F6000");
    _GCode_.append("G90");
    ((MainWindow*)FUI)->octonetwork.SendGCode(_GCode_);
}

void ControlPanel::on_Btn_Y_Dec_clicked()
{
    QList<QString> _GCode_;
    _GCode_.append("G91");
    _GCode_.append("G1 Y-"+QString::number(MoveSpeed)+" F6000");
    _GCode_.append("G90");
    ((MainWindow*)FUI)->octonetwork.SendGCode(_GCode_);
}

void ControlPanel::on_Btn_X_Dec_clicked()
{
    QList<QString> _GCode_;
    _GCode_.append("G91");
    _GCode_.append("G1 X-"+QString::number(MoveSpeed)+" F6000");
    _GCode_.append("G90");
    ((MainWindow*)FUI)->octonetwork.SendGCode(_GCode_);
}

void ControlPanel::on_Btn_X_Add_clicked()
{
    QList<QString> _GCode_;
    _GCode_.append("G91");
    _GCode_.append("G1 X"+QString::number(MoveSpeed)+" F6000");
    _GCode_.append("G90");
    ((MainWindow*)FUI)->octonetwork.SendGCode(_GCode_);
}

void ControlPanel::on_Btn_Z_Add_clicked()
{
    QList<QString> _GCode_;
    _GCode_.append("G91");
    _GCode_.append("G1 Z"+QString::number(MoveSpeed)+" F200");
    _GCode_.append("G90");
    ((MainWindow*)FUI)->octonetwork.SendGCode(_GCode_);
}

void ControlPanel::on_Btn_Z_Dec_clicked()
{
    QList<QString> _GCode_;
    _GCode_.append("G91");
    _GCode_.append("G1 Z-"+QString::number(MoveSpeed)+" F200");
    _GCode_.append("G90");
    ((MainWindow*)FUI)->octonetwork.SendGCode(_GCode_);
}

void ControlPanel::on_Btn_Extruder_R_clicked()
{
    QList<QString> _GCode_;
    _GCode_.append("G91");
    _GCode_.append("G1 E-"+QString::number(MoveSpeed)+" F300");
    _GCode_.append("G90");
    ((MainWindow*)FUI)->octonetwork.SendGCode(_GCode_);
}

void ControlPanel::on_Btn_Extruder_E_clicked()
{
    QList<QString> _GCode_;
    _GCode_.append("G91");
    _GCode_.append("G1 E"+QString::number(MoveSpeed)+" F300");
    _GCode_.append("G90");
    ((MainWindow*)FUI)->octonetwork.SendGCode(_GCode_);
}


void ControlPanel::on_Btn_Z_BS_Add_clicked()
{
    QList<QString> _GCode_;
//    _GCode_.append("M110 N0");
    _GCode_.append("G91");
    _GCode_.append("M290 Z"+QString::number((double)MoveSpeed/100));
    _GCode_.append("M500");
    _GCode_.append("G90");
    ((MainWindow*)FUI)->octonetwork.SendGCode(_GCode_);
}

void ControlPanel::on_Btn_Z_BS_Dec_clicked()
{
    QList<QString> _GCode_;
//    _GCode_.append("M110 N0");
    _GCode_.append("G91");
    _GCode_.append("M290 Z-"+QString::number((double)MoveSpeed/100));
    _GCode_.append("M500");
    _GCode_.append("G90");
    ((MainWindow*)FUI)->octonetwork.SendGCode(_GCode_);
}

//自动调平按键
void ControlPanel::on_Btn_Level_clicked()
{  
    //如果点击退出，关闭弹窗
    if(((MainWindow*)FUI)->octonetwork.ConnectState == "Operational")
    {
        QStringList btnName = {"Yes", "No"} ;
        CustomDialog *newDialog = new CustomDialog();
        QObject::connect(newDialog, &CustomDialog::OutputEvent, newDialog,[=](QString instruct)
        {
            if(instruct == "Yes")
            {
                QList<QString> _GCode_;
                _GCode_.append("M502");
                _GCode_.append("M500");
                _GCode_.append("G91");
                _GCode_.append("G28");
                _GCode_.append("G29");
                _GCode_.append("G90");
                _GCode_.append("M500");
                ((MainWindow*)FUI)->octonetwork.SendGCode(_GCode_);
            }
        });
        newDialog->showCustomDialog("Self-leveling",":/assets/kongzhimianban_B.svg",btnName,"Whether the automatic leveling sensor has been installed ?",this);
    }
    else
    {
        CustomDialog *newDialog = new CustomDialog();
        newDialog->showCustomDialog("Self-leveling",":/assets/kongzhimianban_B.svg","Current state not supported !",this);
    }
}

