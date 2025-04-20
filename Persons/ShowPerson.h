#ifndef SHOWPERSON_H
#define SHOWPERSON_H


#include "PersonWidget.h"
#include <qpushbutton.h>


class ShowPerson : public PersonWidget
{
    Q_OBJECT

public:
    ShowPerson(QWidget* parent = nullptr);

signals:
    void updatePerson();

private:
    QPushButton* updateBtn;
};


#endif // SHOWPERSON_H
