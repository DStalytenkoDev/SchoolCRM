#include <QtSql/QSqlQuery>
#include <QtSql/QSqlError>

#include "Role.h"

using dbapi::Role;

bool Role::Key::operator ==(const Key& key) const noexcept
{
    return this->id == key.id;
}

bool Role::Key::operator !=(const Key& key) const noexcept
{
    return this->id != key.id;
}

Role::Role(Connection* p_connection) noexcept : p_connection(p_connection)
{
}

Role::Role(const Key& key, Connection* p_connection) noexcept : _key(key), p_connection(p_connection)
{
}

void Role::setKey(const Key& key) noexcept
{
    this->_key = key;
}

const Role::Key& Role::key() const noexcept
{
    return this->_key;
}

bool Role::valid() const noexcept
{
    if(this->p_connection == nullptr)
        return false;

    if(not this->p_connection->valid())
        return false;

    return true;
}

const dbapi::ApiError& Role::error() const noexcept
{
    return this->_error;
}

dbapi::Connection* Role::connection() const noexcept
{
    return this->p_connection;
}

void Role::setConnection(dbapi::Connection* p_connection) noexcept
{
    this->p_connection = p_connection;
}

bool Role::load() noexcept
{
    if(not this->valid())
    {
        this->_error.type = ApiError::ConnectionInvalidError;
        this->_error.text = "dbapi::Role has invalid connection, connection may be nullptr or it did not open";
        return false;
    }

    QSqlQuery query(this->p_connection->database);
    query.prepare("select (name) from Roles where id = :id");
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

bool Role::store() noexcept
{    
    if(not this->valid())
    {
        this->_error.type = ApiError::ConnectionInvalidError;
        this->_error.text = "dbapi::Role has invalid connection, connection may be nullptr or it did not open";
        return false;
    }

    QSqlQuery query(this->p_connection->database);
    query.prepare("insert Roles (name) values (:name)");
    query.bindValue(":name", this->_name);

    if(not query.exec())
    {
        this->_error = query.lastError();
        return false;
    }

    query.prepare("select IDENT_CURRENT('Roles')");

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

bool Role::remove() noexcept
{
    if(not this->valid())
    {
        this->_error.type = ApiError::ConnectionInvalidError;
        this->_error.text = "dbapi::Role has invalid connection, connection may be nullptr or it did not open";
        return false;
    }

    QSqlQuery query(this->p_connection->database);
    query.prepare("delete from Roles where id = :id");
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

bool Role::update() noexcept
{
    if(not this->valid())
    {
        this->_error.type = ApiError::ConnectionInvalidError;
        this->_error.text = "dbapi::Role has invalid connection, connection may be nullptr or it did not open";
        return false;
    }

    QSqlQuery query(this->p_connection->database);
    query.prepare("update Roles set name = :name where id = :id");
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

QVector<Role*> Role::loadAll(Connection* p_connection, ApiError* p_error) noexcept
{
    if(p_connection == nullptr)
    {
        if(p_error)
        {
            p_error->type = ApiError::ConnectionInvalidError;
            p_error->text = "dbapi::Role has invalid connection, connection may be nullptr or it did not open";
        }

        return {};
    }

    if(not p_connection->valid())
    {
        if(p_error)
        {
            p_error->type = ApiError::ConnectionInvalidError;
            p_error->text = "dbapi::Role has invalid connection, connection may be nullptr or it did not open";
        }

        return {};
    }

    QSqlQuery query(p_connection->database);
    query.prepare("select * from Roles");

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

    QVector<Role*> roles;

    while(query.next())
    {
        auto p_role = new Role(Key{query.value(0).toInt()}, p_connection);
        p_role->_name = query.value(1).toString();

        roles.append(p_role);
    }

    return QVector<Role*>(std::move(roles));
}

QVector<Role*> Role::makeAll(Connection* p_connection, ApiError* p_error) noexcept
{
    if(p_connection == nullptr)
    {
        if(p_error)
        {
            p_error->type = ApiError::ConnectionInvalidError;
            p_error->text = "dbapi::Role has invalid connection, connection may be nullptr or it did not open";
        }

        return {};
    }

    if(not p_connection->valid())
    {
        if(p_error)
        {
            p_error->type = ApiError::ConnectionInvalidError;
            p_error->text = "dbapi::Role has invalid connection, connection may be nullptr or it did not open";
        }

        return {};
    }

    QSqlQuery query(p_connection->database);
    query.prepare("select (id) from Roles");

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

    QVector<Role*> roles;

    while(query.next())
    {
        auto p_role = new Role(Key{query.value(0).toInt()}, p_connection);
        roles.append(p_role);
    }

    return QVector<Role*>(std::move(roles));
}

const QString& Role::name() const noexcept
{
    return this->_name;
}

void Role::setName(const QString& name) noexcept
{
    this->_name = name;
}

bool Role::updateName(const QString& name) noexcept
{
    if(not this->valid())
    {
        this->_error.type = ApiError::ConnectionInvalidError;
        this->_error.text = "dbapi::Role has invalid connection, connection may be nullptr or it did not open";
        return false;
    }

    this->_name = name;

    QSqlQuery query(this->p_connection->database);
    query.prepare("update Roles set name = :name where id = :id");
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

bool Role::loadName() noexcept
{
    if(not this->valid())
    {
        this->_error.type = ApiError::ConnectionInvalidError;
        this->_error.text = "dbapi::Role has invalid connection, connection may be nullptr or it did not open";
        return false;
    }

    QSqlQuery query(this->p_connection->database);
    query.prepare("select (name) from Roles where id = :id");
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

dbapi::BoolResult Role::existsByKey() const noexcept
{    
    if(not this->valid())
    {
        this->_error.type = ApiError::ConnectionInvalidError;
        this->_error.text = "dbapi::Role has invalid connection, connection may be nullptr or it did not open";
        return ErrorResult;
    }

    QSqlQuery query(this->p_connection->database);
    query.prepare("select NULL from Roles where id = :id");
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

dbapi::BoolResult Role::existsByTuple() const noexcept
{
    if(not this->valid())
    {
        this->_error.type = ApiError::ConnectionInvalidError;
        this->_error.text = "dbapi::Role has invalid connection, connection may be nullptr or it did not open";
        return ErrorResult;
    }

    QSqlQuery query(this->p_connection->database);
    query.prepare("select NULL from Roles where id = :id and name = :name");

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

