#include "mainwindow.h"
//#include "controlpanel.h"

#include <QApplication>
#include <QScreen>
#include <QProcess>
#include <QSharedMemory>
#include <QTimer>
#include <QTime>
#include <QFile>
#include <QTextCodec>
#include <QSizeF>
#include <QDir>
#include <QFile>

QSharedMemory mem("OctoBTT");
QByteArray Global_X_API_Key = "C1B64D24AE99475EAA9385B5DBC77820";//Test Key
int DebugFlat = 0;//Normal Flat : 0 ;Debug Mode : 1
QSizeF SizePercent = QSizeF(1.0,1.0);//Design Size 800:480
//QSize SizePercent(1,1);

//存放配置信息
QString strConfiguration = "";


int main(int argc, char *argv[])
{
    //Application Encoding
    QTextCodec *codec = QTextCodec::codecForName("UTF-8");
    QTextCodec::setCodecForLocale(codec);

    //Program Statement Code
    QApplication a(argc, argv);

    //设置全局按键默认样式表---12.24

    //判断config.conf文件在不在
    if(!QFile("config.conf").exists())
    {
        //复制资源文件中的config.conf到用户本地文件夹里
        QFile::copy(":/config.conf", "config.conf");
        //设置权限
        QFile::setPermissions("config.conf", QFileDevice::ReadOwner|QFileDevice::WriteOwner);

        //设置默认按键的样式表
        QFile file("config.conf");
        file.open(QFile::ReadOnly);
        if(file.isOpen())
        {
            strConfiguration = file.readAll();
            //全局默认设置
            //
//            qApp->setStyleSheet(strConfiguration);
        }
        file.close();
    }
    else
    {
        QFile file(":/config.conf");
        file.open(QFile::ReadOnly);

        //获取用户本地的版本号
        QString userNumber = file.readLine().trimmed();
        QStringList userNumberList = userNumber.replace(QString("/*"),QString("")).replace(QString("*/"),QString("")).split(".");

        //判断版本号是否更新，如果版本号大于旧的版本号，就更新，否则不变
        bool upDateFlag = false;

        QFile userFile("config.conf");
        userFile.open(QFile::ReadOnly);
        QString strUpDate = userFile.readLine().trimmed();
        QStringList strUpDateList = strUpDate.replace(QString("/*"),QString("")).replace(QString("*/"),QString("")).split(".");

        if(QString(strUpDateList.at(0)).toInt() > QString(userNumberList.at(0)).toInt())
            upDateFlag = true;

        else if(QString(strUpDateList.at(0)).toInt() == QString(userNumberList.at(0)).toInt())
        {
            if(QString(strUpDateList.at(1)).toInt() > QString(userNumberList.at(1)).toInt())
                upDateFlag = true;

            else if(QString(strUpDateList.at(1)).toInt() == QString(userNumberList.at(1)).toInt())
                if(QString(strUpDateList.at(2)).toInt() > QString(userNumberList.at(2)).toInt())
                    upDateFlag = true;
        }

        if(upDateFlag)
        {
            file.close();
            //删除旧版本并复制新版本到用户本地文件中
            //配置文件修改函数=======================

//            QFile::remove("/config.conf");
//            QFile::copy(":/config.conf", "/config.conf");
            //设置权限
            QFile::setPermissions("config.conf", QFileDevice::ReadOwner|QFileDevice::WriteOwner);
            file.open(QFile::ReadOnly);
            if(file.isOpen())
            {
                strConfiguration = file.readAll();
//                qApp->setStyleSheet(strConfiguration);
            }
        }
        else
        {
            if(file.isOpen())
            {
                strConfiguration = file.readAll();
//                qApp->setStyleSheet(strConfiguration);
            }
        }
        file.close();
        userFile.close();

    }

    //设置全局按键默认样式表---12.22
    QString strStyleSheet = "";
    //判断StyleSheet目录在不在
    if(!QDir("StyleSheet").exists())
    {
        QDir().mkdir("StyleSheet");

        //复制资源文件中的globalStyleSheet.qss到用户本地文件夹里
        QFile::copy(":/StyleSheet/globalStyleSheet.qss", "StyleSheet/globalStyleSheet.qss");
        //设置权限
        QFile::setPermissions("StyleSheet/globalStyleSheet.qss", QFileDevice::ReadOwner|QFileDevice::WriteOwner);

        //设置默认按键的样式表
        QFile file("StyleSheet/globalStyleSheet.qss");
        file.open(QFile::ReadOnly);
        if(file.isOpen())
        {
            strStyleSheet = file.readAll();
            //全局默认设置
            qApp->setStyleSheet(strStyleSheet);
        }
        file.close();
    }
    else
    {
        QFile file("StyleSheet/globalStyleSheet.qss");
        file.open(QFile::ReadOnly);

        //获取用户本地的版本号
        QString userNumber = file.readLine().trimmed();
        QStringList userNumberList = userNumber.replace(QString("/*"),QString("")).replace(QString("*/"),QString("")).split(".");

        //判断用户是否自定义了样式表
        if(0 == QString(userNumberList.at(0)).toInt() && DebugFlat == 0)
        {
            //使用用户定义的样式表文件qss
            if(file.isOpen())
            {
                strStyleSheet = file.readAll();
                qApp->setStyleSheet(strStyleSheet);
            }
        }
        //如果不是就判断版本号是否更新
        else
        {
            //如果资源版本号大于旧的版本号，就更新，否则不变
            bool upDateFlag = false;

            QFile userFile(":/StyleSheet/globalStyleSheet.qss");
            userFile.open(QFile::ReadOnly);
            QString strUpDate = userFile.readLine().trimmed();
            QStringList strUpDateList = strUpDate.replace(QString("/*"),QString("")).replace(QString("*/"),QString("")).split(".");

            if(QString(strUpDateList.at(0)).toInt() > QString(userNumberList.at(0)).toInt())
                upDateFlag = true;

            else if(QString(strUpDateList.at(0)).toInt() == QString(userNumberList.at(0)).toInt())
            {
                if(QString(strUpDateList.at(1)).toInt() > QString(userNumberList.at(1)).toInt())
                    upDateFlag = true;

                else if(QString(strUpDateList.at(1)).toInt() == QString(userNumberList.at(1)).toInt())
                    if(QString(strUpDateList.at(2)).toInt() > QString(userNumberList.at(2)).toInt())
                        upDateFlag = true;
            }

            if(upDateFlag || DebugFlat == 1)
            {
                file.close();
                //删除旧版本并复制新版本到用户本地文件中
                QFile::remove("StyleSheet/globalStyleSheet.qss");
                QFile::copy(":/StyleSheet/globalStyleSheet.qss", "StyleSheet/globalStyleSheet.qss");
                //设置权限
                QFile::setPermissions("StyleSheet/globalStyleSheet.qss", QFileDevice::ReadOwner|QFileDevice::WriteOwner);
                file.open(QFile::ReadOnly);
                if(file.isOpen())
                {
                    strStyleSheet = file.readAll();
                    qApp->setStyleSheet(strStyleSheet);
                }
            }
            else
            {
                if(file.isOpen())
                {
                    strStyleSheet = file.readAll();
                    qApp->setStyleSheet(strStyleSheet);
                }
            }
            file.close();
            userFile.close();
        }
    }




    //Find Api Key
    if(QSysInfo::productType() == "raspbian")
    {
        QFile configfile("/home/pi/.octoprint/config.yaml");
        if(configfile.open(QIODevice::ReadOnly))
        {
            QTextStream txtInput(&configfile);
            QStringList ConfigArgu = txtInput.readAll().split('\n');
            configfile.close();
            bool has_api = false;
            foreach(QString Argu,ConfigArgu)
            {
                if(Argu.indexOf("api:")>=0)
                {
                    has_api = true;
                    continue;
                }
                else if(has_api && Argu.indexOf("key:")>=0)
                {
                    Global_X_API_Key = ((Argu.trimmed()).remove(0,(Argu.trimmed()).indexOf("key:")+4)).trimmed().toUtf8();
                    break;
                }
            }
        }
        else
        {
            DebugFlat = 1;
        }
        //Calculate the scale
        QScreen *screen=QGuiApplication::primaryScreen();
        QRect mm=screen->availableGeometry();
        SizePercent.setWidth((float)mm.width()/(float)800);
        SizePercent.setHeight((float)mm.height()/(float)480);
//        SizePercent = QSize(mm.width()/800,mm.height()/480);
    }
    MainWindow w;

//Release Code
    if(QSysInfo::productType() == "raspbian")
    {
        //Single Program//Need a life cycle as the program was accidenta
        if (DebugFlat != 1 && !mem.create(1))
        {
            QApplication::exit();
            QApplication::quit();
            return 0;
        }
        w.setWindowState(Qt::WindowFullScreen);
//        qDebug()<<mem.key();
    }
//DeBug Code
//    qDebug()<<mm.width()<<"@"<< mm.height();
//    qDebug()<<QSysInfo::productType();

    w.show();

//Boot Mode
    int MCode = a.exec();
    if(MCode == Base_Restart)
    {
        if (mem.isAttached())
        {
            mem.unlock();
            mem.detach();
        }
        if(QSysInfo::productType() == "raspbian")
        {
//            QProcess P(0);
//            P.start("startx");
//            P.waitForStarted();
            QProcess::startDetached("bash "+qApp->applicationDirPath()+"/start.sh");
        }
        else
        {
            QProcess::startDetached(qApp->applicationFilePath(), QStringList());
        }
        QApplication::exit();
        QApplication::quit();
        return 0;
    }
    else if(MCode == Base_Shutdown)
    {
        QProcess P(0);
        P.start("sudo shutdown now");
        P.waitForStarted();
        P.waitForFinished();
        QApplication::exit();
        QApplication::quit();
        return 0;
    }
    else if(MCode == Base_OnlyExitApp)
    {
        if (mem.isAttached())
        {
            mem.unlock();
            mem.detach();
        }
        QApplication::exit();
        QApplication::quit();
        return 0;
    }

    return MCode;
}
