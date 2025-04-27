#ifndef SUBJECTSOFTEACHERMODULE_H
#define SUBJECTSOFTEACHERMODULE_H


#include <QWidget>
#include <QItemSelection>
#include "../ComboBoxFinderView.h"
#include "SubjectsOfTeacherModel.h"
#include "../Persons/PersonsModel.h"
#include "../Subjects/SubjectsModel.h"


namespace Ui {
class SubjectsOfTeacherModule;
}


class SubjectsOfTeacherModule : public QWidget
{
    Q_OBJECT

public:
    SubjectsOfTeacherModule(QWidget *parent = nullptr);

    void setConnection(dbapi::Connection* connection);
    void prepare();

    ~SubjectsOfTeacherModule();

private:
    Ui::SubjectsOfTeacherModule *ui;

    ComboBoxFinderView* personFinder;
    ComboBoxFinderView* subjectFinder;

    PersonsModel* personsModel;
    SubjectsModel* subjectsModel;
    SubjectsOfTeacherModel* model;

    QPersistentModelIndex activePerson;
    QItemSelection selectedSubjects;

    dbapi::Connection* connection = nullptr;

    void handleFoundPerson(QModelIndex index);

    void handleClickedSubject(const QModelIndex &index);
    void handleSelectedSubjects(const QItemSelection &selected, const QItemSelection &deselected);

    void deleteSubjects();

    void initSubjectAddition();
    void abortSubjectAddition();
    void completeSubjectAddition();

    void loadSubjects();

    void setupFinders();
    void setupSubjectsList();
    void setupToolBar();

    bool tryConnect();
    void showInternalError();
};

#endif // SUBJECTSOFTEACHERMODULE_H
