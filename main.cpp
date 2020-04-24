#include "mainwindow.h"
//#include "controlpanel.h"

#include <QApplication>
#include <QScreen>
#include <QDebug>
#include <QProcess>
#include <QSharedMemory>
#include <QMessageBox>
#include <QTimer>
#include <QTime>
#include <QFile>

QSharedMemory mem("BingoTouch");
QByteArray Global_X_API_Key = "C1B64D24AE99475EAA9385B5DBC77820";//Test Key

int main(int argc, char *argv[])
{
//Single Program//Need a life cycle as the program was accidenta
//    if (!mem.create(1))
//    {
//        return 0;TestLog
//    }
//    qDebug()<<mem.key();

//Program Statement Code
    QApplication a(argc, argv);
    //Find Api Key
    if(QSysInfo::productType() == "raspbian")
    {
        QFile configfile("/home/pi/.octoprint/config.yaml");
        if(configfile.open(QIODevice::ReadOnly))
        {
            QTextStream txtInput(&configfile);
            QString lineStr;
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
    }
    MainWindow w;

//Release Code
    if(QSysInfo::productType() == "raspbian")
    {
        //Single Program//Need a life cycle as the program was accidenta
        if (!mem.create(1))
        {
            QApplication::exit();
            QApplication::quit();
            return 0;
        }

        QScreen *screen=QGuiApplication::primaryScreen ();
        QRect mm=screen->availableGeometry();
        w.resize(mm.width(),mm.height());
        w.setMaximumSize(mm.width(),mm.height());
        w.setFixedSize(mm.width(),mm.height());
        w.setWindowState(Qt::WindowFullScreen);
//        w.octonetwork.MainUrl = "http://octopi.local/api/";//undefine aboard

//        qDebug()<<mem.key();
    }
//DeBug Code
//    qDebug()<<mm.width()<<"@"<< mm.height();
//    qDebug()<<QSysInfo::productType();
//    QMessageBox::information(NULL, "System", QSysInfo::productType());

    w.show();

//Boot Function Code
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
