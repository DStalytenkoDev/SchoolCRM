#include <QMessageBox>
#include <SchoolApi/TeacherSubjectsList.h>
#include <qlabel.h>
#include <qsignaltransition.h>

#include "JournalModule.h"
#include "../TransitionFactrory.h"
#include "ui_JournalModule.h"


JournalModule::JournalModule(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::JournalModule)
{
    ui->setupUi(this);
    this->journalSpacer = new QSpacerItem(0, 0, QSizePolicy::Preferred, QSizePolicy::MinimumExpanding);

    this->journalModel = new JournalModel(this);
    this->classesModel = new ClassesModel(this);
    this->classStudentsModel = new ClassStudentsModel(this);
    this->classSubjectsModel = new ClassSubjectsModel(this);
    this->personsModel = new FakePersonsModel(this);

    this->markCreatingDialog = new MarkCreatingDialog(this);
    this->markCreatingDialog->setTeachersModel(this->personsModel);
    connect(this->markCreatingDialog, &QDialog::finished, this, &JournalModule::completeMarkCreating);

    this->setupFinders();
    this->setupJournal();
    this->setupToolBar();
    this->setupStateMachine();
}

void JournalModule::setConnection(dbapi::Connection *connection)
{
    this->connection = connection;

    this->journalModel->setConnection(connection);
    this->classesModel->setConnection(connection);
    this->classStudentsModel->setConnection(connection);
    this->classSubjectsModel->setConnection(connection);
}

void JournalModule::prepare()
{
    // reset state machine
    if(not this->stateMachine->isRunning())
        this->stateMachine->start();

    emit this->setClassesNotLoadedState();
}

JournalModule::~JournalModule()
{
    delete this->journalSpacer;
    delete ui;
}

void JournalModule::enterClassesNotLoadedState()
{
    this->classFinder->hide();
    this->subjectFinder->hide();
    this->markTypeFinder->hide();

    this->ui->beginDate->hide();
    this->ui->endDate->hide();

    this->ui->createMark->hide();
    this->ui->deleteMark->hide();
    this->ui->abortSelection->hide();

    this->ui->journal->hide();

    this->layout()->addItem(this->journalSpacer);

    if(not this->tryConnect())
        return;

    auto error = this->classesModel->loadAll();
    this->connection->close();

    if(error.isError())
        return error.show(this);

    emit this->setClassesLoadedState();
}

void JournalModule::enterClassesLoadedState()
{
    this->classFinder->setHidden(false);
    this->classFinder->setCurrentIndex(-1);
}

void JournalModule::enterClassSelectedState()
{
    this->classSubjectsModel->setClass(this->currentClass()->key());

    if(not this->tryConnect())
        return;

    auto error = this->classSubjectsModel->loadAll();
    this->connection->close();

    if(error.isError())
        return error.show(this);

    this->classFinder->setHidden(false);
    this->subjectFinder->setHidden(false);
    this->subjectFinder->setCurrentIndex(-1);

    this->markTypeFinder->setHidden(false);
    this->markTypeFinder->setCurrentIndex(0);

    this->ui->beginDate->setHidden(false);
    this->ui->beginDate->setDate(QDate::currentDate());
    this->ui->endDate->setHidden(false);
    this->ui->endDate->setDate(QDate::currentDate());

    emit this->setKeySelectedState();
}

void JournalModule::enterKeySelectedState()
{
    this->ui->journal->setHidden(false);
    this->layout()->removeItem(this->journalSpacer);

    this->ui->createMark->hide();
    this->ui->deleteMark->hide();
    this->ui->abortSelection->hide();
}

void JournalModule::enterEmptyCellSelectedState()
{
    this->ui->createMark->setHidden(false);
    this->ui->deleteMark->setHidden(true);
    this->ui->abortSelection->setHidden(false);
}

void JournalModule::enterRangeSelectedState()
{
    this->ui->createMark->setHidden(true);
    this->ui->deleteMark->setHidden(false);
    this->ui->abortSelection->setHidden(false);
}

void JournalModule::handleSelectedMarks()
{
    auto& selection = this->ui->journal->selectionModel()->selection();

    if(selection.empty())
    {
        emit this->setKeySelectedState();
        return;
    }

    auto indexes = selection.indexes();

    if(indexes.size() == 1 and this->journalModel->mark(indexes[0]) == nullptr)
        emit this->setEmptyCellSelectedState();
    else
        emit this->setRangeSelectedState();
}

void JournalModule::handleMarksDeleting()
{
    if(not this->tryConnect())
    {
        emit this->setKeySelectedState();
        return;
    }

    auto error = this->journalModel->remove(this->ui->journal->selectionModel()->selection());
    this->connection->close();

    if(error.isError())
    {
        emit this->setKeySelectedState();
        return error.show(this);
    }

    QMessageBox::information(this, "Info", "All selected item had been deleted");
    emit this->setKeySelectedState();
}

void JournalModule::handleJournalLoading()
{
    if(not this->tryConnect())
        return;

    auto subject = this->classSubjectsModel->subject(this->subjectFinder->currentIndex());

    this->journalModel->setClass(this->currentClass()->key());
    this->journalModel->setSubject(subject->key());
    this->journalModel->setType(this->markTypeFinder->getType());
    this->journalModel->setDateRange(this->ui->beginDate->date(), this->ui->endDate->date());

    auto error = this->journalModel->load();
    this->connection->close();

    if(error.isError())
        return error.show(this);
}

void JournalModule::handleJournalAndTeachersLoading()
{
    if(not this->tryConnect())
        return;

    auto subject = this->classSubjectsModel->subject(this->subjectFinder->currentIndex());

    this->journalModel->setClass(this->currentClass()->key());
    this->journalModel->setSubject(subject->key());
    this->journalModel->setType(this->markTypeFinder->getType());
    this->journalModel->setDateRange(this->ui->beginDate->date(), this->ui->endDate->date());

    auto error = this->journalModel->load();

    if(error.isError())
    {
        this->connection->close();
        return error.show(this);
    }

    if(not this->loadCompatibleTeachers())
    {
        this->connection->close();
        return UserError::internalError("Journal", "be loadded 'cause something went wrong", "Try again or contact support").show(this);
    }

    this->connection->close();
}

void JournalModule::initMarkCreating()
{
    this->markCreatingDialog->clear();
    this->markCreatingDialog->show();

    this->selectedMark = this->ui->journal->selectionModel()->currentIndex();
}

void JournalModule::completeMarkCreating()
{
    emit this->setKeySelectedState();

    if(this->markCreatingDialog->result() == QDialog::Rejected)
        return;

    auto teacherIndex = this->markCreatingDialog->currentTeacher();

    if(teacherIndex.row() < 0)
        return this->showInternalError();

    auto teacherKey = this->personsModel->person(teacherIndex.row());

    if(not this->tryConnect())
        return;

    auto error = this->journalModel->store(this->selectedMark, this->markCreatingDialog->value(), teacherKey);
    this->connection->close();

    if(error.isError())
        return error.show(this);

    QMessageBox::information(this, "Info", "New mark was created");
}

void JournalModule::setupFinders()
{
    this->classFinder = new ComboBoxFinderView(this);
    this->subjectFinder = new ComboBoxFinderView(this);
    this->markTypeFinder = new MarkTypeSelector(this);

    this->classFinder->setModel(this->classesModel);
    this->subjectFinder->setModel(this->classSubjectsModel);

    this->ui->menuLayout->insertWidget(0, this->classFinder);
    this->ui->menuLayout->insertWidget(1, this->subjectFinder);
    this->ui->menuLayout->insertWidget(2, this->markTypeFinder);
}


void JournalModule::setupJournal()
{
    this->ui->journal->setModel(this->journalModel);

    this->ui->journal->setSelectionBehavior(QAbstractItemView::SelectItems);
    this->ui->journal->setSelectionMode(QAbstractItemView::ExtendedSelection);

    connect(this->ui->journal->selectionModel(), &QItemSelectionModel::selectionChanged, this, &JournalModule::handleSelectedMarks);
}

void JournalModule::setupToolBar()
{
    connect(this->ui->createMark, &QPushButton::clicked, this, &JournalModule::initMarkCreating);
    connect(this->ui->deleteMark, &QPushButton::clicked, this, &JournalModule::handleMarksDeleting);
    connect(this->ui->abortSelection, &QPushButton::clicked, this->ui->journal, &QTableView::clearSelection);
}

void JournalModule::setupStateMachine()
{
    this->stateMachine = new QStateMachine(this);

    this->resetGroupState = new QState();

    this->classesNotLoaded = new QState(this->resetGroupState);
    this->classesLoaded = new QState(this->resetGroupState);
    this->classSelected = new QState(this->resetGroupState);
    this->keySelected = new QState(this->resetGroupState);
    this->emptyCellSelected = new QState(this->resetGroupState);
    this->rangeSelected = new QState(this->resetGroupState);

    this->classesNotLoaded->addTransition(this, &JournalModule::setClassesLoadedState, this->classesLoaded);
    connect(this->classesNotLoaded, &QState::entered, this, &JournalModule::enterClassesNotLoadedState);

    this->classesLoaded->addTransition(this->classFinder, &ComboBoxFinderView::foundItem, this->classSelected);
    connect(this->classesLoaded, &QState::entered, this, &JournalModule::enterClassesLoadedState);

    connect(this->classSelected, &QState::entered, this, &JournalModule::enterClassSelectedState);
    this->classSelected->addTransition(this->classFinder, &ComboBoxFinderView::foundItem, this->classSelected);
    transition(this->subjectFinder, &ComboBoxFinderView::foundItem, this, &JournalModule::handleJournalAndTeachersLoading, this->classSelected, this->keySelected);

    connect(this->keySelected, &QState::entered, this, &JournalModule::enterKeySelectedState);
    this->keySelected->addTransition(this->classFinder, &ComboBoxFinderView::foundItem, this->classSelected);
    this->keySelected->addTransition(this, &JournalModule::setEmptyCellSelectedState, this->emptyCellSelected);
    this->keySelected->addTransition(this, &JournalModule::setRangeSelectedState, this->rangeSelected);

    transition(this->ui->beginDate, &QDateEdit::userDateChanged, this, &JournalModule::handleJournalLoading, this->keySelected);
    transition(this->ui->endDate, &QDateEdit::userDateChanged, this, &JournalModule::handleJournalLoading, this->keySelected);
    transition(this->markTypeFinder, &MarkTypeSelector::typeChanged, this, &JournalModule::handleJournalLoading, this->keySelected);
    transition(this->subjectFinder, &ComboBoxFinderView::foundItem, this, &JournalModule::handleJournalAndTeachersLoading, this->keySelected);

    connect(this->emptyCellSelected, &QState::entered, this, &JournalModule::enterEmptyCellSelectedState);
    this->emptyCellSelected->addTransition(this, &JournalModule::setKeySelectedState, this->keySelected);
    this->emptyCellSelected->addTransition(this, &JournalModule::setRangeSelectedState, this->rangeSelected);
    this->emptyCellSelected->addTransition(this->classFinder, &ComboBoxFinderView::foundItem, this->classSelected);
    transition(this->ui->beginDate, &QDateEdit::userDateChanged, this, &JournalModule::handleJournalLoading, this->emptyCellSelected, this->keySelected);
    transition(this->ui->endDate, &QDateEdit::userDateChanged, this, &JournalModule::handleJournalLoading, this->emptyCellSelected, this->keySelected);
    transition(this->markTypeFinder, &MarkTypeSelector::typeChanged, this, &JournalModule::handleJournalLoading, this->emptyCellSelected, this->keySelected);
    transition(this->subjectFinder, &ComboBoxFinderView::foundItem, this, &JournalModule::handleJournalAndTeachersLoading, this->emptyCellSelected, this->keySelected);

    connect(this->rangeSelected, &QState::entered, this, &JournalModule::enterRangeSelectedState);
    this->rangeSelected->addTransition(this, &JournalModule::setKeySelectedState, this->keySelected);
    this->rangeSelected->addTransition(this, &JournalModule::setEmptyCellSelectedState, this->emptyCellSelected);
    this->rangeSelected->addTransition(this->classFinder, &ComboBoxFinderView::foundItem, this->classSelected);
    transition(this->ui->beginDate, &QDateEdit::userDateChanged, this, &JournalModule::handleJournalLoading, this->rangeSelected, this->keySelected);
    transition(this->ui->endDate, &QDateEdit::userDateChanged, this, &JournalModule::handleJournalLoading, this->rangeSelected, this->keySelected);
    transition(this->markTypeFinder, &MarkTypeSelector::typeChanged, this, &JournalModule::handleJournalLoading, this->rangeSelected, this->keySelected);
    transition(this->subjectFinder, &ComboBoxFinderView::foundItem, this, &JournalModule::handleJournalAndTeachersLoading, this->rangeSelected, this->keySelected);

    this->resetGroupState->setInitialState(this->classesNotLoaded);
    this->resetGroupState->addTransition(this, &JournalModule::setClassesNotLoadedState, this->classesNotLoaded);

    this->stateMachine->addState(this->resetGroupState);
    this->stateMachine->setInitialState(this->resetGroupState);
}

bool JournalModule::tryConnect()
{
    if(this->connection->open())
        return true;

    UserError::connectionError("Connection", "be established 'cause something went wrong", "Check credentials or internet connection, server might be down").show(this);

    return false;
}

void JournalModule::showInternalError()
{
    UserError::internalError("Command", "be executed 'cause something went wrong", "Try again or contact support").show(this);
}

dbapi::Class *JournalModule::currentClass()
{
    int index = this->classFinder->currentIndex();

    if(index < 0)
        return nullptr;

    return this->classesModel->grade(index);
}

bool JournalModule::loadCompatibleTeachers()
{
    dbapi::ApiError error;
    auto lists = dbapi::TeacherSubjectsList::loadAll(this->connection, &error);

    if(error.type != dbapi::ApiError::NoError)
        return false;

    this->personsModel->clear();
    dbapi::Subject::Key key(this->classSubjectsModel->subject(this->subjectFinder->currentIndex())->key());

    for(auto list : lists)
    {
        if(list->subjects().contains(key))
        {
            dbapi::Person person(list->key().person, this->connection);

            if(person.load())
                this->personsModel->createPerson(person.key(), person.firstName(), person.secondName());
        }

        delete list;
    }

    return true;
}
