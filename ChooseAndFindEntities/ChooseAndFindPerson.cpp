#include "ChooseAndFindPerson.h"

ChooseAndFindPerson::ChooseAndFindPerson(dbapi::Connection* connection, QWidget* parent) noexcept
    : ChooseAndFindConcept(parent),
    connection(connection)
{
    connect(this, &ChooseAndFindPerson::currentIndexChanged, this, &ChooseAndFindPerson::indexChanging);
}

dbapi::ApiError ChooseAndFindPerson::sync() noexcept
{
    this->cleanItemsAndPersons();

    dbapi::ApiError error;
    this->persons = dbapi::Person::loadAll(connection, &error);

    if(error.type != dbapi::ApiError::NoError)
        return error;

    for(auto person : this->persons)
    {
        QString fullName = QString("%1 %2").arg(person->firstName(), person->secondName());
        this->addItem(fullName);
    }

    return {};
}

ChooseAndFindPerson::~ChooseAndFindPerson()
{
    this->cleanItemsAndPersons();
}

dbapi::Person* ChooseAndFindPerson::currentPerson() const noexcept
{
    return this->persons[this->currentIndex()];
}

void ChooseAndFindPerson::indexChanging(int index)
{
    emit this->currentPersonChanged(this->persons[this->currentIndex()]);
}

void ChooseAndFindPerson::cleanItemsAndPersons()
{
    this->clear();
    for(auto person : this->persons)
        delete person;
}
