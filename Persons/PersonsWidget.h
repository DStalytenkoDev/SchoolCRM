#ifndef PERSONSWIDGET_H
#define PERSONSWIDGET_H


#include <SchoolDatabaseApi/Connection/Connection.h>
#include <QWidget>
#include "PersonWidget.h"
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
    PersonWidget* personWidget;
    CreatePersonDialog* personDialog;

    PersonsModel* model;

    dbapi::Connection* connection = nullptr;

    void loadPersons();
    void deletePerson();

    void createPerson();
    void completePersonCreation();

    void manageSelection(const QItemSelection& selected, const QItemSelection& deselected);
};


#endif // PERSONSWIDGET_H
