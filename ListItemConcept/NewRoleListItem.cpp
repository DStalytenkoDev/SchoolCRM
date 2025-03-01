#include "NewRoleListItem.h"
#include <QMessageBox>


NewRoleListItem::NewRoleListItem(dbapi::Connection* connection, QWidget* parent) noexcept
    : ListItemConcept(parent),
    role(connection)
{
    this->lineEditOfRoleName = new QLineEdit(this);
    this->getLeftLayout()->addWidget(this->lineEditOfRoleName);

    this->toolBar = new MenuOfNewListItem(this);
    this->setRightToolBar(this->toolBar);

    connect(this->toolBar, &MenuOfNewListItem::loadUpClicked, this, &NewRoleListItem::tryStore);
    connect(this->toolBar, &MenuOfNewListItem::cancelClicked, this, &NewRoleListItem::canceled);
}

void NewRoleListItem::tryStore() noexcept
{
    this->role.setName(this->lineEditOfRoleName->text());

    auto message = QMessageBox(this);
    message.setStandardButtons(QMessageBox::Retry | QMessageBox::Abort | QMessageBox::Ok);
    message.setDefaultButton(QMessageBox::Abort);
    message.setIcon(QMessageBox::Critical);
    message.setText("Error while trying to store new role");
    message.setDetailedText("Wanna try again?");

    while(true)
    {
        if(this->role.store())
        {
            emit this->stored(&this->role);
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
