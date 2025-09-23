#include "SubjectsModule.h"
#include "ui_SubjectsModule.h"
#include "TransitionFactrory.h"


SubjectsModule::SubjectsModule(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::SubjectsModule)
{
    ui->setupUi(this);

    this->model = new SubjectsModel(this);
    this->proxyModel = new QSortFilterProxyModel(this);

    this->setupSubjectFinder();
    this->setupSubjectsList();
    this->setupStateMachine();
}

void SubjectsModule::setupSubjectFinder()
{
    this->proxyModel->setSourceModel(this->model);
    this->proxyModel->setFilterCaseSensitivity(Qt::CaseInsensitive);
    this->proxyModel->setFilterKeyColumn(0);
}

void SubjectsModule::setupSubjectsList()
{
    this->ui->subjectsList->setModel(this->model);
    this->ui->subjectsList->setSelectionBehavior(QAbstractItemView::SelectRows);
    this->ui->subjectsList->setSelectionMode(QAbstractItemView::SingleSelection);
}

void SubjectsModule::setupStateMachine()
{
    this->stateMachine = new QStateMachine(this);

    this->subjectsLoaded = new QState(this->stateMachine);
    this->subjectsNotLoaded = new QState(this->stateMachine);
    this->itemSelected = new QState(this->stateMachine);
    this->searching = new QState(this->stateMachine);

    this->stateMachine->setInitialState(this->subjectsNotLoaded);

    connect(this->subjectsNotLoaded, &QState::entered, this, &SubjectsModule::enterSubjectsNotLoaded);
    this->subjectsNotLoaded->addTransition(this, &SubjectsModule::subjectsLoadedAre, this->subjectsLoaded);
    this->subjectsNotLoaded->addTransition(this->ui->updateSubjects, &QPushButton::clicked, this->subjectsNotLoaded);

    connect(this->subjectsLoaded, &QState::entered, this, &SubjectsModule::enterSubjectsLoaded);
    this->subjectsLoaded->addTransition(this->ui->subjectsList, &QListView::clicked, this->itemSelected);
    this->subjectsLoaded->addTransition(this->ui->updateSubjects, &QPushButton::clicked, this->subjectsNotLoaded);
    this->subjectsLoaded->addTransition(this->ui->searchSubject, &QLineEdit::textChanged, this->searching);
    transition(this->ui->createSubject, &QPushButton::clicked, this, &SubjectsModule::initSubjectCreation, this->subjectsLoaded);

    connect(this->itemSelected, &QState::entered, this, &SubjectsModule::enterItemSelected);
    this->itemSelected->addTransition(this->ui->abort, &QPushButton::clicked, this->subjectsLoaded);
    transition(this->ui->deleteSubject, &QPushButton::clicked, this, &SubjectsModule::handleSubjectDeletion, this->itemSelected, this->subjectsLoaded);

    connect(this->searching, &QState::entered, this, &SubjectsModule::enterSearching);
    transition(this->ui->subjectsList, &QListView::clicked, this, &SubjectsModule::completeSearching, this->searching, this->itemSelected);
    transition(this->ui->abort, &QPushButton::clicked, this, &SubjectsModule::abortSearching, this->searching, this->subjectsLoaded);
    transition(this->ui->searchSubject, &QLineEdit::textChanged, this, &SubjectsModule::handleSearching, this->searching);
}

bool SubjectsModule::tryConnect()
{
    if(this->connection->open())
        return true;

    UserError::connectionError("Connection", "be established 'cause something went wrong", "Check credentials or internet connection, server might be down").show(this);

    return false;
}

void SubjectsModule::showEvent(QShowEvent *event)
{
    if(not this->stateMachine->isRunning())
        this->stateMachine->start();

    return QWidget::showEvent(event);
}

void SubjectsModule::setConnection(dbapi::Connection *connection)
{
    this->connection = connection;
    this->model->setConnection(connection);
}

SubjectsModule::~SubjectsModule()
{
    delete ui;
}

void SubjectsModule::enterSubjectsNotLoaded()
{
    this->ui->deleteSubject->hide();
    this->ui->abort->hide();
    this->ui->updateSubjects->show();
    this->ui->createSubject->hide();
    this->ui->searchSubject->setDisabled(true);
    this->ui->searchSubject->clear();

    this->model->clear();

    if(not this->tryConnect())
        return;

    auto error = this->model->loadAll();
    this->connection->close();

    if(error.isError())
        return error.show(this);

    emit this->subjectsLoadedAre();
}

void SubjectsModule::enterSubjectsLoaded()
{
    this->ui->deleteSubject->hide();
    this->ui->abort->hide();
    this->ui->updateSubjects->show();
    this->ui->createSubject->show();

    this->ui->subjectsList->clearSelection();

    this->ui->searchSubject->setDisabled(false);
    this->ui->searchSubject->clear();
}

void SubjectsModule::enterItemSelected()
{
    this->ui->deleteSubject->show();
    this->ui->abort->show();
    this->ui->updateSubjects->hide();
    this->ui->createSubject->hide();

    this->ui->searchSubject->setDisabled(true);
    this->ui->searchSubject->clear();
}

void SubjectsModule::enterSearching()
{
    this->ui->deleteSubject->hide();
    this->ui->abort->show();
    this->ui->updateSubjects->hide();
    this->ui->createSubject->hide();

    this->ui->subjectsList->setModel(this->proxyModel);

    this->handleSearching();
}

void SubjectsModule::handleSubjectDeletion()
{
    if(not this->tryConnect())
        return;

    auto indexes = this->ui->subjectsList->selectionModel()->selectedIndexes();

    assert((void("empty"), indexes.size() > 0));

    auto error = this->model->removeSubject(indexes.front().row());
    this->connection->close();

    if(error.isError())
        return error.show(this);

    QMessageBox::information(this, "Info", "All selected subjects have been deleted");
}

void SubjectsModule::initSubjectCreation()
{
    if(this->subjectCreationDialog)
        delete this->subjectCreationDialog;

    this->subjectCreationDialog = new CreateSubjectDialog(this);
    this->subjectCreationDialog->show();

    connect(this->subjectCreationDialog, &QDialog::finished, this, &SubjectsModule::completeSubjectCreation);
}

void SubjectsModule::completeSubjectCreation()
{
    if(this->subjectCreationDialog->result() == QDialog::Rejected)
        return;

    if(not this->tryConnect())
        return;

    auto error = this->model->createSubject(this->subjectCreationDialog->subject());
    this->connection->close();

    if(error.isError())
        return error.show(this);

    QMessageBox::information(this, "Info", "Subject is created");
}

void SubjectsModule::completeSearching()
{
    QModelIndex activeIndex = this->ui->subjectsList->selectionModel()->selectedIndexes().front();
    QPersistentModelIndex sourceIndex = this->proxyModel->mapToSource(activeIndex);

    this->ui->subjectsList->setModel(this->model);
    this->ui->subjectsList->setCurrentIndex(sourceIndex);
    this->ui->subjectsList->selectionModel()->select(sourceIndex, QItemSelectionModel::Select);
}

void SubjectsModule::abortSearching()
{
    this->ui->subjectsList->setModel(this->model);
}

void SubjectsModule::handleSearching()
{
    this->proxyModel->setFilterWildcard(this->ui->searchSubject->text());
}
