#ifndef FILEDIALOG_H
#define FILEDIALOG_H

#include <QDialog>
#include <QSizeF>
//MY Dialog
#include <QNetworkReply>
#include <QTreeWidgetItem>
#include <QTimer>
#include <QNetworkReply>

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
        void on_Btn_Upload_clicked();
        void on_Btn_Print_clicked();
        void on_Btn_Delete_clicked();

    private:
        Ui::FileDialog *ui;

    public:
        QUrl SelectURL;
        QString DisplayName;
        QUrl GetRealURL(QUrl URL);
        QUrl GetRealURL();
        static QString QString2Hex(QString str,bool FullPath = true);
        static QString Hex2QString(QString hx,bool FullPath = true);
        void InitializeUSBDrive();

    private:
        virtual void FilesPraser(QTreeWidgetItem *MotherItem , QJsonArray Data);
        virtual QList<QTreeWidgetItem *> LoadUSBPath(QString USBPath);
        virtual void LoadFiles(QTreeWidgetItem *MotherItem , QString DirPath);
        virtual void RefreshFileList();
        virtual void GetDirlist(QStringList CommandLine);
        virtual void GetDevlist(QStringList CommandLine);
        void DeleteURL(QUrl URL);
        void DeleteFile(QString FilePath);
        QUrl UploadFile(QString FilePath);

    private:
        QNetworkAccessManager *manager = new QNetworkAccessManager();
        QNetworkRequest *request;
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
