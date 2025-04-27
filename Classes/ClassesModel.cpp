#include "ClassesModel.h"

ClassesModel::ClassesModel(QObject *parent)
    : QAbstractListModel{parent}
{}

void ClassesModel::setConnection(dbapi::Connection *connection)
{
    this->connection = connection;
}

dbapi::ApiError ClassesModel::loadAll()
{
    dbapi::ApiError error;

    this->beginResetModel();

    // clear
    for(auto grade : this->classes)
        delete grade;

    // load new
    this->classes = dbapi::Class::loadAll(this->connection, &error);
    this->endResetModel();

    if(error.type != dbapi::ApiError::NoError)
        return error;

    return {};
}

dbapi::Class *ClassesModel::grade(const QModelIndex &index)
{
    return this->classes[index.row()];
}

dbapi::Class *ClassesModel::grade(int row)
{
    return this->classes[row];
}

int ClassesModel::rowCount(const QModelIndex &parent) const
{
    return this->classes.count();
}

QVariant ClassesModel::data(const QModelIndex &index, int role) const
{
    if(role != Qt::DisplayRole)
        return {};

    if(not index.isValid())
        return {};

    if(index.row() >= this->classes.count())
        return {};

    return this->classes[index.row()]->name();
}

void ClassesModel::insertRow(const dbapi::Class &grade)
{
    auto len = this->classes.size();

    beginInsertRows({}, len, len);

    this->classes.push_front(new dbapi::Class(grade));

    endInsertRows();
}

void ClassesModel::removeRow(int row)
{
    beginRemoveRows({}, row, row);

    delete this->classes[row];
    this->classes.remove(row);

    endRemoveRows();
}

ClassesModel::~ClassesModel()
{
    for(auto grade : this->classes)
        delete grade;
}
