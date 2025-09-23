#include <QtSql/QSqlQuery>
#include <QtSql/QSqlError>

#include "Class.h"

using dbapi::Class;

bool Class::Key::operator ==(const Key& key) const noexcept
{
    return this->id == key.id;
}

bool Class::Key::operator !=(const Key& key) const noexcept
{
    return this->id != key.id;
}

Class::Class(Connection* p_connection) noexcept : p_connection(p_connection)
{
}

Class::Class(const Key& key, Connection* p_connection) noexcept : _key(key), p_connection(p_connection)
{
}

void Class::setKey(const Key& key) noexcept
{
    this->_key = key;
}

const Class::Key& Class::key() const noexcept
{
    return this->_key;
}

bool Class::valid() const noexcept
{
    if(this->p_connection == nullptr)
        return false;

    if(not this->p_connection->valid())
        return false;

    return true;
}

const dbapi::ApiError& Class::error() const noexcept
{
    return this->_error;
}

bool Class::load() noexcept
{
    if(not this->valid())
    {
        this->_error.type = ApiError::ConnectionInvalidError;
        this->_error.text = "dbapi::Class has invalid connection, connection may be nullptr or it did not open";
        return false;
    }

    QSqlQuery query(this->p_connection->database);
    query.prepare("select (name) from Classes where id = :id");
    query.bindValue(":id", this->_key.id);

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
    this->_name = query.value(0).toString();

    this->_error.setNoError();
    return true;
}

bool Class::store() noexcept
{
    if(not this->valid())
    {
        this->_error.type = ApiError::ConnectionInvalidError;
        this->_error.text = "dbapi::Class has invalid connection, connection may be nullptr or it did not open";
        return false;
    }

    QSqlQuery query(this->p_connection->database);
    query.prepare("insert Classes (name) values (:name)");
    query.bindValue(":name", this->_name);

    if(not query.exec())
    {
        this->_error = query.lastError();
        return false;
    }

    query.prepare("select IDENT_CURRENT('Classes')");

    if(not query.exec())
    {
        this->_error = query.lastError();
        return false;
    }

    query.next();
    this->_key.id = query.value(0).toInt();

    this->_error.setNoError();
    return true;
}

bool Class::remove() noexcept
{
    if(not this->valid())
    {
        this->_error.type = ApiError::ConnectionInvalidError;
        this->_error.text = "dbapi::Class has invalid connection, connection may be nullptr or it did not open";
        return false;
    }

    QSqlQuery query(this->p_connection->database);
    query.prepare("delete from Classes where id = :id");
    query.bindValue(":id", this->_key.id);

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

bool Class::update() noexcept
{
    if(not this->valid())
    {
        this->_error.type = ApiError::ConnectionInvalidError;
        this->_error.text = "dbapi::Class has invalid connection, connection may be nullptr or it did not open";
        return false;
    }

    QSqlQuery query(this->p_connection->database);
    query.prepare("update Classes set name = :name where id = :id");
    query.bindValue(":name", this->_name);
    query.bindValue(":id", this->_key.id);

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

QVector<Class*> Class::loadAll(Connection* p_connection, ApiError* p_error) noexcept
{
    if(p_connection == nullptr)
    {
        if(p_error)
        {
            p_error->type = ApiError::ConnectionInvalidError;
            p_error->text = "dbapi::Class has invalid connection, connection may be nullptr or it did not open";
        }

        return {};
    }

    if(not p_connection->valid())
    {
        if(p_error)
        {
            p_error->type = ApiError::ConnectionInvalidError;
            p_error->text = "dbapi::Class has invalid connection, connection may be nullptr or it did not open";
        }

        return {};
    }

    QSqlQuery query(p_connection->database);
    query.prepare("select * from Classes");

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

    QVector<Class*> classes;

    while(query.next())
    {
        auto p_class = new Class(Key{query.value(0).toInt()}, p_connection);
        p_class->_name = query.value(1).toString();

        classes.append(p_class);
    }

    return QVector<Class*>(std::move(classes));
}

QVector<Class*> Class::makeAll(Connection* p_connection, ApiError* p_error) noexcept
{
    if(p_connection == nullptr)
    {
        if(p_error)
        {
            p_error->type = ApiError::ConnectionInvalidError;
            p_error->text = "dbapi::Class has invalid connection, connection may be nullptr or it did not open";
        }

        return {};
    }

    if(not p_connection->valid())
    {
        if(p_error)
        {
            p_error->type = ApiError::ConnectionInvalidError;
            p_error->text = "dbapi::Class has invalid connection, connection may be nullptr or it did not open";
        }

        return {};
    }

    QSqlQuery query(p_connection->database);
    query.prepare("select (id) from Classes");

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

    QVector<Class*> classes;

    while(query.next())
    {
        auto p_class = new Class(Key{query.value(0).toInt()}, p_connection);

        classes.append(p_class);
    }

    return QVector<Class*>(std::move(classes));
}

const QString& Class::name() const noexcept
{
    return this->_name;
}

void Class::setName(const QString& name) noexcept
{
    this->_name = name;
}

bool Class::updateName(const QString& name) noexcept
{
    if(not this->valid())
    {
        this->_error.type = ApiError::ConnectionInvalidError;
        this->_error.text = "dbapi::Class has invalid connection, connection may be nullptr or it did not open";
        return false;
    }

    this->_name = name;

    QSqlQuery query(this->p_connection->database);
    query.prepare("update Classes set name = :name where id = :id");
    query.bindValue(":name", this->_name);
    query.bindValue(":id", this->_key.id);

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

bool Class::loadName() noexcept
{
    if(not this->valid())
    {
        this->_error.type = ApiError::ConnectionInvalidError;
        this->_error.text = "dbapi::Class has invalid connection, connection may be nullptr or it did not open";
        return false;
    }

    QSqlQuery query(this->p_connection->database);
    query.prepare("select (name) from Classes where id = :id");
    query.bindValue(":id", this->_key.id);

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
    this->_name = query.value(0).toString();

    this->_error.setNoError();
    return true;
}

dbapi::BoolResult Class::existsByKey() const noexcept
{
    if(not this->valid())
    {
        this->_error.type = ApiError::ConnectionInvalidError;
        this->_error.text = "dbapi::Class has invalid connection, connection may be nullptr or it did not open";
        return ErrorResult;
    }

    QSqlQuery query(this->p_connection->database);
    query.prepare("select NULL from Classes where id = :id");
    query.bindValue(":id", this->_key.id);

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

dbapi::BoolResult Class::existsByTuple() const noexcept
{
    if(not this->valid())
    {
        this->_error.type = ApiError::ConnectionInvalidError;
        this->_error.text = "dbapi::Class has invalid connection, connection may be nullptr or it did not open";
        return ErrorResult;
    }

    QSqlQuery query(this->p_connection->database);
    query.prepare("select NULL from Classes where id = :id and name = :name");

    query.bindValue(":id", this->_key.id);
    query.bindValue(":name", this->_name);

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

dbapi::Connection* Class::connection() const noexcept
{
    return this->p_connection;
}

void Class::setConnection(dbapi::Connection* p_connection) noexcept
{
    this->p_connection = p_connection;
}

