#ifndef FILEDIALOG_H
#define FILEDIALOG_H

#include <QDialog>
#include <QSizeF>
//MY Dialog
#include <QNetworkReply>
#include <QTreeWidgetItem>
#include <QTimer>

extern int DebugFlat ;
extern QSizeF SizePercent;

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
        virtual void on_Btn_back_clicked();

        virtual void on_Btn_Refresh_pressed();

        virtual void on_Btn_Refresh_released();

        virtual void on_Btn_Refresh_clicked();

        virtual void FSReply(QNetworkReply *reply);

        virtual void ReFreshJson();

        virtual void GetDirlistTimeout();

        virtual void GetDevlistTimeout();

        virtual void on_treeWidget_itemClicked(QTreeWidgetItem *item, int column);

        virtual void on_treeWidget_itemDoubleClicked(QTreeWidgetItem *item, int column);

    private:
        Ui::FileDialog *ui;

    public:
        QUrl SelectURL;

    private:
        virtual void FilesPraser(QTreeWidgetItem *MotherItem , QJsonArray Data);
        virtual void RefreshFileList();
        virtual void GetDirlist(QStringList CommandLine);
        virtual void GetDevlist(QStringList CommandLine);

    private:
        QString StyleSheet_Temp;
        QWidget *FUI;
        QTreeWidgetItem *SDcardItem;
        QStringList DirList;
        QStringList DevList;
        QStringList RmList;
        QStringList NewList;
        QTimer *terminal_timer;

        QString TeminalState = "-q-e";

    protected:
        void showEvent(QShowEvent *event);
};

#endif // FILEDIALOG_H
