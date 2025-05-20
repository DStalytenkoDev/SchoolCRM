#include <QMessageBox>
#include <SchoolDatabaseApi/TeacherSubjectsList/TeacherSubjectsList.h>

#include "JournalModule.h"
#include "ui_JournalModule.h"

JournalModule::JournalModule(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::JournalModule)
{
    ui->setupUi(this);
}

void JournalModule::setConnection(dbapi::Connection *connection)
{
    this->connection = connection;
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
    if(not this->tryConnect())
        return;

    auto error = this->classSubjectsModel->loadSubjects(this->currentClass()->key());
    this->connection->close();

    if(error.type != dbapi::ApiError::NoError)
        return this->showInternalError();

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
    this->subjectFinder->setHidden(false);
    this->markTypeFinder->setHidden(false);
    this->ui->beginDate->setHidden(false);
    this->ui->endDate->setHidden(false);
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
    this->ui->markCreatingFrame->setHidden(false);
}

void JournalModule::completeMarkCreating()
{

}

void JournalModule::setupFinders()
{

}

void JournalModule::setupJournal()
{

}

void JournalModule::setupToolBar()
{

}

void JournalModule::setupStateMachine()
{

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
}
