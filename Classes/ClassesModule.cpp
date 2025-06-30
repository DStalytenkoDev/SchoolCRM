#include <QMessageBox>
#include <qmenu.h>
#include <QSignalTransition>
#include <SchoolApi/Classmate.h>
#include "ClassesModule.h"
#include "ui_ClassesModule.h"
#include "../TransitionFactrory.h"


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
    this->setupStateMachine();
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
    // reset state machine
    if(not this->stateMachine->isRunning())
        this->stateMachine->start();

    emit this->reseted();
}

ClassesModule::~ClassesModule()
{
    delete ui;
}

void ClassesModule::enterClassesNotLoaded()
{
    this->classFinder->setDisabled(true);
    this->classDeletionAction->setDisabled(true);

    this->teacherFinder->hide();
    this->ui->itemAdditionMenu->hide();
    this->ui->addItem->hide();
    this->ui->deleteItem->hide();
    this->ui->tabWidget->hide();

    this->ui->verticalSpacer->changeSize(0, 10, QSizePolicy::Preferred, QSizePolicy::MinimumExpanding);

    if(not this->tryConnect())
        return;

    auto error = this->classesModel->loadAll();
    this->connection->close();

    if(error.type != dbapi::ApiError::NoError)
        return this->showInternalError();

    emit this->classesLoadedAre();
}

void ClassesModule::enterClassesLoaded()
{
    this->classDeletionAction->setDisabled(true);

    this->teacherFinder->hide();
    this->ui->itemAdditionMenu->hide();
    this->ui->addItem->hide();
    this->ui->deleteItem->hide();
    this->ui->tabWidget->hide();

    this->ui->verticalSpacer->changeSize(0, 10, QSizePolicy::Preferred, QSizePolicy::MinimumExpanding);

    this->classFinder->setCurrentIndex(-1);
    this->classFinder->setDisabled(false);
}

void ClassesModule::enterClassSelected()
{
    if(not this->tryConnect())
    {
        emit this->dataError();
        return this->abortConnection();
    }

    if(not this->loadHomeroomTeacher())
    {
        emit this->dataError();
        return this->abortConnection();
    }

    if(not this->loadStudentsList())
    {
        emit this->dataError();
        return this->abortConnection();
    }

    if(not this->loadSubjectsList())
    {
        emit this->dataError();
        return this->abortConnection();
    }

    // set gui
    this->ui->itemAdditionMenu->hide();
    this->ui->deleteItem->hide();

    this->classFinder->setDisabled(false);
    this->teacherFinder->setHidden(false);

    this->classDeletionAction->setDisabled(false);
    this->ui->addItem->setHidden(false);
    this->ui->tabWidget->setHidden(false);

    this->ui->verticalSpacer->changeSize(0, 10);

    this->connection->close();
}

void ClassesModule::enterItemAdding()
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
    {
        emit this->dataError();
        return this->showInternalError();
    }

    this->classDeletionAction->setDisabled(true);
    this->ui->itemAdditionMenu->setHidden(false);
    this->ui->addItem->hide();
}

void ClassesModule::enterStudentsSelected()
{
    this->ui->deleteItem->setHidden(false);
    this->ui->addItem->hide();

    this->classDeletionAction->setDisabled(true);
}

void ClassesModule::enterSubjectsSelected()
{
    this->ui->deleteItem->setHidden(false);
    this->ui->addItem->hide();

    this->classDeletionAction->setDisabled(true);
}

void ClassesModule::handleSelectedSubjects()
{
    auto& selection = this->ui->subjectsList->selectionModel()->selection();

    if(selection.empty())
        emit this->itemsDeselectedAre();
    else
        emit this->subjectsSelectedAre();
}

void ClassesModule::handleSelectedStudents()
{
    auto& selection = this->ui->studentsList->selectionModel()->selection();

    if(selection.empty())
        emit this->itemsDeselectedAre();
    else
        emit this->studentsSelectedAre();
}

void ClassesModule::handleSubjectsDeletion()
{
    if(not this->tryConnect())
        return;

    if(not this->deleteSubjects())
        return this->abortConnection();

    this->connection->close();

    QMessageBox::information(this, "Deletion", "All selected items had been deleted");
}

void ClassesModule::handleStudentsDeletion()
{
    if(not this->tryConnect())
        return;

    if(not this->deleteStudents())
        return this->abortConnection();

    this->connection->close();

    QMessageBox::information(this, "Deletion", "All selected items had been deleted");
}

void ClassesModule::handleSelectedTeacher()
{
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

    this->connection->transaction();

    // delete old teacher
    bool teacherDeleted = foundOldTeacher->remove();
    delete foundOldTeacher;

    // create new teacher
    dbapi::Classmate teacher({newTeacher->key()}, this->connection);
    teacher.setGrade(grade->key());

    bool teacherStored = teacher.store();

    if(teacherDeleted and teacherStored)
    {
        this->connection->commit();
        this->connection->close();
        QMessageBox::information(this, "Change", "Teacher was changed");
        return;
    }

    this->connection->rollback();
    this->loadHomeroomTeacher();
    this->abortConnection();
}

void ClassesModule::handleChangedTab()
{
    this->ui->subjectsList->clearSelection();
    this->ui->studentsList->clearSelection();
}

void ClassesModule::handleClassDeleting()
{
    if(not this->tryConnect())
        return;

    this->connection->close();
}

bool ClassesModule::deleteSubjects()
{
    dbapi::ClassSubjectsList list({this->currentClass()->key()}, this->connection);

    for(QModelIndex& index : this->ui->subjectsList->selectionModel()->selectedIndexes())
    {
        auto key = this->classSubjectsModel->subject(index);
        list.appendSubject(key);
    }

    bool deleted = list.remove();

    if(not deleted)
        return false;

    return true;
}

bool ClassesModule::deleteStudents()
{
    dbapi::Student student(this->connection);
    bool failFlag = false;

    for(QModelIndex& index : this->ui->studentsList->selectionModel()->selectedIndexes())
    {
        auto person = this->classStudentsModel->student(index);

        student.setKey({person->key()});

        if(not student.remove())
            failFlag = true;
    }

    if(failFlag)
        return false;

    return true;
}

void ClassesModule::completeItemAddition()
{
    if(not this->tryConnect())
        return;

    this->connection->transaction();

    bool added = false;
    auto grade = this->currentClass();

    // decide weather we add subjects or students
    if(this->itemForAdditionFinder->model() == this->personsModel)
    {
        // add student

        auto person = this->personsModel->person(this->itemForAdditionFinder->currentIndex());
        dbapi::Student student({person->key()}, this->connection);
        student.setGrade(grade->key());

        added = student.store();
    }
    else
    {
        // add subject

        auto subject = this->subjectsModel->subject(this->itemForAdditionFinder->currentIndex());

        dbapi::ClassSubjectsList list({grade->key()}, this->connection);
        list.appendSubject(subject->key());

        added = list.update();
    }

    if(not added)
    {
        this->connection->rollback();
        return this->abortConnection();
    }

    this->connection->commit();
    this->connection->close();

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
    this->teacherFinder->hide();
}

void ClassesModule::completeClassCreation()
{
    if(this->classCreationDialog->result() == QDialog::Rejected)
        return;

    if(not this->tryConnect())
        return;


    this->connection->close();
    emit this->reseted();
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

    this->teacherFinder = new ComboBoxFinderView(this);
    this->teacherFinder->setModel(this->personsModel);
    this->ui->menuLayout->insertWidget(1, this->teacherFinder);

    this->itemForAdditionFinder = new ComboBoxFinderView(this);
    this->ui->additionMenuLayout->insertWidget(0, this->itemForAdditionFinder);
}

void ClassesModule::setupLists()
{
    this->ui->subjectsList->setModel(this->classSubjectsModel);
    this->ui->subjectsList->setSelectionBehavior(QAbstractItemView::SelectRows);
    this->ui->subjectsList->setSelectionMode(QAbstractItemView::ExtendedSelection);

    // handle selection
    connect(this->ui->subjectsList->selectionModel(), &QItemSelectionModel::selectionChanged, this, &ClassesModule::handleSelectedSubjects);

    this->ui->studentsList->setModel(this->classStudentsModel);
    this->ui->studentsList->setSelectionBehavior(QAbstractItemView::SelectRows);
    this->ui->studentsList->setSelectionMode(QAbstractItemView::ExtendedSelection);

    // handle selection
    connect(this->ui->studentsList->selectionModel(), &QItemSelectionModel::selectionChanged, this, &ClassesModule::handleSelectedStudents);

    // handle tab change
    connect(this->ui->tabWidget, &QTabWidget::currentChanged, this, &ClassesModule::handleChangedTab);

}

void ClassesModule::setupToolBar()
{
    this->classCreationAction = new QAction("Create class");
    this->classDeletionAction = new QAction("Delete class");

    this->menu = new QMenu(this);
    this->menu->addAction(this->classCreationAction);
    this->menu->addAction(this->classDeletionAction);

    this->ui->toolButton->setMenu(this->menu);
    this->ui->toolButton->setPopupMode(QToolButton::MenuButtonPopup);

    // handle selected teacher
    connect(this->teacherFinder, &ComboBoxFinderView::foundItem, this, &ClassesModule::handleSelectedTeacher);

    // handle class creation
    connect(this->classCreationAction, &QAction::triggered, this, &ClassesModule::initClassCreation);

    // handle class deletion
    connect(this->classDeletionAction, &QAction::triggered, this, &ClassesModule::handleClassDeleting);

    // handle item adding
    connect(this->ui->confirmItemAddition, &QPushButton::clicked, this, &ClassesModule::completeItemAddition);
}

void ClassesModule::setupStateMachine()
{
    this->stateMachine = new QStateMachine(this);
    this->resetGroupState = new QState();

    this->classesNotLoaded = new QState(this->resetGroupState);
    this->classesLoaded = new QState(this->resetGroupState);
    this->classSelected = new QState(this->resetGroupState);
    this->itemAdding = new QState(this->resetGroupState);
    this->subjectsSelected = new QState(this->resetGroupState);
    this->studentsSelected = new QState(this->resetGroupState);

    connect(this->classesNotLoaded, &QState::entered, this, &ClassesModule::enterClassesNotLoaded);
    this->classesNotLoaded->addTransition(this, &ClassesModule::classesLoadedAre, this->classesLoaded);

    connect(this->classesLoaded, &QState::entered, this, &ClassesModule::enterClassesLoaded);
    this->classesLoaded->addTransition(this->classFinder, &ComboBoxFinderView::foundItem, this->classSelected);

    connect(this->classSelected, &QState::entered, this, &ClassesModule::enterClassSelected);
    this->classSelected->addTransition(this->ui->addItem, &QPushButton::clicked, this->itemAdding);
    this->classSelected->addTransition(this, &ClassesModule::studentsSelectedAre, this->studentsSelected);
    this->classSelected->addTransition(this, &ClassesModule::subjectsSelectedAre, this->subjectsSelected);
    this->classSelected->addTransition(this, &ClassesModule::classDeletedIs, this->classesNotLoaded);
    this->classSelected->addTransition(this, &ClassesModule::dataError, this->classesNotLoaded);
    this->classSelected->addTransition(this->classFinder, &ComboBoxFinderView::foundItem, this->classSelected);

    connect(this->itemAdding, &QState::entered, this, &ClassesModule::enterItemAdding);
    this->itemAdding->addTransition(this->ui->confirmItemAddition, &QPushButton::clicked, this->classSelected);
    this->itemAdding->addTransition(this->ui->abortItemAddition, &QPushButton::clicked, this->classSelected);
    this->itemAdding->addTransition(this->classFinder, &ComboBoxFinderView::foundItem, this->classSelected);
    this->itemAdding->addTransition(this, &ClassesModule::dataError, this->classSelected);

    connect(this->studentsSelected, &QState::entered, this, &ClassesModule::enterStudentsSelected);
    this->studentsSelected->addTransition(this, &ClassesModule::itemsDeselectedAre, this->classSelected);
    this->studentsSelected->addTransition(this->classFinder, &ComboBoxFinderView::foundItem, this->classSelected);
    transition(this->ui->deleteItem, &QPushButton::clicked, this, &ClassesModule::handleStudentsDeletion, this->studentsSelected, this->classSelected);

    connect(this->subjectsSelected, &QState::entered, this, &ClassesModule::enterSubjectsSelected);
    this->subjectsSelected->addTransition(this, &ClassesModule::itemsDeselectedAre, this->classSelected);
    this->subjectsSelected->addTransition(this->classFinder, &ComboBoxFinderView::foundItem, this->classSelected);
    transition(this->ui->deleteItem, &QPushButton::clicked, this, &ClassesModule::handleSubjectsDeletion, this->subjectsSelected, this->classSelected);

    this->resetGroupState->setInitialState(this->classesNotLoaded);
    this->resetGroupState->addTransition(this, &ClassesModule::reseted, this->classesNotLoaded);

    this->stateMachine->addState(this->resetGroupState);
    this->stateMachine->setInitialState(this->resetGroupState);
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
    auto grade = this->classesModel->grade(this->classFinder->currentIndex());
    auto error = this->classStudentsModel->loadStudents(grade->key());

    if(error.type != dbapi::ApiError::NoError)
        return false;

    return true;
}

bool ClassesModule::loadSubjectsList()
{
    auto grade = this->classesModel->grade(this->classFinder->currentIndex());
    auto error = this->classSubjectsModel->loadSubjects(grade->key());

    if(error.type != dbapi::ApiError::NoError)
        return false;

    return true;
}

//void ClassesModule::resetUi()
//{
//    this->classFinder->setDisabled(true);
//    this->classDeletionAction->setDisabled(true);
//
//    this->teacherFinder->hide();
//    this->ui->itemAdditionMenu->hide();
//    this->ui->addItem->hide();
//    this->ui->deleteItem->hide();
//    this->ui->tabWidget->hide();
//
//    this->ui->verticalSpacer->changeSize(0, 10, QSizePolicy::Preferred, QSizePolicy::MinimumExpanding);
//}
