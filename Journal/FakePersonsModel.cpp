#include "FakePersonsModel.h"

FakePersonsModel::FakePersonsModel(QObject *parent)
    : QAbstractListModel{parent}
{}

void FakePersonsModel::createPerson(const dbapi::Person::Key &key, const QString &firstName, const QString &secondName)
{
    QString fullName = QString("%1 %2").arg(firstName, secondName);

    this->beginInsertRows({}, this->persons.size(), this->persons.size());
    this->persons.emplaceBack(key, fullName);
}

void FakePersonsModel::removePerson(int index)
{
    assert((void("out of range"), index >= 0 && index < this->persons.size()));

    this->beginRemoveRows({}, index, index);
    this->persons.remove(index);
    this->endRemoveRows();
}

void FakePersonsModel::clear()
{
    this->beginResetModel();
    this->persons.clear();
    this->endResetModel();
}

const dbapi::Person::Key &FakePersonsModel::person(int index)
{
    assert((void("out of range"), index >= 0 && index < this->persons.size()));

    return this->persons[index].first;
}

int FakePersonsModel::rowCount(const QModelIndex &index) const
{
    return this->persons.size();
}

QVariant FakePersonsModel::data(const QModelIndex &index, int role) const
{
    if(role != Qt::DisplayRole)
        return {};

    if(not index.isValid())
        return {};

    if(index.row() >= this->persons.size())
        return {};

    return this->persons[index.row()].second;
}
