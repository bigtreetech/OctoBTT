#ifndef OCTONETWORK_H
#define OCTONETWORK_H

#include <QJsonDocument>
#include <QJsonObject>

#include <QNetworkReply>

extern QByteArray Global_X_API_Key ;//Test Key

extern int DebugFlat ;
//extern QSize SizePercent;

class OctoNetwork
{
    public:
        OctoNetwork(QWidget *parent = nullptr);
//        void Initialize(QByteArray New_X_API_Key = "");
        QJsonValue SearchJsonValue(QList<QString> Find_List_Name , QJsonObject _QObj);
        QJsonValue SearchJsonValue(QList<QString> Find_List_Name ,QNetworkReply *reply);
        QList<QJsonValue> SearchJsonValue(QList<QList<QString>> Find_List_Names , QJsonObject _QObj);
        QList<QJsonValue> SearchJsonValue(QList<QList<QString>> Find_List_Names ,QNetworkReply *reply);
        void SendGCode(QList<QString> _GCode_);
        void SendGCode(QString _GCode_);
        void JobSwitch(QString FilePath);
        void JobSwitch(QUrl FileUrl);
        void JobSwitch();
        void SD_ReLoad();

    private:
        virtual void SD_CMD(QString CMD);
        QWidget *FUI;

    public:
        QByteArray X_API_Key = Global_X_API_Key;//Test Key
        QString MainIP = "127.0.0.1";
//        QString MainIP = "192.168.100.111";
        QString MainAddress = "http://"+MainIP+"/";
        QString MainUrl = MainAddress + "/api/";
        QString ConnectState = "Close";
        bool ConnectFlat = false;

        QNetworkAccessManager *networkAccessManager = new QNetworkAccessManager();
        QNetworkAccessManager *MCPnetworkAccessManager = new QNetworkAccessManager();
        QNetworkAccessManager *FSnetworkAccessManager = new QNetworkAccessManager();

        QNetworkRequest GetConnectRequest;
        QNetworkRequest GetStateRequest;

        QNetworkRequest SetConnectRequest;
        const QList<QList<QString>> ConnectLink = {{"current","state"},{"options","ports"}};
        const QList<QString> ConnectSerialLink = {"/dev/ttyAMA0","/dev/ttyS0"};
        const QList<QList<QString>> StateLink = {{"state"},{"progress","completion"}};//using with printer in the Printing state

        QNetworkRequest SetCMDRequest;
        QNetworkRequest SDRequest;
        QNetworkRequest FileRequest;

        QNetworkRequest GetTemperatureRequest;
        const QList<QList<QString>> TemperatureLink = {{"temperature","bed","actual"},{"temperature","tool*","actual"}};


    private:
};

#endif // OCTONETWORK_H
