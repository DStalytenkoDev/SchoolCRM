#include <QtSql/QSqlQuery>
#include <QtSql/QSqlError>

#include "Person.h"

using dbapi::Person;
using dbapi::Role;

bool Person::Key::operator ==(const Key& key) const noexcept
{
    return this->id == key.id;
}

bool Person::Key::operator !=(const Key& key) const noexcept
{
    return this->id != key.id;
}

Person::Person(Connection* p_connection) noexcept : p_connection(p_connection)
{
}

Person::Person(const Key& key, Connection* p_connection) noexcept : _key(key), p_connection(p_connection)
{
}

void Person::setKey(const Key& key) noexcept
{
    this->_key = key;
}

const Person::Key& Person::key() const noexcept
{
    return this->_key;
}

bool Person::valid() const noexcept
{
    if(this->p_connection == nullptr)
        return false;

    if(not this->p_connection->valid())
        return false;

    return true;
}

const dbapi::ApiError& Person::error() const noexcept
{
    return this->_error;
}

dbapi::Connection* Person::connection() const noexcept
{
    return this->p_connection;
}

void Person::setConnection(dbapi::Connection* p_connection) noexcept
{
    this->p_connection = p_connection;
}

bool Person::load() noexcept
{
    if(not this->valid())
    {
        this->_error.type = ApiError::ConnectionInvalidError;
        this->_error.text = "dbapi::Person has invalid connection, connection may be nullptr or it did not open";
        return false;
    }

    QSqlQuery query(this->p_connection->database);
    query.prepare("select role_id, first_name, second_name, birthday from Persons where id = :id");
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
    this->_role.id = query.value(0).toInt();
    this->_firstName = query.value(1).toString();
    this->_secondName = query.value(2).toString();
    this->_birthday = query.value(3).toInt();

    this->_error.setNoError();
    return true;
}

bool Person::store() noexcept
{
    if(not this->valid())
    {
        this->_error.type = ApiError::ConnectionInvalidError;
        this->_error.text = "dbapi::Person has invalid connection, connection may be nullptr or it did not open";
        return false;
    }

    QSqlQuery query(this->p_connection->database);
    query.prepare("insert Persons (role_id, first_name, second_name, birthday) values (:role_id, :first_name, :second_name, :birthday)");

    query.bindValue(":role_id", this->_role.id);
    query.bindValue(":first_name", this->_firstName);
    query.bindValue(":second_name", this->_secondName);
    query.bindValue(":birthday", this->_birthday);

    if(not query.exec())
    {
        this->_error = query.lastError();
        return false;
    }

    query.prepare("select IDENT_CURRENT('Persons')");

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

bool Person::remove() noexcept
{
    if(not this->valid())
    {
        this->_error.type = ApiError::ConnectionInvalidError;
        this->_error.text = "dbapi::Person has invalid connection, connection may be nullptr or it did not open";
        return false;
    }

    QSqlQuery query(this->p_connection->database);
    query.prepare("delete from Persons where id = :id");
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

bool Person::update() noexcept
{
    if(not this->valid())
    {
        this->_error.type = ApiError::ConnectionInvalidError;
        this->_error.text = "dbapi::Person has invalid connection, connection may be nullptr or it did not open";
        return false;
    }

    QSqlQuery query(this->p_connection->database);
    query.prepare("update Persons set role_id = :role_id, first_name = :first_name, second_name = :second_name, birthday = :birthday where id = :id");

    query.bindValue(":id", this->_key.id);
    query.bindValue(":role_id", this->_role.id);
    query.bindValue(":first_name", this->_firstName);
    query.bindValue(":second_name", this->_secondName);
    query.bindValue(":birthday", this->_birthday);

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

QVector<Person*> Person::loadAll(Connection* p_connection, ApiError* p_error) noexcept
{
    if(p_connection == nullptr)
    {
        if(p_error)
        {
            p_error->type = ApiError::ConnectionInvalidError;
            p_error->text = "dbapi::Person has invalid connection, connection may be nullptr or it did not open";
        }

        return {};
    }

    if(not p_connection->valid())
    {
        if(p_error)
        {
            p_error->type = ApiError::ConnectionInvalidError;
            p_error->text = "dbapi::Person has invalid connection, connection may be nullptr or it did not open";
        }

        return {};
    }

    QSqlQuery query(p_connection->database);
    query.prepare("select * from Persons");

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

    QVector<Person*> persons;

    while(query.next())
    {
        auto p_person = new Person(Key{query.value(0).toInt()}, p_connection);

        p_person->_role.id = query.value(1).toInt();
        p_person->_firstName = query.value(2).toString();
        p_person->_secondName = query.value(3).toString();
        p_person->_birthday = query.value(4).toInt();

        persons.append(p_person);
    }

    return QVector<Person*>(std::move(persons));
}

QVector<Person*> Person::makeAll(Connection* p_connection, ApiError* p_error) noexcept
{
    if(p_connection == nullptr)
    {
        if(p_error)
        {
            p_error->type = ApiError::ConnectionInvalidError;
            p_error->text = "dbapi::Person has invalid connection, connection may be nullptr or it did not open";
        }

        return {};
    }

    if(not p_connection->valid())
    {
        if(p_error)
        {
            p_error->type = ApiError::ConnectionInvalidError;
            p_error->text = "dbapi::Person has invalid connection, connection may be nullptr or it did not open";
        }

        return {};
    }

    QSqlQuery query(p_connection->database);
    query.prepare("select (id) from Persons");

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

    QVector<Person*> persons;

    while(query.next())
    {
        auto p_person = new Person(Key{query.value(0).toInt()}, p_connection);
        persons.append(p_person);
    }

    return QVector<Person*>(std::move(persons));
}

const Role::Key& Person::role() const noexcept
{
    return this->_role;
}

void Person::setRole(const Role::Key& role) noexcept
{
    this->_role = role;
}

bool Person::updateRole(const Role::Key& role) noexcept
{
    if(not this->valid())
    {
        this->_error.type = ApiError::ConnectionInvalidError;
        this->_error.text = "dbapi::Person has invalid connection, connection may be nullptr or it did not open";
        return false;
    }

    this->_role = role;

    QSqlQuery query(this->p_connection->database);
    query.prepare("update Persons set role_id = :role_id where id = :id");
    query.bindValue(":role_id", this->_role.id);
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

bool Person::loadRole() noexcept
{
    if(not this->valid())
    {
        this->_error.type = ApiError::ConnectionInvalidError;
        this->_error.text = "dbapi::Person has invalid connection, connection may be nullptr or it did not open";
        return false;
    }

    QSqlQuery query(this->p_connection->database);
    query.prepare("select (role_id) from Persons where id = :id");
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
    this->_role.id = query.value(0).toInt();

    this->_error.setNoError();
    return true;
}

// firstName
const QString& Person::firstName() const noexcept
{
    return this->_firstName;
}

void Person::setFirstName(const QString& name) noexcept
{
    this->_firstName = name;
}

bool Person::updateFirstName(const QString& name) noexcept
{
    if(not this->valid())
    {
        this->_error.type = ApiError::ConnectionInvalidError;
        this->_error.text = "dbapi::Person has invalid connection, connection may be nullptr or it did not open";
        return false;
    }

    this->_firstName = name;

    QSqlQuery query(this->p_connection->database);
    query.prepare("update Persons set first_name = :first_name where id = :id");
    query.bindValue(":first_name", this->_firstName);
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

bool Person::loadFirstName() noexcept
{
    if(not this->valid())
    {
        this->_error.type = ApiError::ConnectionInvalidError;
        this->_error.text = "dbapi::Person has invalid connection, connection may be nullptr or it did not open";
        return false;
    }

    QSqlQuery query(this->p_connection->database);
    query.prepare("select (first_name) from Persons where id = :id");
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
    this->_firstName = query.value(0).toString();

    this->_error.setNoError();
    return true;
}

// secondName
const QString& Person::secondName() const noexcept
{
    return this->_secondName;
}

void Person::setSecondName(const QString& name) noexcept
{
    this->_secondName = name;
}

bool Person::updateSecondName(const QString& name) noexcept
{
    if(not this->valid())
    {
        this->_error.type = ApiError::ConnectionInvalidError;
        this->_error.text = "dbapi::Person has invalid connection, connection may be nullptr or it did not open";
        return false;
    }

    this->_secondName = name;

    QSqlQuery query(this->p_connection->database);
    query.prepare("update Persons set second_name = :second_name where id = :id");
    query.bindValue(":second_name", this->_secondName);
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

bool Person::loadSecondName() noexcept
{
    if(not this->valid())
    {
        this->_error.type = ApiError::ConnectionInvalidError;
        this->_error.text = "dbapi::Person has invalid connection, connection may be nullptr or it did not open";
        return false;
    }

    QSqlQuery query(this->p_connection->database);
    query.prepare("select (second_name) from Persons where id = :id");
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
    this->_secondName = query.value(0).toString();

    this->_error.setNoError();
    return true;
}

// birthday
const int& Person::birthday() const noexcept
{
    return this->_birthday;
}

void Person::setBirthday(const int& birthday) noexcept
{
    this->_birthday = birthday;
}

bool Person::updateBirthday(const int& birthday) noexcept
{
    if(not this->valid())
    {
        this->_error.type = ApiError::ConnectionInvalidError;
        this->_error.text = "dbapi::Person has invalid connection, connection may be nullptr or it did not open";
        return false;
    }

    this->_birthday = birthday;

    QSqlQuery query(this->p_connection->database);
    query.prepare("update Persons set birthday = :birthday where id = :id");
    query.bindValue(":birthday", this->_birthday);
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

bool Person::loadBirthday() noexcept
{
    if(not this->valid())
    {
        this->_error.type = ApiError::ConnectionInvalidError;
        this->_error.text = "dbapi::Person has invalid connection, connection may be nullptr or it did not open";
        return false;
    }

    QSqlQuery query(this->p_connection->database);
    query.prepare("select (birthday) from Persons where id = :id");
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
    this->_birthday = query.value(0).toInt();

    this->_error.setNoError();
    return true;
}

dbapi::BoolResult Person::existsByKey() const noexcept
{
    if(not this->valid())
    {
        this->_error.type = ApiError::ConnectionInvalidError;
        this->_error.text = "dbapi::Person has invalid connection, connection may be nullptr or it did not open";
        return ErrorResult;
    }

    QSqlQuery query(this->p_connection->database);
    query.prepare("select NULL from Persons where id = :id");
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

dbapi::BoolResult Person::existsByTuple() const noexcept
{
    if(not this->valid())
    {
        this->_error.type = ApiError::ConnectionInvalidError;
        this->_error.text = "dbapi::Person has invalid connection, connection may be nullptr or it did not open";
        return ErrorResult;
    }

    QSqlQuery query(this->p_connection->database);
    query.prepare("select NULL from Persons where id = :id                   and "\
                                                 "role_id = :role_id         and "\
                                                 "first_name = :first_name   and "\
                                                 "second_name = :second_name and "\
                                                 "birthday = :birthday");

    query.bindValue(":id", this->_key.id);
    query.bindValue(":role_id", this->_role.id);
    query.bindValue(":first_name", this->_firstName);
    query.bindValue(":second_name", this->_secondName);
    query.bindValue(":birthday", this->_birthday);

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

