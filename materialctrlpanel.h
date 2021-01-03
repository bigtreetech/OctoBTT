#ifndef MATERIALCTRLPANEL_H
#define MATERIALCTRLPANEL_H


#include <QDialog>
#include <QNetworkReply>
#include <QSizeF>
#include <QTimer>



extern int DebugFlat ;
extern QSizeF SizePercent;

namespace Ui {
    class MaterialCtrlPanel;
    class mTimer;
}

class mTimer : public QTimer
{
    Q_OBJECT
    public:
        mTimer(double *_value_,QPushButton *_button_,int _addval_)
        {
            value=_value_;
            button=_button_;
            addval=_addval_;
            connect(this, &mTimer::timeout, this, [=](){
                if(count == 9)
                {
                    addval *= 10;
                    count++;
                }
                else
                    count++;
                emit mtimeout();
            });
        }

        //传入变更值，按键以及阈值
        double *value;
        QPushButton *button;
        double addval;

    private:
        int count = 0;

    signals:
        void mtimeout();

};
class MaterialCtrlPanel : public QDialog
{
        Q_OBJECT

    public:
        explicit MaterialCtrlPanel(QWidget *parent = nullptr);
        ~MaterialCtrlPanel();

    public:
    void longPress(const double *maximumCount);

        double Hotend = 0;
        double Heatbed = 0;
        double Tool = 0;
        double Feedrate = 0;
        double Flowrate = 0;
        double FanSpeed = 0;
        const double Hotend_Max = 260;
        const double Heatbed_Max = 260;
        const double Tool_Max = 0;
        const double Feedrate_Max = 100;
        const double Flowrate_Max = 100;
        const double FanSpeed_Max = 255;
        QString Symb_Temp = "℃";
        QString Symb_Per = "%";
        bool ReplyFlag = false;

    private slots:
        virtual void on_Btn_back_clicked();

        virtual void on_Btn_Save_pressed();

        virtual void on_Btn_Save_released();

        virtual void on_Btn_Hotend_A_pressed();

        virtual void on_Btn_Hotend_D_released();

        virtual void on_Btn_Hotend_A_released();

        virtual void on_Btn_Hotend_D_pressed();

        virtual void on_Btn_Heatbed_A_pressed();

        virtual void on_Btn_Heatbed_A_released();

        virtual void on_Btn_Heatbed_D_pressed();

        virtual void on_Btn_Heatbed_D_released();

        virtual void on_Btn_Feedrate_A_pressed();

        virtual void on_Btn_Feedrate_A_released();

        virtual void on_Btn_Feedrate_D_pressed();

        virtual void on_Btn_Feedrate_D_released();

        virtual void on_Btn_Flowrate_A_pressed();

        virtual void on_Btn_Flowrate_A_released();

        virtual void on_Btn_Flowrate_D_pressed();

        virtual void on_Btn_Flowrate_D_released();

        virtual void TimerTimeOut();

        virtual void Reply(QNetworkReply *reply);

    private:
        double Hotend_Target = 0;
        double Heatbed_Target = 0;
        double Tool_Target = 0;
        double Feedrate_Target = 0;
        double Flowrate_Target = 0;
        double FanSpeed_Target = 0;
        double number = 0;
        int SettingStep = 1;

        bool timerStartFlag = false;

        bool firstClickedFlag = false;

        mTimer *timer;

        bool SettingFlag = false;

        enum ReLoop {E_Temperature};
        int ReLoopFlag;

        Ui::MaterialCtrlPanel *ui;
        QWidget *FUI;
        QString StyleSheet_Temp;


};

#endif // MATERIALCTRLPANEL_H
