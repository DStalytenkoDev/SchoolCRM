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
    QState* studentSelected;
    QState* subjectSelected;

    QState* resetGroup;

    dbapi::Connection* connection = nullptr;

    void enterClassesNotLoaded();
    void enterClassesLoaded();
    void enterClassSelected();
    void enterItemAdding();
    void enterItemSelected();

    void handleSelectedSubject();
    void handleSelectedStudent();

    void handleSubjectDeletion();
    void handleStudentDeletion();

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

    private: signals:
        void reseted();
        void dataError();
        void classDeletedIs();
        void classesLoadedAre();
        void itemDeselectedIs();
        void studentSelectedIs();
        void subjectSelectedIs();
};


#endif // CLASSESMODULE_H
