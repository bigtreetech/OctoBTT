#include "cameradialog.h"
#include "ui_cameradialog.h"
#include <mainwindow.h>
//#include <QSizeF>

//#include "QList"
//#include "QCamera"
//#include "QCameraInfo"



CameraDialog::CameraDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::CameraDialog)
{
    ui->setupUi(this);

    FUI = (MainWindow*)parent;

//    QList<QCameraInfo> list;
//    list = QCameraInfo::availableCameras();
//    ui->Box_CameraId->clear();
//    for (int i = 0; i < list.size(); i++)
//    {
//        ui->Box_CameraId->addItem(list.at(i).description());
//    }
    this->resize((int)(SizePercent.width()*800),(int)(SizePercent.height()*480));
    this->setMaximumSize((int)(SizePercent.width()*800),(int)(SizePercent.height()*480));
    this->setFixedSize((int)(SizePercent.width()*800),(int)(SizePercent.height()*480));

    QFont _font = ui->Btn_Sensor->font();
    _font.setPointSize(this->maximumWidth() > this->maximumHeight() ? (int)this->maximumHeight()/30 : (int)this->maximumWidth()/30);
     ui->Btn_Sensor->setFont(_font);
     ui->Btn_FlipH->setFont(_font);
     ui->Btn_FlipV->setFont(_font);

//     if(ui->Windows->geometry().width() > ui->Windows->geometry().height())
//         ui->Windows->setSizePolicy(QSizePolicy::Preferred,QSizePolicy::Expanding);
//     else
//         ui->Windows->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Preferred);

    /* 创建一个用于应用程序发送网络请求和接收网络应答的"管理员" */
    manager = new QNetworkAccessManager(this);
    /* 创建一个要发送的请求实例 */
    request = new QNetworkRequest(QUrl(((MainWindow*)FUI)->octonetwork.MainAddress +"/webcam/?action=snapshot"));
    request->setRawHeader("X_Api_Key",((MainWindow*)FUI)->octonetwork.X_API_Key);

    ui->Btn_Sensor->setChecked(false);
}

CameraDialog::~CameraDialog()
{
    delete ui;
}

void CameraDialog::showEvent(QShowEvent *event)
{
    Q_UNUSED(event);
    ui->Btn_Sensor->setChecked(true);
}

void CameraDialog::on_Btn_Back_clicked()
{
//    ui->Btn_Sensor->setText("Start");
//    ui->Windows->clear();
//    /* 解除关联，这样就不显示了 */
//    disconnect(manager, SIGNAL(finished(QNetworkReply*)), this, NULL);
    ui->Btn_Sensor->setChecked(false);

    this->close();
}

/* 实现槽函数 */
void CameraDialog::mjpeg_streamer_reply(QNetworkReply *reply)
{
    /* 读出全部数据 */
    QByteArray byteArr = reply->readAll();
    //    qDebug() << byteArr;

    /* 构建pixmap */
    //    QPixmap pixmap;
    //    pixmap.loadFromData(byteArr);
    QImage image;
    image.loadFromData(byteArr);
    //    QPixmap pixmap = QPixmap::fromImage(mirroredImage);


    /* Label显示图像 */
    ui->Windows->clear();
    ui->Windows->setPixmap(QPixmap::fromImage(image.mirrored(ui->Btn_FlipH->isChecked(), ui->Btn_FlipV->isChecked())));
//    ui->Windows->setScaledContents(true);
    ui->Windows->show();
    delete(reply);

    /* 继续请求URL */
    manager->get(*request);
}

void CameraDialog::on_Btn_Sensor_toggled(bool checked)
{
    if(checked)
    {
        ui->Btn_Sensor->setText("Stop");
        ui->Btn_Sensor->setIcon(QIcon(":/assets/stop.svg"));
        request->setUrl(QUrl(((MainWindow*)FUI)->octonetwork.MainAddress +"/webcam/?action=snapshot"));

        /* 设置"请求完成"与mjpeg_streamer_reply()这个槽关联 */
        connect(manager, SIGNAL(finished(QNetworkReply*)), this, SLOT(mjpeg_streamer_reply(QNetworkReply*)));

        /* 开始请求URL */
        manager->get(*request);
    }
    else
    {
        ui->Btn_Sensor->setText("Start");

        ui->Btn_Sensor->setIcon(QIcon(":/assets/start.svg"));
        ui->Windows->clear();

        /* 解除关联，这样就不显示了 */
        disconnect(manager, SIGNAL(finished(QNetworkReply*)), this, NULL);
    }
}
