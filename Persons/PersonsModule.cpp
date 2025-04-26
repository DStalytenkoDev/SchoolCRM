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

    this->ui->deletePerson->hide(); // hide delete btn

    // handle creation
    connect(this->ui->createPerson, &QPushButton::clicked, this, &PersonsModule::initPersonCreation);
    connect(this->personDialog, &PersonCreationDialog::finished, this, &PersonsModule::completePersonCreation);

    // handle deletion of selection
    connect(this->ui->deletePerson, &QPushButton::clicked, this, &PersonsModule::deleteSelectedPersons);

    // handle load all
    connect(this->ui->loadPersons, &QPushButton::clicked, this, &PersonsModule::loadPersons);

    this->setupPersonsList();
    this->setupPersonFinder();
    this->setupPersonEditionWidget();
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
    this->ui->personsList->setSelectionMode(QAbstractItemView::MultiSelection);

    // handle click on person (edition)
    connect(this->ui->personsList, &QListView::clicked, this, &PersonsModule::handleClickedPerson);

    // handle selection of persons
    connect(this->ui->personsList->selectionModel(), &QItemSelectionModel::selectionChanged, this, &PersonsModule::handleSelectedPerson);
}

void PersonsModule::setupPersonEditionWidget()
{
    this->personWidget = new PersonEditionWidget(this);

    this->personWidget->hide(); // hide person edit bar
    this->ui->bottomlayout->addWidget(this->personWidget); // add person edit bar

    this->personWidget->setRoles(this->rolesModel); // set roles model to edit bar

    // handle edition completing / aborting / deleting
    connect(this->personWidget, &PersonEditionWidget::saveButtonClicked, this, &PersonsModule::completePersonEdition);
    connect(this->personWidget, &PersonEditionWidget::deleteButtonClicked, this, &PersonsModule::deleteClickedPerson);
    connect(this->personWidget, &PersonEditionWidget::abortButtonClicked, this, &PersonsModule::abortPersonEdition);
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

void PersonsModule::handleFoundPerson(QModelIndex index)
{
    this->ui->personsList->scrollTo(index);
}

void PersonsModule::handleClickedPerson(const QModelIndex &index)
{
    this->selectedPersons.clear();
    this->ui->deletePerson->hide();

    this->clickedPerson = index;

    this->initPersonEdition();
}

void PersonsModule::handleSelectedPerson(const QItemSelection &selected, const QItemSelection &deselected)
{
    this->selectedPersons = selected;

    if(selected.empty())
        this->ui->deletePerson->hide();
    else
        this->ui->deletePerson->setHidden(false);
}

void PersonsModule::deleteSelectedPersons()
{
    if(not this->connection->open())
    {
        QMessageBox::critical(this, "Internal error",
                              QString("Deletion could not be performed due to: %1").arg(this->connection->error().text));
        return;
    }

    bool errorFlag = false;

    for(QModelIndex& index : this->selectedPersons.indexes())
    {
        if(not this->model->person(index)->remove()) // API calling for deletion
            errorFlag = true; // flag any request failed

        this->ui->personsList->setRowHidden(index.row(), true); // hide persons
    }

    if(errorFlag)
    {
        QString msg("Deletion of these persons is failed:\n");

        for(QModelIndex& index : this->selectedPersons.indexes())
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

void PersonsModule::deleteClickedPerson()
{
    if(not this->connection->open())
    {
        QMessageBox::critical(this, "Internal error",
                              QString("Deletion could not be performed due to: %1").arg(this->connection->error().text));
        return;
    }

    auto person = this->model->person(this->clickedPerson);

    if(not person->remove()) // API calling for deletion
    {
        QMessageBox::warning(this, "Deletion",
                             QString("The deletion is failed due to: %1").arg(person->error().text));

        this->connection->close();
        return;
    }

    this->ui->personsList->setRowHidden(this->clickedPerson.row(), true); // hide deleted person
    this->abortPersonEdition();

    this->connection->close();
}

void PersonsModule::initPersonCreation()
{
    // load down roles if they were not
    if(not this->loadRolesIfNeed())
        return;

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

    if(not this->connection->open())
    {
        QMessageBox::critical(this, "Internal error",
                              QString("Creation could not be performed due to: %1").arg(this->connection->error().text));
        return;
    }

    dbapi::Person person(this->connection);

    person.setFirstName(this->personDialog->firstName());
    person.setSecondName(this->personDialog->secondName());
    person.setBirthday(this->personDialog->date().toJulianDay());
    person.setRole(this->rolesModel->role(this->personDialog->role())->key());

    if(not person.store())
    {
        QMessageBox::warning(this, "Creation",
                             QString("Creation of a new person failed due to: %1").arg(person.error().text));

        this->connection->close();
        return;
    }

    this->connection->close();

    this->model->insertRow(0, person);
    this->personDialog->hide();

    QMessageBox::information(this, "Creation", "The person was created");
}

void PersonsModule::initPersonEdition()
{
    if(not this->loadRolesIfNeed())
        return;

    auto person = this->model->person(this->clickedPerson);

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
        return;
    }

    this->personWidget->setRole(this->rolesModel->index(foundRow));

    this->personWidget->show();
}

void PersonsModule::abortPersonEdition()
{
    this->personWidget->hide();
}

void PersonsModule::completePersonEdition()
{
    if(not this->connection->open())
    {
        QMessageBox::critical(this, "Internal error",
                              QString("Creation could not be performed due to: %1").arg(this->connection->error().text));
        return;
    }

    dbapi::Person person(*this->model->person(this->clickedPerson)); // copy data from the model

    person.setFirstName(this->personWidget->firstName());
    person.setSecondName(this->personWidget->secondName());
    person.setBirthday(this->personWidget->date().toJulianDay());
    person.setRole(this->rolesModel->role(this->personWidget->role())->key());

    if(not person.update())
    {
        QMessageBox::warning(this, "Edition",
                             QString("Edition has failed due to: %1").arg(person.error().text));

        this->abortPersonEdition();
        this->connection->close();
        return;
    }

    this->connection->close();

    QMessageBox::information(this, "Edition", "Edited successfully");

    (*this->model->person(this->clickedPerson)) = person; // copy data back

    this->model->personNameEdited(this->clickedPerson); // tell model about change

    this->personWidget->hide();
}

void PersonsModule::loadPersons()
{
    if(not this->connection->open())
    {
        QMessageBox::critical(this, "Internal error",
                              QString("Loading could not be performed due to: %1").arg(this->connection->error().text));
        return;
    }

    auto error = this->model->loadAll();

    if(error.type != dbapi::ApiError::NoError)
        QMessageBox::warning(this, "Loading", QString("Loading of all persons is failed due to: %1").arg(error.text));

    this->connection->close();
}

bool PersonsModule::loadRolesIfNeed()
{
    if(this->rolesModel->rowCount() == 0)
    {
        if(not this->connection->open())
        {
            QMessageBox::critical(this, "Internal error",
                              QString("Action could not be performed due to: %1").arg(this->connection->error().text));
            return false;
        }

        auto error = this->rolesModel->loadAll();
        this->connection->close();

        if(error.type != dbapi::ApiError::NoError)
        {
            QMessageBox::critical(this, "Internal error", "You cannnot interact with any person due to roles downloading error");
            return false;
        }
    }

    return true;
}

