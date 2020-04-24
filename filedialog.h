#ifndef FILEDIALOG_H
#define FILEDIALOG_H

#include <QDialog>
//MY Dialog
#include "selectfile.h"
#include <QNetworkReply>
#include <QTreeWidgetItem>

namespace Ui {
    class FileDialog;
}

class FileDialog : public QDialog
{
        Q_OBJECT

    public:
        explicit FileDialog(QWidget *parent = nullptr);
        ~FileDialog();

    private slots:
        void on_Btn_back_clicked();

        void on_Btn_Refresh_pressed();

        void on_Btn_Refresh_released();

        void on_Btn_Refresh_clicked();

        void FSReply(QNetworkReply *reply);

        void on_treeWidget_itemClicked(QTreeWidgetItem *item, int column);

        void on_treeWidget_itemDoubleClicked(QTreeWidgetItem *item, int column);

    private:
        Ui::FileDialog *ui;

    public:
        QUrl SelectURL;
        void RefreshFileList();
    private:
        void FilesPraser(QTreeWidgetItem *MotherItem , QJsonArray Data);
    private:
        SelectFile *selectfile = new SelectFile(this);
        QString StyleSheet_Temp;
        QWidget *FUI;
        QTreeWidgetItem *SDcardItem;
};

#endif // FILEDIALOG_H
