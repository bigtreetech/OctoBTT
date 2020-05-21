#ifndef INPUTDIALOG_H
#define INPUTDIALOG_H

#include <QDialog>
#include <QSizeF>

extern int DebugFlat ;
extern QSizeF SizePercent;

namespace Ui {
class InputDialog;
}

class InputDialog : public QDialog
{
    Q_OBJECT

signals:
    void closedialog(QString &value,bool &SettingMark);

public:
    explicit InputDialog(QWidget *parent = nullptr);
    ~InputDialog();

public:
//    QString ReturnValue;
//    bool SettingMark = false;
    enum Option
    {
        All = 0,
        AD_int = 1,
        A_int = 2,
        AD_double = 3,
        A_double = 4,
        C_Lower = 5,
        C_Upper = 6 ,
        F_ip = 7
    };
    QStringList Options
    {
        "-all",
        "-int",
        "int",
        "-double",
        "double",
        "-lower",
        "-upper",
        "-ip"
    };

public:
    void setvalue(QString PlaceholdText , QString InputMode , QString DefaultValue = QString::Null());

private slots:
    virtual void on_Btn_Undo_clicked();

    virtual void on_Btn_Confirm_clicked();

    virtual void on_Func_UL_clicked(bool checked);

    virtual void on_Func_Ang_clicked(bool checked);

    virtual void on_Func_BS_clicked();

    virtual void on_Key_Left_clicked();

    virtual void on_Key_Right_clicked();

    virtual void on_CharKey_click();

    virtual void on_NumKey_click();

    virtual void on_SymbKey_click();

    virtual void on_Space_clicked();

    virtual void on_Beep_clicked();

private:
    Ui::InputDialog *ui;
//    QWidget *FUI;
    QList<QPushButton *> NumBtns;
    QList<QPushButton *> CharBtns;
    QList<QPushButton *> SymbBtns;
    QList<QPushButton *> AllBtns;

    bool Func_UL_State = false;
    bool Func_Ang_State = false;

    Qt::KeyboardModifier Key_Modifier = Qt::NoModifier;

protected:
    void showEvent(QShowEvent *event);
};

#endif // INPUTDIALOG_H
