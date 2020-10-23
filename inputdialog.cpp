#include "inputdialog.h"
#include "ui_inputdialog.h"
#include <mainwindow.h>
//#include <QSizeF>
//#include <QSound>
//#include <QSoundEffect>

#include <QMessageBox>
InputDialog::InputDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::InputDialog)
{
    ui->setupUi(this);
//    FUI = (MainWindow*)parent;//((MainWindow*)FUI)->octonetwork.SendGCode(_GCode_Setting);

    AllBtns = this->findChildren<QPushButton *>();
    NumBtns = this->findChildren<QPushButton *>(QRegExp("Num_[0-9]"));
    CharBtns = this->findChildren<QPushButton *>(QRegExp("Char_[A-Z]"));
    SymbBtns = this->findChildren<QPushButton *>(QRegExp("Symb_[.]*"));

    foreach(QPushButton *item , CharBtns)
    {
        connect(item,SIGNAL(clicked()),this,SLOT(on_CharKey_click()));
    }
    foreach(QPushButton *item , NumBtns)
    {
        connect(item,SIGNAL(clicked()),this,SLOT(on_NumKey_click()));
    }
    foreach(QPushButton *item , SymbBtns)
    {
        connect(item,SIGNAL(clicked()),this,SLOT(on_SymbKey_click()));
    }
    foreach(QPushButton *item , AllBtns)
    {
        if(item->objectName()!=ui->Btn_Undo->objectName()&&item->objectName()!=ui->Btn_Confirm->objectName())
            connect(item,SIGNAL(clicked()),this,SLOT(on_Beep_clicked()));

        connect(item,SIGNAL(released()),this,SLOT(released_clicked()));
    }

    QFont font;
    //font.setPointSize(32*(SizePercent.width() < SizePercent.height() ? SizePercent.width():SizePercent.height()));
    foreach(QPushButton *item , AllBtns)
    {
//        if(item->objectName() == ui->Btn_Back->objectName()
//            || item->objectName() == ui->Btn_Confirm->objectName())
//            item->setIconSize(QSize(72*SizePercent.width(),72*SizePercent.height()));
//        if(item->objectName() == ui->Space->objectName()
//            || item->objectName() == ui->Key_Left->objectName()
//            || item->objectName() == ui->Key_Right->objectName()
//            || item->objectName() == ui->Symb_Wave->objectName()
//            || item->objectName() == ui->Symb_Semi->objectName()
//            || item->objectName() == ui->Symb_Quo->objectName())
        item->setIconSize(QSize((int)(item->iconSize().width()*SizePercent.width()),(int)(item->iconSize().height()*SizePercent.height())));
        font.setPointSize((int)(item->font().pointSize()*(SizePercent.width() < SizePercent.height() ? SizePercent.width():SizePercent.height())));
        item->setFont(font);
    }
    font.setPointSize((int)(ui->TextBlock->font().pointSize()*(SizePercent.width() < SizePercent.height() ? SizePercent.width():SizePercent.height())));
    ui->TextBlock->setFont(font);

    this->resize((int)(SizePercent.width()*800),(int)(SizePercent.height()*480));
    this->setMaximumSize((int)(SizePercent.width()*800),(int)(SizePercent.height()*480));
    this->setFixedSize((int)(SizePercent.width()*800),(int)(SizePercent.height()*480));
}

InputDialog::~InputDialog()
{
    delete ui;
}

void InputDialog::setvalue(QString PlaceholdText , QString InputMode , QString DefaultValue)
{
    ui->TextBlock->setPlaceholderText(PlaceholdText);
    if(!DefaultValue.isNull() && !DefaultValue.trimmed().isEmpty())
        ui->TextBlock->setText(DefaultValue);

    InputMode = InputMode.trimmed();
    QRegExp MyRegExp;
    if(InputMode.toLower().contains("-int"))//Multi Validator
    {
        MyRegExp.setPattern((MyRegExp.pattern().count() > 0 ? MyRegExp.pattern() +"|" : "") + "[+,-]?(0|[0-9]+)");
    }
    if(InputMode.toLower().contains("-double"))//Multi Validator
    {
        MyRegExp.setPattern((MyRegExp.pattern().count() > 0 ? MyRegExp.pattern() +"|" : "") + "[+,-]?(0|[0-9]+)[.]?[0-9]*");
    }
    if(InputMode.toLower().contains("-lower"))//Multi Validator
    {
        MyRegExp.setPattern((MyRegExp.pattern().count() > 0 ? MyRegExp.pattern() +"|" : "") + "[a-z]*");
    }
    if(InputMode.toLower().contains("-upper"))//Multi Validator
    {
        MyRegExp.setPattern((MyRegExp.pattern().count() > 0 ? MyRegExp.pattern() +"|" : "") + "[A-Z]*");
    }
    if(InputMode.toLower().contains(" int"))//single Validator
    {
        MyRegExp.setPattern("0|[0-9]+");
    }
    if(InputMode.toLower().contains(" double"))//single Validator
    {
        MyRegExp.setPattern("(0|[0-9]+)[.]?[0-9]*");
    }
    if(InputMode.toLower().contains("-ip"))//single Validator
    {
        MyRegExp.setPattern("((?:(?:25[0-5]|2[0-4]\\d|[01]?\\d?\\d)\\.){3}(?:25[0-5]|2[0-4]\\d|[01]?\\d?\\d))");
    }
    if(!InputMode.toLower().contains("-all") && InputMode != "")//single Validator
    {
        ui->TextBlock->setValidator(new QRegExpValidator(MyRegExp,this));
    }

    this->show();
}


void InputDialog::on_Btn_Undo_clicked()
{
    bool SettingMark = false;
    QString ReturnValue = ui->TextBlock->text();

    this->close();
    emit closedialog(ReturnValue,SettingMark);
}

void InputDialog::on_CharKey_click()
{
    //发送信号者的对象
    QPushButton *optBtn = qobject_cast<QPushButton *>(sender());
    //发送信号者的对象名
    //QString name = sender()->objectName();
    QKeySequence Key = QKeySequence(QString(optBtn->text().toUpper()[0]));
//    Qt::Key key = Qt::Key();
//    QMessageBox::information(NULL, "System",QString::number(Key[0] == Qt::Key_A));

//    Qt::KeyboardModifier Key_Modifier = Func_Ang_State ? Qt::ShiftModifier : Qt::NoModifier;

//    ui->TextBlock->setFocus();
    QKeyEvent keyPress(QEvent::KeyPress, Key[0], Key_Modifier, QString(optBtn->text()[0]));
//    QKeyEvent keyPress(QEvent::KeyPress, Qt::Key_A, Qt::NoModifier, optBtn->text());
    QCoreApplication::sendEvent(ui->TextBlock, &keyPress);

    ui->TextBlock->setFocus();
}

void InputDialog::on_NumKey_click()
{
    QPushButton *optBtn = qobject_cast<QPushButton *>(sender());
    QKeySequence Key = QKeySequence(QString(optBtn->text().toUpper()[0]));
//    Qt::KeyboardModifier Key_Modifier = Func_Ang_State ? Qt::ShiftModifier : Qt::NoModifier;

    int KeyData;
    if(Func_UL_State)
    {
        KeyData = Qt::Key();
    }
    else
    {
        KeyData = Key[0];
    }

    QKeyEvent keyPress(QEvent::KeyPress, KeyData, Key_Modifier, QString(optBtn->text()[0]));
    QCoreApplication::sendEvent(ui->TextBlock, &keyPress);

    ui->TextBlock->setFocus();
}

void InputDialog::on_SymbKey_click()
{
    QPushButton *optBtn = qobject_cast<QPushButton *>(sender());
//    QKeySequence Key = QKeySequence(QString(optBtn->text()[0]));
//    Qt::KeyboardModifier Key_Modifier = Func_Ang_State ? Qt::ShiftModifier : Qt::NoModifier;

    QKeyEvent keyPress(QEvent::KeyPress, Qt::Key(), Key_Modifier, QString(optBtn->text()[0]));
    QCoreApplication::sendEvent(ui->TextBlock, &keyPress);

    ui->TextBlock->setFocus();
}

void InputDialog::released_clicked()
{
    ui->TextBlock->setFocus();
}

void InputDialog::on_Beep_clicked()
{
    //QApplication::beep();
//    QSoundEffect effect;
//    effect.setSource(QUrl::fromLocalFile(QCoreApplication ::applicationDirPath() + "/wav/KBClick.wav"));//获取资源文件中的声音文件
//    effect.setLoopCount(1);//设置循环次数
//    effect.setVolume(1.0);
//    effect.play();//播放

//    QSound::play(QCoreApplication ::applicationDirPath() + "/wav/KBClick.wav");

//    QSound::play(".../wav/KBClick.wav");

//    QPoint pos(ui->TextBlock->cursor().pos());
//    QMouseEvent cursorrvrnt(QEvent::MouseButtonPress, pos, Qt::LeftButton, Qt::LeftButton, Qt::NoModifier) ;
    ui->TextBlock->setFocus();
//    QApplication::sendEvent(ui->TextBlock, &cursorrvrnt);
}

void InputDialog::showEvent(QShowEvent *event)
{
    Q_UNUSED(event);

    ui->TextBlock->setFocus();
//    ui->TextBlock->setText(QString::number( NumBtns.count()));
}

void InputDialog::on_Btn_Confirm_clicked()
{
    bool SettingMark = true;
    QString ReturnValue = ui->TextBlock->text();

    this->close();
    emit closedialog(ReturnValue,SettingMark);
}

void InputDialog::on_Func_UL_clicked(bool checked)
{
    Func_UL_State = checked;
    if(checked)
    {
        ui->Func_UL->setText(ui->Func_UL->text().toUpper());
        foreach(QPushButton *item , CharBtns)
        {
            item->setText(item->objectName().replace("Char_","").toUpper());
        }
        foreach(QPushButton *item , NumBtns)
        {
            switch (item->objectName().replace("Num_","").toInt())
            {
                case 0:
                    item->setText(Func_Ang_State? "）" : ")");
                    break;
                case 1:
                    item->setText(Func_Ang_State? "！" : "!");
                    break;
                case 2:
                    item->setText(Func_Ang_State? "@" : "@");
                    break;
                case 3:
                    item->setText(Func_Ang_State? "#" : "#");
                    break;
                case 4:
                    item->setText(Func_Ang_State? "¥" : "$");
                    break;
                case 5:
                    item->setText(Func_Ang_State? "%" : "%");
                    break;
                case 6:
                    item->setText(Func_Ang_State? "…" : "^");
                    break;
                case 7:
                    item->setText(Func_Ang_State? "&&" : "&&");
                    break;
                case 8:
                    item->setText(Func_Ang_State? "*" : "*");
                    break;
                case 9:
                    item->setText(Func_Ang_State? "（" : "(");
                    break;
            }
        }

        ui->Symb_Dot->setText(Func_Ang_State ? "，":",");
        ui->Symb_Par_L->setText(Func_Ang_State ? "「":"[");
        ui->Symb_Par_R->setText(Func_Ang_State ? "」":"]");
        ui->Symb_Add->setText(Func_Ang_State ? "+":"+");
        ui->Symb_Dec->setText(Func_Ang_State ? "—":"-");
        ui->Symb_Bra_L->setText(Func_Ang_State ? "？":"?");
        ui->Symb_Bra_R->setText(Func_Ang_State ? "/":"/");
        ui->Symb_Ver->setText(Func_Ang_State ? "、":"\\");
        ui->Symb_Wave->setText(Func_Ang_State ? "·":"·");
        ui->Symb_Semi->setText(Func_Ang_State ? "：":":");
        ui->Symb_Quo->setText(Func_Ang_State ? "”":"“");
    }
    else
    {
        ui->Func_UL->setText(ui->Func_UL->text().toLower());
        foreach(QPushButton *item , CharBtns)
        {
            item->setText(item->objectName().replace("Char_","").toLower());
        }
        foreach(QPushButton *item , NumBtns)
        {
            item->setText(item->objectName().replace("Num_",""));
        }

        ui->Symb_Dot->setText(Func_Ang_State ? "。":".");
        ui->Symb_Par_L->setText(Func_Ang_State ? "【":"[");
        ui->Symb_Par_R->setText(Func_Ang_State ? "】":"]");
        ui->Symb_Add->setText(Func_Ang_State ? "=":"+");
        ui->Symb_Dec->setText(Func_Ang_State ? "-":"-");
        ui->Symb_Bra_L->setText(Func_Ang_State ? "《":"<");
        ui->Symb_Bra_R->setText(Func_Ang_State ? "》":">");
        ui->Symb_Ver->setText(Func_Ang_State ? "｜":"|");
        ui->Symb_Wave->setText(Func_Ang_State ? "～":"~");
        ui->Symb_Semi->setText(Func_Ang_State ? "；":";");
        ui->Symb_Quo->setText(Func_Ang_State ? "'":"\"");
    }

    ui->TextBlock->setFocus();
//    QKeyEvent keyPress(QEvent::KeyPress, Qt::Key_CapsLock, Qt::NoModifier, QString());
//    QCoreApplication::sendEvent(this, &keyPress);
}

void InputDialog::on_Func_Ang_clicked(bool checked)
{
    Key_Modifier = checked ? Qt::ShiftModifier : Qt::NoModifier;
    Func_Ang_State = checked;
    ui->Func_Ang->setText(Func_Ang_State ? "●":"☾");
    if(Func_UL_State)
    {
        foreach(QPushButton *item , NumBtns)
        {
            switch (item->objectName().replace("Num_","").toInt())
            {
                case 0:
                    item->setText(Func_Ang_State? "）" : ")");
                    break;
                case 1:
                    item->setText(Func_Ang_State? "！" : "!");
                    break;
                case 2:
                    item->setText(Func_Ang_State? "@" : "@");
                    break;
                case 3:
                    item->setText(Func_Ang_State? "#" : "#");
                    break;
                case 4:
                    item->setText(Func_Ang_State? "¥" : "$");
                    break;
                case 5:
                    item->setText(Func_Ang_State? "%" : "%");
                    break;
                case 6:
                    item->setText(Func_Ang_State? "…" : "^");
                    break;
                case 7:
                    item->setText(Func_Ang_State? "&&" : "&&");
                    break;
                case 8:
                    item->setText(Func_Ang_State? "*" : "*");
                    break;
                case 9:
                    item->setText(Func_Ang_State? "（" : "(");
                    break;
            }
        }
        ui->Symb_Dot->setText(Func_Ang_State ? "，":",");
        ui->Symb_Par_L->setText(Func_Ang_State ? "「":"[");
        ui->Symb_Par_R->setText(Func_Ang_State ? "」":"]");
        ui->Symb_Add->setText(Func_Ang_State ? "+":"+");
        ui->Symb_Dec->setText(Func_Ang_State ? "—":"-");
        ui->Symb_Bra_L->setText(Func_Ang_State ? "？":"?");
        ui->Symb_Bra_R->setText(Func_Ang_State ? "/":"/");
        ui->Symb_Ver->setText(Func_Ang_State ? "、":"\\");
        ui->Symb_Wave->setText(Func_Ang_State ? "·":"·");
        ui->Symb_Semi->setText(Func_Ang_State ? "：":":");
        ui->Symb_Quo->setText(Func_Ang_State ? "”":"“");
    }
    else
    {
        ui->Symb_Dot->setText(Func_Ang_State ? "。":".");
        ui->Symb_Par_L->setText(Func_Ang_State ? "【":"[");
        ui->Symb_Par_R->setText(Func_Ang_State ? "】":"]");
        ui->Symb_Add->setText(Func_Ang_State ? "=":"+");
        ui->Symb_Dec->setText(Func_Ang_State ? "-":"-");
        ui->Symb_Bra_L->setText(Func_Ang_State ? "《":"<");
        ui->Symb_Bra_R->setText(Func_Ang_State ? "》":">");
        ui->Symb_Ver->setText(Func_Ang_State ? "|":"｜");
        ui->Symb_Wave->setText(Func_Ang_State ? "～":"~");
        ui->Symb_Semi->setText(Func_Ang_State ? "；":";");
        ui->Symb_Quo->setText(Func_Ang_State ? "'":"\"");
    }

    ui->TextBlock->setFocus();
}

void InputDialog::on_Func_BS_clicked()
{
//    QKeyEvent keyPress(QEvent::KeyPress, Qt::Key(), Qt::NoModifier, QString());
//    ui->TextBlock->keyPressEvent(key);
    QKeyEvent keyPress(QEvent::KeyPress, Qt::Key_Backspace, Key_Modifier, QString());
    QCoreApplication::sendEvent(ui->TextBlock, &keyPress);

    ui->TextBlock->setFocus();
}

void InputDialog::on_Key_Left_clicked()
{
    QKeyEvent keyPress(QEvent::KeyPress, Qt::Key_Left, Key_Modifier, QString());
    QCoreApplication::sendEvent(ui->TextBlock, &keyPress);

    ui->TextBlock->setFocus();
}

void InputDialog::on_Key_Right_clicked()
{
    QKeyEvent keyPress(QEvent::KeyPress, Qt::Key_Right, Key_Modifier, QString());
    QCoreApplication::sendEvent(ui->TextBlock, &keyPress);

    ui->TextBlock->setFocus();
}

void InputDialog::on_Space_clicked()
{
    QKeyEvent keyPress(QEvent::KeyPress, Qt::Key_Space, Key_Modifier, " ");
    QCoreApplication::sendEvent(ui->TextBlock, &keyPress);

    ui->TextBlock->setFocus();
}
