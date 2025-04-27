#include <QMessageBox>
#include <qmenu.h>
#include <SchoolDatabaseApi/Classmate/Classmate.h>
#include "ClassesModule.h"
#include "ui_ClassesModule.h"


ClassesModule::ClassesModule(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::ClassesModule)
{
    ui->setupUi(this);
}

void ClassesModule::setConnection(dbapi::Connection *connection)
{
    this->connection = connection;

    this->personsModel->setConnection(connection);
    this->subjectsModel->setConnection(connection);
    this->classesModel->setConnection(connection);
    this->classSubjectsModel->setConnection(connection);
    this->classStudentsModel->setConnection(connection);
}

void ClassesModule::prepare()
{
    if(this->tryConnect())
        return;

    auto error = this->classesModel->loadAll();

    if(error.type != dbapi::ApiError::NoError)
    {
        this->showInternalError();
        return;
    }

    this->connection->close();
}

ClassesModule::~ClassesModule()
{
    delete ui;
}

void ClassesModule::handleFoundClass(QModelIndex index)
{
    if(not this->tryConnect())
        return;

    if(not this->loadHomeroomTeacher())
        return this->connection->close();

    if(not this->loadStudentsList())
        return this->connection->close();

    if(not this->loadSubjectsList())
        return this->connection->close();

    this->connection->close();
}

void ClassesModule::handleClickedItem(const QModelIndex &index)
{
    this->selectedItems.clear();
    this->selectedItems.select(index, index);

    this->ui->deleteItem->setHidden(false);
}

void ClassesModule::handleSelectedItems(const QItemSelection &selected, const QItemSelection &deselected)
{
    this->selectedItems = selected;

    if(selected.empty())
        this->ui->deleteItem->hide();
    else
        this->ui->deleteItem->setHidden(false);
}

void ClassesModule::handleTeacherChanging(const QModelIndex &index)
{
    if(not this->tryConnect())
        return;

    dbapi::ApiError error;
    auto teachers = dbapi::Classmate::loadAll(this->connection, &error);

    if(error.type != dbapi::ApiError::NoError)
    {
        this->showInternalError();
        this->connection->close();
        return;
    }

    dbapi::Class* grade = this->classesModel->grade(this->classFinder->currentIndex());
    dbapi::Person* newTeacher = this->personsModel->person(this->teacherFinder->currentIndex());
    dbapi::Classmate* foundOldTeacher = nullptr;

    // find old teacher
    for(auto teacher : teachers)
    {
        if(teacher->grade() == grade->key())
            foundOldTeacher = teacher;
        else
            delete teacher;
    }

    if(not foundOldTeacher)
    {
        this->showInternalError();
        this->connection->close();
        return;
    }

    // delete old teacher
    bool teacherDeleted = foundOldTeacher->remove();
    delete foundOldTeacher;

    if(not teacherDeleted)
    {
        this->showInternalError();
        this->connection->close();
        return;
    }

    // create new teacher
    dbapi::Classmate teacher({newTeacher->key()}, this->connection);

    if(not teacher.store())
    {
        this->showInternalError();
        this->connection->close();
        return;
    }

    this->connection->close();
    QMessageBox::information(this, "Change", "Teacher was changed");
}

void ClassesModule::deleteItems()
{
    if(not this->tryConnect())
        return;

    auto grade = this->classesModel->grade(this->classFinder->currentIndex());

    bool deleted;

    // decide which items are for deletion and delete
    if(selectedItems.front().model() == this->classStudentsModel)
        deleted = this->deleteStudents();
    else
        deleted = this->deleteSubjects(grade->key());

    this->connection->close();

    if(not deleted)
        this->showInternalError();
    else
        QMessageBox::information(this, "Deletion", "All selected items had been deleted");
}

bool ClassesModule::deleteSubjects(const dbapi::Class::Key& key)
{
    dbapi::ClassSubjectsList list({key}, this->connection);

    for(QModelIndex& index : this->selectedItems.indexes())
    {
        auto key = this->classSubjectsModel->subject(index);

        list.appendSubject(key); // must add before delition 'cause must be deleted an item of the list by an iterator
        list.removeSubject(list.subjects().cend() - 1);
    }

    bool deleted = list.remove();

    if(not deleted)
        return false;

    // hide (delete) deleted items
    for(QModelIndex& index : this->selectedItems.indexes())
        this->classSubjectsModel->removeRow(index.row());

    return true;
}

bool ClassesModule::deleteStudents()
{
    dbapi::Student student(this->connection);
    bool failFlag = false;

    for(QModelIndex& index : this->selectedItems.indexes())
    {
        auto person = this->classStudentsModel->student(index);

        student.setKey({person->key()});

        if(not student.remove())
            failFlag = true;
        else
            this->classStudentsModel->removeRow(index.row());
    }

    if(failFlag)
        return false;

    return true;
}

void ClassesModule::deleteClass()
{
    if(not this->tryConnect())
        return;

    auto grade = this->classesModel->grade(this->classFinder->currentIndex());

    // delete class subjects
    dbapi::ClassSubjectsList subjectsList({grade->key()}, this->connection);
    subjectsList.remove();

    // delete class students
    dbapi::ApiError studentsDeletionError;
    auto students = dbapi::Student::loadAll(this->connection, &studentsDeletionError);

    for(auto student : students)
    {
        if(student->grade() == grade->key())
            student->remove();

        delete student;
    }

    // delete class teacher
    dbapi::ApiError teacherDelitionError;
    auto teachers = dbapi::Classmate::loadAll(this->connection, &teacherDelitionError);

    for(auto teacher : teachers)
    {
        if(teacher->grade() == grade->key())
            teacher->remove();

        delete teacher;
    }

    // delete class
    grade->remove();

    this->connection->close();

    if(grade->error().type != dbapi::ApiError::NoError)
        this->showInternalError();
    else
        QMessageBox::information(this, "Delition", "Class was deleted");
}

void ClassesModule::initItemAddition()
{
    if(not this->tryConnect())
        return;

    dbapi::ApiError error;

    // decide weather we add subjects or students
    if(this->ui->tabWidget->currentWidget() == this->ui->studentsTab)
    {
        this->itemForAdditionFinder->setModel(this->personsModel);
        error = this->personsModel->loadAll();
    }
    else
    {
        this->itemForAdditionFinder->setModel(this->subjectsModel);
        error = this->subjectsModel->loadAll();
    }

    this->connection->close();

    if(error.type != dbapi::ApiError::NoError)
        return this->showInternalError();

    this->ui->itemAdditionMenu->setHidden(false); // show addition menu
}

void ClassesModule::abortItemAddition()
{
    this->ui->itemAdditionMenu->hide();
}

void ClassesModule::completeItemAddition()
{
    if(not this->tryConnect())
        return;

    bool added = false;

    // decide weather we add subjects or students
    if(this->itemForAdditionFinder->model() == this->personsModel)
    {
        // add student

        auto person = this->personsModel->person(this->itemForAdditionFinder->currentIndex());
        dbapi::Student student({person->key()}, this->connection);

        if(student.store())
        {
            added = true;
            this->classStudentsModel->insertRow(*person);
        }
    }
    else
    {
        // add subject

        auto subject = this->subjectsModel->subject(this->itemForAdditionFinder->currentIndex());
        auto grade = this->classesModel->grade(this->classFinder->currentIndex());

        dbapi::ClassSubjectsList list({grade->key()}, this->connection);
        list.appendSubject(subject->key());

        if(list.update())
        {
            added = true;
            this->classSubjectsModel->insertRow(*subject);
        }
    }

    if(not added)
        return this->showInternalError();

    QMessageBox::information(this, "Addition", "Item was added");
}

void ClassesModule::initClassCreation()
{
}

void ClassesModule::completeClassCreation()
{

}

void ClassesModule::abortClassCreation()
{

}

void ClassesModule::setupFinders()
{
    this->classFinder = new ComboBoxFinderView(this);
    this->classFinder->setModel(this->classesModel);
    this->ui->menuLayout->insertWidget(0, this->classFinder);

    connect(this->classFinder, &ComboBoxFinderView::foundItem, this, &ClassesModule::handleFoundClass);

    this->teacherFinder = new ComboBoxFinderView(this);
    this->teacherFinder->setModel(this->personsModel);
    this->teacherFinder->hide();
    this->ui->menuLayout->insertWidget(1, this->teacherFinder);

    connect(this->teacherFinder, &ComboBoxFinderView::foundItem, this, &ClassesModule::handleTeacherChanging);

    this->itemForAdditionFinder = new ComboBoxFinderView(this);
    this->ui->additionMenuLayout->insertWidget(0, this->itemForAdditionFinder);
}

void ClassesModule::setupLists()
{
    this->ui->subjectsList->setModel(this->classSubjectsModel);
    this->ui->subjectsList->setSelectionBehavior(QAbstractItemView::SelectRows);
    this->ui->subjectsList->setSelectionMode(QAbstractItemView::MultiSelection);

    // handle click
    connect(this->ui->subjectsList, &QListView::clicked, this, &ClassesModule::handleClickedItem);

    // handle selection
    connect(this->ui->subjectsList->selectionModel(), &QItemSelectionModel::selectionChanged, this, &ClassesModule::handleSelectedItems);

    this->ui->studentsList->setModel(this->classSubjectsModel);
    this->ui->studentsList->setSelectionBehavior(QAbstractItemView::SelectRows);
    this->ui->studentsList->setSelectionMode(QAbstractItemView::MultiSelection);

    // handle click
    connect(this->ui->subjectsList, &QListView::clicked, this, &ClassesModule::handleClickedItem);

    // handle selection
    connect(this->ui->subjectsList->selectionModel(), &QItemSelectionModel::selectionChanged, this, &ClassesModule::handleSelectedItems);
}

void ClassesModule::setupToolBar()
{
    this->ui->itemAdditionMenu->hide();
    this->ui->deleteItem->hide();

    // handle item deletion
    connect(this->ui->deleteItem, &QPushButton::clicked, this, &ClassesModule::deleteItems);

    // handle item addition
    connect(this->ui->addItem, &QPushButton::clicked, this, &ClassesModule::initItemAddition);

    this->menu = new QMenu(this);
    this->ui->toolButton->setMenu(this->menu);

    this->classCreationAction = new QAction("Create class", this->menu);
    connect(this->classCreationAction, &QAction::triggered, this, &ClassesModule::initClassCreation);

    this->classDeletionAction = new QAction("Delete class");
    connect(this->classDeletionAction, &QAction::triggered, this, &ClassesModule::deleteClass);
}

bool ClassesModule::tryConnect()
{
    if(this->connection->open())
        return true;

    this->showInternalError();

    return false;
}

void ClassesModule::showInternalError()
{
    QMessageBox::critical(this, "Internal error", "Please check your connection to the database");
}

bool ClassesModule::loadHomeroomTeacher()
{
    dbapi::ApiError error;
    auto teachers = dbapi::Classmate::loadAll(this->connection, &error);

    auto grade = this->classesModel->grade(this->classFinder->currentIndex());
    dbapi::Person person(this->connection);

    for(auto teacher : teachers)
    {
        if(teacher->grade() == grade->key())
            person.setKey(teacher->key().person);

        delete teacher;
    }

    if(not person.load())
    {
        this->showInternalError();
        return false;
    }

    this->personsModel->



}

bool ClassesModule::loadStudentsList()
{

}

bool ClassesModule::loadSubjectsList()
{

}
