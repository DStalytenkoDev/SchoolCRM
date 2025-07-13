#ifndef SUBJECTSMODULE_H
#define SUBJECTSMODULE_H


#include <QWidget>
#include <QSortFilterProxyModel>
#include <QStateMachine>
#include "SubjectsModel.h"
#include "CreateSubjectDialog.h"


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

    SubjectsModel* model = nullptr;
    QSortFilterProxyModel* proxyModel;

    dbapi::Connection* connection = nullptr;

    QStateMachine* stateMachine;
    QState* updateGroup;
    QState* subjectsNotLoaded;
    QState* subjectsLoaded;
    QState* itemSelected;
    QState* searching;

    void enterSubjectsNotLoaded();
    void enterSubjectsLoaded();
    void enterItemSelected();
    void enterSearching();

    void handleSubjectDeletion();

    void initSubjectCreation();
    void completeSubjectCreation();

    void completeSearching();
    void abortSearching();
    void handleSearching();

    void setupSubjectFinder();
    void setupSubjectsList();
    void setupStateMachine();

    bool tryConnect();

    void showEvent(QShowEvent* event) override;

private: signals:
    void subjectsLoadedAre();
};


#endif // SUBJECTSMODULE_H
