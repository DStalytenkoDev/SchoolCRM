#ifndef CLASSESMODULE_H
#define CLASSESMODULE_H


#include <QWidget>
#include <QItemSelection>
#include <QStateMachine>
#include "../ComboBoxFinderView.h"
#include "../Persons/PersonsModel.h"
#include "../Subjects/SubjectsModel.h"
#include "ClassSubjectsModel.h"
#include "ClassStudentsModel.h"
#include "ClassesModel.h"
#include "ClassCreationDialog.h"


namespace Ui {
class ClassesModule;
}


class ClassesModule : public QWidget
{
    Q_OBJECT

public:
    ClassesModule(QWidget *parent = nullptr);

    void setConnection(dbapi::Connection* connection);
    void prepare();

    ~ClassesModule();

private:
    Ui::ClassesModule *ui;

    QMenu* menu;
    QAction* classCreationAction;
    QAction* classDeletionAction;

    ClassCreationDialog* classCreationDialog = nullptr;

    ComboBoxFinderView* classFinder;
    ComboBoxFinderView* teacherFinder;
    ComboBoxFinderView* itemForAdditionFinder;

    PersonsModel* personsModel;
    SubjectsModel* subjectsModel;
    ClassesModel* classesModel;
    ClassSubjectsModel* classSubjectsModel;
    ClassStudentsModel* classStudentsModel;

    QStateMachine* stateMachine;
    QState* classesNotLoaded;
    QState* classesLoaded;
    QState* classSelected;
    QState* itemAdding;
    QState* studentsSelected;
    QState* subjectsSelected;

    QState* resetGroupState;

    dbapi::Connection* connection = nullptr;

    void enterClassesNotLoaded();
    void enterClassesLoaded();
    void enterClassSelected();
    void enterItemAdding();
    void enterStudentsSelected();
    void enterSubjectsSelected();

    void handleSelectedSubjects();
    void handleSelectedStudents();

    void handleSubjectsDeletion();
    void handleStudentsDeletion();

    void handleSelectedTeacher();
    void handleChangedTab();
    void handleClassDeleting();

    void completeItemAddition();

    void initClassCreation();
    void completeClassCreation();

    /// if not selected returns nullptr
    dbapi::Class* currentClass();

    /// if not selected returns nullptr
    dbapi::Person* currentTeacher();

    void setupFinders();
    void setupLists();
    void setupToolBar();
    void setupStateMachine();

    /// trys to open the connection, otherwise shows error
    bool tryConnect();
    /// shows generall error
    void showInternalError();
    /// closes connection and shows generall error
    void abortConnection();

    /// does not manage the connection
    bool loadHomeroomTeacher();
    /// does not manage the connection
    bool loadStudentsList();
    /// does not manage the connection
    bool loadSubjectsList();

    /// does not manages the connection
    bool deleteSubjects();
    /// does not manages the connection
    bool deleteStudents();

    private: signals:
        void reseted();
        void dataError();
        void classDeletedIs();
        void classesLoadedAre();
        void itemsDeselectedAre();
        void studentsSelectedAre();
        void subjectsSelectedAre();
};


#endif // CLASSESMODULE_H
