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

    if(error.type == dbapi::ApiError::NoError)
        return;

    this->showInternalError();
}

SubjectsOfTeacherModule::~SubjectsOfTeacherModule()
{
    delete ui;
}

void SubjectsOfTeacherModule::handleFoundPerson(QModelIndex index)
{
    if(not index.isValid())
        return;

    if(not this->tryConnect())
        return;

    this->activePerson = index;

    auto error = this->model->loadSubjects(this->personsModel->person(index)->key());

    this->connection->close();

    if(error.type != dbapi::ApiError::NoError)
    {
        if(error.type == dbapi::ApiError::KeyError)
            return;

        this->showInternalError();
    }
}

void SubjectsOfTeacherModule::handleClickedSubject(const QModelIndex &index)
{
    this->selectedSubjects.clear();
    this->selectedSubjects.select(index, index);

    this->ui->deleteSubject->setHidden(false);
}

void SubjectsOfTeacherModule::handleSelectedSubjects(const QItemSelection &selected, const QItemSelection &deselected)
{
    this->selectedSubjects = selected;

    if(selected.empty())
        this->ui->deleteSubject->hide();
    else
        this->ui->deleteSubject->setHidden(false);
}

void SubjectsOfTeacherModule::deleteSubjects()
{
    if(not this->tryConnect())
        return;

    dbapi::TeacherSubjectsList list({this->personsModel->person(this->activePerson)->key()}, this->connection);

    for(QModelIndex& index : this->selectedSubjects.indexes())
    {
        auto key = this->model->subject(index);

        list.appendSubject(key); // must add before delition 'cause must be deleted an item of the list by an iterator
        list.removeSubject(list.subjects().cend() - 1);
    }

    bool deleted = list.remove();
    this->connection->close();

    if(not deleted)
    {
        this->showInternalError();
        return;
    }

    for(QModelIndex& index : this->selectedSubjects.indexes())
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
    {
        this->showInternalError();
        return;
    }

    this->ui->additionMenu->setHidden(false);
}

void SubjectsOfTeacherModule::abortSubjectAddition()
{
    this->ui->additionMenu->setHidden(true);
}

void SubjectsOfTeacherModule::completeSubjectAddition()
{
    if(not this->tryConnect())
        return;

    dbapi::TeacherSubjectsList list({this->personsModel->person(this->activePerson)->key()}, this->connection);

    auto subject = this->subjectsModel->subject(this->subjectFinder->currentIndex());

    list.appendSubject(subject->key());

    bool updated = list.update();
    this->connection->close();

    if(not updated)
    {
        this->showInternalError();
        return;
    }

    this->model->insertRow(*subject);

    this->abortSubjectAddition();
}

void SubjectsOfTeacherModule::loadSubjects()
{
    if(not this->tryConnect())
        return;

    auto error = this->model->loadSubjects(this->personsModel->person(this->activePerson)->key());
    this->connection->close();

    if(error.type != dbapi::ApiError::NoError)
        this->showInternalError();
}

void SubjectsOfTeacherModule::setupFinders()
{
    this->personFinder = new ComboBoxFinderView(this);
    this->personFinder->setModel(this->personsModel);
    this->ui->menuLayout->insertWidget(0, this->personFinder);

    connect(this->personFinder, &ComboBoxFinderView::foundItem, this, &SubjectsOfTeacherModule::handleFoundPerson);

    this->subjectFinder = new ComboBoxFinderView(this);
    this->subjectFinder->setModel(this->subjectsModel);
    this->ui->additionMenuLayout->insertWidget(0, this->subjectFinder);
}

void SubjectsOfTeacherModule::setupSubjectsList()
{
    this->ui->subjectsList->setModel(this->model);
    this->ui->subjectsList->setSelectionBehavior(QAbstractItemView::SelectRows);
    this->ui->subjectsList->setSelectionMode(QAbstractItemView::MultiSelection);

    // handle click
    connect(this->ui->subjectsList, &QListView::clicked, this, &SubjectsOfTeacherModule::handleClickedSubject);

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
    connect(this->ui->deleteSubject, &QPushButton::clicked, this, &SubjectsOfTeacherModule::deleteSubjects);
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
