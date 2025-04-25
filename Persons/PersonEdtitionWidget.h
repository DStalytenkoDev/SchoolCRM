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
    void deleteButtonClicked();
    void abortButtonClicked();

private:
    QPushButton* saveBtn;
    QPushButton* deleteBtn;
    QPushButton* abortButton;
};


#endif // PERSONEDTITIONWIDGET_H
