#include "accesscontrol.h"
//#include "octonetwork.h"//Global prama
#include <QJsonDocument>
#include <QJsonObject>
#include <QNetworkReply>
#include <QJsonArray>
//#include <filedialog.h>
//#include <mainwindow.h>

AccessControl::AccessControl()
{
//    GetACRequest.setUrl(QUrl(MainUrl));//http://IP/api/login
    GetACRequest.setHeader(QNetworkRequest::ContentTypeHeader,QVariant("application/json"));
}

//login
void AccessControl::acLogin()
{
    if(userinfo.uid == NULL || userinfo.uid.trimmed() == "" || userinfo.pwd == NULL)
    {
        emit CommunicationStateChange(false);
        return;
    }
    GetACRequest.setUrl(QUrl(MainUrl + "login"));//http://IP/api/login
    QJsonObject TemperatureJsonObj;

//    {
//        "user": "uid",
//        "pass": "pwd",
//        "remember": "remember me"
//    }
    TemperatureJsonObj.insert("user",userinfo.uid);
    TemperatureJsonObj.insert("pass",userinfo.pwd);
    TemperatureJsonObj.insert("remember","remember me");
    TemperatureJsonObj.insert("passive",true);//被动模式登录，如果免登录依旧能够获取session///需要允许被动登录才可以获得免登录方式
    QJsonDocument _SenderJson;
    _SenderJson.setObject(TemperatureJsonObj);

    ACManager->disconnect(SIGNAL(finished(QNetworkReply*)));
    connect(ACManager,&QNetworkAccessManager::finished,this,&AccessControl::LoginReply);

    ACManager->post(GetACRequest,_SenderJson.toJson());
}

void AccessControl::LoginReply(QNetworkReply *reply)
{
    bool _return=false;
//    ACManager->disconnect(SIGNAL(finished(QNetworkReply*)));
    if(reply->error() == QNetworkReply::NoError)
    {
        int statusCode = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();
        if(statusCode == 200)//Have receive
        {
//            userinfo.login = true;
            //创建接收对象
            QByteArray replyArray = reply->readAll();
            QJsonDocument loadDoc(QJsonDocument::fromJson(replyArray));
            QJsonObject QObj = loadDoc.object();
            //获取登录反馈内容
            QList<QJsonValue> result = SearchJsonValue(LoginMsg , QObj);//{{"active"},{"admin"},{"apikey"},{"name"},{"session"}};
            userinfo.login = result[0].toBool();
            userinfo.admin = result[1].toBool();
            userinfo.API_Key = result[2].toString().toUtf8();
            userinfo.onlineUid = result[3].toString();
            userinfo.session = result[4].toString().toUtf8();
            _return = true;
        }
    }
    ACManager->disconnect(SIGNAL(finished(QNetworkReply*)));
    emit CommunicationStateChange(_return);
}

//logout
void AccessControl::acLogout()
{
    GetACRequest.setUrl(QUrl(MainUrl + "logout"));//http://IP/api/logout
    ACManager->disconnect(SIGNAL(finished(QNetworkReply*)));
    connect(ACManager,&QNetworkAccessManager::finished,this,&AccessControl::LogoutReply);
}

void AccessControl::LogoutReply(QNetworkReply *reply)
{
    bool _return=false;
    if(reply->error() == QNetworkReply::NoError)
    {
        int statusCode = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();
        if(statusCode == 200)//Have receive
        {
            //重置登录参数
            userinfo = UserInfo(userinfo.uid,userinfo.pwd);
            //创建接收对象
            QByteArray replyArray = reply->readAll();
            QJsonDocument loadDoc(QJsonDocument::fromJson(replyArray));
            QJsonObject QObj = loadDoc.object();
//            QList<QString> SearchLink ;
            //获取登录反馈内容
            QList<QJsonValue> result = SearchJsonValue(LoginMsg , QObj);//{{"active"},{"admin"},{"apikey"},{"name"},{"session"}};///被动账户执行退出登录才会返回账户信息
            if(!result[3].isNull() && result[3].isString() && result[3].toString() == "_admin")
            {
                userinfo.login = result[0].toBool();
                userinfo.admin = result[1].toBool();
                userinfo.onlineUid = result[3].toString();//判定免登录账户:_admin
            }
            _return = true;
        }
    }
    ACManager->disconnect(SIGNAL(finished(QNetworkReply*)));
    emit CommunicationStateChange(_return);
}

//Function
//Find_List_Name : Retrieved List Sequence --> [Grandma,Mother,Me]
//_QObj:Retrieved Retrieved Json Objecet
QJsonValue AccessControl::SearchJsonValue(QList<QString> Find_List_Name , QJsonObject _QObj)
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
QList<QJsonValue> AccessControl::SearchJsonValue(QList<QList<QString>> Find_List_Names , QJsonObject _QObj)
{
    QList<QJsonValue> _returnValue ;
    foreach(QList<QString> item , Find_List_Names)
    {
        _returnValue.append(SearchJsonValue(item,_QObj));
    }
    return _returnValue;
}
