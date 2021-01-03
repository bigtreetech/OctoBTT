#include "liblog.h"
#include <QDebug>


void LibLog::LogRec(QString message, QString exceptionName)
{

    QTime systemTime = QTime::currentTime();
    QDate systemDate = QDate::currentDate();

    //文件名 日期+.log
    QString fileName = QString("%1%2.log").arg(systemDate.toString("yyyy_MM_dd")).arg((exceptionName.isNull()? "":"_" + exceptionName));

    QFile file;

    //文件路径
    file.setFileName("Log/" + fileName);

    //判断文件夹是否存在
    if(!QDir("Log").exists())
    {
        QDir().mkdir("Log");
    }

    //以可读可写的形式打开文件，并将信息保存于该文件
    file.open(QFile::Append);
    QFile::setPermissions(file.fileName(), QFileDevice::ReadOwner|QFileDevice::WriteOwner);

    if(file.isOpen())
    {
        QString saveMessage = systemTime.toString("hh:mm:ss.zzz") + "\t" + message+ "\n";
        file.write(saveMessage.toUtf8());
//        debugShow(QSysInfo::productType() != "raspbian",systemTime.toString("hh:mm:ss.zzz"),message,exceptionName);
    }

    file.close();
}

void LibLog::debugShow(bool flag, QString times, QString message, QString exceptionName)
{

    if(flag)
    {
        //清除头尾的非必要字符并以\t划分
        QStringList messageModule = message.trimmed().split('\t');
        QStringList messageModules;
        foreach(QString inf , messageModule)
        {
            //排除空格
            if(inf.trimmed().count() == 0)
                continue;
            //剩下以\n划分
            messageModules.append(inf.trimmed().split('\n'));
        }
        //组合
        message = messageModules.join(" > ");
        message = (exceptionName.isNull() ? QString("%1 >>> %2").arg(times).arg(message) : QString("%1 >>> %2 *** %3").arg(times).arg(exceptionName).arg(message));
        //输出信息
        qDebug() << message;
    }
}
