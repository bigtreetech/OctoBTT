#include "selectfile.h"
#include "ui_selectfile.h"

SelectFile::SelectFile(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SelectFile)
{
    ui->setupUi(this);
}

SelectFile::~SelectFile()
{
    delete ui;
}
