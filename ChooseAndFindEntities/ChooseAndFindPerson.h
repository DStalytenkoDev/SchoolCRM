#ifndef CHOOSEANDFINDPERSON_H
#define CHOOSEANDFINDPERSON_H


#include "ChooseAndFindConcept.h"
#include <SchoolDatabaseApi/Person/Person.h>


class ChooseAndFindPerson : public ChooseAndFindConcept
{
    Q_OBJECT

public:
    ChooseAndFindPerson(dbapi::Connection* connection, QWidget* parent = nullptr) noexcept;

    dbapi::Person* currentPerson() const noexcept;
    dbapi::ApiError sync() noexcept;

    ~ChooseAndFindPerson();

signals:
    void currentPersonChanged(dbapi::Person* newPerson);

private:
    dbapi::Connection* connection;
    QVector<dbapi::Person*> persons;

    void indexChanging(int index);
    void cleanItemsAndPersons();
};


#endif // CHOOSEANDFINDPERSON_H
