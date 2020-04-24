#ifndef CONTROLPANEL_H
#define CONTROLPANEL_H

#include <QDialog>

const static int Base_Restart = 773;
const static int Base_Shutdown = 772;
const static int Base_OnlyExitApp = 770;

namespace Ui {
    class ControlPanel;
}

class ControlPanel : public QDialog
{
        Q_OBJECT

    public:
        explicit ControlPanel(QWidget *parent = nullptr);
        ~ControlPanel();

    private slots:
        void on_Btn_Back_clicked();

        void on_Btn_R_1_clicked();

        void on_Btn_R_5_clicked();

        void on_Btn_R_10_clicked();

        void on_Btn_R_50_clicked();

        void on_Btn_R_100_clicked();

        void on_Btn_Heat_toggled(bool checked);

        void on_Btn_Cool_toggled(bool checked);

        void on_Btn_Axle_Home_pressed();

        void on_Btn_Axle_Home_released();

        void on_Btn_BackButton_pressed();

        void on_Btn_BackButton_released();

        void on_Btn_Restore_released();

        void on_Btn_Restore_pressed();

        void on_Btn_Shutdown_pressed();

        void on_Btn_Shutdown_released();

        void on_Btn_Axle_X_clicked();

        void on_Btn_Axle_Y_clicked();

        void on_Btn_Axle_Z_clicked();

        void on_Btn_XY_Res_clicked();

        void on_Btn_Y_Add_clicked();

        void on_Btn_Y_Dec_clicked();

        void on_Btn_X_Dec_clicked();

        void on_Btn_X_Add_clicked();

        void on_Btn_Z_Add_clicked();

        void on_Btn_Z_Dec_clicked();

        void on_Btn_Extruder_R_clicked();

        void on_Btn_Extruder_E_clicked();

        void on_Btn_Extruder_R_released();

        void on_Btn_Extruder_E_pressed();

        void on_Btn_Extruder_E_released();

        void on_Btn_Extruder_R_pressed();

        void on_Btn_Z_BS_Add_clicked();

        void on_Btn_Z_BS_Dec_clicked();

        void on_Btn_Level_clicked();

    private:
        Ui::ControlPanel *ui;
        QString UnSelectKey = "\ncolor: rgb(245, 246, 250);";
        QString SelectKey = "background-color: rgb(128,128,128);\ncolor: rgb(245, 246, 250);\nborder-color:rgb(0, 255, 0)";
        QString StyleSheet_Temp;
        int MoveSpeed = 10;
        QWidget *FUI;
};

#endif // CONTROLPANEL_H
