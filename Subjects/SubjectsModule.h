#ifndef SUBJECTSMODULE_H
#define SUBJECTSMODULE_H

#include <QWidget>
#include <qitemselectionmodel.h>
#include "SubjectsModel.h"
#include "CreateSubjectDialog.h"
#include "../ComboBoxFinderView.h"

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
    CreateSubjectDialog* roleCreatationDialog;
    ComboBoxFinderView* roleFinder;

    SubjectsModel* model;
    QItemSelection selectedSubjects;

    dbapi::Connection* connection;

    void handleFoundSUbject(QModelIndex index);
    void handleClickedRole(const QModelIndex &index);
    void handleSelectedRoles(const QItemSelection &selected, const QItemSelection &deselected);

    void deleteRole();

    void initRoleCreation();
    void completeRoleCreation();

    void loadRoles();

};

#endif // SUBJECTSMODULE_H
