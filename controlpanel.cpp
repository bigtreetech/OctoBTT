#include "controlpanel.h"
#include "ui_controlpanel.h"
#include <QProcess>
#include <mainwindow.h>
#include <QMessageBox>

ControlPanel::ControlPanel(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ControlPanel)
{
    ui->setupUi(this);
    FUI = (MainWindow*)parent;//((MainWindow*)FUI)->octonetwork.SendGCode(_GCode_Setting);
}

ControlPanel::~ControlPanel()
{
    delete ui;
}

void ControlPanel::on_Btn_Back_clicked()
{
    this->close();
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
    ui->Btn_Heat->setStyleSheet(checked?ui->Btn_Heat->styleSheet()+"background-color:rgb(108, 15, 3);":"padding: 10px;\nborder-style: solid;\nborder-width: 5px;\nborder-radius: 20px;\nborder-color: rgb(216, 30, 6);");
}

void ControlPanel::on_Btn_Cool_toggled(bool checked)
{
    ui->Btn_Cool->setStyleSheet(checked?ui->Btn_Cool->styleSheet()+"background-color:rgb(18, 74, 125);":"padding: 10px;\nborder-style: solid;\nborder-width: 5px;\nborder-radius: 20px;\nborder-color: rgb(36, 148, 251);");

    QList<QString> _GCode_;
    if(checked)
    {
        _GCode_.append("M106 S255");
        _GCode_.append("M104 S0");
        _GCode_.append("M140 S0");
    }
    else
    {
        _GCode_.append("M106 S0");
    }
    ((MainWindow*)FUI)->octonetwork.SendGCode(_GCode_);
}

void ControlPanel::on_Btn_Axle_Home_pressed()
{
    StyleSheet_Temp = ui->Btn_Axle_Home->styleSheet();
    ui->Btn_Axle_Home->setStyleSheet(StyleSheet_Temp+"\nbackground-color:rgb(128,128,128,128);");

    QList<QString> _GCode_;
    _GCode_.append("G91");
    _GCode_.append("G28");
    _GCode_.append("G90");
    ((MainWindow*)FUI)->octonetwork.SendGCode(_GCode_);
}

void ControlPanel::on_Btn_Axle_Home_released()
{

    ui->Btn_Axle_Home->setStyleSheet(StyleSheet_Temp);
}
//Unlock Motors
void ControlPanel::on_Btn_BackButton_pressed()
{
    StyleSheet_Temp = ui->Btn_BackButton->styleSheet();
    ui->Btn_BackButton->setStyleSheet(StyleSheet_Temp+"\nbackground-color:rgb(128,128,128,128);");

    QList<QString> _GCode_;
    _GCode_.append("M18");
    ((MainWindow*)FUI)->octonetwork.SendGCode(_GCode_);
}

void ControlPanel::on_Btn_BackButton_released()
{
    ui->Btn_BackButton->setStyleSheet(StyleSheet_Temp);
}

void ControlPanel::on_Btn_Restore_pressed()
{
    StyleSheet_Temp = ui->Btn_Restore->styleSheet();
    ui->Btn_Restore->setStyleSheet(StyleSheet_Temp+"\nbackground-color:rgb(128,128,128,128);");
    if(QMessageBox::information(NULL, "Warning", "Do you want to Restart OctoPrint ?", QMessageBox::Yes  | QMessageBox::No , QMessageBox::No) == QMessageBox::Yes)
    {
        //Release Version
        QProcess P(0);
        P.start("sudo service octoprint restart");
        P.waitForStarted();
        P.waitForFinished();

        qApp->exit(Base_Restart);
    }
    else if(QMessageBox::information(NULL, "Warning", "Do you want to just Restart OctoBTT ?", QMessageBox::Yes  | QMessageBox::No , QMessageBox::No) == QMessageBox::Yes)
    {
        qApp->exit(Base_Restart);
    }
    ui->Btn_Restore->setStyleSheet(StyleSheet_Temp);
}

void ControlPanel::on_Btn_Restore_released()
{
    //ui->Btn_Restore->setStyleSheet(StyleSheet_Temp);
}

void ControlPanel::on_Btn_Shutdown_pressed()
{
    StyleSheet_Temp = ui->Btn_Shutdown->styleSheet();
    ui->Btn_Shutdown->setStyleSheet(StyleSheet_Temp+"\nbackground-color:rgb(128,128,128,128);");

    if(QMessageBox::information(NULL, "Warning", "Do you want to Shutdown?", QMessageBox::Yes  | QMessageBox::No , QMessageBox::No) == QMessageBox::Yes)
    {
        QProcess P(0);
        P.start("xset dpms force off");
        P.waitForStarted();
        P.waitForFinished();
        qApp->exit(Base_Shutdown);
    }
    else if(QMessageBox::information(NULL, "Warning", "Do you want to Emergency Stop?", QMessageBox::Yes  | QMessageBox::No , QMessageBox::No) == QMessageBox::Yes)
    {
        QList<QString> _GCode_;
//        _GCode_.append("M110 N0");
        _GCode_.append("M112");
        ((MainWindow*)FUI)->octonetwork.SendGCode(_GCode_);
    }

    ui->Btn_Shutdown->setStyleSheet(StyleSheet_Temp);
}

void ControlPanel::on_Btn_Shutdown_released()
{
//    ui->Btn_Shutdown->setStyleSheet(StyleSheet_Temp);
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

void ControlPanel::on_Btn_Extruder_R_released()
{
    ui->Btn_Extruder_R->setStyleSheet(StyleSheet_Temp);
}

void ControlPanel::on_Btn_Extruder_E_pressed()
{
    StyleSheet_Temp = ui->Btn_Extruder_E->styleSheet();
    ui->Btn_Extruder_E->setStyleSheet(StyleSheet_Temp+"\nbackground-color:rgb(128,128,128,128);");
}

void ControlPanel::on_Btn_Extruder_E_released()
{
    ui->Btn_Extruder_E->setStyleSheet(StyleSheet_Temp);
}

void ControlPanel::on_Btn_Extruder_R_pressed()
{
    StyleSheet_Temp = ui->Btn_Extruder_R->styleSheet();
    ui->Btn_Extruder_R->setStyleSheet(StyleSheet_Temp+"\nbackground-color:rgb(128,128,128,128);");
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

void ControlPanel::on_Btn_Level_clicked()
{
    //check the level sensor
    if(((MainWindow*)FUI)->octonetwork.ConnectState == "Operational")
    {
        if(QMessageBox::information(NULL, "Questions", "Whether the automatic leveling sensor has been installed ?", QMessageBox::Yes | QMessageBox::No , QMessageBox::No) == QMessageBox::No)
        {
            return;
        }
//        if(((MainWindow*)FUI)->materialctrlpanel->Hotend < 50 && ((MainWindow*)FUI)->materialctrlpanel->Heatbed < 50)
//        {
            QList<QString> _GCode_;
            _GCode_.append("M502");
            _GCode_.append("M500");
            _GCode_.append("G91");
            _GCode_.append("G28");
            _GCode_.append("G29");
            _GCode_.append("G90");
            _GCode_.append("M500");
            ((MainWindow*)FUI)->octonetwork.SendGCode(_GCode_);
//        }
//        else
//        {
//            QMessageBox::information(NULL, "Warning", "Do not level out if the temperature is too high !", QMessageBox::Ok , QMessageBox::Ok);
//        }
    }
    else
    {
        QMessageBox::information(NULL, "Infomation", "Current state not supported !", QMessageBox::Ok , QMessageBox::Ok);
    }
}
