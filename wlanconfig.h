#ifndef WLANCONFIG_H
#define WLANCONFIG_H

#include <QDialog>
#include <QMap>
#include <QSizeF>
#include <QMetaType>
#include <QStandardItemModel>

extern int DebugFlat ;
extern QSizeF SizePercent;

struct WPAInfo{
    int id;
    double level;
    bool connectflags;
    bool pwdError;
    WPAInfo()
    {
        id = -1;
        level = -99;
        connectflags = false;
        pwdError = false;
    }
};
Q_DECLARE_METATYPE(WPAInfo);

namespace Ui {
class Wlanconfig;
}

class Wlanconfig : public QDialog
{
    Q_OBJECT

public:
    explicit Wlanconfig(QWidget *parent = nullptr);
    ~Wlanconfig();

    void GetNetworkInfo();

private slots:
    virtual void on_Btn_Back_clicked();

    void on_Btn_DevRefresh_clicked();

    void on_Btn_DevSwitch_clicked();

    void on_Btn_Connect_clicked();

    void on_Btn_Scan_clicked();

    void on_DevList_currentIndexChanged(const QString &arg1);

    void on_WPA_List_doubleClicked(const QModelIndex &index);

    void on_Btn_Help_clicked();

private:
    void GetDevState();
    void GetDevState(QStringList &Responses);
    void ScanWPAList();
    void ScanWPAList(QStringList &Responses);
    void ScanWPA();
    void ScanWPA(QStringList &Responses);
    void ScanWPA_Ressult();
    void ScanWPA_Ressult(QStringList &Responses);
    QString ReverseStr(QString args);
    void ConnectNetwork();
    void SetPassword(QStringList &Responses);
    void SetPassword();
    void SetSSID();
    void SetSSID(QStringList &Responses);
    void CreateNetwork();
//    void GetNetworkInfo();
    void GetNetworkInfo(QStringList &Responses);
    void RemoveNetwork();
    void RemoveNetwork(QStringList &Responses);


private:
    Ui::Wlanconfig *ui;
    QWidget *FUI;

    QString TeminalState = "-q-e";

    QMap<QString,bool> WPA_Switch ;
    QMap<QString,QString> WPA_SSID ;

    QMap<QString,WPAInfo> WPA_Scan_SSID_List ;

    QStandardItemModel *WPAModel = new QStandardItemModel(this);
    QString LastDev;

protected:
    void showEvent(QShowEvent *event);
};

#endif // WLANCONFIG_H
