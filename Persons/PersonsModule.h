#ifndef PERSONSMODULE_H
#define PERSONSMODULE_H


#include <QWidget>
#include <QItemSelection>
#include <QStateMachine>
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
    void prepare();

    ~PersonsModule();

private:
    Ui::PersonsModule *ui;
    PersonEditionWidget* personWidget = nullptr;
    PersonCreationDialog* personDialog = nullptr;

    ComboBoxFinderView* personFinder = nullptr;

    PersonsModel* model = nullptr;
    RolesModel* rolesModel = nullptr;

    QStateMachine* stateMachine;
    QState* resetState;
    QState* personsNotLoaded;
    QState* personsLoaded;
    QState* itemSelected;
    QState* personEditing;

    dbapi::Connection* connection = nullptr;

    void enterPersonsNotLoaded();
    void enterPersonsLoaded();
    void enterItemSelected();
    void enterPersonEditing();

    void handleFoundPerson(QModelIndex index);
    void handleSelectedItem();

    void handlePersonDeletion();

    void initPersonCreation();
    void completePersonCreation(QDialog::DialogCode result);

    void completePersonEdition();

    void setupPersonFinder();
    void setupPersonsList();
    void setupPersonEditionWidget();
    void setupStateMachine();

    /// trys to open the connection, otherwise shows error
    bool tryConnect();

private: signals:
    void reseted();
    void itemSelectedIs();
    void personsLoadedAre();
    void dataError();
    void itemDeselectedIs();
};


#endif // PERSONSMODULE_H
