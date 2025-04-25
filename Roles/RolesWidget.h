#ifndef ROLESWIDGET_H
#define ROLESWIDGET_H


#include <QWidget>
#include <QItemSelection>

#include <SchoolDatabaseApi/Role/Role.h>

#include "RolesModel.h"
#include "CreateRoleDialog.h"
#include "../ComboBoxFinderView.h"


namespace Ui {
class RolesWidget;
}


class RolesWidget : public QWidget
{
    Q_OBJECT

public:
    RolesWidget(QWidget *parent = nullptr);

    void setConnection(dbapi::Connection* connection);

    ~RolesWidget();

private:
    Ui::RolesWidget *ui;
    CreateRoleDialog* roleCreationDialog = nullptr;
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
};


#endif // ROLESWIDGET_H
