#ifndef SUBJECTSMODULE_H
#define SUBJECTSMODULE_H

#include <QWidget>
#include "SubjectsModel.h"

namespace Ui {
class SubjectsModule;
}

class SubjectsModule : public QWidget
{
    Q_OBJECT

public:
    explicit SubjectsModule(QWidget *parent = nullptr);
    ~SubjectsModule();

private:
    Ui::SubjectsModule *ui;
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

#endif // SUBJECTSMODULE_H
