#include "mylineedit.h"
#include <QDebug>

MyLineEdit::MyLineEdit(QWidget *parent):QLineEdit(parent)
{
    FUI = (QWidget*)parent;//((MainWindow*)FUI)->octonetwork.SendGCode(_GCode_Setting);

//    connect(inputdialog,&InputDialog::closedialog,this,[=](QString value,bool SettingMark){
//        FUI->show();
//        if(SettingMark)
//        {
//            this->setText(value);
//        }
////        inputdialog->close();
////        this->show();
//    });
}

//MyLineEdit::~MyLineEdit()
//{
////    inputdialog->~InputDialog();
////    delete this;
//    this->close();
//}

void MyLineEdit::mousePressEvent(QMouseEvent *event)
{
    Q_UNUSED(event);
//    inputdialog->setvalue("Please Input Commands","",this->text());
    InputDialog *inputdialog = new InputDialog(FUI);
    connect(inputdialog,&InputDialog::closedialog,this,[=](QString value,bool SettingMark){
        if(SettingMark)
        {
              this->setText(value);
        }
        FUI->show();
        delete sender();
    });
    inputdialog->setvalue("Please Input Commands","",this->text());

}
