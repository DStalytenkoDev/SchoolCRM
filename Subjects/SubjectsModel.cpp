#include <SchoolDatabaseApi/Subject/Subject.h>
#include "SubjectsModel.h"


SubjectsModel::SubjectsModel(QObject *parent)
    : QAbstractListModel{parent}
{}

void SubjectsModel::setConnection(dbapi::Connection *connection)
{
    this->connection = connection;
}

dbapi::ApiError SubjectsModel::loadAll()
{
    dbapi::ApiError error;

    this->cleanSubjects();

    this->subjects = dbapi::Subject::loadAll(this->connection, &error);

    if(error.type != dbapi::ApiError::NoError)
        return error;

    this->beginInsertRows({}, 0, this->subjects.size() - 1);
    this->endInsertRows();

    return {};
}

dbapi::Subject *SubjectsModel::subject(const QModelIndex &index)
{
    return this->subjects[index.row()];
}

dbapi::Subject *SubjectsModel::subject(int row)
{
    return this->subjects[row];
}

int SubjectsModel::rowCount(const QModelIndex &parent) const
{
    return this->subjects.size();
}

QVariant SubjectsModel::data(const QModelIndex &index, int role) const
{
    if(role != Qt::DisplayRole)
        return {};

    return this->subjects[index.row()]->name();
}

bool SubjectsModel::insertRow(int rowBefore, const dbapi::Subject &subject, const QModelIndex &parent)
{
    auto len = this->subjects.size();

    if(len == 0)
    {
        beginInsertRows({}, rowBefore, rowBefore);
        this->subjects.append(new dbapi::Subject(subject));
        endInsertRows();

        return true;
    }

    if(rowBefore >= 0 and rowBefore <= len)
    {
        beginInsertRows({}, rowBefore, rowBefore);
        this->subjects.insert(rowBefore, new dbapi::Subject(subject));
        endInsertRows();

        return true;
    }

    return false;
}

SubjectsModel::~SubjectsModel()
{
    for(auto subject : this->subjects)
        delete subject;
}

void SubjectsModel::cleanSubjects()
{
    auto len = this->subjects.size();

    if(len == 0)
        return;

    beginRemoveRows(QModelIndex(), 0, len - 1);

    for(auto subjects : this->subjects)
        delete subjects;

    this->subjects.clear();

    endRemoveRows();
}
