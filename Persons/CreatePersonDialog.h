#ifndef CREATEPERSONDIALOG_H
#define CREATEPERSONDIALOG_H


#include "PersonWidget.h"
#include <qpushbutton.h>


/// this class does not operate the database
class CreatePersonDialog : public PersonWidget
{
    Q_OBJECT

public:
    CreatePersonDialog(QWidget* parent = nullptr);

signals:
    /// says when dialog is submitted
    void created();

    /// says when dialog is canceled
    void canceled();

private:
    QPushButton* createBtn;
    QPushButton* cancelBtn;
};


#endif // CREATEPERSONDIALOG_H
