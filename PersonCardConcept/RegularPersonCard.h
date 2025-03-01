#ifndef REGULARPERSONCARD_H
#define REGULARPERSONCARD_H

#include <QLineEdit>
#include <SchoolDatabaseApi/Person/Person.h>
#include "PersonCardConcept.h"
#include "../ListItemConcept/MenuOfRegularListItem.h"


class RegularPersonCard : public PersonCardConcept
{
    Q_OBJECT

public:
    RegularPersonCard(dbapi::Person* person, ChooseAndFindRole* rolesSource, QWidget* parent = nullptr);
    ~RegularPersonCard();

signals:
    void removed();

private:
    dbapi::Person* person;
    dbapi::Person* tempPerson = nullptr;

    QLineEdit* firstName;
    QLineEdit* secondName;
    ChooseAndFindRole* roleChooser;
    MenuOfRegularListItem* toolBar;

    bool editingStarted = false;

    void cardOnEdit();
    void cardEdited();
    void editionCanceled();

    void firstNameEdited();
    void secondNameEdited();
    void dateEdited();
    void roleEdited();

    void fillUiWithPerson(dbapi::Person* person);

    void tryRemove() noexcept;

};

#endif // REGULARPERSONCARD_H
