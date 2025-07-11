#include <SchoolApi/Student.h>
#include "ClassStudentsModel.h"

ClassStudentsModel::ClassStudentsModel(QObject *parent)
    : QAbstractListModel{parent}
{}

void ClassStudentsModel::setConnection(dbapi::Connection *connection)
{
    assert((void("nullptr"), connection));

    this->connection = connection;
}

void ClassStudentsModel::setClass(const dbapi::Class::Key &key)
{
    this->classKey = key;
}

UserError ClassStudentsModel::loadAll()
{
    dbapi::ApiError error;

    this->beginResetModel();

    for(auto student : this->students)
        delete student;

    this->students.clear();

    auto students = dbapi::Student::loadAll(this->connection, &error);

    this->students.reserve(students.size());

    if(error.type != dbapi::ApiError::NoError)
        return UserError::internalError("Students", "be loaded 'cause an unknown error", "Try again or contact support");

    for(auto student : students)
    {
        if(student->grade() == this->classKey)
        {
            this->students.append(new dbapi::Person(student->key().person, this->connection));

            if(not this->students.back()->load())
            {
                this->endResetModel();
                this->clear();

                return UserError::internalError("Students", "be loaded 'cause an unknown error", "Try again or contact support");
            }
        }
    }

    this->endResetModel();

    return {};
}

UserError ClassStudentsModel::appendStudent(const dbapi::Person::Key &key)
{
    dbapi::ApiError error;
    auto students = dbapi::Student::loadAll(this->connection, &error);

    if(error.type != dbapi::ApiError::NoError)
        return UserError::internalError("Student", "be appended 'cause an unknown error", "Try again or contact support");

    for(auto student : students)
        if(student->key().person == key && student->grade() == this->classKey)
            return UserError::keyError("Student", "be appended 'cause its already in the list");

    dbapi::Student student({key}, this->connection);
    student.setGrade(this->classKey);

    if(not student.store())
        return UserError::keyError("Student", "be appended 'cause it might paricipate in other class", "Try again or contact support");

    this->beginInsertRows({}, this->students.size(), this->students.size());

    this->students.append(new dbapi::Person(key, this->connection));

    if(not this->students.back()->load())
    {
        this->students.back()->setFirstName("_Error_value_");
        this->students.back()->setSecondName("...");
    }

    this->endInsertRows();

    return {};
}

UserError ClassStudentsModel::removeStudent(int index)
{
    assert((void("out of range"), index >= 0 && index < this->students.size()));

    dbapi::Student student({this->students[index]->key()}, this->connection);

    if(not student.remove())
        return UserError::internalError("Student", "be removed 'cause an unknown error", "Try again or contact support");

    this->beginRemoveRows({}, index, index);

    delete this->students[index];
    this->students.remove(index);

    this->endRemoveRows();

    return {};
}

dbapi::Person* ClassStudentsModel::student(const QModelIndex &index)
{
    assert((void("out of range"), index.row() >= 0 && index.row() < this->students.size()));

    return this->students[index.row()];
}

dbapi::Person *ClassStudentsModel::student(int row)
{
    assert((void("out of range"), row >= 0 && row < this->students.size()));

    return this->students[row];
}

int ClassStudentsModel::rowCount(const QModelIndex &parent) const
{
    return this->students.count();
}

QVariant ClassStudentsModel::data(const QModelIndex &index, int role) const
{
    if(role != Qt::DisplayRole)
        return {};

    if(not index.isValid())
        return {};

    if(index.row() >= this->students.count())
        return {};

    QString fullName("%1 %2");

    return fullName.arg(this->students[index.row()]->firstName(), this->students[index.row()]->secondName());
}

void ClassStudentsModel::clear()
{
    if(this->students.size() == 0)
        return;

    this->beginResetModel();

    for(auto student : this->students)
        delete student;

    this->students.clear();

    this->endResetModel();
}
