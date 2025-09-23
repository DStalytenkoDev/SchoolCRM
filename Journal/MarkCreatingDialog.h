#ifndef MARKCREATINGDIALOG_H
#define MARKCREATINGDIALOG_H


#include <QDialog>
#include "../ComboBoxFinderView.h"


namespace Ui {
class MarkCreatingDialog;
}


class MarkCreatingDialog : public QDialog
{
    Q_OBJECT

public:
    MarkCreatingDialog(QWidget *parent = nullptr);

    void setTeachersModel(QAbstractItemModel* model);

    QModelIndex currentTeacher();
    int value();

    void clear();

    ~MarkCreatingDialog();

private:
    Ui::MarkCreatingDialog *ui;

    ComboBoxFinderView* teacherFinder;
};


#endif // MARKCREATINGDIALOG_H
