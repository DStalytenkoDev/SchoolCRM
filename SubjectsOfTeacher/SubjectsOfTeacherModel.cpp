#include "SubjectsOfTeacherModel.h"
#include <QMessageBox>

SubjectsOfTeacherModel::SubjectsOfTeacherModel(QObject *parent)
    : QAbstractListModel{parent}
{}

void SubjectsOfTeacherModel::setConnection(dbapi::Connection *connection)
{
    this->teacherSubjectsList.setConnection(connection);
}

void SubjectsOfTeacherModel::setTeacher(const dbapi::Person::Key &key)
{
    this->teacherSubjectsList.setKey({key});
}

dbapi::ApiError SubjectsOfTeacherModel::loadSubjects()
{
    this->subjects.clear();

    if(not this->teacherSubjectsList.load())
        return this->teacherSubjectsList.error();

    int len = this->teacherSubjectsList.subjects().count();

    this->beginInsertRows({}, 0, len - 1);
    this->subjects.resize(len);

    for(int idx = 0; idx < len; idx++)
    {
        auto& subject = this->subjects[idx];

        subject.setConnection(this->teacherSubjectsList.connection());
        subject.setKey(this->teacherSubjectsList.subjects()[idx]);

        if(not subject.load())
        {
            this->endInsertRows();
            return subject.error();
        }
    }

    this->endInsertRows();
    return {};
}

dbapi::Subject *SubjectsOfTeacherModel::subject(const QModelIndex &index)
{
    return &this->subjects[index.row()];
}

dbapi::Subject *SubjectsOfTeacherModel::subject(int row)
{
    return &this->subjects[row];
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

    return this->subjects[index.row()].name();
}

void SubjectsOfTeacherModel::insertRow(int rowBefore, const dbapi::Subject &subject)
{
    auto len = this->subjects.size();

    beginInsertRows({}, rowBefore, rowBefore);

    if(len == 0)
        this->subjects.append(subject);
    else
        this->subjects.insert(rowBefore, subject);

    endInsertRows();
}

void SubjectsOfTeacherModel::removeRow(int row)
{
    beginRemoveRows({}, row, row);

    this->subjects.remove(row);

    endRemoveRows();
}
