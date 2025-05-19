#ifndef JOURNALMODULE_H
#define JOURNALMODULE_H


#include <QWidget>
#include <QStateMachine>
#include <QDateEdit>

#include "../Classes/ClassStudentsModel.h"
#include "../Classes/ClassSubjectsModel.h"
#include "../Classes/ClassesModel.h"

#include "../ComboBoxFinderView.h"

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

    ComboBoxFinderView* classFinder;
    ComboBoxFinderView* subjectFinder;
    MarkTypeSelector* markTypeFinder;
    ComboBoxFinderView* teacherFinder;

    JournalModel* journalModel;

    ClassesModel* classesModel;
    ClassStudentsModel* classStudentsModel;
    ClassSubjectsModel* classSubjectsModel;

    QStateMachine* stateMachine;
    QState* classesNotLoaded;
    QState* classesLoaded;
    QState* classSelected;
    QState* keySelected;
    QState* emptyCellSelected;
    QState* rangeSelected;

    QState* resetGroupState;

    dbapi::Connection* connection = nullptr;

    void enterClassesNotLoadedState();
    void enterClassesLoadedState();
    void enterClassSelectedState();
    void enterKeySelectedState();
    void enterEmptyCellSelectedState();
    void enterRangeSelectedState();

    void handleSelectedMarks(const QItemSelection &selected, const QItemSelection &deselected);
    void handleMarksDeleting();

    void handleKeyChange();

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
    void setClassSelectedState();
    void setKeySelectedState();
    void setEmptyCellSelectedState();
    void setRangeSelectedState();
};


#endif // JOURNALMODULE_H
