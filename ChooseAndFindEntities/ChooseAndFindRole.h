#ifndef CHOOSEANDFINDROLE_H
#define CHOOSEANDFINDROLE_H

#include "ChooseAndFindConcept.h"
#include <SchoolDatabaseApi/Role/Role.h>

class ChooseAndFindRole : public ChooseAndFindConcept
{
    Q_OBJECT

public:
    ChooseAndFindRole(dbapi::Connection* connection, QWidget* parent = nullptr) noexcept;
    ChooseAndFindRole(ChooseAndFindRole* dataSource, QWidget* parent = nullptr) noexcept;

    dbapi::Role* currentRole() const noexcept;

    dbapi::ApiError sync() noexcept;
    void sync(ChooseAndFindRole* source) noexcept;

    void setCurrentRole(dbapi::Role::Key key) noexcept;

    ~ChooseAndFindRole();

signals:
    void currentRoleChanged(dbapi::Role* role);

private:
    dbapi::Connection* connection;
    QVector<dbapi::Role*> roles;

    void indexChanging(int index);
    void cleanItemsAndRoles();
};

#endif // CHOOSEANDFINDROLE_H
