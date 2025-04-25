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
    SubjectsModule(QWidget *parent = nullptr);

    void setConnection(dbapi::Connection* connection);

    ~SubjectsModule();

private:
    Ui::SubjectsModule *ui;
    CreateSubjectDialog* subjectCreationDialog = nullptr;
    ComboBoxFinderView* subjectFinder = nullptr;

    SubjectsModel* model = nullptr;
    QItemSelection selectedSubjects;

    dbapi::Connection* connection = nullptr;

    void handleFoundSubject(QModelIndex index);
    void handleClickedSubject(const QModelIndex &index);
    void handleSelectedSubjects(const QItemSelection &selected, const QItemSelection &deselected);

    void deleteSubject();

    void initSubjectCreation();
    void completeSubjectCreation();

    void loadSubjects();
};


#endif // SUBJECTSMODULE_H
