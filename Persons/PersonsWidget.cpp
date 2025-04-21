#include "PersonsWidget.h"
#include "ui_PersonsWidget.h"
#include <qdatetime.h>
#include <qmessagebox.h>

PersonsWidget::PersonsWidget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::PersonsWidget)
{
    ui->setupUi(this);

    // create person dialog
    this->personDialog = new CreatePersonDialog(this);
    connect(this->personDialog, &CreatePersonDialog::finished, this, &PersonsWidget::completePersonCreation);

    // person widget for an item
    this->personWidget = new ShowPerson(this);
    this->personWidget->hide();
    this->ui->bottomlayout->addWidget(this->personWidget);
    connect(this->personWidget, &ShowPerson::updatePerson, this, &PersonsWidget::updatePerson);

    // model
    this->model = new PersonsModel(this);
    this->ui->personsList->setModel(this->model);

    this->ui->deletePerson->setEnabled(false);
}

void PersonsWidget::setConnection(dbapi::Connection *connection)
{
    this->connection = connection;
    this->currentPerson.setConnection(connection);
}

PersonsWidget::~PersonsWidget()
{
    delete ui;
}

void PersonsWidget::loadPersons()
{
    auto error = this->model->loadAll();

    if(error.type != dbapi::ApiError::NoError)
        QMessageBox::warning(this, "Load of Persons", "Loading is failed");
}

void PersonsWidget::updatePerson()
{
    this->currentPerson.setFirstName(this->personWidget->firstName());
    this->currentPerson.setSecondName(this->personWidget->secondName());
    this->currentPerson.setBirthday(this->personWidget->date().toJulianDay());
    this->currentPerson.setRole(this->personWidget->role()->)
}

void PersonsWidget::deletePerson()
{

}

void PersonsWidget::createPerson()
{
}

void PersonsWidget::completePersonCreation()
{
}

void PersonsWidget::manageSelection(const QItemSelection &selected, const QItemSelection &deselected)
{

}
