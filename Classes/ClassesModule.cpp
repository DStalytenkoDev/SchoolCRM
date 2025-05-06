#include <QMessageBox>
#include <qmenu.h>
#include <QSignalTransition>
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

    emit this->goInClassesNotLoaded();
}

ClassesModule::~ClassesModule()
{
    delete ui;
}

void ClassesModule::enterToClassesNotLoaded()
{
    this->resetUi();

    if(not this->tryConnect())
        return;

    auto error = this->classesModel->loadAll();
    this->connection->close();

    if(error.type != dbapi::ApiError::NoError)
        return this->showInternalError();

    emit this->goInClassesLoaded();
}

void ClassesModule::enterToClassesLoaded()
{
    this->resetUi();

    this->classFinder->setCurrentIndex(-1);
    this->classFinder->setDisabled(false);
}

void ClassesModule::enterToClassSelected()
{
    this->resetUi();

    if(not this->tryConnect())
    {
        emit this->goInClassesLoaded();
        return this->abortConnection();
    }

    if(not this->loadHomeroomTeacher())
    {
        emit this->goInClassesLoaded();
        return this->abortConnection();
    }

    if(not this->loadStudentsList())
    {
        emit this->goInClassesLoaded();
        return this->abortConnection();
    }

    if(not this->loadSubjectsList())
    {
        emit this->goInClassesLoaded();
        return this->abortConnection();
    }

    // set gui
    this->classFinder->setDisabled(false);
    this->teacherFinder->setHidden(false);

    this->classDeletionAction->setDisabled(false);
    this->ui->addItem->setHidden(false);
    this->ui->tabWidget->setHidden(false);

    this->ui->verticalSpacer->changeSize(0, 10);

    this->connection->close();
}

void ClassesModule::enterToInItemDelition()
{
    this->ui->deleteItem->setHidden(false);
    this->ui->addItem->hide();
}

void ClassesModule::enterToInTeacherChanging()
{
    if(not this->tryConnect())
        return;

    emit this->goInClassSelected();

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

void ClassesModule::enterToInClassDelition()
{
    if(not this->tryConnect())
        return;

    this->connection->transaction();

    auto grade = this->currentClass();

    // delete class subjects
    dbapi::ClassSubjectsList subjectsList({grade->key()}, this->connection);
    subjectsList.load();
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
    bool classDeleted = grade->remove();

    if(not classDeleted)
    {
        this->connection->rollback();
        this->showInternalError();
        emit this->goInClassSelected();
    }
    else
    {
        this->connection->commit();
        QMessageBox::information(this, "Delition", "Class was deleted");
        emit this->goInClassesNotLoaded();
    }

    this->connection->close();
}

void ClassesModule::handleSelectedSubjects(const QItemSelection &selected, const QItemSelection &deselected)
{
    if(selected.empty())
        emit this->goInClassSelected();
    else
        emit this->goInInSubjectsDeletion();
}

void ClassesModule::handleSelectedStudents(const QItemSelection &selected, const QItemSelection &deselected)
{
    if(selected.empty())
        emit this->goInClassSelected();
    else
        emit this->goInInStudentsDeletion();
}

void ClassesModule::handleSubjectsDeletion()
{
    if(not this->tryConnect())
        return;

    emit this->goInClassSelected();

    if(not this->deleteSubjects())
        return this->abortConnection();

    this->connection->close();

    QMessageBox::information(this, "Deletion", "All selected items had been deleted");
}

void ClassesModule::handleStudentsDeletion()
{
    if(not this->tryConnect())
        return;

    emit this->goInClassSelected();

    if(not this->deleteStudents())
        return this->abortConnection();

    this->connection->close();

    QMessageBox::information(this, "Deletion", "All selected items had been deleted");
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
    {
        emit this->goInClassSelected();
        return this->showInternalError();
    }

    this->ui->itemAdditionMenu->setHidden(false);
    this->ui->addItem->hide();
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

    emit this->goInClassSelected();

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
    {
        if(this->currentClass())
            emit this->goInClassSelected();
        else
            emit this->goInClassesNotLoaded();

        return this->showInternalError();
    }

    if(this->classCreationDialog)
        delete this->classCreationDialog;

    this->classCreationDialog = new ClassCreationDialog(this);
    this->classCreationDialog->setModel(this->personsModel);

    connect(this->classCreationDialog, &QDialog::finished, this, &ClassesModule::completeClassCreation);

    this->resetUi();
    this->classCreationDialog->show();
}

void ClassesModule::completeClassCreation()
{
    if(this->classCreationDialog->result() == QDialog::Rejected)
        return this->abortClassCreation();

    if(not this->tryConnect())
        return this->abortClassCreation();

    this->connection->transaction();

    dbapi::Class grade(this->connection);
    grade.setName(this->classCreationDialog->name());
    bool gradeStored = grade.store();

    dbapi::Classmate teacher(this->connection);

    auto person = this->personsModel->person(this->classCreationDialog->currentIndex());

    teacher.setKey({person->key()});
    teacher.setGrade(grade.key());
    bool teacherStored = teacher.store();

    if(teacherStored and gradeStored)
    {
        this->connection->commit();

        // add new class to widgets
        this->classesModel->insertRow(grade);
        this->classFinder->setCurrentIndex(this->classesModel->rowCount() - 1);

        // add teacher to widgets
        this->teacherFinder->setCurrentIndex(this->classCreationDialog->currentIndex().row());

        emit this->goInClassSelected();
        QMessageBox::information(this, "Creation", "The class was created");
    }
    else
    {
        this->connection->rollback();
        this->showInternalError();
        this->abortClassCreation();
    }

    this->connection->close();
}

void ClassesModule::abortClassCreation()
{
    if(this->currentClass())
        emit this->goInClassSelected();
    else
        emit this->goInClassesNotLoaded();
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

    // handle item addition confirm
    connect(this->ui->confirmItemAddition, &QPushButton::clicked, this, &ClassesModule::completeItemAddition);
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
}

void ClassesModule::setupStateMachine()
{
    this->stateMachine = new QStateMachine(this);
    this->resetGroupState = new QState();

    this->classesNotLoaded = new QState(this->resetGroupState);
    this->classesLoaded = new QState(this->resetGroupState);
    this->inClassCreation = new QState(this->resetGroupState);
    this->classSelected = new QState(this->resetGroupState);
    this->inItemAddition = new QState(this->resetGroupState);
    this->inSubjectsDeletion = new QState(this->resetGroupState);
    this->inStudentsDeletion = new QState(this->resetGroupState);
    this->inTeacherChanging = new QState(this->resetGroupState);
    this->inClassDelition = new QState(this->resetGroupState);

    this->classesNotLoaded->addTransition(this, &ClassesModule::goInClassesLoaded, this->classesLoaded); // manual
    this->classesNotLoaded->addTransition(this->classCreationAction, &QAction::triggered, this->inClassCreation);
    connect(this->classesNotLoaded, &QState::entered, this, &ClassesModule::enterToClassesNotLoaded);

    this->classesLoaded->addTransition(this->classFinder, &ComboBoxFinderView::foundItem, this->classSelected);
    this->classesLoaded->addTransition(this->classCreationAction, &QAction::triggered, this->inClassCreation);
    connect(this->classesLoaded, &QState::entered, this, &ClassesModule::enterToClassesLoaded);

    this->inClassCreation->addTransition(this, &ClassesModule::goInClassSelected, this->classSelected); // manual
    this->inClassCreation->addTransition(this, &ClassesModule::goInClassesNotLoaded, this->classesNotLoaded); // manual
    connect(this->inClassCreation, &QState::entered, this, &ClassesModule::initClassCreation);

    this->classSelected->addTransition(this->ui->addItem, &QPushButton::clicked, this->inItemAddition);
    this->classSelected->addTransition(this, &ClassesModule::goInInStudentsDeletion, this->inStudentsDeletion); // manual
    this->classSelected->addTransition(this, &ClassesModule::goInInSubjectsDeletion, this->inSubjectsDeletion); // manual
    this->classSelected->addTransition(this->teacherFinder, &ComboBoxFinderView::foundItem, this->inTeacherChanging);
    this->classSelected->addTransition(this->classDeletionAction, &QAction::triggered, this->inClassDelition);
    this->classSelected->addTransition(this->classCreationAction, &QAction::triggered, this->inClassCreation);
    this->classSelected->addTransition(this, &ClassesModule::goInClassesLoaded, this->classesLoaded); // manual
    this->classSelected->addTransition(this->classFinder, &ComboBoxFinderView::foundItem, this->classSelected);
    connect(this->classSelected, &QState::entered, this, &ClassesModule::enterToClassSelected);

    this->inItemAddition->addTransition(this->ui->confirmItemAddition, &QPushButton::clicked, this->classSelected);
    this->inItemAddition->addTransition(this->ui->abortItemAddition, &QPushButton::clicked, this->classSelected);
    connect(this->inItemAddition, &QState::entered, this, &ClassesModule::initItemAddition);

    this->deleteStudentsTransition = new QSignalTransition(this->ui->deleteItem, &QPushButton::clicked); // targetless transition
    this->deleteSubjectsTransition = new QSignalTransition(this->ui->deleteItem, &QPushButton::clicked); // targetless transition

    connect(this->deleteStudentsTransition, &QSignalTransition::triggered, this, &ClassesModule::handleStudentsDeletion);
    connect(this->deleteSubjectsTransition, &QSignalTransition::triggered, this, &ClassesModule::handleSubjectsDeletion);

    this->inSubjectsDeletion->addTransition(this, &ClassesModule::goInClassSelected, this->classSelected); // manual
    this->inSubjectsDeletion->addTransition(this->deleteSubjectsTransition);
    connect(this->inSubjectsDeletion, &QState::entered, this, &ClassesModule::enterToInItemDelition);

    this->inStudentsDeletion->addTransition(this, &ClassesModule::goInClassSelected, this->classSelected); // manual
    this->inStudentsDeletion->addTransition(this->deleteStudentsTransition);
    connect(this->inStudentsDeletion, &QState::entered, this, &ClassesModule::enterToInItemDelition);

    this->inTeacherChanging->addTransition(this, &ClassesModule::goInClassSelected, this->classSelected); // manual
    connect(this->inTeacherChanging, &QState::entered, this, &ClassesModule::enterToInTeacherChanging);

    this->inClassDelition->addTransition(this, &ClassesModule::goInClassSelected, this->classSelected); // manual
    this->inClassDelition->addTransition(this, &ClassesModule::goInClassesNotLoaded, this->classesNotLoaded); // manual
    connect(this->inClassDelition, &QState::entered, this, &ClassesModule::enterToInClassDelition);

    this->resetGroupState->setInitialState(this->classesNotLoaded);
    this->resetGroupState->addTransition(this, &ClassesModule::goInClassesNotLoaded, this->classesNotLoaded);

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

void ClassesModule::resetUi()
{
    this->classFinder->setDisabled(true);
    this->classDeletionAction->setDisabled(true);

    this->teacherFinder->hide();
    this->ui->itemAdditionMenu->hide();
    this->ui->addItem->hide();
    this->ui->deleteItem->hide();
    this->ui->tabWidget->hide();

    this->ui->verticalSpacer->changeSize(0, 10, QSizePolicy::Preferred, QSizePolicy::MinimumExpanding);
}
