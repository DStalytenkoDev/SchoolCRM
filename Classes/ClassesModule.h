#ifndef CLASSESMODULE_H
#define CLASSESMODULE_H


#include <QWidget>
#include <QItemSelection>
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

    ClassCreationDialog* classCreationDialog;

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

    dbapi::Connection* connection = nullptr;

    void handleFoundClass(QModelIndex index);

    void handleClickedItem(const QModelIndex &index);
    void handleSelectedItems(const QItemSelection &selected, const QItemSelection &deselected);
    void handleTeacherChanging(const QModelIndex &index);

    void deleteItems();
    bool deleteSubjects(const dbapi::Class::Key& key);
    bool deleteStudents();

    void deleteClass();

    void initItemAddition();
    void abortItemAddition();
    void completeItemAddition();

    void initClassCreation();
    void completeClassCreation();
    void abortClassCreation();

    void setupFinders();
    void setupLists();
    void setupToolBar();

    /// trys to open the connection, otherwise shows error
    bool tryConnect();
    void showInternalError();

    /// does not manage the connection, manages erorr message
    bool loadHomeroomTeacher();
    /// does not manage the connection, manages erorr message
    bool loadStudentsList();
    /// does not manage the connection, manages erorr message
    bool loadSubjectsList();
};

#endif // CLASSESMODULE_H
