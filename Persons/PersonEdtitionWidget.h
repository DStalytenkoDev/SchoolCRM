#ifndef PERSONEDTITIONWIDGET_H
#define PERSONEDTITIONWIDGET_H


#include "PersonWidget.h"
#include <qpushbutton.h>


class PersonEditionWidget : public PersonWidget
{
    Q_OBJECT

public:
    PersonEditionWidget(QWidget* parent = nullptr);

signals:
    void saveButtonClicked();
    void abortButtonClicked();

private:
    QPushButton* saveBtn;
    QPushButton* abortButton;
};


#endif // PERSONEDTITIONWIDGET_H
