#ifndef ROLESMODULE_H
#define ROLESMODULE_H


#include <QWidget>

#include <QItemSelection>
#include <QStateMachine>
#include <QSortFilterProxyModel>

#include <SchoolApi/Role.h>

#include "RolesModel.h"
#include "RoleCreationDialog.h"


namespace Ui {
class RolesModule;
}


class RolesModule : public QWidget
{
    Q_OBJECT

public:
    RolesModule(QWidget *parent = nullptr);

    void setConnection(dbapi::Connection* connection);

    ~RolesModule();

private:
    Ui::RolesModule *ui;

    RoleCreationDialog* roleCreationDialog = nullptr;

    RolesModel* model = nullptr;
    QSortFilterProxyModel* proxyModel;

    dbapi::Connection* connection = nullptr;

    QStateMachine* stateMachine;
    QState* updateGroup;
    QState* rolesNotLoaded;
    QState* rolesLoaded;
    QState* itemSelected;
    QState* searching;

    void enterRolesNotLoaded();
    void enterRolesLoaded();
    void enterItemSelected();
    void enterSearching();

    void handleRoleDeletion();

    void initRoleCreation();
    void completeRoleCreation();

    void completeSearching();
    void abortSearching();
    void handleSearching();

    void setupRoleFinder();
    void setupRolesList();
    void setupStateMachine();

    bool tryConnect();

    void showEvent(QShowEvent* event) override;

private: signals:
    void rolesLoadedAre();
};


#endif // ROLESMODULE_H
