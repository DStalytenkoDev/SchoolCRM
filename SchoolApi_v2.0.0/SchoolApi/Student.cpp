#include <QtSql/QSqlQuery>
#include <QtSql/QSqlError>

#include "Student.h"

using dbapi::Student;

bool Student::Key::operator ==(const Key& key) const noexcept
{
    return this->person == key.person;
}

bool Student::Key::operator !=(const Key& key) const noexcept
{
    return not (this->person == key.person);
}

Student::Student(Connection* p_connection) noexcept : p_connection(p_connection)
{
}

Student::Student(const Key& key, Connection* p_connection) noexcept : _key(key), p_connection(p_connection)
{
}

void Student::setKey(const Key& key) noexcept
{
    this->_key = key;
}

const Student::Key& Student::key() const noexcept
{
    return this->_key;
}

bool Student::valid() const noexcept
{
    if(this->p_connection == nullptr)
        return false;

    if(not this->p_connection->valid())
        return false;

    return true;
}

const dbapi::ApiError& Student::error() const noexcept
{
    return this->_error;
}

dbapi::Connection* Student::connection() const noexcept
{
    return this->p_connection;
}

void Student::setConnection(dbapi::Connection* p_connection) noexcept
{
    this->p_connection = p_connection;
}

bool Student::load() noexcept
{
    if(not this->valid())
    {
        this->_error.type = ApiError::ConnectionInvalidError;
        this->_error.text = "dbapi::Student has invalid connection, connection may be nullptr or it did not open";
        return false;
    }

    QSqlQuery query(this->p_connection->database);
    query.prepare("select (grade_id) from Students where person_id = :person_id");
    query.bindValue(":person_id", this->_key.person.id);

    if(not query.exec())
    {
        this->_error = query.lastError();
        return false;
    }

    if(not query.numRowsAffected())
    {
        this->_error.type = ApiError::KeyError;
        this->_error.text = "Invalid key, it does not exists";
        return false;
    }

    query.next();
    this->_grade.id = query.value(0).toInt();

    this->_error.setNoError();
    return true;
}

bool Student::store() noexcept
{
    if(not this->valid())
    {
        this->_error.type = ApiError::ConnectionInvalidError;
        this->_error.text = "dbapi::Student has invalid connection, connection may be nullptr or it did not open";
        return false;
    }

    QSqlQuery query(this->p_connection->database);
    query.prepare("insert Students (person_id, grade_id) values (:person_id, :grade_id)");

    query.bindValue(":person_id", this->_key.person.id);
    query.bindValue(":grade_id", this->_grade.id);

    if(not query.exec())
    {
        this->_error = query.lastError();
        return false;
    }

    this->_error.setNoError();
    return true;
}

bool Student::remove() noexcept
{
    if(not this->valid())
    {
        this->_error.type = ApiError::ConnectionInvalidError;
        this->_error.text = "dbapi::Student has invalid connection, connection may be nullptr or it did not open";
        return false;
    }

    QSqlQuery query(this->p_connection->database);
    query.prepare("delete from Students where person_id = :person_id");

    query.bindValue(":person_id", this->_key.person.id);

    if(not query.exec())
    {
        this->_error = query.lastError();
        return false;
    }

    if(not query.numRowsAffected())
    {
        this->_error.type = ApiError::KeyError;
        this->_error.text = "Invalid key, it does not exists";
        return false;
    }

    this->_error.setNoError();
    return true;
}

bool Student::update() noexcept
{
    if(not this->valid())
    {
        this->_error.type = ApiError::ConnectionInvalidError;
        this->_error.text = "dbapi::Student has invalid connection, connection may be nullptr or it did not open";
        return false;
    }

    QSqlQuery query(this->p_connection->database);
    query.prepare("update Students set grade_id = :grade_id where person_id = :person_id");

    query.bindValue(":person_id", this->_key.person.id);
    query.bindValue(":grade_id", this->_grade.id);

    if(not query.exec())
    {
        this->_error = query.lastError();
        return false;
    }

    if(not query.numRowsAffected())
    {
        this->_error.type = ApiError::KeyError;
        this->_error.text = "Invalid key, it does not exists";
        return false;
    }

    this->_error.setNoError();
    return true;
}

QVector<Student*> Student::loadAll(Connection* p_connection, ApiError* p_error) noexcept
{
    if(p_connection == nullptr)
    {
        if(p_error)
        {
            p_error->type = ApiError::ConnectionInvalidError;
            p_error->text = "dbapi::Student has invalid connection, connection may be nullptr or it did not open";
        }

        return {};
    }

    if(not p_connection->valid())
    {
        if(p_error)
        {
            p_error->type = ApiError::ConnectionInvalidError;
            p_error->text = "dbapi::Student has invalid connection, connection may be nullptr or it did not open";
        }

        return {};
    }

    QSqlQuery query(p_connection->database);
    query.prepare("select * from Students");

    if(not query.exec())
    {
        if(p_error)
            *p_error = query.lastError();

        return {};
    }

    if(p_error)
        p_error->setNoError();

    if(not query.numRowsAffected())
        return {};

    QVector<Student*> students;

    while(query.next())
    {
        auto p_student = new Student(Key{query.value(0).toInt()}, p_connection);
        p_student->_grade.id = query.value(1).toInt();
        students.append(p_student);
    }

    return QVector<Student*>(std::move(students));
}

QVector<Student*> Student::makeAll(Connection* p_connection, ApiError* p_error) noexcept
{
    if(p_connection == nullptr)
    {
        if(p_error)
        {
            p_error->type = ApiError::ConnectionInvalidError;
            p_error->text = "dbapi::Student has invalid connection, connection may be nullptr or it did not open";
        }

        return {};
    }

    if(not p_connection->valid())
    {
        if(p_error)
        {
            p_error->type = ApiError::ConnectionInvalidError;
            p_error->text = "dbapi::Student has invalid connection, connection may be nullptr or it did not open";
        }

        return {};
    }

    QSqlQuery query(p_connection->database);
    query.prepare("select (person_id) from Students");

    if(not query.exec())
    {
        if(p_error)
            *p_error = query.lastError();

        return {};
    }

    if(p_error)
        p_error->setNoError();

    if(not query.numRowsAffected())
        return {};

    QVector<Student*> students;

    while(query.next())
    {
        auto p_student = new Student(Key{query.value(0).toInt()}, p_connection);
        students.append(p_student);
    }

    return QVector<Student*>(std::move(students));
}

const dbapi::Class::Key& Student::grade() const noexcept
{
    return this->_grade;
}

void Student::setGrade(const Class::Key& grade) noexcept
{
    this->_grade = grade;
}

bool Student::loadGrade() noexcept
{
    return this->load();
}

bool Student::updateGrade(const Class::Key& grade) noexcept
{
    this->_grade = grade;
    return this->update();
}

dbapi::BoolResult Student::existsByKey() const noexcept
{
    if(not this->valid())
    {
        this->_error.type = ApiError::ConnectionInvalidError;
        this->_error.text = "dbapi::Student has invalid connection, connection may be nullptr or it did not open";
        return ErrorResult;
    }

    QSqlQuery query(this->p_connection->database);
    query.prepare("select NULL from Students where person_id = :person_id");

    query.bindValue(":person_id", this->_key.person.id);

    if(not query.exec())
    {
        this->_error = query.lastError();
        return ErrorResult;
    }

    this->_error.setNoError();

    if(not query.numRowsAffected())
        return FalseResult;

    return TrueResult;
}

dbapi::BoolResult Student::existsByTuple() const noexcept
{
    if(not this->valid())
    {
        this->_error.type = ApiError::ConnectionInvalidError;
        this->_error.text = "dbapi::Student has invalid connection, connection may be nullptr or it did not open";
        return ErrorResult;
    }

    QSqlQuery query(this->p_connection->database);
    query.prepare("select NULL from Students where person_id = :person_id and grade_id = :grade_id");

    query.bindValue(":person_id", this->_key.person.id);
    query.bindValue(":grade_id", this->_grade.id);

    if(not query.exec())
    {
        this->_error = query.lastError();
        return ErrorResult;
    }

    this->_error.setNoError();

    if(not query.numRowsAffected())
        return FalseResult;

    return TrueResult;
}

