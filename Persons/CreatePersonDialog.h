#ifndef CREATEPERSONDIALOG_H
#define CREATEPERSONDIALOG_H


#include "PersonWidget.h"
#include <qpushbutton.h>
#include <QDialog>


/// this class does not operate the database
class CreatePersonDialog : public PersonWidget
{
    Q_OBJECT

public:
    CreatePersonDialog(QWidget* parent = nullptr);

signals:
    void finished(QDialog::DialogCode code);

private:
    QPushButton* createBtn;
    QPushButton* cancelBtn;
};


#endif // CREATEPERSONDIALOG_H
