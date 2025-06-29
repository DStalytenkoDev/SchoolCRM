#include "SubjectsOfTeacherModel.h"
#include <QMessageBox>

SubjectsOfTeacherModel::SubjectsOfTeacherModel(QObject *parent)
    : QAbstractListModel{parent}
{}

void SubjectsOfTeacherModel::setConnection(dbapi::Connection* connection)
{
    assert((void("nullptr"), connection != nullptr));

    this->connection = connection;
}

void SubjectsOfTeacherModel::setTeacher(const dbapi::Person::Key &key)
{
    this->teacher = key;
}

UserError SubjectsOfTeacherModel::loadSubjects()
{
    dbapi::TeacherSubjectsList list({this->teacher}, this->connection);

    if(not list.load())
    {
        if(list.error().type == dbapi::ApiError::KeyError)
        {
            this->clear();
            return {};
        }

        return UserError::internalError("Subjects", "be loaded 'cause an unknown error", "Try again or contact support");
    }

    this->beginResetModel();

    for(auto subject : this->subjects)
        delete subject;

    this->subjects.resize(list.subjects().count());

    for(int idx = 0; idx < this->subjects.size(); idx++)
        this->subjects[idx] = new dbapi::Subject(list.subjects()[idx], this->connection);

    UserError error;

    for(auto subject : this->subjects)
    {
        if(not subject->load())
        {
            for(auto subject : this->subjects)
                delete subject;

            this->subjects.clear();

            error = UserError::internalError("Subjects", "be loaded 'cause an unknown error", "Try again or contact support");

            break;
        }
    }

    this->endResetModel();
    return error;
}

UserError SubjectsOfTeacherModel::appendSubject(const dbapi::Subject::Key &key)
{
    dbapi::TeacherSubjectsList list({this->teacher}, this->connection);

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

UserError SubjectsOfTeacherModel::removeSubject(int index)
{
    assert((void("out of range"), index > 0 && index < this->subjects.size()));

    dbapi::TeacherSubjectsList list({this->teacher}, this->connection);

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

dbapi::Subject* SubjectsOfTeacherModel::subject(const QModelIndex &index)
{
    assert((void("out of range"), index.row() > 0 && index.row() < this->subjects.size()));

    return this->subjects[index.row()];
}

dbapi::Subject* SubjectsOfTeacherModel::subject(int row)
{
    assert((void("out of range"), row > 0 && row < this->subjects.size()));

    return this->subjects[row];
}

int SubjectsOfTeacherModel::rowCount(const QModelIndex &parent) const
{
    return this->subjects.count();
}

QVariant SubjectsOfTeacherModel::data(const QModelIndex &index, int role) const
{
    if(role != Qt::DisplayRole)
        return {};

    if(not index.isValid())
        return {};

    if(index.row() >= this->subjects.count())
        return {};

    return this->subjects[index.row()]->name();
}

void SubjectsOfTeacherModel::clear()
{
    if(this->subjects.size() == 0)
        return;

    this->beginResetModel();

    for(auto subject : this->subjects)
        delete subject;

    this->subjects.clear();

    this->endResetModel();
}

SubjectsOfTeacherModel::~SubjectsOfTeacherModel()
{
    for(auto subject : this->subjects)
        delete subject;
}
