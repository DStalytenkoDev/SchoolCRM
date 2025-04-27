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

    void deleteItems();

    void initItemAddition();
    void abortItemAddition();
    void completeItemAddition();

    void loadAll();

    void setupFinders();
    void setupLists();
    void setupToolBar();

    bool tryConnect();
    void showInternalError();
};

#endif // CLASSESMODULE_H
