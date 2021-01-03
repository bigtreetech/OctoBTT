#include "customdialog.h"

/// windowName:窗体标题
/// windowIcon:窗体标识图片路径
/// buttonName：按键水平布局---纯按键(垂直布局)
/// textContent：文本垂直布局
void CustomDialog::showCustomDialog(QString windowName, QString windowIcon, QStringList buttonName, QString textContent, QWidget *parent)
{
    //传参
    _WindowName = windowName;
    _WindowIcon = windowIcon;
    _ButtonName = buttonName;
    _TextContent = textContent;
    _Parent = parent;

     //背景颜色
    this->setWindowOpacity(0.9);    //透明度
    this->setStyleSheet("background-color: rgba(128, 128, 128, 25%)");  //背景

    //设置窗口的标题和标识
    this->setWindowTitle(_WindowName);
    this->setWindowIcon(QIcon(_WindowIcon));
    //适应屏幕大小
    this->resize(_Parent->width()*0.9,_Parent->height()*0.9);
    this->setFixedSize(_Parent->width()*0.9,_Parent->height()*0.9);

    //使弹窗居中显示
    this->move(_Parent->pos().x() + (_Parent->width() - this->width()) / 2,_Parent->pos().y() + (_Parent->height() - this->height()) / 2);

    //设置文本显示
    QTextBrowser textBrowser;
    textBrowser.setStyleSheet("border: none; color: rgb(245, 246, 255);background-color: rgba(128, 128, 128, 0%);");
    textBrowser.setText(_TextContent);
    textBrowser.setFixedSize(_Parent->width()*0.877,_Parent->height()*0.5);
//    textBrowser.move();
//    textBrowser.setGeometry(50,50,_Parent->width()*800*0.8,_Parent->height()*480*0.5);
    textBrowser.setAlignment(Qt::AlignCenter);

    //根据QStringList buttonName创建按键
    QList<QPushButton*> btn;
    for(int i = 0;i < _ButtonName.count() ;i++)
    {
        btn.append(new QPushButton(_ButtonName[i]));
        btn[i] = new QPushButton(_ButtonName[i]);
        //指定按键样式
        btn[i]->setWhatsThis("BTStyleSheet");

        //调整字体大小
        QFont _font = btn[i]->font();
        _font.setPointSize(_Parent->maximumWidth() > _Parent->maximumHeight() ? (int)_Parent->maximumHeight()/30 : (int)_Parent->maximumWidth()/30);
        btn[i]->setFont(_font);
        textBrowser.setFont(_font);
    }

    //调整按键和文本的位置

    //文本垂直布局
    QVBoxLayout VLayout(this);
    VLayout.addSpacing(150);
    VLayout.addWidget(&textBrowser);
    VLayout.addSpacing(300);

    //按键水平布局
    QHBoxLayout RestartBT;
    foreach(QPushButton *item , btn)
    {
        RestartBT.addWidget(item);

        //按键信号连接槽
        QObject::connect(item, &QPushButton::clicked, this,[=]()
        {
            QPushButton *optBtn = qobject_cast<QPushButton *>(sender());
            emit OutputEvent(optBtn->text());
            this->close();
        });
    }
    VLayout.addLayout(&RestartBT);

    //模态显示
    this->exec();
}

void CustomDialog::showCustomDialog(QString windowName, QString windowIcon, QStringList buttonName, QWidget *parent)
{
    //传参
    _WindowName = windowName;
    _WindowIcon = windowIcon;
    _ButtonName = buttonName;
    _Parent = parent;

     //背景颜色
    this->setWindowOpacity(0.9);    //透明度
    this->setStyleSheet("background-color: rgba(128, 128, 128, 25%)");  //背景

    //设置窗口的标题和标识
    this->setWindowTitle(_WindowName);
    this->setWindowIcon(QIcon(_WindowIcon));
    //适应屏幕大小
    this->resize(_Parent->width()*0.9,_Parent->height()*0.9);
    this->setFixedSize(_Parent->width()*0.9,_Parent->height()*0.9);

    //使弹窗居中显示
    this->move(_Parent->pos().x() + (_Parent->width() - this->width()) / 2,_Parent->pos().y() + (_Parent->height() - this->height()) / 2);

    //根据QStringList buttonName创建按键
    QList<QPushButton*> btn;
    for(int i = 0;i < _ButtonName.count() ;i++)
    {
        btn.append(new QPushButton(_ButtonName[i]));
        btn[i] = new QPushButton(_ButtonName[i]);
        //指定按键样式
        btn[i]->setWhatsThis("BTStyleSheet");

        //调整字体大小
        QFont _font = btn[i]->font();
        _font.setPointSize(_Parent->maximumWidth() > _Parent->maximumHeight() ? (int)_Parent->maximumHeight()/30 : (int)_Parent->maximumWidth()/30);
        btn[i]->setFont(_font);
    }

    //调整按键的位置
    QVBoxLayout VLayout(this);

    //按键水平布局

    foreach(QPushButton *item , btn)
    {
        VLayout.addWidget(item);

        //按键信号连接槽
        QObject::connect(item, &QPushButton::clicked, this,[=]()
        {
            QPushButton *optBtn = qobject_cast<QPushButton *>(sender());
            emit OutputEvent(optBtn->text());
            this->close();
        });
    }

    //模态显示
    this->exec();
}

void CustomDialog::showCustomDialog(QString windowName, QString windowIcon, QString textContent, QWidget *parent)
{
    QStringList btnName = {"Ok"};
    showCustomDialog(windowName, windowIcon, btnName, textContent, parent);
}

