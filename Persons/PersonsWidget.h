#ifndef PERSONSWIDGET_H
#define PERSONSWIDGET_H


#include <SchoolDatabaseApi/Connection/Connection.h>
#include <QWidget>
#include "ShowPerson.h"
#include "CreatePersonDialog.h"
#include "PersonsModel.h"


namespace Ui {
class PersonsWidget;
}


class PersonsWidget : public QWidget
{
    Q_OBJECT

public:
    PersonsWidget(QWidget *parent = nullptr);

    void setConnection(dbapi::Connection* connection);

    ~PersonsWidget();

private:
    Ui::PersonsWidget *ui;
    ShowPerson* personWidget;
    CreatePersonDialog* personDialog;

    PersonsModel* model;
    dbapi::Person currentPerson;

    dbapi::Connection* connection = nullptr;

    void loadPersons();

    void updatePerson();
    void deletePerson();

    void createPerson();
    void completePersonCreation();

    void showPerson();
    void manageSelection(const QItemSelection& selected, const QItemSelection& deselected);
};


#endif // PERSONSWIDGET_H
