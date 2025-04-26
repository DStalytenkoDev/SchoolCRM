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

    this->cleanPersons();

    this->persons = dbapi::Person::loadAll(this->connection, &error);

    if(error.type != dbapi::ApiError::NoError)
        return error;

    this->beginInsertRows(QModelIndex(), 0, persons.size() - 1);

    this->beginInsertRows({}, 0, this->persons.size() - 1);
    this->endInsertRows();

    return {};
}

dbapi::Person *PersonsModel::person(const QModelIndex &index)
{
    return this->persons[index.row()];
}

dbapi::Person *PersonsModel::person(int row)
{
    return this->persons[row];
}

void PersonsModel::personNameEdited(const QModelIndex &index)
{
    emit this->dataChanged(index, index, {Qt::DisplayRole});
}

int PersonsModel::rowCount(const QModelIndex &parent) const
{
    return this->persons.size();
}

QVariant PersonsModel::data(const QModelIndex &index, int role) const
{
    if(role != Qt::DisplayRole)
        return {};

    if(not index.isValid())
        return {};

    if(index.row() >= this->persons.count())
        return {};

    QString fullName;
    auto person = this->persons[index.row()];

    fullName.append(person->firstName());
    fullName.append(' ');
    fullName.append(person->secondName());

    return fullName;
}

bool PersonsModel::insertRow(int rowBefore, const dbapi::Person &person, const QModelIndex &parent)
{
    auto len = this->persons.size();

    if(len == 0)
    {
        beginInsertRows({}, rowBefore, rowBefore);
        this->persons.append(new dbapi::Person(person));
        endInsertRows();

        return true;
    }

    if(rowBefore >= 0 and rowBefore <= len)
    {
        beginInsertRows({}, rowBefore, rowBefore);
        this->persons.insert(rowBefore, new dbapi::Person(person));
        endInsertRows();

        return true;
    }

    return false;
}

PersonsModel::~PersonsModel()
{
    for(auto person : this->persons)
        delete person;
}

void PersonsModel::cleanPersons()
{
    auto len = this->persons.size();

    if(len == 0)
        return;

    beginRemoveRows(QModelIndex(), 0, len - 1);

    for(auto person : this->persons)
        delete person;

    this->persons.clear();

    endRemoveRows();
}
