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
        return UserError::internalError("Subjects", "be loaded 'cause an unknown error", "Try again or contact support");

    this->beginResetModel();

    this->subjects.resize(list.subjects().count());

    for(int idx = 0; idx < this->subjects.size(); idx++)
        this->subjects[idx] = new dbapi::Subject(list.subjects()[idx], this->connection);

    UserError error;

    for(auto subject : this->subjects)
    {
        if(subject->load())
        {
            for(auto subject : this->subjects)
                delete subject;

            this->subjects.clear();

            error = UserError::internalError("Subjects", "be loaded 'cause an unknown error", "Try again or contact support");

            break;
        }
    }

    this->connection->close();
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

    this->subjects.append(subject);

    return {};
}

UserError SubjectsOfTeacherModel::removeSubject(int index)
{
    assert((void("out of range"), index > 0 && index < this->subjects.size()));


}

dbapi::Subject::Key SubjectsOfTeacherModel::subject(const QModelIndex &index)
{
    return this->subjects[index.row()].key;
}

dbapi::Subject::Key SubjectsOfTeacherModel::subject(int row)
{
    return this->subjects[row].key;
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

    return this->subjects[index.row()].text;
}

void SubjectsOfTeacherModel::insertRow(const dbapi::Subject &subject)
{
    auto len = this->subjects.size();

    beginInsertRows({}, len, len);

    this->subjects.push_front({subject.name(), subject.key()});

    endInsertRows();
}

void SubjectsOfTeacherModel::removeRow(int row)
{
    beginRemoveRows({}, row, row);

    this->subjects.remove(row);

    endRemoveRows();
}

void SubjectsOfTeacherModel::clear()
{
    this->beginResetModel();

    this->subjects.clear();

    this->endResetModel();
}

SubjectsOfTeacherModel::~SubjectsOfTeacherModel()
{
    for(auto subject : this->subjects)
        delete subject;
}
