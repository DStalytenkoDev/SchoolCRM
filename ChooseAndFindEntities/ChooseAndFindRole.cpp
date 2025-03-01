#include "ChooseAndFindRole.h"
#include "SchoolDatabaseApi/Role/Role.h"

ChooseAndFindRole::ChooseAndFindRole(dbapi::Connection* connection, QWidget* parent) noexcept : ChooseAndFindConcept(parent), connection(connection)
{
    connect(this, &ChooseAndFindRole::currentIndexChanged, this, &ChooseAndFindRole::indexChanging);
}

ChooseAndFindRole::ChooseAndFindRole(ChooseAndFindRole *dataSource, QWidget *parent) noexcept
    : ChooseAndFindConcept(parent)
{
    this->sync(dataSource);
}

dbapi::Role *ChooseAndFindRole::currentRole() const noexcept
{
    return this->roles[this->currentIndex()];
}

dbapi::ApiError ChooseAndFindRole::sync() noexcept
{
    this->cleanItemsAndRoles();

    dbapi::ApiError error;
    this->roles = dbapi::Role::loadAll(this->connection, &error);

    if(error.type != dbapi::ApiError::NoError)
        return error;

    for(auto role : this->roles)
        this->addItem(role->name());

    return {};
}

void ChooseAndFindRole::sync(ChooseAndFindRole *source) noexcept
{
    this->cleanItemsAndRoles();
    this->roles.reserve(source->roles.size());

    for(auto role : source->roles)
    {
        this->roles.push_back(new dbapi::Role(*role));
        this->addItem(role->name());
    }

    this->connection = source->connection;
}

void ChooseAndFindRole::setCurrentRole(dbapi::Role::Key key) noexcept
{
    int index = 0;

    for(auto role : this->roles)
    {
        if(role->key() == key)
            break;

        index++;
    }

    if(index < this->roles.size())
        this->setCurrentIndex(index);
}

ChooseAndFindRole::~ChooseAndFindRole()
{
    this->cleanItemsAndRoles();
}

void ChooseAndFindRole::indexChanging(int index)
{
    emit this->currentRoleChanged(this->roles[index]);
}

void ChooseAndFindRole::cleanItemsAndRoles()
{
    this->clear();

    for(auto role : this->roles)
        delete role;
}
