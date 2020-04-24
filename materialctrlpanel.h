#ifndef MATERIALCTRLPANEL_H
#define MATERIALCTRLPANEL_H

#include <QDialog>
#include <QNetworkReply>

namespace Ui {
    class MaterialCtrlPanel;
}

class MaterialCtrlPanel : public QDialog
{
        Q_OBJECT

    public:
        explicit MaterialCtrlPanel(QWidget *parent = nullptr);
        ~MaterialCtrlPanel();

    public:
        double Hotend = 0;
        double Heatbed = 0;
        double Tool = 0;
        double Feedrate = 0;
        double Flowrate = 0;
        double FanSpeed = 0;
        double Hotend_Max = 200;
        double Heatbed_Max = 200;
        double Tool_Max = 0;
        double Feedrate_Max = 100;
        double Flowrate_Max = 100;
        double FanSpeed_Max = 255;
        QString Symb_Temp = "℃";
        QString Symb_Per = "%";
        bool ReplyFlag = false;

    private slots:
        void on_Btn_back_clicked();

        void on_Btn_Save_pressed();

        void on_Btn_Save_released();

        void on_Btn_Hotend_A_pressed();

        void on_Btn_Hotend_D_released();

        void on_Btn_Hotend_A_released();

        void on_Btn_Hotend_D_pressed();

        void on_Btn_Heatbed_A_pressed();

        void on_Btn_Heatbed_A_released();

        void on_Btn_Heatbed_D_pressed();

        void on_Btn_Heatbed_D_released();

        void on_Btn_Feedrate_A_pressed();

        void on_Btn_Feedrate_A_released();

        void on_Btn_Feedrate_D_pressed();

        void on_Btn_Feedrate_D_released();

        void on_Btn_Flowrate_A_pressed();

        void on_Btn_Flowrate_A_released();

        void on_Btn_Flowrate_D_pressed();

        void on_Btn_Flowrate_D_released();

        void TimerTimeOut();

        void Reply(QNetworkReply *reply);

        void on_Box_SettingStep_valueChanged(int arg1);

    private:
        double Hotend_Target = 0;
        double Heatbed_Target = 0;
        double Tool_Target = 0;
        double Feedrate_Target = 0;
        double Flowrate_Target = 0;
        double FanSpeed_Target = 0;
        int SettingStep = 1;

        bool SettingFlag = false;

        enum ReLoop {E_Temperature};
        int ReLoopFlag;

        Ui::MaterialCtrlPanel *ui;
        QWidget *FUI;
        QString StyleSheet_Temp;
};

#endif // MATERIALCTRLPANEL_H
