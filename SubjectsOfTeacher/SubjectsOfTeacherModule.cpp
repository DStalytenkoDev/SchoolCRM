#include <QMessageBox>
#include <QTimer>
#include "SubjectsOfTeacherModule.h"
#include "ui_SubjectsOfTeacherModule.h"


SubjectsOfTeacherModule::SubjectsOfTeacherModule(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::SubjectsOfTeacherModule)
{
    ui->setupUi(this);

    this->model = new SubjectsOfTeacherModel(this);
    this->personsModel = new PersonsModel(this);
    this->subjectsModel = new SubjectsModel(this);

    this->setupSubjectsList();
    this->setupFinders();
    this->setupToolBar();
}

void SubjectsOfTeacherModule::setConnection(dbapi::Connection *connection)
{
    this->connection = connection;

    this->model->setConnection(connection);
    this->subjectsModel->setConnection(connection);
    this->personsModel->setConnection(connection);
}

void SubjectsOfTeacherModule::prepare()
{
    if(not this->tryConnect())
        return;

    auto error = this->personsModel->loadAll();
    this->connection->close();

    this->ui->additionMenu->hide();

    if(error.type == dbapi::ApiError::NoError)
    {
        this->personFinder->setDisabled(false);
        this->personFinder->setCurrentIndex(-1);
        this->ui->addSubject->setHidden(false);
        return;
    }

    this->showInternalError();
    this->personFinder->setDisabled(true);
    this->ui->addSubject->hide();
}

SubjectsOfTeacherModule::~SubjectsOfTeacherModule()
{
    delete ui;
}

void SubjectsOfTeacherModule::handleSelectedTeacher()
{
    this->model->clear();
    this->ui->additionMenu->hide();
    this->ui->addSubject->hide();
    this->ui->deleteSubject->hide();

    if(not this->tryConnect())
        return;

    auto teacherKey = this->currentTeacher()->key();
    auto error = this->model->loadSubjects(teacherKey);

    this->connection->close();

    if(error.type != dbapi::ApiError::NoError)
        if(error.type != dbapi::ApiError::KeyError)
            this->showInternalError();

    this->ui->addSubject->setHidden(false);
}

void SubjectsOfTeacherModule::handleSelectedSubjects()
{
    auto selection = this->ui->subjectsList->selectionModel()->selection();

    if(selection.empty())
    {
        this->ui->deleteSubject->hide();
        this->ui->addSubject->setHidden(false);
    }
    else
    {
        this->ui->deleteSubject->setHidden(false);
        this->ui->addSubject->hide();
    }
}

void SubjectsOfTeacherModule::handleSubjectsDeleting()
{
    if(not this->tryConnect())
        return;

    dbapi::TeacherSubjectsList list({this->currentTeacher()->key()}, this->connection);

    auto selectedIndexes = this->ui->subjectsList->selectionModel()->selectedIndexes();

    for(QModelIndex& index : selectedIndexes)
    {
        auto key = this->model->subject(index);

        list.appendSubject(key);
    }

    bool deleted = list.remove();
    this->connection->close();

    if(not deleted)
        return this->showInternalError();

    for(QModelIndex& index : selectedIndexes)
        this->model->removeRow(index.row());

    QMessageBox::information(this, "Deletion", "All selected subjects had been deleted");
}

void SubjectsOfTeacherModule::initSubjectAddition()
{
    if(not this->tryConnect())
        return;

    auto error = this->subjectsModel->loadAll();
    this->connection->close();

    if(error.type != dbapi::ApiError::NoError)
        return this->showInternalError();

    this->ui->additionMenu->setHidden(false);
    this->ui->subjectsList->clearSelection();
    this->ui->addSubject->hide();
}

void SubjectsOfTeacherModule::abortSubjectAddition()
{
    this->ui->additionMenu->hide();
    this->ui->addSubject->setHidden(false);
}

void SubjectsOfTeacherModule::completeSubjectAddition()
{
    if(not this->tryConnect())
        return;

    dbapi::TeacherSubjectsList list({this->currentTeacher()->key()}, this->connection);

    auto subject = this->subjectsModel->subject(this->subjectFinder->currentIndex());

    list.appendSubject(subject->key());

    bool updated = list.update();
    this->connection->close();

    if(updated)
        this->model->insertRow(*subject);
    else
        this->showInternalError();

    this->abortSubjectAddition();
}

void SubjectsOfTeacherModule::loadSubjects()
{
    if(not this->tryConnect())
        return;

    auto error = this->model->loadSubjects(this->currentTeacher()->key());
    this->connection->close();

    this->ui->subjectsList->clearSelection();

    if(error.type == dbapi::ApiError::NoError)
        return;

    this->showInternalError();
}

void SubjectsOfTeacherModule::setupFinders()
{
    this->personFinder = new ComboBoxFinderView(this);
    this->personFinder->setModel(this->personsModel);
    this->ui->menuLayout->insertWidget(0, this->personFinder);

    connect(this->personFinder, &ComboBoxFinderView::foundItem, this, &SubjectsOfTeacherModule::handleSelectedTeacher);

    this->subjectFinder = new ComboBoxFinderView(this);
    this->subjectFinder->setModel(this->subjectsModel);
    this->ui->additionMenuLayout->insertWidget(0, this->subjectFinder);
}

void SubjectsOfTeacherModule::setupSubjectsList()
{
    this->ui->subjectsList->setModel(this->model);
    this->ui->subjectsList->setSelectionBehavior(QAbstractItemView::SelectRows);
    this->ui->subjectsList->setSelectionMode(QAbstractItemView::MultiSelection);

    // handle selection
    connect(this->ui->subjectsList->selectionModel(), &QItemSelectionModel::selectionChanged, this, &SubjectsOfTeacherModule::handleSelectedSubjects);
}

void SubjectsOfTeacherModule::setupToolBar()
{
    // hide deltion button
    this->ui->deleteSubject->hide();

    // hide addition layout
    this->ui->additionMenu->hide();

    // handle addition
    connect(this->ui->addSubject, &QPushButton::clicked, this, &SubjectsOfTeacherModule::initSubjectAddition);
    connect(this->ui->confirmAddition, &QPushButton::clicked, this, &SubjectsOfTeacherModule::completeSubjectAddition);
    connect(this->ui->abortAddition, &QPushButton::clicked, this, &SubjectsOfTeacherModule::abortSubjectAddition);

    // handle deletion
    connect(this->ui->deleteSubject, &QPushButton::clicked, this, &SubjectsOfTeacherModule::handleSubjectsDeleting);

    this->personFinder->setDisabled(true);
    this->ui->addSubject->hide();
}

bool SubjectsOfTeacherModule::tryConnect()
{
    if(this->connection->open())
        return true;

    this->showInternalError();

    return false;
}

void SubjectsOfTeacherModule::showInternalError()
{
    QMessageBox::critical(this, "Internal error", "Please check your connection to the database");
}

dbapi::Person *SubjectsOfTeacherModule::currentTeacher()
{
    int index = this->personFinder->currentIndex();

    if(index < 0)
        return nullptr;

    return this->personsModel->person(index);
}
