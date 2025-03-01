#ifndef NEWPERSONCARD_H
#define NEWPERSONCARD_H

#include <SchoolDatabaseApi/Person/Person.h>
#include "PersonCardConcept.h"
#include "../ListItemConcept/MenuOfNewListItem.h"

class NewPersonCard : public PersonCardConcept
{
    Q_OBJECT

public:
    NewPersonCard(dbapi::Connection* connection, QWidget* parent = nullptr);

    dbapi::ApiError syncRoles() noexcept;
    void syncRoles(ChooseAndFindRole* rolesSource) noexcept;

signals:
    void canceled();
    void stored(dbapi::Person* person);

private:
    dbapi::Person person;

    MenuOfNewListItem* toolBar;
    ChooseAndFindRole* roleChooser;
    QLineEdit* firstName;
    QLineEdit* secondName;

    void  tryStore() noexcept;
};

#endif // NEWPERSONCARD_H
