#ifndef URL_CAMERA_H
#define URL_CAMERA_H

#include <QDialog>

namespace Ui {
    class URL_Camera;
}

class URL_Camera : public QDialog
{
        Q_OBJECT

    public:
        explicit URL_Camera(QWidget *parent = nullptr);
        ~URL_Camera();

    private slots:
        void on_Btn_Back_clicked();

    private:
        Ui::URL_Camera *ui;
};

#endif // URL_CAMERA_H
