#include <cassert>
#include "RolesModel.h"

RolesModel::RolesModel(QObject *parent)
    : QAbstractListModel{parent}
{}

void RolesModel::setConnection(dbapi::Connection *connection)
{
    assert((void("nullptr"), connection != nullptr));

    this->connection = connection;
}

UserError RolesModel::loadAll()
{
    dbapi::ApiError error;

    this->beginResetModel();

    for(auto role : this->roles)
        delete role;

    this->roles = dbapi::Role::loadAll(this->connection, &error);

    this->endResetModel();

    if(error.type != dbapi::ApiError::NoError)
        return UserError::internalError("Roles", "be loaded 'cause an unknown error", "Try again or contact support");

    return {};
}

UserError RolesModel::removeRole(int index)
{
    bool indexInRange = index >= 0 && index < this->roles.size();

    assert((void("out of range"), indexInRange));

    if(not indexInRange)
        return UserError::internalError("Roles", "be removed 'cause an unknown error", "Try again or contact support");

    auto role = this->roles[index];

    if(not role->remove())
        return UserError::referenceError("Role", "be removed 'cause its might be related", "Try first removing objects are using certain role");

    this->beginRemoveRows({}, index, index);

    delete role;
    this->roles.removeAt(index);

    this->endRemoveRows();

    return {};
}

UserError RolesModel::createRole(const QString &name)
{
    auto error = this->loadAll();

    if(error.isError())
        return UserError::internalError("Role", "be created 'cause an unknown error", "Try again or contact support");

    QString trimmedName = name.trimmed();

    if(trimmedName.isEmpty())
        return UserError::validityError("Role", "be created 'cause its empty");

    for(auto role : this->roles)
        if(role->name() == trimmedName)
            return UserError::validityError("Role", "be created 'cause it already exists");

    auto role = new dbapi::Role(this->connection);
    role->setName(trimmedName);

    if(not role->store())
    {
        delete role;
        return UserError::internalError("Role", "be created 'cause an unknown error", "Try again or contact support");
    }

    this->beginInsertRows({}, this->roles.size(), this->roles.size());
    this->roles.append(role);
    this->endInsertRows();

    return {};
}

dbapi::Role *RolesModel::role(int row)
{
    assert((void("out of range"), row >= 0 && row < this->roles.count()));

    return this->roles[row];
}

dbapi::Role* RolesModel::role(const QModelIndex &index)
{
    assert((void("out of range"), index.row() >= 0 && index.row() < this->roles.count()));

    return this->roles[index.row()];
}

int RolesModel::rowCount(const QModelIndex &parent) const
{
    return this->roles.size();
}

QVariant RolesModel::data(const QModelIndex &index, int role) const
{
    if(role != Qt::DisplayRole)
        return {};

    if(not index.isValid())
        return {};

    if(index.row() >= this->roles.count())
        return {};

    return this->roles[index.row()]->name();
}

void RolesModel::clear()
{
    if(this->roles.size() == 0)
        return;

    this->beginResetModel();

    for(auto role : this->roles)
        delete role;

    this->roles.clear();

    this->endResetModel();
}

RolesModel::~RolesModel()
{
    for(auto role : this->roles)
        delete role;
}
