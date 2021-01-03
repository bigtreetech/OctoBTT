#ifndef TERMINALDIALOG_H
#define TERMINALDIALOG_H

#include <QDialog>
#include <QProcess>
#include <QShowEvent>
#include <QSizeF>
//#include <QEvent>


extern int DebugFlat ;
extern QSizeF SizePercent;

namespace Ui {
class TerminalDialog;
}

class TerminalDialog : public QDialog
{
    Q_OBJECT
signals:
    void CMD_Reply(QStringList &value);

public:
    explicit TerminalDialog(QWidget *parent = nullptr);
    ~TerminalDialog();

    void SendCMD(QString CommandLine, QString argu = QString());

    void Show_debug(QString msg);

private slots:
    virtual void on_Btn_Back_clicked();

    virtual void on_Btn_Send_clicked();

    virtual void on_pushButton_clicked();

    virtual void on_Btn_Logo_clicked();

    void on_Btn_Kill_clicked();

private:
    Ui::TerminalDialog *ui;
    QProcess *cmd;
//    bool CMD_NoPassword = false;
    QString command;
    QWidget *FUI;
    bool FirstLoad = true;
    QString SendCMD_argu;
    bool CloseDialog = false;

//    QString unicodeToUtf8(const QString resStr);

public:
    QString CMD_Password = "raspberry";

protected:
    void showEvent(QShowEvent *event);
};

//class CMD_Reply : public QEvent
//{
//public:
//    CMD_Reply(QStringList _value);
//    ~CMD_Reply();

//    static Type eventType();
//    QStringList getValue(void);

//private:
//    static Type m_EventType;
//    QStringList m_StringList;
//};

#endif // TERMINALDIALOG_H
