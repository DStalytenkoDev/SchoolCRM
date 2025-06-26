#include "RolesModel.h"

RolesModel::RolesModel(QObject *parent)
    : QAbstractListModel{parent}
{}

void RolesModel::setConnection(dbapi::Connection *connection)
{
    this->connection = connection;
}

UserError RolesModel::loadAll()
{
    dbapi::ApiError error;

    this->beginResetModel();
    this->clear();

    this->roles = dbapi::Role::loadAll(this->connection, &error);

    if(error.type != dbapi::ApiError::NoError)
        return error;

    this->beginInsertRows({}, 0, this->roles.size() - 1);
    this->endInsertRows();

    return {};
}

UserError RolesModel::removeRoles(int start, int end)
{

}

UserError RolesModel::createRole(const QString &name)
{

}

dbapi::Role *RolesModel::role(int row)
{
    if(row >= this->roles.count())
        return nullptr;

    return this->roles[row];
}

dbapi::Role* RolesModel::role(const QModelIndex &index)
{
    if(index.row() >= this->roles.count())
        return nullptr;

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
