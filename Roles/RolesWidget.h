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

    // manage search bar
    // manage create dialog
    // manage selection

    ~RolesWidget();

private:
    Ui::RolesWidget *ui;
    CreateRoleDialog* roleCreatationDialog;
    ComboBoxFinderView* roleFinder;

    RolesModel* model;
    QItemSelection selectedRoles;

    dbapi::Connection* connection;

    void handleFoundRole(QModelIndex index);
    void handleClickedRole(const QModelIndex &index);
    void handleSelectedRoles(const QItemSelection &selected, const QItemSelection &deselected);

    void deleteRole();

    void initRoleCreation();
    void completeRoleCreation();

    void loadRoles();
};


#endif // ROLESWIDGET_H
