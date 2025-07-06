#include <SchoolApi/Subject.h>
#include "SubjectsModel.h"


SubjectsModel::SubjectsModel(QObject *parent)
    : QAbstractListModel{parent}
{}

void SubjectsModel::setConnection(dbapi::Connection *connection)
{
    assert((void("nullptr"), connection != nullptr));

    this->connection = connection;
}

UserError SubjectsModel::loadAll()
{
    dbapi::ApiError error;

    this->beginResetModel();

    for(auto subject : this->subjects)
        delete subject;

    this->subjects = dbapi::Subject::loadAll(this->connection, &error);

    this->endResetModel();

    if(error.type != dbapi::ApiError::NoError)
        return UserError::internalError("Subjects", "be loaded 'cause an unknown error", "Try again or contact support");

    return {};
}

UserError SubjectsModel::removeSubject(int index)
{
    bool indexInRange = index >= 0 && index < this->subjects.size();

    assert((void("out of range"), indexInRange));

    if(not indexInRange)
        return UserError::internalError("Subjects", "be removed 'cause an unknown error", "Try again or contact support");

    auto subject = this->subjects[index];

    if(not subject->remove())
        return UserError::referenceError("Subject", "be removed 'cause its might be related", "Try first removing objects are using certain subject");

    this->beginRemoveRows({}, index, index);

    delete subject;
    this->subjects.removeAt(index);

    this->endRemoveRows();

    return {};
}

UserError SubjectsModel::createSubject(const QString &name)
{
    auto error = this->loadAll();

    if(error.isError())
        return UserError::internalError("Subject", "be created 'cause an unknown error", "Try again or contact support");

    QString trimmedName = name.trimmed();

    if(trimmedName.isEmpty())
        return UserError::validityError("Subject", "be created 'cause its empty");

    for(auto subject : this->subjects)
        if(subject->name() == trimmedName)
            return UserError::validityError("Subject", "be created 'cause it already exists");

    auto subject = new dbapi::Subject(this->connection);
    subject->setName(trimmedName);

    if(not subject->store())
    {
        delete subject;
        return UserError::internalError("Subject", "be created 'cause an unknown error", "Try again or contact support");
    }

    this->beginInsertRows({}, this->subjects.size(), this->subjects.size());
    this->subjects.append(subject);
    this->endInsertRows();

    return {};
}

dbapi::Subject *SubjectsModel::subject(const QModelIndex &index)
{
    assert((void("out of range"), index.row() >= 0 && index.row() < this->subjects.count()));

    return this->subjects[index.row()];
}

dbapi::Subject *SubjectsModel::subject(int row)
{
    assert((void("out of range"), row >= 0 && row < this->subjects.count()));

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

    if(not index.isValid())
        return {};

    if(index.row() >= this->subjects.count())
        return {};

    return this->subjects[index.row()]->name();
}

void SubjectsModel::clear()
{
    if(this->subjects.size() == 0)
        return;

    this->beginResetModel();

    for(auto subject : this->subjects)
        delete subject;

    this->subjects.clear();

    this->endResetModel();
}

SubjectsModel::~SubjectsModel()
{
    for(auto subject : this->subjects)
        delete subject;
}
