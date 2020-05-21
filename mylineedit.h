#ifndef MYLINEEDIT_H
#define MYLINEEDIT_H

#include <QLineEdit>
#include "inputdialog.h"

class MyLineEdit : public QLineEdit
{
public:
//    MyLineEdit(QWidget *parent=0);
    explicit MyLineEdit(QWidget *parent = Q_NULLPTR);
//    explicit MyLineEdit(const QString &, QWidget *parent = Q_NULLPTR);
//    ~MyLineEdit();
public:

private:
//    InputDialog *inputdialog = new InputDialog(this);
    QWidget *FUI;
protected:
    void mousePressEvent(QMouseEvent *);
};

#endif // MYLINEEDIT_H
