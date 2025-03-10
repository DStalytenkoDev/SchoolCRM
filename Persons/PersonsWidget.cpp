#include "PersonsWidget.h"
#include "ui_PersonsWidget.h"
#include <qmessagebox.h>

PersonsWidget::PersonsWidget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::PersonsWidget)
{
    ui->setupUi(this);

    // create person dialog
    this->personDialog = new CreatePersonDialog(this);

    // person control for an item
    this->personWidget = new PersonWidget(this);
    this->personWidget->hide();
    this->ui->bottomlayout->addWidget(this->personWidget);

    // model
    this->model = new PersonsModel(this);
    this->ui->personsList->setModel(this->model);

    this->ui->deletePerson->setEnabled(false);
}

void PersonsWidget::setConnection(dbapi::Connection *connection)
{
    this->connection = connection;
}

PersonsWidget::~PersonsWidget()
{
    delete ui;
}

void PersonsWidget::loadPersons()
{
    auto error = this->model->loadAll();

    if(error.type != dbapi::ApiError::NoError)
    {
        QMessageBox::warning(this, "Load of Persons", "Loading is failed");
        return;
    }
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
