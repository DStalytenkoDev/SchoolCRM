#ifndef ROLESMODULE_H
#define ROLESMODULE_H


#include <QWidget>
#include <QItemSelection>

#include <SchoolDatabaseApi/Role/Role.h>

#include "RolesModel.h"
#include "RoleCreationDialog.h"
#include "../ComboBoxFinderView.h"


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
    ComboBoxFinderView* roleFinder = nullptr;

    RolesModel* model = nullptr;
    QItemSelection selectedRoles;

    dbapi::Connection* connection = nullptr;

    void handleFoundRole(QModelIndex index);
    void handleClickedRole(const QModelIndex &index);
    void handleSelectedRoles(const QItemSelection &selected, const QItemSelection &deselected);

    void deleteRole();

    void initRoleCreation();
    void completeRoleCreation();

    void loadRoles();

    void setupRoleFinder();
    void setupRolesList();
};


#endif // ROLESMODULE_H
