#include <QMessageBox>
#include "RegularRoleListItem.h"


RegularRoleListItem::RegularRoleListItem(dbapi::Role *role, QWidget *parent) : ListItemConcept(parent), role(role)
{
    this->roleLabel = new QLabel(this);
    this->roleLabel->setText(this->role->name());

    this->toolBar = new MenuOfRegularListItem(this);

    this->setRightToolBar(this->toolBar);
    this->getLeftLayout()->addWidget(this->roleLabel);

    connect(this->toolBar, &MenuOfRegularListItem::removeClicked, this, &RegularRoleListItem::tryRemove);
}

RegularRoleListItem::~RegularRoleListItem()
{
    delete this->role;
}

void RegularRoleListItem::tryRemove()
{
    auto message = QMessageBox(this);
    message.setStandardButtons(QMessageBox::Ok);
    message.setDefaultButton(QMessageBox::Ok);
    message.setIcon(QMessageBox::Critical);
    message.setText("Error while tryna remove the subject");

    if(not this->role->remove())
    {
        message.exec();
        return;
    }

    emit this->removed();
}
