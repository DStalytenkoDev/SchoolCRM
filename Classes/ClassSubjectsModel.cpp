#include "ClassSubjectsModel.h"


ClassSubjectsModel::ClassSubjectsModel(QObject *parent)
    : QAbstractListModel{parent}
{}

void ClassSubjectsModel::setConnection(dbapi::Connection *connection)
{
    this->connection = connection;
}

dbapi::ApiError ClassSubjectsModel::loadSubjects(const dbapi::Class::Key &key)
{
    dbapi::ClassSubjectsList list({key}, this->connection);

    this->items.clear();

    list.load();

    if(list.error().type != dbapi::ApiError::NoError)
        if(list.error().type != dbapi::ApiError::KeyError)
            return list.error();

    int len = list.subjects().count();
    dbapi::Subject subject(this->connection);

    this->beginResetModel();
    this->items.resize(len);

    for(int idx = 0; idx < len; idx++)
    {
        subject.setKey(list.subjects()[idx]);

        if(not subject.load())
        {
            this->endInsertRows();
            return subject.error();
        }

        this->items[idx].key = subject.key();
        this->items[idx].text = subject.name();
    }

    this->endResetModel();
    return {};
}

dbapi::Subject::Key ClassSubjectsModel::subject(const QModelIndex &index)
{
    return this->items[index.row()].key;
}

dbapi::Subject::Key ClassSubjectsModel::subject(int row)
{
    return this->items[row].key;
}

int ClassSubjectsModel::rowCount(const QModelIndex &parent) const
{
    return this->items.count();
}

QVariant ClassSubjectsModel::data(const QModelIndex &index, int role) const
{
    if(role != Qt::DisplayRole)
        return {};

    if(not index.isValid())
        return {};

    if(index.row() >= this->items.count())
        return {};

    return this->items[index.row()].text;
}

void ClassSubjectsModel::insertRow(const dbapi::Subject &subject)
{
    auto len = this->items.size();

    beginInsertRows({}, len, len);

    this->items.push_front({subject.name(), subject.key()});

    endInsertRows();
}

void ClassSubjectsModel::removeRow(int row)
{
    beginRemoveRows({}, row, row);

    this->items.remove(row);

    endRemoveRows();
}
