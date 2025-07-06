#include "PersonsModule.h"
#include "ui_PersonsModule.h"


#include <qdatetime.h>
#include <qmessagebox.h>


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

    // handle deletion of selection
    connect(this->ui->deletePerson, &QPushButton::clicked, this, &PersonsModule::handlePersonDeletion);

    this->setupPersonsList();
    this->setupPersonFinder();
    this->setupPersonEditionWidget();
    this->setupStateMachine();
}

void PersonsModule::setupPersonFinder()
{
    this->personFinder = new ComboBoxFinderView(this);

    auto layout = this->ui->menuLayout->replaceWidget(this->ui->searchPerson, this->personFinder);
    this->ui->searchPerson->hide();

    delete layout;

    this->personFinder->setModel(this->model); // set model for search bar

    // handle searching
    connect(this->personFinder, &ComboBoxFinderView::foundItem, this, &PersonsModule::handleFoundPerson);
}

void PersonsModule::setupPersonsList()
{
    this->ui->personsList->setModel(this->model); // set model to main list
    this->ui->personsList->setSelectionBehavior(QAbstractItemView::SelectRows);
    this->ui->personsList->setSelectionMode(QAbstractItemView::SingleSelection);

    // handle selection of persons
    connect(this->ui->personsList->selectionModel(), &QItemSelectionModel::selectionChanged, this, &PersonsModule::handleSelectedItem);
}

void PersonsModule::setupPersonEditionWidget()
{
    this->personWidget = new PersonEditionWidget(this);
    this->personWidget->setRoles(this->rolesModel); // set roles model to edit bar

    this->ui->bottomlayout->addWidget(this->personWidget); // add person edit bar

    // handle edition completing
    connect(this->personWidget, &PersonEditionWidget::saveButtonClicked, this, &PersonsModule::completePersonEdition);
}

void PersonsModule::setupStateMachine()
{
    this->stateMachine = new QStateMachine(this);

    this->resetState = new QState();
    this->personsNotLoaded = new QState(this->resetState);
    this->personsLoaded = new QState(this->resetState);
    this->itemSelected = new QState(this->resetState);
    this->personEditing = new QState(this->resetState);

    connect(this->personsNotLoaded, &QState::entered, this, &PersonsModule::enterPersonsNotLoaded);
    this->personsNotLoaded->addTransition(this, &PersonsModule::personsLoadedAre, this->personsLoaded);

    connect(this->personsLoaded, &QState::entered, this, &PersonsModule::enterPersonsLoaded);
    this->personsLoaded->addTransition(this, &PersonsModule::itemSelectedIs, this->itemSelected);

    connect(this->itemSelected, &QState::entered, this, &PersonsModule::enterItemSelected);
    this->itemSelected->addTransition(this->ui->editPerson, &QPushButton::clicked, this->personEditing);
    this->itemSelected->addTransition(this, &PersonsModule::itemDeselectedIs, this->personsLoaded);
    this->itemSelected->addTransition(this->ui->deletePerson, &QPushButton::clicked, this->personsLoaded);

    connect(this->personEditing, &QState::entered, this, &PersonsModule::enterPersonEditing);
    this->personEditing->addTransition(this->personWidget, &PersonEditionWidget::saveButtonClicked, this->itemSelected);
    this->personEditing->addTransition(this->personWidget, &PersonEditionWidget::abortButtonClicked, this->itemSelected);
    this->personEditing->addTransition(this, &PersonsModule::dataError, this->itemSelected);

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

void PersonsModule::setConnection(dbapi::Connection *connection)
{
    this->connection = connection;

    this->model->setConnection(connection);
    this->rolesModel->setConnection(connection);
}

void PersonsModule::prepare()
{
    if(not this->stateMachine->isRunning())
        this->stateMachine->start();

    emit this->reseted();
}

PersonsModule::~PersonsModule()
{
    delete ui;
}

void PersonsModule::enterPersonsNotLoaded()
{
    this->personFinder->setCurrentIndex(-1);
    this->personFinder->setDisabled(true);
    this->ui->createPerson->setHidden(false);
    this->ui->deletePerson->hide();
    this->ui->editPerson->hide();
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
    this->ui->createPerson->setHidden(false);
    this->ui->deletePerson->hide();
    this->ui->editPerson->hide();
    this->ui->personsList->clearSelection();
    this->ui->personsList->setDisabled(false);
}

void PersonsModule::enterItemSelected()
{
    this->ui->editPerson->setHidden(false);
    this->ui->deletePerson->setHidden(false);
    this->ui->createPerson->hide();
    this->ui->personsList->setDisabled(false);
    this->personWidget->hide();
}

void PersonsModule::enterPersonEditing()
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
        UserError::internalError("Person", "be edited 'cause an unknown error", "Try again or contact support").show(this);
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
        UserError::internalError("Person", "be edited 'cause an unknown error", "Try again or contact support").show(this);
        emit this->dataError();
        return;
    }

    this->ui->personsList->setDisabled(true);
    this->ui->editPerson->hide();
    this->ui->deletePerson->hide();
    this->personWidget->setRole(this->rolesModel->index(foundRow));
    this->personWidget->show();
}

void PersonsModule::handleFoundPerson(QModelIndex index)
{
    this->ui->personsList->scrollTo(index);
}

void PersonsModule::handleSelectedItem()
{
    auto& selection = this->ui->personsList->selectionModel()->selection();

    if(selection.empty())
    {
        emit this->itemDeselectedIs();
        return;
    }

    emit this->itemSelectedIs();
}

void PersonsModule::handlePersonDeletion()
{
    if(not this->tryConnect())
        return;

    auto error = this->model->removePerson(this->ui->personsList->selectionModel()->selection().indexes().front().row());
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

    auto error = this->model->editPerson(this->ui->personsList->currentIndex().row(),
                                         this->personWidget->firstName(),
                                         this->personWidget->secondName(),
                                         this->personWidget->date(),
                                         this->rolesModel->role(this->personWidget->role())->key());
    this->connection->close();

    if(error.isError())
        return error.show(this);

    QMessageBox::information(this, "Info", "Person edited successfully");
}
