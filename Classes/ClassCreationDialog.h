#ifndef CLASSCREATIONDIALOG_H
#define CLASSCREATIONDIALOG_H


#include <QDialog>
#include <QAbstractItemModel>
#include "../ComboBoxFinderView.h"


namespace Ui {
class ClassCreationDialog;
}

class ClassCreationDialog : public QDialog
{
    Q_OBJECT

public:
    ClassCreationDialog(QWidget *parent = nullptr);

    QString name() const;
    QModelIndex currentIndex() const;

    void setModel(QAbstractItemModel* model);

    ~ClassCreationDialog();

private:
    Ui::ClassCreationDialog *ui;

    ComboBoxFinderView* teacherFinder;
};


#endif // CLASSCREATIONDIALOG_H
