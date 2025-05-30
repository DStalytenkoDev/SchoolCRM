#include "RolesModel.h"

RolesModel::RolesModel(QObject *parent)
    : QAbstractListModel{parent}
{}

void RolesModel::setConnection(dbapi::Connection *connection)
{
    this->connection = connection;
}

dbapi::ApiError RolesModel::loadAll()
{
    dbapi::ApiError error;

    this->cleanRoles();

    this->roles = dbapi::Role::loadAll(this->connection, &error);

    if(error.type != dbapi::ApiError::NoError)
        return error;

    this->beginInsertRows({}, 0, this->roles.size() - 1);
    this->endInsertRows();

    return {};
}

dbapi::Role *RolesModel::role(int row)
{
    return this->roles[row];
}

dbapi::Role* RolesModel::role(const QModelIndex &index)
{
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

bool RolesModel::insertRow(int rowBefore, const dbapi::Role &role, const QModelIndex &parent)
{
    auto len = this->roles.size();

    if(len == 0)
    {
        beginInsertRows({}, rowBefore, rowBefore);
        this->roles.append(new dbapi::Role(role));
        endInsertRows();

        return true;
    }

    if(rowBefore >= 0 and rowBefore <= len)
    {
        beginInsertRows({}, rowBefore, rowBefore);
        this->roles.insert(rowBefore, new dbapi::Role(role));
        endInsertRows();

        return true;
    }

    return false;
}

void RolesModel::clear()
{
    this->cleanRoles();
}

RolesModel::~RolesModel()
{
    for(auto role : this->roles)
        delete role;
}

void RolesModel::cleanRoles()
{
    auto len = this->roles.size();

    if(len == 0)
        return;

    beginRemoveRows(QModelIndex(), 0, len - 1);

    for(auto role : this->roles)
        delete role;

    this->roles.clear();

    endRemoveRows();
}
