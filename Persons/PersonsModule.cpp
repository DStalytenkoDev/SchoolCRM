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
    connect(this->ui->deletePerson, &QPushButton::clicked, this, &PersonsModule::handlePersonsDeletion);

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
    this->ui->personsList->setSelectionMode(QAbstractItemView::ExtendedSelection);

    // handle selection of persons
    connect(this->ui->personsList->selectionModel(), &QItemSelectionModel::selectionChanged, this, &PersonsModule::handleSelectedItems);
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
    this->rangeSelected = new QState(this->resetState);
    this->personEditing = new QState(this->resetState);

    connect(this->personsNotLoaded, &QState::entered, this, &PersonsModule::enterPersonsNotLoaded);
    this->personsNotLoaded->addTransition(this, &PersonsModule::personsLoadedAre, this->personsLoaded);

    connect(this->personsLoaded, &QState::entered, this, &PersonsModule::enterPersonsLoaded);
    this->personsLoaded->addTransition(this, &PersonsModule::itemSelectedIs, this->itemSelected);
    this->personsLoaded->addTransition(this, &PersonsModule::rangeSelectedIs, this->rangeSelected);

    connect(this->itemSelected, &QState::entered, this, &PersonsModule::enterItemSelected);
    this->itemSelected->addTransition(this, &PersonsModule::rangeSelectedIs, this->rangeSelected);
    this->itemSelected->addTransition(this->ui->editPerson, &QPushButton::clicked, this->personEditing);
    this->itemSelected->addTransition(this, &PersonsModule::itemsDeselectedAre, this->personsLoaded);
    this->itemSelected->addTransition(this->ui->deletePerson, &QPushButton::clicked, this->personsLoaded);

    connect(this->personEditing, &QState::entered, this, &PersonsModule::enterPersonEditing);
    this->personEditing->addTransition(this->personWidget, &PersonEditionWidget::saveButtonClicked, this->itemSelected);
    this->personEditing->addTransition(this->personWidget, &PersonEditionWidget::abortButtonClicked, this->itemSelected);
    this->personEditing->addTransition(this, &PersonsModule::dataError, this->itemSelected);

    connect(this->rangeSelected, &QState::entered, this, &PersonsModule::enterRangeSelected);
    this->rangeSelected->addTransition(this, &PersonsModule::itemSelectedIs, this->itemSelected);
    this->rangeSelected->addTransition(this, &PersonsModule::itemsDeselectedAre, this->personsLoaded);
    this->rangeSelected->addTransition(this->ui->deletePerson, &QPushButton::clicked, this->personsLoaded);

    this->resetState->addTransition(this, &PersonsModule::reseted, this->personsNotLoaded);
    this->resetState->setInitialState(this->personsNotLoaded);

    this->stateMachine->addState(this->resetState);
    this->stateMachine->setInitialState(this->resetState);
}

bool PersonsModule::tryConnect()
{
    if(this->connection->open())
        return true;

    this->showInternalError();

    return false;
}

void PersonsModule::showInternalError()
{
    QMessageBox::critical(this, "Internal error", "Please check your connection to the database");
}

void PersonsModule::abortConnection()
{
    this->connection->close();
    this->showInternalError();
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

    if(error.type != dbapi::ApiError::NoError)
        return this->showInternalError();

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

void PersonsModule::enterRangeSelected()
{
    this->ui->deletePerson->setHidden(false);
    this->ui->createPerson->hide();
    this->ui->editPerson->hide();
    this->ui->personsList->setDisabled(false);
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

    if(error.type != dbapi::ApiError::NoError)
    {
        emit this->dataError();
        return;
    }

    auto person = this->model->person(this->ui->personsList->currentIndex());

    this->personWidget->setFirstName(person->firstName());
    this->personWidget->setSecondName(person->secondName());
    this->personWidget->setDate(QDate::fromJulianDay(person->birthday()));

    int foundRow = -1;

    for(int row = 0; row < this->rolesModel->rowCount(); row++)
        if(this->rolesModel->role(row)->key() == person->role())
            foundRow = row;

    if(foundRow < 0)
    {
        QMessageBox::critical(this, "Internal error", "Person edition could not be performed due to an error");
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

void PersonsModule::handleSelectedItems()
{
    auto& selection = this->ui->personsList->selectionModel()->selection();

    if(selection.empty())
    {
        emit this->itemsDeselectedAre();
        return;
    }

    if(selection.indexes().size() == 1)
        emit this->itemSelectedIs();
    else
        emit this->rangeSelectedIs();
}

void PersonsModule::handlePersonsDeletion()
{
    if(not this->tryConnect())
        return;

    bool errorFlag = false;

    for(QModelIndex& index : this->ui->personsList->selectionModel()->selectedIndexes())
    {
        if(not this->model->person(index)->remove()) // API calling for deletion
            errorFlag = true; // flag any request failed

        this->ui->personsList->setRowHidden(index.row(), true); // hide persons
    }

    if(errorFlag)
    {
        QString msg("Deletion of these persons is failed:\n");

        for(QModelIndex& index : this->ui->personsList->selectionModel()->selectedIndexes())
        {
            auto person = this->model->person(index);

            if(person->error().type != dbapi::ApiError::NoError)
            {
                msg.append(QString("%1 %2 due to: %3\n").arg(person->firstName(),
                                                             person->secondName(),
                                                             person->error().text));
                this->ui->personsList->setRowHidden(index.row(), false); // unhide not deleted persons
            }
        }

        QMessageBox::warning(this, "Deletion", msg);
    }
    else
        QMessageBox::information(this, "Deletion", "All selected persons had been deleted");

    this->connection->close();
}

void PersonsModule::initPersonCreation()
{
    if(not this->tryConnect())
        return;

    auto error = this->rolesModel->loadAll();
    this->connection->close();

    if(error.type != dbapi::ApiError::NoError)
        return this->showInternalError();

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

    person.setFirstName(this->personDialog->firstName());
    person.setSecondName(this->personDialog->secondName());
    person.setBirthday(this->personDialog->date().toJulianDay());
    person.setRole(this->rolesModel->role(this->personDialog->role())->key());

    bool updated = person.store();
    this->connection->close();

    if(not updated)
    {
        QMessageBox::warning(this, "Creation",
                             QString("Creation of a new person failed due to: %1").arg(person.error().text));
        return;
    }

    this->model->insertRow(0, person);
    this->personDialog->hide();

    QMessageBox::information(this, "Creation", "The person was created");
}

void PersonsModule::completePersonEdition()
{
    if(not this->tryConnect())
        return;

    auto p_person = this->model->person(this->ui->personsList->currentIndex());
    dbapi::Person person(*p_person); // copy data from the model

    person.setFirstName(this->personWidget->firstName());
    person.setSecondName(this->personWidget->secondName());
    person.setBirthday(this->personWidget->date().toJulianDay());
    person.setRole(this->rolesModel->role(this->personWidget->role())->key());
    person.update();

    this->connection->close();

    if(person.error().type != dbapi::ApiError::NoError)
    {
        QMessageBox::warning(this, "Edition",
                             QString("Edition has failed due to: %1").arg(person.error().text));

        return;
    }

    QMessageBox::information(this, "Edition", "Edited successfully");

    (*p_person) = person; // copy data back

    this->model->personNameEdited(this->ui->personsList->currentIndex()); // tell model about change
}
