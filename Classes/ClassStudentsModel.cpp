#include "ClassStudentsModel.h"

ClassStudentsModel::ClassStudentsModel(QObject *parent)
    : QAbstractListModel{parent}
{}

void ClassStudentsModel::setConnection(dbapi::Connection *connection)
{
    this->connection = connection;
}

dbapi::ApiError ClassStudentsModel::loadStudents(const dbapi::Class::Key &key)
{
    dbapi::ApiError error;
    auto students = dbapi::Student::loadAll(this->connection, &error);

    if(error.type != dbapi::ApiError::NoError)
        return error;

    this->beginResetModel();

    this->students.reserve(students.count());
    this->students.clear();

    for(auto student : students)
    {
        // filter students of the class

        if(student->grade() == key)
            this->students.emplaceBack(student->key().person, this->connection);

        delete student; // clean up students
    }

    // load names
    for(auto& student : this->students)
        student.load();

    this->endResetModel();
    return {};
}

dbapi::Person* ClassStudentsModel::student(const QModelIndex &index)
{
    return &this->students[index.row()];
}

dbapi::Person *ClassStudentsModel::student(int row)
{
    return &this->students[row];
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

    return fullName.arg(this->students[index.row()].firstName(), this->students[index.row()].secondName());
}

void ClassStudentsModel::insertRow(const dbapi::Person& person)
{
    auto len = this->students.size();

    beginInsertRows({}, len, len);

    this->students.push_front(person);

    endInsertRows();
}

void ClassStudentsModel::removeRow(int row)
{
    beginRemoveRows({}, row, row);

    this->students.remove(row);

    endRemoveRows();
}
