#ifndef URL_CAMERA_H
#define URL_CAMERA_H

#include <QDialog>
#include <QSizeF>

extern int DebugFlat ;
extern QSizeF SizePercent;

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
        virtual void on_Btn_Back_clicked();

    private:
        Ui::URL_Camera *ui;
};

#endif // URL_CAMERA_H
