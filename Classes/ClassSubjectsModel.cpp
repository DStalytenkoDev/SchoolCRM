#include "ClassSubjectsModel.h"


ClassSubjectsModel::ClassSubjectsModel(QObject *parent)
    : QAbstractListModel{parent}
{}

void ClassSubjectsModel::setConnection(dbapi::Connection *connection)
{
    assert((void("nullptr"), connection));

    this->connection = connection;
}

void ClassSubjectsModel::setClass(const dbapi::Class::Key &key)
{
    this->classKey = key;
}

UserError ClassSubjectsModel::loadAll()
{
    dbapi::ClassSubjectsList list({this->classKey}, this->connection);

    if(not list.load())
    {
        if(list.error().type != dbapi::ApiError::KeyError)
            return UserError::internalError("Subjects", "be loadded 'cause an unknown error", "Try again or contact support");

        this->clear();
        return {};
    }

    this->beginResetModel();

    for(auto subject : this->subjects)
        delete subject;

    this->subjects.clear();
    this->subjects.reserve(subjects.size());

    for(auto subjectKey : list.subjects())
    {
        this->subjects.append(new dbapi::Subject(subjectKey, this->connection));

        if(not this->subjects.back()->load())
        {
            delete this->subjects.back();
            this->subjects.pop_back();
            this->endResetModel();

            return UserError::internalError("Subjects", "be loaded 'cause an unknown error", "Try again or contact support");
        }
    }

    this->endResetModel();
    return {};
}

UserError ClassSubjectsModel::appendSubject(const dbapi::Subject::Key &key)
{
    dbapi::ClassSubjectsList list({this->classKey}, this->connection);

    if(not list.load())
        return UserError::internalError("Subject", "be appended 'cause an unknown error", "Try again or contact support");

    for(auto& subject : list.subjects())
        if(subject == key)
            return UserError::keyError("Subject", "be appended 'cause its already in the list");

    list.appendSubject(key);
    if(not list.store())
        return UserError::internalError("Subject", "be appended 'cause an unknown error", "Try again or contact support");

    auto subject = new dbapi::Subject(key, this->connection);

    if(not subject->load())
        subject->setName("_Error_value_");

    this->beginInsertRows({}, this->subjects.size(), this->subjects.size());
    this->subjects.append(subject);
    this->endInsertRows();

    return {};
}

UserError ClassSubjectsModel::removeSubject(int index)
{
    assert((void("out of range"), index > 0 && index < this->subjects.size()));

    dbapi::ClassSubjectsList list({this->classKey}, this->connection);

    auto subject = this->subjects[index];
    list.appendSubject(subject->key());

    if(not list.remove())
        return UserError::internalError("Subject", "be removed 'cause an unknown error", "Try again or contact support");

    this->beginRemoveRows({}, index, index);

    delete subject;
    this->subjects.remove(index);

    this->endRemoveRows();

    return {};
}

dbapi::Subject* ClassSubjectsModel::subject(const QModelIndex &index)
{
    assert((void("out of range"), index.row() > 0 && index.row() < this->subjects.size()));

    return this->subjects[index.row()];
}

dbapi::Subject *ClassSubjectsModel::subject(int row)
{
    assert((void("out of range"), row > 0 && row < this->subjects.size()));

    return this->subjects[row];
}

int ClassSubjectsModel::rowCount(const QModelIndex &parent) const
{
    return this->subjects.count();
}

QVariant ClassSubjectsModel::data(const QModelIndex &index, int role) const
{
    if(role != Qt::DisplayRole)
        return {};

    if(not index.isValid())
        return {};

    if(index.row() >= this->subjects.count())
        return {};

    return this->subjects[index.row()]->name();
}

void ClassSubjectsModel::clear()
{
    if(this->subjects.size() == 0)
        return;

    this->beginResetModel();

    for(auto subject : this->subjects)
        delete subject;

    this->endResetModel();
}
