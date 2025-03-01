#include <QMessageBox>
#include "NewPersonCard.h"


NewPersonCard::NewPersonCard(dbapi::Connection* connection, QWidget *parent) : PersonCardConcept(parent), person(connection)
{
    this->toolBar = new MenuOfNewListItem(this);
    this->firstName = new QLineEdit(this);
    this->secondName = new QLineEdit(this);

    this->roleChooser = new ChooseAndFindRole(connection, this);

    this->setToolBar(this->toolBar);
    this->setFirstNameWidget(this->firstName);
    this->setSecondNameWidget(this->secondName);
    this->setRolesWidget(this->roleChooser);

    connect(this->toolBar, &MenuOfNewListItem::cancelClicked, this, &NewPersonCard::canceled);
    connect(this->toolBar, &MenuOfNewListItem::loadUpClicked, this, &NewPersonCard::tryStore);
}

dbapi::ApiError NewPersonCard::syncRoles() noexcept
{
    return this->roleChooser->sync();
}

void NewPersonCard::syncRoles(ChooseAndFindRole *rolesSource) noexcept
{
    this->roleChooser->sync(rolesSource);
}

void NewPersonCard::tryStore() noexcept
{
    QDate date = this->dateWidget()->date();

    this->person.setFirstName(this->firstName->text());
    this->person.setSecondName(this->secondName->text());
    this->person.setRole(this->roleChooser->currentRole()->key());
    this->person.setBirthday(date.toJulianDay());

    auto message = QMessageBox(this);
    message.setStandardButtons(QMessageBox::Retry | QMessageBox::Abort | QMessageBox::Ok);
    message.setDefaultButton(QMessageBox::Abort);
    message.setIcon(QMessageBox::Critical);
    message.setText("Error while trying to store new person");
    message.setDetailedText("Wanna try again?");

    while(true)
    {
        if(this->person.store())
        {
            emit this->stored(&this->person);
            return;
        }

        int res = message.exec();

        switch(res)
        {
        case QMessageBox::Retry:
            break;
        case QMessageBox::Abort:
            emit this->canceled();
            return;
        case QMessageBox::Ok:
            return;
        default:
            return;
        };
    }

}
