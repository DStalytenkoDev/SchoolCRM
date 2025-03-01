#ifndef NEWROLELISTITEM_H
#define NEWROLELISTITEM_H

#include "ListItemConcept.h"
#include "MenuOfNewListItem.h"
#include <SchoolDatabaseApi/Role/Role.h>
#include <QLineEdit>

class NewRoleListItem : public ListItemConcept
{
    Q_OBJECT

public:
    NewRoleListItem(dbapi::Connection* connection, QWidget* parent = nullptr) noexcept;

signals:
    void canceled();
    void stored(dbapi::Role* key);

private:
    dbapi::Role role;

    MenuOfNewListItem* toolBar;
    QLineEdit* lineEditOfRoleName;


    void tryStore() noexcept;
};

#endif // NEWROLELISTITEM_H
