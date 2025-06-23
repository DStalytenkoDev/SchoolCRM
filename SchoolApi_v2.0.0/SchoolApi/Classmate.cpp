#include <QtSql/QSqlQuery>
#include <QtSql/QSqlError>

#include "Classmate.h"

using dbapi::Classmate;

bool Classmate::Key::operator ==(const Key& key) const noexcept
{
    return this->person == key.person;
}

bool Classmate::Key::operator !=(const Key& key) const noexcept
{
    return not (this->person == key.person);
}

Classmate::Classmate(Connection* p_connection) noexcept : p_connection(p_connection)
{
}

Classmate::Classmate(const Key& key, Connection* p_connection) noexcept : _key(key), p_connection(p_connection)
{
}

void Classmate::setKey(const Key& key) noexcept
{
    this->_key = key;
}

const Classmate::Key& Classmate::key() const noexcept
{
    return this->_key;
}

bool Classmate::valid() const noexcept
{
    if(this->p_connection == nullptr)
        return false;

    if(not this->p_connection->valid())
        return false;

    return true;
}

const dbapi::ApiError& Classmate::error() const noexcept
{
    return this->_error;
}

dbapi::Connection* Classmate::connection() const noexcept
{
    return this->p_connection;
}

void Classmate::setConnection(dbapi::Connection* p_connection) noexcept
{
    this->p_connection = p_connection;
}

bool Classmate::load() noexcept
{
    if(not this->valid())
    {
        this->_error.type = ApiError::ConnectionInvalidError;
        this->_error.text = "dbapi::Classmate has invalid connection, connection may be nullptr or it did not open";
        return false;
    }

    QSqlQuery query(this->p_connection->database);
    query.prepare("select (class_id) from Classmates where person_id = :person_id");

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

bool Classmate::store() noexcept
{
    if(not this->valid())
    {
        this->_error.type = ApiError::ConnectionInvalidError;
        this->_error.text = "dbapi::Classmate has invalid connection, connection may be nullptr or it did not open";
        return false;
    }

    QSqlQuery query(this->p_connection->database);
    query.prepare("insert Classmates (person_id, class_id) values (:person_id, :class_id)");

    query.bindValue(":person_id", this->_key.person.id);
    query.bindValue(":class_id", this->_grade.id);

    if(not query.exec())
    {
        this->_error = query.lastError();
        return false;
    }

    this->_error.setNoError();
    return true;
}

bool Classmate::remove() noexcept
{
    if(not this->valid())
    {
        this->_error.type = ApiError::ConnectionInvalidError;
        this->_error.text = "dbapi::Classmate has invalid connection, connection may be nullptr or it did not open";
        return false;
    }

    QSqlQuery query(this->p_connection->database);
    query.prepare("delete from Classmates where person_id = :person_id");

    query.bindValue(":person_id", this->_key.person.id);

    if(not query.exec())
    {
        this->_error = query.lastError();
        return false;
    }

    if(not query.numRowsAffected())
    {
        this->_error.type = ApiError::KeyError;
        this->_error.text = "Invalid key, personId or classId";
        return false;
    }

    this->_error.setNoError();
    return true;
}

bool Classmate::update() noexcept
{
    if(not this->valid())
    {
        this->_error.type = ApiError::ConnectionInvalidError;
        this->_error.text = "dbapi::Classmate has invalid connection, connection may be nullptr or it did not open";
        return false;
    }

    QSqlQuery query(this->p_connection->database);
    query.prepare("update Classmates set class_id = :class_id where person_id = :person_id");

    query.bindValue(":person_id", this->_key.person.id);
    query.bindValue(":class_id", this->_grade.id);

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

QVector<Classmate*> Classmate::loadAll(Connection* p_connection, ApiError* p_error) noexcept
{
    if(p_connection == nullptr)
    {
        if(p_error)
        {
            p_error->type = ApiError::ConnectionInvalidError;
            p_error->text = "dbapi::Classmate has invalid connection, connection may be nullptr or it did not open";
        }

        return {};
    }

    if(not p_connection->valid())
    {
        if(p_error)
        {
            p_error->type = ApiError::ConnectionInvalidError;
            p_error->text = "dbapi::Classmate has invalid connection, connection may be nullptr or it did not open";
        }

        return {};
    }

    QSqlQuery query(p_connection->database);
    query.prepare("select * from Classmates");

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

    QVector<Classmate*> classmates;

    while(query.next())
    {
        auto p_classmate = new Classmate(Key{query.value(0).toInt()}, p_connection);
        p_classmate->_grade.id = query.value(1).toInt();

        classmates.append(p_classmate);
    }

    return QVector<Classmate*>(std::move(classmates));
}

QVector<Classmate*> Classmate::makeAll(Connection* p_connection, ApiError* p_error) noexcept
{
    if(p_connection == nullptr)
    {
        if(p_error)
        {
            p_error->type = ApiError::ConnectionInvalidError;
            p_error->text = "dbapi::Classmate has invalid connection, connection may be nullptr or it did not open";
        }

        return {};
    }

    if(not p_connection->valid())
    {
        if(p_error)
        {
            p_error->type = ApiError::ConnectionInvalidError;
            p_error->text = "dbapi::Classmate has invalid connection, connection may be nullptr or it did not open";
        }

        return {};
    }

    QSqlQuery query(p_connection->database);
    query.prepare("select (person_id) from Classmates");

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

    QVector<Classmate*> classmates;

    while(query.next())
    {
        auto p_classmate = new Classmate(Key{query.value(0).toInt()}, p_connection);
        classmates.append(p_classmate);
    }

    return QVector<Classmate*>(std::move(classmates));
}

dbapi::BoolResult Classmate::existsByKey() const noexcept
{
    if(not this->valid())
    {
        this->_error.type = ApiError::ConnectionInvalidError;
        this->_error.text = "dbapi::Classmate has invalid connection, connection may be nullptr or it did not open";
        return ErrorResult;
    }

    QSqlQuery query(this->p_connection->database);
    query.prepare("select NULL from Classmates where person_id = :person_id");

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

dbapi::BoolResult Classmate::existsByTuple() const noexcept
{
    if(not this->valid())
    {
        this->_error.type = ApiError::ConnectionInvalidError;
        this->_error.text = "dbapi::Classmate has invalid connection, connection may be nullptr or it did not open";
        return ErrorResult;
    }

    QSqlQuery query(this->p_connection->database);
    query.prepare("select NULL from Classmates where person_id = :person_id and class_id = :class_id");

    query.bindValue(":person_id", this->_key.person.id);
    query.bindValue(":class_id", this->_grade.id);

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

const dbapi::Class::Key& Classmate::grade() const noexcept
{
    return this->_grade;
}

void Classmate::setGrade(const Class::Key& grade) noexcept
{
    this->_grade = grade;
}

bool Classmate::updateGrade(const Class::Key& grade) noexcept
{
    if(not this->valid())
    {
        this->_error.type = ApiError::ConnectionInvalidError;
        this->_error.text = "dbapi::Classmate has invalid connection, connection may be nullptr or it did not open";
        return false;
    }

    this->_grade = grade;

    QSqlQuery query(this->p_connection->database);
    query.prepare("update Classmates set class_id = :class_id where person_id = :person_id");

    query.bindValue(":person_id", this->_key.person.id);
    query.bindValue(":class_id", this->_grade.id);

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

bool Classmate::loadGrade() noexcept
{
    if(not this->valid())
    {
        this->_error.type = ApiError::ConnectionInvalidError;
        this->_error.text = "dbapi::Classmate has invalid connection, connection may be nullptr or it did not open";
        return false;
    }

    QSqlQuery query(this->p_connection->database);
    query.prepare("select (class_id) from Classmates where person_id = :person_id");

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

