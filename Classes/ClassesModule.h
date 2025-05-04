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

    QPersistentModelIndex activeItem;
    QItemSelection selectedItems;

    QStateMachine* stateMachine;
    QState* classesNotLoaded;
    QState* classesLoaded;
    QState* inClassCreation;
    QState* classSelected;
    QState* inItemAddition;
    QState* inItemDelition;
    QState* inClassDelition;
    QState* inTeacherChanging;

    dbapi::Connection* connection = nullptr;

    void enterToClassesNotLoaded();
    void enterToClassesLoaded();
    void enterToClassSelected();
    void enterToInItemDelition();
    void enterToInTeacherChanging();
    void enterToInClassDelition();

    void handleClickedItem(const QModelIndex &index);
    void handleSelectedItems(const QItemSelection &selected, const QItemSelection &deselected);
    void handleItemsDeletion();

    void initItemAddition();
    void completeItemAddition();

    void initClassCreation();
    void completeClassCreation();
    void abortClassCreation();

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

    void resetUi();

    private: signals:
        void goInClassesLoaded();
        void goInClassesNotLoaded();
        void goInClassSelected();
        void goInInItemDelition();
};


#endif // CLASSESMODULE_H
