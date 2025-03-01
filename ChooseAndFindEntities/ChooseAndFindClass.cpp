#include "ChooseAndFindClass.h"

ChooseAndFindClass::ChooseAndFindClass(dbapi::Connection* connection, QWidget* parent) noexcept
    : ChooseAndFindConcept(parent),
    connection(connection)
{
    connect(this, &ChooseAndFindClass::currentIndexChanged, this, &ChooseAndFindClass::indexChanging);
}

dbapi::ApiError ChooseAndFindClass::sync() noexcept
{
    this->cleanItemsAndClasses();

    dbapi::ApiError error;
    this->classes = dbapi::Class::loadAll(connection, &error);

    if(error.type != dbapi::ApiError::NoError)
        return error;

    for(auto _class : this->classes)
        this->addItem(_class->name());

    return error;
}

ChooseAndFindClass::~ChooseAndFindClass()
{
    this->cleanItemsAndClasses();
}

dbapi::Class* ChooseAndFindClass::currentClass() const noexcept
{
    auto index = this->currentIndex();

    if(index < 0)
        return nullptr;

    return this->classes[index];
}

void ChooseAndFindClass::indexChanging(int index)
{
    auto newClass = this->currentClass();

    if(newClass)
        emit this->currentClassChanged(newClass);
}

void ChooseAndFindClass::cleanItemsAndClasses()
{
    this->clear();

    for(auto _class : this->classes)
        delete _class;
}
