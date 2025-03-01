#include "RolesWidget.h"
#include "../ListItemConcept/RegularRoleListItem.h"
#include "../ListItemConcept/NewRoleListItem.h"

RolesWidget::RolesWidget(dbapi::Connection *connection, QWidget *parent) : ListedAddAndFindWZC(parent), connection(connection)
{
    connect(this, &RolesWidget::addButtonClicked, this, &RolesWidget::addNewRole);
}

dbapi::ApiError RolesWidget::sync()
{
    this->getListWidget()->clear();

    dbapi::ApiError error;

    auto roles = dbapi::Role::loadAll(this->connection, &error);

    if(error.type != dbapi::ApiError::NoError)
        return error;

    for(auto role : roles)
    {
        RegularRoleListItem* roleItemWidget = new RegularRoleListItem(role, this);
        QListWidgetItem* listItem = new QListWidgetItem;

        this->getListWidget()->addItem(listItem);
        this->getListWidget()->setItemWidget(listItem, roleItemWidget);

        connect(roleItemWidget, &RegularRoleListItem::removed, [=](){
            this->getListWidget()->
        })


    }


}

void RolesWidget::addNewRole()
{

}
