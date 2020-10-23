#ifndef GCODEDIALOG_H
#define GCODEDIALOG_H

#include <QDialog>
#include <QProcess>
#include <QShowEvent>
#include <QSizeF>
#include <QDebug>
//#include <QEvent>

#include <QWebSocket>
#include <QTimer>
#include <QJsonDocument>
#include <QJsonObject>

extern int DebugFlat ;
extern QSizeF SizePercent;

namespace Ui {
class GcodeDialog;
}

class GcodeDialog : public QDialog
{
    Q_OBJECT
signals:

private slots:
    void onWSDisconnected();
    void onWSConnected();
    void onWSTextReceived(QString msg);
    void onWSReconnect();

public:
    explicit GcodeDialog(QWidget *parent = nullptr);
    ~GcodeDialog();

private slots:
    virtual void on_Btn_Back_clicked();

    virtual void on_Btn_Send_clicked();

    virtual void on_pushButton_clicked();

    virtual void on_Btn_Logo_clicked();

private:
    Ui::GcodeDialog *ui;
    QWidget *FUI;

private:
    QJsonValue SearchWSJsonValue(QList<QString> Find_List_Name , QJsonObject _QObj);
    QList<QJsonValue> SearchWSJsonValue(QList<QList<QString>> Find_List_Names , QJsonObject _QObj);

    QTimer* _ptimer;

public:
    QWebSocket* _pdataRecvWS = new QWebSocket();

protected:
    void showEvent(QShowEvent *event);
};

#endif // GCODEDIALOG_H
