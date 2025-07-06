#include <QMessageBox>
#include <QMenu>
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

    if(error.isError())
        return error.show(this);

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
    this->classStudentsModel->setClass(this->currentClass()->key());
    this->classSubjectsModel->setClass(this->currentClass()->key());

    if(not this->tryConnect())
    {
        emit this->dataError();
        return;
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

    this->connection->close();

    // set gui
    this->ui->itemAdditionMenu->hide();
    this->ui->deleteItem->hide();

    this->classFinder->setDisabled(false);
    this->teacherFinder->setHidden(false);

    this->classDeletionAction->setDisabled(false);
    this->ui->addItem->setHidden(false);
    this->ui->tabWidget->setHidden(false);

    this->ui->verticalSpacer->changeSize(0, 10);
}

void ClassesModule::enterItemAdding()
{
    if(not this->tryConnect())
        return;

    UserError error;

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

    if(error.isError())
    {
        emit this->dataError();
        return this->showInternalError();
    }

    this->classDeletionAction->setDisabled(true);
    this->ui->itemAdditionMenu->setHidden(false);
    this->ui->addItem->hide();
}

void ClassesModule::enterItemSelected()
{
    this->ui->deleteItem->setHidden(false);
    this->ui->addItem->hide();

    this->classDeletionAction->setDisabled(true);
}

void ClassesModule::handleSelectedSubject()
{
    auto& selection = this->ui->subjectsList->selectionModel()->selection();

    if(selection.empty())
        emit this->itemDeselectedIs();
    else
        emit this->subjectSelectedIs();
}

void ClassesModule::handleSelectedStudent()
{
    auto& selection = this->ui->studentsList->selectionModel()->selection();

    if(selection.empty())
        emit this->itemDeselectedIs();
    else
        emit this->studentSelectedIs();
}

void ClassesModule::handleSubjectDeletion()
{
    if(not this->tryConnect())
        return;

    auto error = this->classSubjectsModel->removeSubject(this->ui->subjectsList->currentIndex().row());
    this->connection->close();

    if(error.isError())
        return error.show(this);

    QMessageBox::information(this, "Info", "All selected items had been deleted");
}

void ClassesModule::handleStudentDeletion()
{
    if(not this->tryConnect())
        return;

    auto error = this->classStudentsModel->removeStudent(this->ui->studentsList->currentIndex().row());
    this->connection->close();

    if(error.isError())
        return error.show(this);

    QMessageBox::information(this, "Info", "All selected items had been deleted");
}

void ClassesModule::handleSelectedTeacher()
{
    if(not this->tryConnect())
        return;

    auto error = this->classesModel->changeHomeroomTeacher(this->classFinder->currentIndex(),
                                                           this->currentTeacher()->key());
    this->connection->close();

    if(error.isError())
        error.show(this);

    QMessageBox::information(this, "Info", "Teacher was changed");
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

    auto error = this->classesModel->removeClass(this->classFinder->currentIndex());
    this->connection->close();

    if(error.isError())
        return error.show(this);

    emit this->classDeletedIs();
    QMessageBox::information(this, "Info", "Class was deleted");
}

void ClassesModule::completeItemAddition()
{
    if(not this->tryConnect())
        return;

    UserError error;

    // decide weather we add subjects or students
    if(this->itemForAdditionFinder->model() == this->personsModel)
    {
        // add student

        auto person = this->personsModel->person(this->itemForAdditionFinder->currentIndex());
        error = this->classStudentsModel->appendStudent(person->key());
    }
    else
    {
        // add subject

        auto subject = this->subjectsModel->subject(this->itemForAdditionFinder->currentIndex());
        error = this->classSubjectsModel->appendSubject(subject->key());
    }

    this->connection->close();

    if(error.isError())
        return error.show(this);

    QMessageBox::information(this, "Info", "Item was added");
}

void ClassesModule::initClassCreation()
{
    if(not this->tryConnect())
        return;

    auto error = this->personsModel->loadAll();
    this->connection->close();

    if(error.isError())
        return UserError::internalError("Subdata", "be loadded in order to create class", "Try again or contact support").show(this);

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

    auto teacher = this->personsModel->person(this->classCreationDialog->currentIndex());

    auto error = this->classesModel->createClass(this->classCreationDialog->name(),
                                    teacher->key());
    this->connection->close();

    if(error.isError())
        return error.show(this);

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
    this->ui->subjectsList->setSelectionMode(QAbstractItemView::SingleSelection);

    // handle selection
    connect(this->ui->subjectsList->selectionModel(), &QItemSelectionModel::selectionChanged, this, &ClassesModule::handleSelectedSubject);

    this->ui->studentsList->setModel(this->classStudentsModel);
    this->ui->studentsList->setSelectionBehavior(QAbstractItemView::SelectRows);
    this->ui->studentsList->setSelectionMode(QAbstractItemView::SingleSelection);

    // handle selection
    connect(this->ui->studentsList->selectionModel(), &QItemSelectionModel::selectionChanged, this, &ClassesModule::handleSelectedStudent);

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
    this->resetGroup= new QState();

    this->classesNotLoaded = new QState(this->resetGroup);
    this->classesLoaded = new QState(this->resetGroup);
    this->classSelected = new QState(this->resetGroup);
    this->itemAdding = new QState(this->resetGroup);
    this->subjectSelected = new QState(this->resetGroup);
    this->studentSelected = new QState(this->resetGroup);

    connect(this->classesNotLoaded, &QState::entered, this, &ClassesModule::enterClassesNotLoaded);
    this->classesNotLoaded->addTransition(this, &ClassesModule::classesLoadedAre, this->classesLoaded);

    connect(this->classesLoaded, &QState::entered, this, &ClassesModule::enterClassesLoaded);
    this->classesLoaded->addTransition(this->classFinder, &ComboBoxFinderView::foundItem, this->classSelected);

    connect(this->classSelected, &QState::entered, this, &ClassesModule::enterClassSelected);
    this->classSelected->addTransition(this->ui->addItem, &QPushButton::clicked, this->itemAdding);
    this->classSelected->addTransition(this, &ClassesModule::studentSelectedIs, this->studentSelected);
    this->classSelected->addTransition(this, &ClassesModule::subjectSelectedIs, this->subjectSelected);
    this->classSelected->addTransition(this, &ClassesModule::classDeletedIs, this->classesNotLoaded);
    this->classSelected->addTransition(this, &ClassesModule::dataError, this->classesNotLoaded);
    this->classSelected->addTransition(this->classFinder, &ComboBoxFinderView::foundItem, this->classSelected);

    connect(this->itemAdding, &QState::entered, this, &ClassesModule::enterItemAdding);
    this->itemAdding->addTransition(this->ui->confirmItemAddition, &QPushButton::clicked, this->classSelected);
    this->itemAdding->addTransition(this->ui->abortItemAddition, &QPushButton::clicked, this->classSelected);
    this->itemAdding->addTransition(this->classFinder, &ComboBoxFinderView::foundItem, this->classSelected);
    this->itemAdding->addTransition(this, &ClassesModule::dataError, this->classSelected);

    connect(this->studentSelected, &QState::entered, this, &ClassesModule::enterItemSelected);
    this->studentSelected->addTransition(this, &ClassesModule::itemDeselectedIs, this->classSelected);
    this->studentSelected->addTransition(this->classFinder, &ComboBoxFinderView::foundItem, this->classSelected);
    transition(this->ui->deleteItem, &QPushButton::clicked, this, &ClassesModule::handleStudentDeletion, this->studentSelected, this->classSelected);

    connect(this->subjectSelected, &QState::entered, this, &ClassesModule::enterItemSelected);
    this->subjectSelected->addTransition(this, &ClassesModule::itemDeselectedIs, this->classSelected);
    this->subjectSelected->addTransition(this->classFinder, &ComboBoxFinderView::foundItem, this->classSelected);
    transition(this->ui->deleteItem, &QPushButton::clicked, this, &ClassesModule::handleSubjectDeletion, this->subjectSelected, this->classSelected);

    this->resetGroup->setInitialState(this->classesNotLoaded);
    this->resetGroup->addTransition(this, &ClassesModule::reseted, this->classesNotLoaded);

    this->stateMachine->addState(this->resetGroup);
    this->stateMachine->setInitialState(this->resetGroup);
}

bool ClassesModule::tryConnect()
{
    if(this->connection->open())
        return true;

    UserError::connectionError("Connection", "be established 'cause something went wrong", "Check credentials or internet connection, server might be down").show(this);

    return false;
}

void ClassesModule::showInternalError()
{
    UserError::internalError("Command", "be executed 'cause something went wrong", "Try again or contact support").show(this);
}

void ClassesModule::abortConnection()
{
    this->connection->close();
    this->showInternalError();
}

bool ClassesModule::loadHomeroomTeacher()
{
    auto teacherKey = this->classesModel->getHomeroomTeacher(this->classFinder->currentIndex());

    if(teacherKey.index() == 1)
        return false;

    auto error = this->personsModel->loadAll();

    if(error.isError())
        return false;

    // match found person with a person in the model
    for(int index = 0; index < this->personsModel->rowCount(); index++)
    {
        if(this->personsModel->person(index)->key() == std::get<dbapi::Person::Key>(teacherKey))
            this->teacherFinder->setCurrentIndex(index);
    }

    return true;
}

bool ClassesModule::loadStudentsList()
{
    auto error = this->classStudentsModel->loadAll();

    if(error.isError())
        return false;

    return true;
}

bool ClassesModule::loadSubjectsList()
{
    auto error = this->classSubjectsModel->loadAll();

    if(error.isError())
        return false;

    return true;
}
