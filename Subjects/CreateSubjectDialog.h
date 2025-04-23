#ifndef CREATESUBJECTDIALOG_H
#define CREATESUBJECTDIALOG_H

#include <QDialog>

namespace Ui {
class CreateSubjectDialog;
}

class CreateSubjectDialog : public QDialog
{
    Q_OBJECT

public:
    CreateSubjectDialog(QWidget *parent = nullptr);

    QString subject() const;

    ~CreateSubjectDialog();

private:
    Ui::CreateSubjectDialog *ui;
};

#endif // CREATESUBJECTDIALOG_H
