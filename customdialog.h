#ifndef CUSTOMDIALOG_H
#define CUSTOMDIALOG_H

#include <QWidget>
#include <QDialog>
#include <QPushButton>
#include <QTextBrowser>
#include <QLayout>

class CustomDialog : public QDialog
{
    Q_OBJECT
public:
    //文本加按键(按键属于水平布局，文本和按键属于垂直布局)
    void showCustomDialog(QString windowName, QString windowIcon, QStringList buttonName, QString textContent, QWidget *parent = nullptr);

    //纯按键(垂直布局)
    void showCustomDialog(QString windowName, QString windowIcon, QStringList buttonName, QWidget *parent = nullptr);

    //纯文本(居中显示,提醒语句)
    void showCustomDialog(QString windowName, QString windowIcon, QString textContent, QWidget *parent = nullptr);

private:
    QString _WindowName;
    QString _WindowIcon;
    QStringList _ButtonName;
    QString _TextContent;
    QWidget *_Parent;

signals:
    void OutputEvent(QString);

};

#endif // CUSTOMDIALOG_H
