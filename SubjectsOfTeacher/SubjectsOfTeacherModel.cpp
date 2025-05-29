#include "SubjectsOfTeacherModel.h"
#include <QMessageBox>

SubjectsOfTeacherModel::SubjectsOfTeacherModel(QObject *parent)
    : QAbstractListModel{parent}
{}

void SubjectsOfTeacherModel::setConnection(dbapi::Connection* connection)
{
    this->connection = connection;
}

dbapi::ApiError SubjectsOfTeacherModel::loadSubjects(const dbapi::Person::Key& key)
{
    dbapi::TeacherSubjectsList list({key}, this->connection);

    if(not list.load())
        return list.error();

    this->beginResetModel();

    int len = list.subjects().count();
    dbapi::Subject subject(this->connection);

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

    this->connection->close();
    this->endResetModel();
    return {};
}

dbapi::Subject::Key SubjectsOfTeacherModel::subject(const QModelIndex &index)
{
    return this->items[index.row()].key;
}

dbapi::Subject::Key SubjectsOfTeacherModel::subject(int row)
{
    return this->items[row].key;
}

int SubjectsOfTeacherModel::rowCount(const QModelIndex &parent) const
{
    return this->items.count();
}

QVariant SubjectsOfTeacherModel::data(const QModelIndex &index, int role) const
{
    if(role != Qt::DisplayRole)
        return {};

    if(not index.isValid())
        return {};

    if(index.row() >= this->items.count())
        return {};

    return this->items[index.row()].text;
}

void SubjectsOfTeacherModel::insertRow(const dbapi::Subject &subject)
{
    auto len = this->items.size();

    beginInsertRows({}, len, len);

    this->items.push_front({subject.name(), subject.key()});

    endInsertRows();
}

void SubjectsOfTeacherModel::removeRow(int row)
{
    beginRemoveRows({}, row, row);

    this->items.remove(row);

    endRemoveRows();
}

void SubjectsOfTeacherModel::clear()
{
    this->beginResetModel();

    this->items.clear();

    this->endResetModel();
}
