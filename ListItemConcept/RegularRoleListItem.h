#ifndef REGULARROLELISTITEM_H
#define REGULARROLELISTITEM_H


#include <QLabel>
#include <SchoolDatabaseApi/Role/Role.h>
#include "ListItemConcept.h"
#include "MenuOfRegularListItem.h"


class RegularRoleListItem : public ListItemConcept
{
    Q_OBJECT

public:
    RegularRoleListItem(dbapi::Role* role, QWidget* parent = nullptr);
    ~RegularRoleListItem();

signals:
    void removed();

private:
    MenuOfRegularListItem* toolBar;
    QLabel* roleLabel;
    dbapi::Role* role;

    void tryRemove();
};


#endif // REGULARROLELISTITEM_H
