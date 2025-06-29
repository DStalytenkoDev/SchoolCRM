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

    dbapi::Connection* connection = nullptr;

    void handleSelectedTeacher();
    void handleSelectedSubject();

    void handleSubjectDeleting();

    void initSubjectAddition();
    void abortSubjectAddition();
    void completeSubjectAddition();

    void loadSubjects();

    void setupFinders();
    void setupSubjectsList();
    void setupToolBar();

    bool tryConnect();

    /// if not selected returns nullptr
    dbapi::Person* currentTeacher();
};

#endif // SUBJECTSOFTEACHERMODULE_H
