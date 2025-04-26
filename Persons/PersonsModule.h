#ifndef PERSONSMODULE_H
#define PERSONSMODULE_H


#include <QWidget>
#include <QItemSelection>
#include "PersonCreationDialog.h"
#include "PersonEdtitionWidget.h"
#include "PersonsModel.h"
#include "../ComboBoxFinderView.h"
#include "../Roles/RolesModel.h"


namespace Ui {
class PersonsModule;
}


class PersonsModule : public QWidget
{
    Q_OBJECT

public:
    PersonsModule(QWidget *parent = nullptr);

    void setConnection(dbapi::Connection* connection);

    ~PersonsModule();

private:
    Ui::PersonsModule *ui;
    PersonEditionWidget* personWidget = nullptr;
    PersonCreationDialog* personDialog = nullptr;

    ComboBoxFinderView* personFinder = nullptr;

    PersonsModel* model = nullptr;
    RolesModel* rolesModel = nullptr;

    QItemSelection selectedPersons;
    QPersistentModelIndex clickedPerson;

    dbapi::Connection* connection = nullptr;

    void handleFoundPerson(QModelIndex index);
    void handleClickedPerson(const QModelIndex &index);
    void handleSelectedPerson(const QItemSelection &selected, const QItemSelection &deselected);

    void deleteSelectedPersons();
    void deleteClickedPerson();

    void initPersonCreation();
    void completePersonCreation(QDialog::DialogCode result);

    void initPersonEdition();
    void abortPersonEdition();
    void completePersonEdition();

    void loadPersons();
    bool loadRolesIfNeed();

    void setupPersonFinder();
    void setupPersonsList();
    void setupPersonEditionWidget();
};


#endif // PERSONSMODULE_H
