#ifndef ACCESSCONTROL_H
#define ACCESSCONTROL_H

#include <QJsonDocument>
#include <QJsonObject>
#include <QMetaType>

#include <QNetworkReply>

struct UserInfo{
    QString uid;//用户设置登录账号
    QString onlineUid;//当前登录账号
    QString pwd;//账户密码
    bool admin;//{"admin"}//判断权限是否充足，仅仅高级账户才能运行UI运作
    bool login;//{"active"}//登录失败403拒绝访问，布尔值归位默认false
    QByteArray API_Key;//{"apikey"}//如果没设置就不存在，引用全局Key
    QByteArray session;//{"session"}//每次登录成功会获得新的对话ID，不区分超级账户和用户账户///为websocket提供接入ID
    UserInfo(QString _uid = NULL , QString _pwd = NULL)
    {
        if(_uid != NULL && _pwd != NULL)
        {
            uid = _uid;
            pwd = _pwd;
        }
        admin = false;
        login = false;
        API_Key = "";
        session = "";
    }
};
Q_DECLARE_METATYPE(UserInfo);

class AccessControl: public QObject
{
    Q_OBJECT
public:
    AccessControl();
    QNetworkAccessManager *ACManager = new QNetworkAccessManager();
    QNetworkRequest GetACRequest;

    void acLogin();
    void acLogout();
    UserInfo userinfo = UserInfo();
//    QByteArray user_X_API_Key = "";
signals:
    void CommunicationStateChange(bool);
private:
//    QWidget *FUI;
    QString MainIP = "127.0.0.1";
    QString MainAddress = "http://"+MainIP+"/";
    QString MainUrl = MainAddress + "api/";

    QJsonValue SearchJsonValue(QList<QString> Find_List_Name , QJsonObject _QObj);
    QList<QJsonValue> SearchJsonValue(QList<QList<QString>> Find_List_Names , QJsonObject _QObj);

    const QList<QList<QString>> LoginMsg = {{"active"},{"admin"},{"apikey"},{"name"},{"session"}};

private slots:
    void LoginReply(QNetworkReply *reply);
    void LogoutReply(QNetworkReply *reply);
};

#endif // ACCESSCONTROL_H
