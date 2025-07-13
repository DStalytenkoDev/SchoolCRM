#include "PersonsModule.h"
#include "ui_PersonsModule.h"


#include <qdatetime.h>
#include <qmessagebox.h>

#include "TransitionFactrory.h"


PersonsModule::PersonsModule(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::PersonsModule)
{
    ui->setupUi(this);

    this->model = new PersonsModel(this);
    this->rolesModel = new RolesModel(this);

    this->personDialog = new PersonCreationDialog(this);
    this->personDialog->hide(); // hide creation dialog
    this->personDialog->setRoles(this->rolesModel);
    connect(this->personDialog, &PersonCreationDialog::finished, this, &PersonsModule::completePersonCreation);

    // handle creation
    connect(this->ui->createPerson, &QPushButton::clicked, this, &PersonsModule::initPersonCreation);

    this->setupPersonsList();
    this->setupPersonFinder();
    this->setupPersonEditionWidget();
    this->setupStateMachine();
}

void PersonsModule::setupPersonFinder()
{
    this->personFinder = new ComboBoxFinderView(this);
    this->personFinder->setModel(this->model);

    this->ui->menuLayout->insertWidget(0, this->personFinder);

    // handle searching
    connect(this->personFinder, &ComboBoxFinderView::foundItem, this, &PersonsModule::handleFoundPerson);
}

void PersonsModule::setupPersonsList()
{
    this->ui->personsList->setModel(this->model); // set model to main list
    this->ui->personsList->setSelectionBehavior(QAbstractItemView::SelectRows);
    this->ui->personsList->setSelectionMode(QAbstractItemView::SingleSelection);
}

void PersonsModule::setupPersonEditionWidget()
{
    this->personWidget = new PersonEditionWidget(this);
    this->personWidget->setRoles(this->rolesModel);

    this->ui->bottomlayout->addWidget(this->personWidget); // add person edit bar
}

void PersonsModule::setupStateMachine()
{
    this->stateMachine = new QStateMachine(this);

    this->resetState = new QState();
    this->personsNotLoaded = new QState(this->resetState);
    this->personsLoaded = new QState(this->resetState);
    this->itemSelected = new QState(this->resetState);

    connect(this->personsNotLoaded, &QState::entered, this, &PersonsModule::enterPersonsNotLoaded);
    this->personsNotLoaded->addTransition(this, &PersonsModule::personsLoadedAre, this->personsLoaded);

    connect(this->personsLoaded, &QState::entered, this, &PersonsModule::enterPersonsLoaded);
    this->personsLoaded->addTransition(this->ui->personsList, &QListView::clicked, this->itemSelected);
    this->personsLoaded->addTransition(this, &PersonsModule::personFoundIs, this->itemSelected);

    connect(this->itemSelected, &QState::entered, this, &PersonsModule::enterItemSelected);
    connect(this->itemSelected, &QState::exited, this->ui->personsList, &QListView::clearSelection);
    this->itemSelected->addTransition(this, &PersonsModule::dataError, this->personsLoaded);
    this->itemSelected->addTransition(this->personWidget, &PersonEditionWidget::abortButtonClicked, this->personsLoaded);
    transition(this->personWidget, &PersonEditionWidget::saveButtonClicked, this, &PersonsModule::completePersonEdition, this->itemSelected);
    transition(this->ui->deletePerson, &QPushButton::clicked, this, &PersonsModule::handlePersonDeletion, this->itemSelected, this->personsLoaded);

    this->resetState->addTransition(this, &PersonsModule::reseted, this->personsNotLoaded);
    this->resetState->setInitialState(this->personsNotLoaded);

    this->stateMachine->addState(this->resetState);
    this->stateMachine->setInitialState(this->resetState);
}

bool PersonsModule::tryConnect()
{
    if(this->connection->open())
        return true;

    UserError::connectionError("Connection", "be established 'cause something went wrong", "Check credentials or internet connection, server might be down").show(this);

    return false;
}

void PersonsModule::showEvent(QShowEvent *event)
{
    if(not this->stateMachine->isRunning())
        this->stateMachine->start();

    emit this->reseted();

    return QWidget::showEvent(event);
}

void PersonsModule::setConnection(dbapi::Connection *connection)
{
    this->connection = connection;

    this->model->setConnection(connection);
    this->rolesModel->setConnection(connection);
}

PersonsModule::~PersonsModule()
{
    delete ui;
}

void PersonsModule::enterPersonsNotLoaded()
{
    this->personFinder->setCurrentIndex(-1);
    this->personFinder->setDisabled(true);
    this->ui->createPerson->show();
    this->ui->deletePerson->hide();
    this->model->clear();
    this->personWidget->hide();

    if(not this->tryConnect())
        return;

    auto error = this->model->loadAll();
    this->connection->close();

    if(error.isError())
        return error.show(this);

    emit this->personsLoadedAre();
}

void PersonsModule::enterPersonsLoaded()
{
    this->personFinder->setDisabled(false);
    this->ui->personsList->setDisabled(false);
    this->ui->createPerson->show();
    this->ui->deletePerson->hide();
    this->personWidget->hide();
}

void PersonsModule::enterItemSelected()
{
    if(not this->tryConnect())
    {
        emit this->dataError();
        return;
    }

    auto error = this->rolesModel->loadAll();
    this->connection->close();

    if(error.isError())
    {
        UserError::internalError("Person", "be shown and/or edited 'cause an unknown error", "Try again or contact support").show(this);
        emit this->dataError();
        return;
    }

    auto person = this->model->person(this->ui->personsList->currentIndex());

    this->personWidget->setFirstName(person->firstName());
    this->personWidget->setSecondName(person->secondName());
    this->personWidget->setDate(QDate::fromJulianDay(person->birthday()));

    // finding the role by text
    int foundRow = -1;

    for(int row = 0; row < this->rolesModel->rowCount(); row++)
        if(this->rolesModel->role(row)->key() == person->role())
            foundRow = row;

    if(foundRow < 0)
    {
        UserError::internalError("Person", "be shown and/or edited 'cause an unknown error", "Try again or contact support").show(this);
        emit this->dataError();
        return;
    }

    this->personWidget->setRole(this->rolesModel->index(foundRow));

    this->ui->deletePerson->show();
    this->personWidget->show();
    this->ui->createPerson->hide();
    this->ui->personsList->setDisabled(true);
}

void PersonsModule::handleFoundPerson(QModelIndex index)
{
    this->ui->personsList->scrollTo(index);
    this->ui->personsList->setCurrentIndex(index);
    this->ui->personsList->selectionModel()->select(index, QItemSelectionModel::Select);

    emit this->personFoundIs();
}

void PersonsModule::handlePersonDeletion()
{
    if(not this->tryConnect())
        return;

    auto error = this->model->removePerson(this->ui->personsList->currentIndex().row());
    this->connection->close();

    if(error.isError())
        return error.show(this);

    QMessageBox::information(this, "Info", "Selected person has been deleted");
}

void PersonsModule::initPersonCreation()
{
    if(not this->tryConnect())
        return;

    auto error = this->rolesModel->loadAll();
    this->connection->close();

    if(error.isError())
        return error.show(this);

    this->personDialog->clear();
    this->personDialog->show();
}

void PersonsModule::completePersonCreation(QDialog::DialogCode result)
{
    if(result == QDialog::Rejected)
    {
        this->personDialog->hide();
        return;
    }

    if(not this->tryConnect())
        return;

    dbapi::Person person(this->connection);

    auto error = this->model->createPerson(this->personDialog->firstName(),
                                           this->personDialog->secondName(),
                                           this->personDialog->date(),
                                           this->rolesModel->role(this->personDialog->role())->key());
    this->connection->close();

    if(error.isError())
        return error.show(this);

    this->personDialog->hide();

    QMessageBox::information(this, "Info", "The person was created");
}

void PersonsModule::completePersonEdition()
{
    if(not this->tryConnect())
        return;

    QModelIndex index = this->ui->personsList->currentIndex();

    auto error = this->model->editPerson(index.row(),
                                         this->personWidget->firstName(),
                                         this->personWidget->secondName(),
                                         this->personWidget->date(),
                                         this->rolesModel->role(this->personWidget->role())->key());
    this->connection->close();

    this->ui->personsList->setCurrentIndex(index);
    this->ui->personsList->selectionModel()->select(index, QItemSelectionModel::Select);

    if(error.isError())
        return error.show(this);

    QMessageBox::information(this, "Info", "Person edited successfully");
}


