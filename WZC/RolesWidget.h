#ifndef ROLESWIDGET_H
#define ROLESWIDGET_H


#include <SchoolDatabaseApi/Role/Role.h>
#include "ListedAddAndFindWZC.h"


class RolesWidget : public ListedAddAndFindWZC
{
    Q_OBJECT

public:
    RolesWidget(dbapi::Connection* connection, QWidget* parent);

    dbapi::ApiError sync();

private:
    dbapi::Connection* connection;

    void addNewRole();
};


#endif // ROLESWIDGET_H
