#ifndef JOURNALMODULE_H
#define JOURNALMODULE_H


#include <QWidget>
#include <QStateMachine>
#include <QDateEdit>
#include <QSpacerItem>

#include "../Classes/ClassStudentsModel.h"
#include "../Classes/ClassSubjectsModel.h"
#include "../Classes/ClassesModel.h"
#include "../Persons/PersonsModel.h"

#include "../ComboBoxFinderView.h"
#include "MarkCreatingDialog.h"

#include "MarkTypeSelector.h"
#include "JournalModel.h"


namespace Ui {
class JournalModule;
}


class JournalModule : public QWidget
{
    Q_OBJECT

public:
    JournalModule(QWidget *parent = nullptr);

    void setConnection(dbapi::Connection* connection);
    void prepare();

    ~JournalModule();

private:
    Ui::JournalModule *ui;
    QSpacerItem* journalSpacer;

    ComboBoxFinderView* classFinder;
    ComboBoxFinderView* subjectFinder;
    MarkTypeSelector* markTypeFinder;

    JournalModel* journalModel;

    ClassesModel* classesModel;
    ClassStudentsModel* classStudentsModel;
    ClassSubjectsModel* classSubjectsModel;
    PersonsModel* personsModel;

    QStateMachine* stateMachine;
    QState* classesNotLoaded;
    QState* classesLoaded;
    QState* classSelected;
    QState* keySelected;
    QState* emptyCellSelected;
    QState* rangeSelected;

    QState* resetGroupState;

    dbapi::Connection* connection = nullptr;

    MarkCreatingDialog* markCreatingDialog = nullptr;
    QPersistentModelIndex selectedMark;

    void enterClassesNotLoadedState();
    void enterClassesLoadedState();
    void enterClassSelectedState();
    void enterKeySelectedState();
    void enterEmptyCellSelectedState();
    void enterRangeSelectedState();

    void handleSelectedMarks();
    void handleMarksDeleting();

    void handleJournalLoading();
    void handleJournalAndTeachersLoading();

    void initMarkCreating();
    void completeMarkCreating();

    void setupFinders();
    void setupJournal();
    void setupToolBar();
    void setupStateMachine();

    /// trys to open the connection, otherwise shows error
    bool tryConnect();
    /// shows generall error
    void showInternalError();
    /// closes connection and shows generall error
    void abortConnection();
    /// if not selected returns nullptr
    dbapi::Class* currentClass();

    bool loadCompatibleTeachers();

    void resetUi();

private: signals:
    void setClassesNotLoadedState();
    void setClassesLoadedState();
    void setKeySelectedState();
    void setEmptyCellSelectedState();
    void setRangeSelectedState();
};


#endif // JOURNALMODULE_H
