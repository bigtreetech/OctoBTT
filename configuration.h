#ifndef CONFIGURATION_H
#define CONFIGURATION_H

#include <QDialog>
#include <QSizeF>
#include <QStandardItemModel>

extern int DebugFlat ;
extern QSizeF SizePercent;

namespace Ui {
class Configuration;
}

class Configuration : public QDialog
{
    Q_OBJECT

public:
    explicit Configuration(QWidget *parent = nullptr);
    ~Configuration();

private slots:
    virtual void on_Btn_Back_clicked();

    void on_Frm_ListView_clicked(const QModelIndex &index);

    void on_Btn_Help_clicked();

private:
    Ui::Configuration *ui;
    QWidget *FUI;

    QStandardItemModel *ItemModel = new QStandardItemModel(this);
protected:
    void showEvent(QShowEvent *event);
};

#endif // CONFIGURATION_H
