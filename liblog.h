#ifndef LIBLOG_H
#define LIBLOG_H

#include <QDir>
#include <QFile>
#include <QTime>
#include <QDate>
#include <QException>


class LibLog
{
public:
    static void LogRec(QString Message, QString exceptionName = NULL);



private:
    static void debugShow(bool flag,QString times, QString message,QString exceptionName = NULL);
//    QTime systemTime = QTime::currentTime();
//    QDate systemDate = QDate::currentDate();
};

#endif // LIBLOG_H
