#include "url_camera.h"
#include "ui_url_camera.h"
//#include <QMediaPlayer>
//#include <QVideoWidget>

URL_Camera::URL_Camera(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::URL_Camera)
{
    ui->setupUi(this);

//        QMediaPlayer* player = new QMediaPlayer(0, QMediaPlayer::VideoSurface);
//        QVideoWidget* vw = new QVideoWidget;
//        QVBoxLayout* lay =  new QVBoxLayout;
//        lay->addWidget(vw);
//        vw->setMinimumSize(400, 300);
//        lay->setSizeConstraint(QLayout::SetMinimumSize);
//        ui->wi->setLayout(lay);

//        player->setVideoOutput(vw);

//        QNetworkRequest req(QUrl("http://192.168.3.111:8080/?action=stream"));

//        QMediaContent c(req);
//        player->setMedia(c);
//        player->play();
}

URL_Camera::~URL_Camera()
{
    delete ui;
}

void URL_Camera::on_Btn_Back_clicked()
{
    this->close();
}
