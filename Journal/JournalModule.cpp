#include <QMessageBox>
#include <SchoolDatabaseApi/TeacherSubjectsList/TeacherSubjectsList.h>
#include <qlabel.h>
#include <qsignaltransition.h>

#include "JournalModule.h"
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
    this->personsModel = new PersonsModel(this);

    this->markCreatingDialog = new MarkCreatingDialog(this);

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
    this->personsModel->setConnection(connection);
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
    this->resetUi();

    if(not this->tryConnect())
        return;

    auto error = this->classesModel->loadAll();
    this->connection->close();

    if(error.type != dbapi::ApiError::NoError)
        return this->showInternalError();

    emit this->setClassesLoadedState();
}

void JournalModule::enterClassesLoadedState()
{
    this->classFinder->setHidden(false);
    this->classFinder->setCurrentIndex(-1);
}

void JournalModule::enterClassSelectedState()
{
    this->resetUi();

    if(not this->tryConnect())
        return;

    auto error = this->classSubjectsModel->loadSubjects(this->currentClass()->key());
    this->connection->close();

    if(error.type != dbapi::ApiError::NoError)
        return this->showInternalError();

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
    this->resetUi();

    this->ui->journal->setHidden(false);
    this->ui->beginDate->setHidden(false);
    this->ui->endDate->setHidden(false);

    this->subjectFinder->setHidden(false);
    this->classFinder->setHidden(false);
    this->markTypeFinder->setHidden(false);

    this->layout()->removeItem(this->journalSpacer);
}

void JournalModule::enterEmptyCellSelectedState()
{
    this->ui->createMark->setHidden(false);
}

void JournalModule::enterRangeSelectedState()
{
    this->ui->deleteMark->setHidden(false);
}

void JournalModule::handleSelectedMarks(const QItemSelection &selected, const QItemSelection &deselected)
{
    if(selected.empty())
    {
        emit this->setKeySelectedState();
        return;
    }

    auto indexes = selected.indexes();

    if(indexes.size() == 1 and this->journalModel->mark(indexes[0]) == nullptr)
        emit this->setEmptyCellSelectedState();
    else
        emit this->setRangeSelectedState();
}

void JournalModule::handleKeyChange()
{
    if(not this->tryConnect())
        return;

    auto subject = this->classSubjectsModel->subject(this->subjectFinder->currentIndex());
    this->journalModel->setSubject(subject);
    this->journalModel->setType(this->markTypeFinder->getType());
    this->journalModel->setDateRange(this->ui->beginDate->date(), this->ui->endDate->date());

    auto error = this->journalModel->load();
    this->connection->close();

    if(error.type != dbapi::ApiError::NoError)
        return this->showInternalError();
}

void JournalModule::handleSelectedSubject()
{
    if(not this->tryConnect())
        return;

    bool loaded = this->loadCompatibleTeachers();
    this->connection->close();

    if(not loaded)
        this->showInternalError();
}

void JournalModule::onClassSelectedToKeySelectedTransition()
{
    if(not this->tryConnect())
        return;

    auto error = this->journalModel->load();

    if(error.type != dbapi::ApiError::NoError)
        return this->abortConnection();

    if(not this->loadCompatibleTeachers())
        return this->abortConnection();

    this->connection->close();
}

void JournalModule::handleMarksDeleting()
{
    emit this->setKeySelectedState();

    if(not this->tryConnect())
        return;

    for(auto& selectionRange : this->ui->journal->selectionModel()->selection())
    {
        auto error = this->journalModel->remove(selectionRange.topLeft(), selectionRange.bottomRight());

        if(error.type != dbapi::ApiError::NoError)
            return this->abortConnection();
    }

    QMessageBox::information(this, "Deletion", "All selected item had been deleted");
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

    auto teacherKey = this->personsModel->person(teacherIndex)->key();

    if(not this->tryConnect())
        return;

    auto error = this->journalModel->store(this->selectedMark, this->markCreatingDialog->value(), teacherKey);
    this->connection->close();

    if(error.type != dbapi::ApiError::NoError)
        return this->showInternalError();

    QMessageBox::information(this, "Creating", "New mark was created");
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
    auto transition = this->classSelected->addTransition(this->subjectFinder, &ComboBoxFinderView::foundItem, this->keySelected);
    connect(transition, &QSignalTransition::triggered, this, &JournalModule::onClassSelectedToKeySelectedTransition);
    this->classSelected->addTransition(this->classFinder, &ComboBoxFinderView::foundItem, this->classSelected);

    connect(this->keySelected, &QState::entered, this, &JournalModule::enterKeySelectedState);
    this->keySelected->addTransition(this->classFinder, &ComboBoxFinderView::foundItem, this->classSelected);
    this->keySelected->addTransition(this, &JournalModule::setEmptyCellSelectedState, this->emptyCellSelected);
    this->keySelected->addTransition(this, &JournalModule::setRangeSelectedState, this->rangeSelected);

    transition = new QSignalTransition(this->ui->beginDate, &QDateEdit::userDateChanged);
    this->keySelected->addTransition(transition);
    connect(transition, &QSignalTransition::triggered, this, &JournalModule::handleKeyChange);

    transition = new QSignalTransition(this->ui->endDate, &QDateEdit::userDateChanged);
    this->keySelected->addTransition(transition);
    connect(transition, &QSignalTransition::triggered, this, &JournalModule::handleKeyChange);

    transition = new QSignalTransition(this->markTypeFinder, &MarkTypeSelector::typeChanged);
    this->keySelected->addTransition(transition);
    connect(transition, &QSignalTransition::triggered, this, &JournalModule::handleKeyChange);

    transition = new QSignalTransition(this->subjectFinder, &ComboBoxFinderView::foundItem);
    this->keySelected->addTransition(transition);
    connect(transition, &QSignalTransition::triggered, this, &JournalModule::handleSelectedSubject);
    connect(transition, &QSignalTransition::triggered, this, &JournalModule::handleKeyChange);

    this->emptyCellSelected->addTransition(this, &JournalModule::setKeySelectedState, this->keySelected);
    connect(this->emptyCellSelected, &QState::entered, this, &JournalModule::enterEmptyCellSelectedState);

    this->rangeSelected->addTransition(this, &JournalModule::setKeySelectedState, this->keySelected);
    connect(this->rangeSelected, &QState::entered, this, &JournalModule::enterRangeSelectedState);

    this->resetGroupState->setInitialState(this->classesNotLoaded);
    this->resetGroupState->addTransition(this, &JournalModule::setClassesNotLoadedState, this->classesNotLoaded);

    this->stateMachine->addState(this->resetGroupState);
    this->stateMachine->setInitialState(this->resetGroupState);
}

bool JournalModule::tryConnect()
{
    if(this->connection->open())
        return true;

    this->showInternalError();

    return false;
}

void JournalModule::showInternalError()
{
    QMessageBox::critical(this, "Internal error", "Please check your connection to the database");
}

void JournalModule::abortConnection()
{
    this->connection->close();
    this->showInternalError();
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

    for(auto list : lists)
    {
        dbapi::Subject::Key key(this->classSubjectsModel->subject(this->subjectFinder->currentIndex()));

        // personsModel must be cleared

        if(list->subjects().contains(key))
            this->personsModel->insertRow(0, dbapi::Person(list->key().person, this->connection));

        delete list;
    }

    return true;
}

void JournalModule::resetUi()
{
    this->classFinder->hide();
    this->subjectFinder->hide();
    this->markTypeFinder->hide();

    this->ui->beginDate->hide();
    this->ui->endDate->hide();

    this->ui->createMark->hide();
    this->ui->deleteMark->hide();

    this->ui->journal->hide();

    this->layout()->addItem(this->journalSpacer);
}
