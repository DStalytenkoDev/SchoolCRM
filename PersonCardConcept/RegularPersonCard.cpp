#include <SchoolDatabaseApi/Person/Person.h>
#include <QMessageBox>
#include "RegularPersonCard.h"
#include "../ListItemConcept/MenuOfNewListItem.h"

RegularPersonCard::RegularPersonCard(dbapi::Person *person, ChooseAndFindRole* rolesSource, QWidget *parent) : PersonCardConcept(parent), person(person)
{
    this->firstName = new QLineEdit(this);
    this->setFirstNameWidget(this->firstName);

    this->secondName = new QLineEdit(this);
    this->setSecondNameWidget(this->secondName);

    this->roleChooser = new ChooseAndFindRole(person->connection(), this);
    this->roleChooser->sync(rolesSource);
    this->setRolesWidget(this->roleChooser);

    this->toolBar = new MenuOfRegularListItem(this);
    this->setToolBar(this->toolBar);
    connect(this->toolBar, &MenuOfRegularListItem::removeClicked, this, &RegularPersonCard::tryRemove);

    connect(this->firstName, &QLineEdit::editingFinished, this, &RegularPersonCard::firstNameEdited);
    connect(this->secondName, &QLineEdit::editingFinished, this, &RegularPersonCard::secondNameEdited);
    connect(this->roleChooser, &ChooseAndFindRole::currentRoleChanged, this, &RegularPersonCard::roleEdited);
    connect(this->dateWidget(), &QDateEdit::editingFinished, this, &RegularPersonCard::dateEdited);

    this->fillUiWithPerson(this->person);
}

RegularPersonCard::~RegularPersonCard()
{
    delete this->person;

    if(this->tempPerson)
        delete this->tempPerson;
}

void RegularPersonCard::cardOnEdit()
{
    if(this->editingStarted)
        return;

    this->editingStarted = true;

    auto tempToolBar = new MenuOfNewListItem(this);
    this->setToolBar(tempToolBar);

    connect(tempToolBar, &MenuOfNewListItem::loadUpClicked, this, &RegularPersonCard::cardEdited);
    connect(tempToolBar, &MenuOfNewListItem::cancelClicked, this, &RegularPersonCard::editionCanceled);

    this->tempPerson = new dbapi::Person(*this->person);
}

void RegularPersonCard::cardEdited()
{
    auto message = QMessageBox(this);
    message.setStandardButtons(QMessageBox::Retry | QMessageBox::Abort | QMessageBox::Ok);
    message.setDefaultButton(QMessageBox::Abort);
    message.setIcon(QMessageBox::Critical);
    message.setText("Error while trying to change person");
    message.setDetailedText("Wanna try again?");

    while(true)
    {
        if(this->tempPerson->update())
            break;

        int res = message.exec();

        switch(res)
        {
        case QMessageBox::Retry:
            break;
        case QMessageBox::Abort:
            this->editionCanceled();
            return;
        case QMessageBox::Ok:
            return;
        default:
            return;
        };
    }

    this->editingStarted = false;

    this->toolBar = new MenuOfRegularListItem(this);
    this->setToolBar(this->toolBar);
    connect(this->toolBar, &MenuOfRegularListItem::removeClicked, this, &RegularPersonCard::tryRemove);

    delete this->person;

    this->person = this->tempPerson;
    this->tempPerson = nullptr;
}

void RegularPersonCard::editionCanceled()
{
    delete this->tempPerson;
    this->tempPerson = nullptr;

    this->editingStarted = false;

    this->toolBar = new MenuOfRegularListItem(this);
    this->setToolBar(this->toolBar);
    connect(this->toolBar, &MenuOfRegularListItem::removeClicked, this, &RegularPersonCard::tryRemove);

    this->fillUiWithPerson(this->person);
}

void RegularPersonCard::firstNameEdited()
{
    this->tempPerson->setFirstName(this->firstName->text());
}

void RegularPersonCard::secondNameEdited()
{
    this->tempPerson->setSecondName(this->secondName->text());
}

void RegularPersonCard::dateEdited()
{
    this->tempPerson->setBirthday(this->dateWidget()->date().toJulianDay());
}

void RegularPersonCard::roleEdited()
{
    this->tempPerson->setRole(this->roleChooser->currentRole()->key());
}

void RegularPersonCard::fillUiWithPerson(dbapi::Person *person)
{
    this->firstName->setText(person->firstName());
    this->secondName->setText(person->secondName());
    this->roleChooser->setCurrentRole(person->role());
    this->dateWidget()->setDate(QDate::fromJulianDay(person->birthday()));
}

void RegularPersonCard::tryRemove() noexcept
{
    auto message = QMessageBox(this);
    message.setStandardButtons(QMessageBox::Retry | QMessageBox::Abort | QMessageBox::Ok);
    message.setDefaultButton(QMessageBox::Abort);
    message.setIcon(QMessageBox::Critical);
    message.setText("Error while trying remove person");
    message.setDetailedText("Wanna try again?");

    while(true)
    {
        if(this->person->remove())
        {
            emit this->removed();
            this->setEnabled(false);
            return;
        }

        int res = message.exec();

        switch(res)
        {
        case QMessageBox::Retry:
            break;
        case QMessageBox::Abort:
            this->editionCanceled();
            return;
        case QMessageBox::Ok:
            return;
        default:
            return;
        };
    }
}
