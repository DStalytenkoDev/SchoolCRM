#include "PersonsModel.h"

PersonsModel::PersonsModel(QObject *parent) : QAbstractListModel(parent)
{
}

void PersonsModel::setConnection(dbapi::Connection *connection)
{
    this->connection = connection;
}

dbapi::ApiError PersonsModel::loadAll()
{
    dbapi::ApiError error;
    auto persons = dbapi::Person::loadAll(this->connection, &error);

    if(error.type != dbapi::ApiError::NoError)
        return error;

    this->beginInsertRows(QModelIndex(), 0, persons.size() - 1);

    this->items.clear();
    this->items.reserve(persons.size());

    for(auto person : persons)
    {
        QString fullName = QString("%1 %2").arg(person->firstName(), person->secondName());

        this->items.emplaceBack(fullName, person->key());

        delete person;
    }

    this->endInsertRows();

    return {};
}

dbapi::Person::Key PersonsModel::personKeyByIndex(const QModelIndex &index)
{
    if(index.isValid())
        return this->items[index.row()].key;

    return dbapi::Person::Key{-1};
}

int PersonsModel::rowCount(const QModelIndex &parent) const
{
    return this->items.count();
}

QVariant PersonsModel::data(const QModelIndex &index, int role) const
{
    if(index.isValid() and role == Qt::DisplayRole)
        return this->items[index.row()].name;

    return {};
}

PersonsModel::Item::Item(const QString &name, const dbapi::Person::Key &key)
{
    this->name = name;
    this->key = key;
}
