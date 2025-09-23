#ifndef PERSONCREATIONDIALOG_H
#define PERSONCREATIONDIALOG_H


#include "PersonWidget.h"
#include <qpushbutton.h>
#include <QDialog>


/// this class does not operate the database, when finished does not disappear
class PersonCreationDialog : public PersonWidget
{
    Q_OBJECT

public:
    PersonCreationDialog(QWidget* parent = nullptr);

signals:
    void finished(QDialog::DialogCode code);

private:
    QPushButton* createBtn;
    QPushButton* cancelBtn;
};


#endif // PERSONCREATIONDIALOG_H
