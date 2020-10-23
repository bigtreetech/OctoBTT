#include "octonetwork.h"
#include <QJsonDocument>
#include <QJsonObject>
#include <QNetworkReply>
#include <QDebug>
#include <QJsonArray>
#include <QMessageBox>
#include <filedialog.h>
#include <mainwindow.h>

OctoNetwork::OctoNetwork(QWidget *parent)
{
    FUI = (MainWindow*)parent;
//    QMessageBox::information(NULL, "Warning", X_API_Key, QMessageBox::Yes  | QMessageBox::No , QMessageBox::No);

    GetConnectRequest.setUrl(QUrl(MainUrl + "connection"));
    GetConnectRequest.setRawHeader("X_Api_Key",X_API_Key);
    GetConnectRequest.setHeader(QNetworkRequest::ContentTypeHeader,QVariant("application/json"));

    GetStateRequest.setUrl(QUrl(MainUrl + "job"));
    GetStateRequest.setRawHeader("X_Api_Key",X_API_Key);
    GetStateRequest.setHeader(QNetworkRequest::ContentTypeHeader,QVariant("application/json"));

    SetConnectRequest.setUrl(QUrl(MainUrl + "connection"));
    SetConnectRequest.setRawHeader("X_Api_Key",X_API_Key);
    SetConnectRequest.setHeader(QNetworkRequest::ContentTypeHeader,QVariant("application/json"));

    GetTemperatureRequest.setUrl(QUrl(MainUrl + "printer"));
    GetTemperatureRequest.setRawHeader("X_Api_Key",X_API_Key);
    GetTemperatureRequest.setHeader(QNetworkRequest::ContentTypeHeader,QVariant("application/json"));

    SetCMDRequest.setUrl(QUrl(MainUrl + "printer/command"));
    SetCMDRequest.setRawHeader("X_Api_Key",X_API_Key);
    SetCMDRequest.setHeader(QNetworkRequest::ContentTypeHeader,QVariant("application/json"));

    SDRequest.setUrl(QUrl(MainUrl + "printer/sd"));
    SDRequest.setRawHeader("X_Api_Key",X_API_Key);
    SDRequest.setHeader(QNetworkRequest::ContentTypeHeader,QVariant("application/json"));

    FileRequest.setUrl(QUrl(MainUrl + "files"));
    FileRequest.setRawHeader("X_Api_Key",X_API_Key);
    FileRequest.setHeader(QNetworkRequest::ContentTypeHeader,QVariant("application/json"));
    //    TemperatureJson.object().insert("command","target");

    //save code
    //QMetaObject::Connection connRet = QObject::connect(networkAccessManager, SIGNAL(finished(QNetworkReply*)), this, SLOT(ConnectReplay(QNetworkReply*)));
    //Q_ASSERT(connRet);
}

QJsonValue OctoNetwork::SearchJsonValue(QList<QString> Find_List_Name ,QNetworkReply *reply)
{
    int statusCode = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();
    if(QSysInfo::productType() != "raspbian"){qDebug() << "statusCode:" << statusCode;}
    if(statusCode != 200)
    {
        ConnectState = "Error";
        return QJsonValue::Null;
    }
    if(reply->error() == QNetworkReply::NoError)
    {
        QByteArray replyArray = reply->readAll();
        QJsonDocument loadDoc(QJsonDocument::fromJson(replyArray));
        QJsonObject QObj = loadDoc.object();
        if(QSysInfo::productType() != "raspbian")
        {
            qDebug()<<replyArray;
            qDebug()<<"***************************************";
        }
        QList<QString> SearchLink ;
        SearchLink << Find_List_Name;
        return SearchJsonValue(SearchLink , QObj);
    }
    else
    {
        return QJsonValue::Null;
    }
}
QList<QJsonValue> OctoNetwork::SearchJsonValue(QList<QList<QString>> Find_List_Names ,QNetworkReply *reply)
{
    int statusCode = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();
    if(QSysInfo::productType() != "raspbian"){qDebug() << "statusCode:" << statusCode;}
    if(statusCode != 200)
    {
        ConnectState = "Error";
        return {};
    }
    if(reply->error() == QNetworkReply::NoError)
    {
        QByteArray replyArray = reply->readAll();
        QJsonDocument loadDoc(QJsonDocument::fromJson(replyArray));
        QJsonObject QObj = loadDoc.object();

        if(QSysInfo::productType() != "raspbian")
        {
            qDebug()<<replyArray;
            qDebug()<<"***************************************";
        }
        QList<QList<QString>> SearchLink ;
        SearchLink << Find_List_Names;
        return SearchJsonValue(SearchLink , QObj);
    }
    else
    {
        return {};
    }
}
//Find_List_Name : Retrieved List Sequence --> [Grandma,Mother,Me]
//_QObj:Retrieved Retrieved Json Objecet
QJsonValue OctoNetwork::SearchJsonValue(QList<QString> Find_List_Name , QJsonObject _QObj)
{
    QJsonValue returnValue;
    QList<QString> SearchLink ;
    SearchLink << Find_List_Name;
    if(SearchLink.count() > 1)
    {
        QString MotherKey = SearchLink[0];
        SearchLink.removeFirst();
        if(MotherKey.indexOf("*") >= 0)//wildcard
        {
            QJsonArray ArrayValue;
            for(int i = 0 ; i <= 9 ; i++)
            {
                QString TempMotherKey = MotherKey;
                if(_QObj.find(TempMotherKey.replace("*",QString::number(i))).value().isUndefined())continue;
//                if(QSysInfo::productType() != "raspbian"){qDebug()<<_QObj.find(TempMotherKey.replace("*",QString::number(i))).value().isUndefined();};
                QJsonValue data = SearchJsonValue(SearchLink , _QObj.constFind(TempMotherKey.replace("*",QString::number(i))).value().toObject());
                ArrayValue.append(data);
            }
            returnValue = ArrayValue;
        }
        else
            returnValue = SearchJsonValue(SearchLink , _QObj.constFind(MotherKey).value().toObject());
    }
    else
    {
        if(SearchLink[0].indexOf("*") >= 0)//wildcard
        {

        }
        else
            returnValue = _QObj.constFind(SearchLink[0]).value();
    }
    return returnValue;
}
QList<QJsonValue> OctoNetwork::SearchJsonValue(QList<QList<QString>> Find_List_Names , QJsonObject _QObj)
{
    QList<QJsonValue> _returnValue ;
    foreach(QList<QString> item , Find_List_Names)
    {
        _returnValue.append(SearchJsonValue(item,_QObj));
    }
    return _returnValue;
}

void OctoNetwork::SendGCode(QList<QString> _GCode_)
{
    if(_GCode_.count() > 0 && (ConnectState.contains("Printing") || ConnectState == "Operational"))
    {
        QJsonObject TemperatureJsonObj;
        QJsonArray _Commands;
        foreach(QString item,_GCode_)
        {
            _Commands.append(item);
        }

        TemperatureJsonObj.insert("commands",_Commands);

        QJsonDocument _SenderJson;
        _SenderJson.setObject(TemperatureJsonObj);

        //        qDebug()<<_SenderJson.toJson();

        MCPnetworkAccessManager->post(SetCMDRequest,_SenderJson.toJson());

    }
}
void OctoNetwork::SendGCode(QString _GCode_)
{
    if(_GCode_.length() > 0 && (ConnectState == "Operational" || ConnectState.contains("Printing")))
    {
        QJsonObject TemperatureJsonObj;
        QJsonArray _Commands;
        _Commands.append(_GCode_);

        TemperatureJsonObj.insert("commands",_Commands);

        QJsonDocument _SenderJson;
        _SenderJson.setObject(TemperatureJsonObj);

        //        qDebug()<<_SenderJson.toJson();

        MCPnetworkAccessManager->post(SetCMDRequest,_SenderJson.toJson());
    }
}
void OctoNetwork::SD_ReLoad()
{
    if(!ConnectState.contains("Printing")  && ConnectState != "Pausing")
    {
        SD_CMD("release");
        SD_CMD("init");
    }
    SD_CMD("refresh");
}
void OctoNetwork::SD_CMD(QString CMD)
{
    if(ConnectState == "Operational")
    {
        QJsonObject TemperatureJsonObj;

        TemperatureJsonObj.insert("command",CMD);

        QJsonDocument _SenderJson;
        _SenderJson.setObject(TemperatureJsonObj);
        if(QSysInfo::productType() != "raspbian"){qDebug()<<_SenderJson.toJson();}
        FSnetworkAccessManager->post(SDRequest,_SenderJson.toJson());
    }
}
//Start Or Stop
void OctoNetwork::JobSwitch(QString FilePath)
{
    if(ConnectState == "Operational")
    {
        if(QMessageBox::information(NULL, "Warning", "Do you want to Start Print ?", QMessageBox::Yes  | QMessageBox::No , QMessageBox::No) == QMessageBox::Yes)
        {JobSwitch(QUrl(MainUrl + "files/" +FilePath));}
    }
    else if(ConnectState.contains("Printing") || ConnectState == "Pausing")
    {
        JobSwitch();//Stop
    }
}
void OctoNetwork::JobSwitch(QUrl FileUrl)
{
    if(ConnectState == "Operational")
    {
        QString FileName = FileDialog::Hex2QString(FileUrl.url(),false);
        if(QMessageBox::information(NULL, "Warning", "Do you want to Start Print "+FileName+"?", QMessageBox::Yes  | QMessageBox::No , QMessageBox::No) == QMessageBox::Yes)
        {
            FileUrl = ((MainWindow*)FUI)->filedialog->GetRealURL(FileUrl);
            //Auto Level
//            QList<QString> _GCode_;
//            _GCode_.append("G91");
//            _GCode_.append("G28");
//            _GCode_.append("G29");
//            _GCode_.append("G28");
//            _GCode_.append("G90");
//            SendGCode(_GCode_);

            QJsonObject TemperatureJsonObj;
            TemperatureJsonObj.insert("command","select");
            TemperatureJsonObj.insert("print",true);

            QNetworkRequest _Request;
            _Request.setUrl(FileUrl);
            _Request.setRawHeader("X_Api_Key",X_API_Key);
            _Request.setHeader(QNetworkRequest::ContentTypeHeader,QVariant("application/json"));

            QJsonDocument _SenderJson;
            _SenderJson.setObject(TemperatureJsonObj);

            MCPnetworkAccessManager->post(_Request,_SenderJson.toJson());
        }
    }
    else if(ConnectState.contains("Printing")  || ConnectState == "Pausing")
    {
        JobSwitch();//Stop
    }
}
//Only Stop
void OctoNetwork::JobSwitch()
{
    //up z-axis 5cm
    if(ConnectState.contains("Printing")  || ConnectState == "Pausing")
    {
        if(QMessageBox::information(NULL, "Warning", "Do you want to Stop Print ?", QMessageBox::Yes  | QMessageBox::No , QMessageBox::No) == QMessageBox::Yes)
        {
            QJsonObject TemperatureJsonObj;
            TemperatureJsonObj.insert("command","cancel");

            QNetworkRequest _Request;
            _Request.setUrl(QUrl(MainUrl + "job"));
            _Request.setRawHeader("X_Api_Key",X_API_Key);
            _Request.setHeader(QNetworkRequest::ContentTypeHeader,QVariant("application/json"));

            QJsonDocument _SenderJson;
            _SenderJson.setObject(TemperatureJsonObj);

            MCPnetworkAccessManager->post(_Request,_SenderJson.toJson());
        }
    }
}
