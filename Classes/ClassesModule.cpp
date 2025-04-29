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

    this->personsModel = new PersonsModel(this);
    this->subjectsModel = new SubjectsModel(this);
    this->classesModel = new ClassesModel(this);
    this->classSubjectsModel = new ClassSubjectsModel(this);
    this->classStudentsModel = new ClassStudentsModel(this);

    this->setupFinders();
    this->setupLists();
    this->setupToolBar();
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
    if(not this->tryConnect())
        return;

    auto error = this->classesModel->loadAll();
    this->connection->close();

    this->strictGuiForUnselectedClass();

    if(error.type != dbapi::ApiError::NoError)
        this->showInternalError();
}

ClassesModule::~ClassesModule()
{
    delete ui;
}

void ClassesModule::handleFoundClass(QModelIndex index)
{
    // reset gui
    this->strictGuiForUnselectedClass();

    int _index = index.row();
    int __index = this->classesModel->rowCount();
    int ___index = this->classFinder->currentIndex();

    if(not this->currentClass())
        return;

    if(not this->tryConnect())
        return;

    if(not this->loadHomeroomTeacher())
        return this->abortConnection();

    if(not this->loadStudentsList())
        return this->abortConnection();

    if(not this->loadSubjectsList())
        return this->abortConnection();

    // set gui for selected class
    this->teacherFinder->setHidden(false);
    this->classDeletionAction->setDisabled(false);

    this->connection->close();
}

void ClassesModule::handleClickedItem(const QModelIndex &index)
{
    this->strictGuiForNoItemAddition();
    this->strictGuiForNoItemDelition();

    if(not this->currentClass())
        return;

    if(not index.isValid())
        return;

    this->selectedItems.clear();
    this->selectedItems.select(index, index);

    this->ui->deleteItem->setHidden(false);
}

void ClassesModule::handleSelectedItems(const QItemSelection &selected, const QItemSelection &deselected)
{
    this->selectedItems = selected;
    this->strictGuiForNoItemAddition();
    this->strictGuiForNoItemDelition();

    if(selected.empty())
        this->ui->addItem->setHidden(false);
    else
        this->ui->deleteItem->setHidden(false);
}

void ClassesModule::handleTeacherChanging(const QModelIndex &index)
{
    if(not this->currentClass())
        return;

    if(not index.isValid())
        return;

    if(not this->tryConnect())
        return;

    dbapi::ApiError error;
    auto teachers = dbapi::Classmate::loadAll(this->connection, &error);

    if(error.type != dbapi::ApiError::NoError)
        return this->abortConnection();

    dbapi::Class* grade = this->currentClass();
    dbapi::Person* newTeacher = this->currentTeacher();
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
        return this->abortConnection();

    // delete old teacher
    bool teacherDeleted = foundOldTeacher->remove();
    delete foundOldTeacher;

    if(not teacherDeleted)
        return this->abortConnection();

    // create new teacher
    dbapi::Classmate teacher({newTeacher->key()}, this->connection);

    if(not teacher.store())
        return this->abortConnection();

    this->connection->close();
    QMessageBox::information(this, "Change", "Teacher was changed");
}

void ClassesModule::handleItemsDeletion()
{
    if(not this->currentClass())
        return;

    if(not this->tryConnect())
        return;

    bool deleted;

    // decide which items are for deletion and delete
    if(selectedItems.front().model() == this->classStudentsModel)
        deleted = this->deleteStudents();
    else
        deleted = this->deleteSubjects();

    this->connection->close();

    if(not deleted)
        return this->showInternalError();

    this->strictGuiForNoItemDelition();

    this->selectedItems.clear();

    QMessageBox::information(this, "Deletion", "All selected items had been deleted");
}

bool ClassesModule::deleteSubjects()
{
    dbapi::ClassSubjectsList list({this->currentClass()->key()}, this->connection);

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

void ClassesModule::handleClassDeletion()
{
    if(not this->currentClass())
        return;

    if(not this->tryConnect())
        return;

    auto grade = this->currentClass();

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
    if(not grade->remove())
        return this->abortConnection();

    this->connection->close();

    QMessageBox::information(this, "Delition", "Class was deleted");

    this->strictGuiForUnselectedClass();
}

void ClassesModule::initItemAddition()
{
    this->strictGuiForNoItemAddition();
    this->strictGuiForNoItemDelition();

    if(not this->currentClass())
        return;

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
    this->ui->addItem->setHidden(false);
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

    this->abortItemAddition();

    QMessageBox::information(this, "Addition", "Item was added");
}

void ClassesModule::initClassCreation()
{
    if(not this->tryConnect())
        return;

    auto error = this->personsModel->loadAll();
    this->connection->close();

    if(error.type != dbapi::ApiError::NoError)
        return this->showInternalError();

    if(this->classCreationDialog)
        delete this->classCreationDialog;

    this->classCreationDialog = new ClassCreationDialog(this);
    this->classCreationDialog->setModel(this->personsModel);

    connect(this->classCreationDialog, &QDialog::finished, this, &ClassesModule::completeClassCreation);

    this->classCreationDialog->show();
}

void ClassesModule::completeClassCreation()
{
    if(this->classCreationDialog->result() == QDialog::Rejected)
        return;

    if(not this->tryConnect())
        return;

    dbapi::Class grade(this->connection);
    grade.setName(this->classCreationDialog->name());

    if(not grade.store())
    {
        this->abortClassCreation();
        this->abortConnection();
        return;
    }

    dbapi::Classmate teacher(this->connection);

    auto person = this->personsModel->person(this->classCreationDialog->currentIndex());

    teacher.setKey({person->key()});
    teacher.setGrade(grade.key());

    if(not teacher.store())
    {
        this->abortClassCreation();

        grade.remove();

        this->abortConnection();
        return;
    }

    // add new class to widgets
    this->classesModel->insertRow(grade);
    this->classFinder->setCurrentIndex(this->classesModel->rowCount());

    // add teacher to widgets
    this->teacherFinder->setCurrentIndex(this->classCreationDialog->currentIndex().row());

    this->connection->close();

    this->classDeletionAction->setDisabled(false);

    QMessageBox::information(this, "Creation", "The class was created");
}

void ClassesModule::abortClassCreation()
{

}

dbapi::Class *ClassesModule::currentClass()
{
    int index = this->classFinder->currentIndex();

    if(index < 0)
        return nullptr;

    return this->classesModel->grade(index);
}

dbapi::Person *ClassesModule::currentTeacher()
{
    int index = this->teacherFinder->currentIndex();

    if(index < 0)
        return nullptr;

    return this->personsModel->person(index);
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
    this->ui->subjectsList->setSelectionMode(QAbstractItemView::ExtendedSelection);

    // handle click
    connect(this->ui->subjectsList, &QListView::doubleClicked, this, &ClassesModule::handleClickedItem);

    // handle selection
    connect(this->ui->subjectsList->selectionModel(), &QItemSelectionModel::selectionChanged, this, &ClassesModule::handleSelectedItems);

    this->ui->studentsList->setModel(this->classSubjectsModel);
    this->ui->studentsList->setSelectionBehavior(QAbstractItemView::SelectRows);
    this->ui->studentsList->setSelectionMode(QAbstractItemView::ExtendedSelection);

    // handle click
    connect(this->ui->subjectsList, &QListView::doubleClicked, this, &ClassesModule::handleClickedItem);

    // handle selection
    connect(this->ui->subjectsList->selectionModel(), &QItemSelectionModel::selectionChanged, this, &ClassesModule::handleSelectedItems);
}

void ClassesModule::setupToolBar()
{
    this->ui->itemAdditionMenu->hide();
    this->ui->deleteItem->hide();
    this->ui->addItem->hide();

    // handle item deletion
    connect(this->ui->deleteItem, &QPushButton::clicked, this, &ClassesModule::handleItemsDeletion);

    // handle item addition
    connect(this->ui->addItem, &QPushButton::clicked, this, &ClassesModule::initItemAddition);

    this->classCreationAction = new QAction("Create class");
    connect(this->classCreationAction, &QAction::triggered, this, &ClassesModule::initClassCreation);

    this->classDeletionAction = new QAction("Delete class");
    this->classDeletionAction->setDisabled(true);
    connect(this->classDeletionAction, &QAction::triggered, this, &ClassesModule::handleClassDeletion);

    this->menu = new QMenu(this);
    this->menu->addAction(this->classCreationAction);
    this->menu->addAction(this->classDeletionAction);

    this->ui->toolButton->setMenu(this->menu);
    this->ui->toolButton->setPopupMode(QToolButton::MenuButtonPopup);
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

void ClassesModule::abortConnection()
{
    this->connection->close();
    this->showInternalError();
}

bool ClassesModule::loadHomeroomTeacher()
{
    if(not this->currentClass())
        return false;

    dbapi::ApiError error;
    auto teachers = dbapi::Classmate::loadAll(this->connection, &error);

    if(error.type != dbapi::ApiError::NoError)
        return false;

    auto grade = this->classesModel->grade(this->classFinder->currentIndex());
    dbapi::Person::Key personKey;

    // find homeroom teacher and extract person key
    for(auto teacher : teachers)
    {
        if(teacher->grade() == grade->key())
            personKey = teacher->key().person;

        delete teacher;
    }

    // load all persons
    error = this->personsModel->loadAll();

    if(error.type != dbapi::ApiError::NoError)
        return false;

    // match found person with a person in the model
    for(int index = 0; index < this->personsModel->rowCount(); index++)
    {
        if(this->personsModel->person(index)->key() == personKey)
            this->teacherFinder->setCurrentIndex(index);
    }

    return true;
}

bool ClassesModule::loadStudentsList()
{
    if(not this->currentClass())
        return false;

    auto grade = this->classesModel->grade(this->classFinder->currentIndex());
    auto error = this->classStudentsModel->loadStudents(grade->key());

    if(error.type != dbapi::ApiError::NoError)
        return false;

    return true;
}

bool ClassesModule::loadSubjectsList()
{
    if(not this->currentClass())
        return false;

    auto grade = this->classesModel->grade(this->classFinder->currentIndex());
    auto error = this->classSubjectsModel->loadSubjects(grade->key());

    if(error.type != dbapi::ApiError::NoError)
        return false;

    return true;
}

void ClassesModule::strictGuiForUnselectedClass()
{
    this->teacherFinder->hide();

    this->strictGuiForNoItemAddition();
    this->strictGuiForNoItemDelition();

    this->classDeletionAction->setDisabled(true);
}

void ClassesModule::strictGuiForNoItemAddition()
{
    this->ui->itemAdditionMenu->hide();
    this->ui->addItem->hide();
}

void ClassesModule::strictGuiForNoItemDelition()
{
    this->ui->deleteItem->hide();
}
