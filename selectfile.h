#ifndef SELECTFILE_H
#define SELECTFILE_H

#include <QDialog>

namespace Ui {
    class SelectFile;
}

class SelectFile : public QDialog
{
        Q_OBJECT

    public:
        explicit SelectFile(QWidget *parent = nullptr);
        ~SelectFile();

    private:
        Ui::SelectFile *ui;
};

#endif // SELECTFILE_H
