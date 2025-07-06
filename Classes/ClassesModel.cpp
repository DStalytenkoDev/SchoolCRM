#include <variant>
#include <QMessageBox>
#include <SchoolApi/Classmate.h>
#include "ClassStudentsModel.h"
#include "ClassSubjectsModel.h"
#include "ClassesModel.h"

ClassesModel::ClassesModel(QObject *parent)
    : QAbstractListModel{parent}
{}

void ClassesModel::setConnection(dbapi::Connection *connection)
{
    assert((void("nullptr"), connection));

    this->connection = connection;
}

UserError ClassesModel::loadAll()
{
    dbapi::ApiError error;

    this->beginResetModel();

    // clear
    for(auto grade : this->classes)
        delete grade;

    // load new
    this->classes = dbapi::Class::loadAll(this->connection, &error);

    this->endResetModel();

    if(error.type != dbapi::ApiError::NoError)
        return UserError::internalError("Classes", "be loaded 'cause an unknown error", "Try again or contact support");

    return {};
}

UserError ClassesModel::createClass(const QString &name, const dbapi::Person::Key &homeroomTeacher)
{
    QString trimmedName = name.trimmed();

    if(trimmedName.isEmpty())
        return UserError::validityError("Class", "be created 'cause its name is empty");

    if(this->loadAll().isError())
        return UserError::internalError("Class", "be created 'cause an unknown error", "Try again or contact support");

    for(auto grade : this->classes)
        if(grade->name() == trimmedName)
            return UserError::keyError("Class", "be created 'cause it already exists", "Try different name");

    if(not this->connection->transaction())
        return UserError::internalError("Class", "be created 'cause an unknown error", "Try again or contact support");

    dbapi::Class grade(this->connection);
    grade.setName(name);

    if(not grade.store())
    {
        this->connection->rollback();
        return UserError::internalError("Class", "be created 'cause an unknown error", "Try again or contact support");
    }

    dbapi::Classmate teacher({homeroomTeacher}, this->connection);
    teacher.setGrade(grade.key());

    if(not teacher.store())
    {
        this->connection->rollback();
        return UserError::internalError("Class", "be created 'cause an unknown error", "Try again or contact support");
    }

    this->connection->commit();

    this->beginInsertRows({}, this->classes.size(), this->classes.size());
    this->classes.append(new dbapi::Class(grade));
    this->endInsertRows();

    return {};
}

UserError ClassesModel::removeClass(int index)
{
    assert((void("out of range"), index >= 0 && index < this->classes.size()));

    auto grade = this->classes[index];

    ClassStudentsModel studentsModel(this);
    ClassSubjectsModel subjectsModel(this);

    studentsModel.setClass(grade->key());
    subjectsModel.setClass(grade->key());

    if(studentsModel.loadAll().isError())
        return UserError::internalError("Class", "be removed 'cause an unknown error", "Try again or contact support");

    if(subjectsModel.loadAll().isError())
        return UserError::internalError("Class", "be removed 'cause an unknown error", "Try again or contact support");

    if(not this->connection->transaction())
        return UserError::internalError("Class", "be removed 'cause an unknown error", "Try again or contact support");

    while(studentsModel.rowCount() > 0)
    {
        if(studentsModel.removeStudent(0).isError())
        {
            this->connection->rollback();
            return UserError::internalError("Class", "be removed 'cause an unknown error", "Try again or contact support");
        }
    }

    while(subjectsModel.rowCount() > 0)
    {
        if(subjectsModel.removeSubject(0).isError())
        {
            this->connection->rollback();
            return UserError::internalError("Class", "be removed 'cause an unknown error", "Try again or contact support");
        }
    }

    if(this->removeTeacher(index))
    {
        this->connection->rollback();
        return UserError::internalError("Class", "be removed 'cause an unknown error", "Try again or contact support");
    }

    if(not grade->remove())
    {
        this->connection->rollback();
        return UserError::internalError("Class", "be removed 'cause an unknown error", "Try again or contact support");
    }

    this->connection->commit();

    return {};
}

UserError ClassesModel::changeHomeroomTeacher(int index, const dbapi::Person::Key &homeroomTeacher)
{
    assert((void("out of range"), index >= 0 && index < this->classes.size()));

    if(not this->connection->transaction())
        return UserError::internalError("Homeroom teacher", "be changed 'cause an unknown error", "Try again or contact support");

    if(not this->removeTeacher(index))
    {
        this->connection->rollback();
        return UserError::internalError("Homeroom teacher", "be changed 'cause an unknown error", "Try again or contact support");
    }

    dbapi::Classmate teacher({homeroomTeacher}, this->connection);

    if(not teacher.store())
    {
        this->connection->rollback();
        return UserError::internalError("Homeroom teacher", "be changed 'cause an unknown error", "Try again or contact support");
    }

    this->connection->commit();
    return {};
}

std::variant<dbapi::Person::Key, UserError> ClassesModel::getHomeroomTeacher(int index)
{
    assert((void("out of range"), index >= 0 and index < this->classes.size()));

    auto teacher = this->findTeacher(index);

    if(teacher == nullptr)
        return UserError::internalError("Homeroom teacher", "be loaded 'cause an unknown error", "Try again or contact support");

    auto key = teacher->key().person;
    delete teacher;

    return key;
}

dbapi::Class *ClassesModel::grade(const QModelIndex &index)
{
    assert((void("out of range"), index.row() >= 0 and index.row() < this->classes.size()));

    return this->classes[index.row()];
}

dbapi::Class *ClassesModel::grade(int row)
{
    assert((void("out of range"), row >= 0 and row < this->classes.size()));

    return this->classes[row];
}

int ClassesModel::rowCount(const QModelIndex &parent) const
{
    return this->classes.count();
}

QVariant ClassesModel::data(const QModelIndex &index, int role) const
{
    if(role != Qt::DisplayRole)
        return {};

    if(not index.isValid())
        return {};

    if(index.row() >= this->classes.count())
        return {};

    return this->classes[index.row()]->name();
}

void ClassesModel::clear()
{
    if(this->classes.size() == 0)
        return;

    this->beginResetModel();

    for(auto grade : this->classes)
        delete grade;

    this->classes.clear();

    this->endResetModel();
}

ClassesModel::~ClassesModel()
{
    for(auto grade : this->classes)
        delete grade;
}

dbapi::Classmate *ClassesModel::findTeacher(int index)
{
    dbapi::ApiError error;
    auto teachers = dbapi::Classmate::loadAll(this->connection, &error);

    if(error.type != dbapi::ApiError::NoError)
        return nullptr;

    dbapi::Classmate* teacherFound = nullptr;

    for(auto teacher : teachers)
    {
        if(teacher->grade() == this->classes[index]->key())
            teacherFound = teacher;
        else
            delete teacher;
    }

    return teacherFound;
}

bool ClassesModel::removeTeacher(int index)
{
    auto teacher = this->findTeacher(index);

    if(teacher == nullptr)
        return false;

    if(not teacher->remove())
        return false;

    return true;
}
