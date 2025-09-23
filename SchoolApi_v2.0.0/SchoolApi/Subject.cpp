#include <QtSql/QSqlQuery>
#include <QtSql/QSqlError>

#include "Subject.h"


using dbapi::Subject;


bool Subject::Key::operator ==(const Key& key) const noexcept
{
    return this->id == key.id;
}

bool Subject::Key::operator !=(const Key& key) const noexcept
{
    return this->id != key.id;
}

Subject::Subject(Connection* p_connection) noexcept : p_connection(p_connection)
{
}

Subject::Subject(const Key& key, Connection* p_connection) noexcept : _key(key), p_connection(p_connection)
{
}

void Subject::setKey(const Key& key) noexcept
{
    this->_key = key;
}

const Subject::Key& Subject::key() const noexcept
{
    return this->_key;
}

bool Subject::valid() const noexcept
{
    if(this->p_connection == nullptr)
        return false;

    if(not this->p_connection->valid())
        return false;

    return true;
}

const dbapi::ApiError& Subject::error() const noexcept
{
    return this->_error;
}

dbapi::Connection* Subject::connection() const noexcept
{
    return this->p_connection;
}

void Subject::setConnection(dbapi::Connection* p_connection) noexcept
{
    this->p_connection = p_connection;
}

bool Subject::load() noexcept
{
    if(not this->valid())
    {
        this->_error.type = ApiError::ConnectionInvalidError;
        this->_error.text = "dbapi::Subject has invalid connection, connection may be nullptr or it did not open";
        return false;
    }

    QSqlQuery query(this->p_connection->database);
    query.prepare("select (name) from Subjects where id = :id");
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

bool Subject::store() noexcept
{
    if(not this->valid())
    {
        this->_error.type = ApiError::ConnectionInvalidError;
        this->_error.text = "dbapi::Subject has invalid connection, connection may be nullptr or it did not open";
        return false;
    }

    QSqlQuery query(this->p_connection->database);
    query.prepare("insert Subjects (name) values (:name)");
    query.bindValue(":name", this->_name);

    if(not query.exec())
    {
        this->_error = query.lastError();
        return false;
    }

    query.prepare("select IDENT_CURRENT('Subjects')");

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

bool Subject::remove() noexcept
{
    if(not this->valid())
    {
        this->_error.type = ApiError::ConnectionInvalidError;
        this->_error.text = "dbapi::Subject has invalid connection, connection may be nullptr or it did not open";
        return false;
    }

    QSqlQuery query(this->p_connection->database);
    query.prepare("delete from Subjects where id = :id");
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

bool Subject::update() noexcept
{
    if(not this->valid())
    {
        this->_error.type = ApiError::ConnectionInvalidError;
        this->_error.text = "dbapi::Subject has invalid connection, connection may be nullptr or it did not open";
        return false;
    }

    QSqlQuery query(this->p_connection->database);
    query.prepare("update Subjects set name = :name where id = :id");
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

QVector<Subject*> Subject::loadAll(Connection* p_connection, ApiError* p_error) noexcept
{
    if(p_connection == nullptr)
    {
        if(p_error)
        {
            p_error->type = ApiError::ConnectionInvalidError;
            p_error->text = "dbapi::Subject has invalid connection, connection may be nullptr or it did not open";
        }

        return {};
    }

    if(not p_connection->valid())
    {
        if(p_error)
        {
            p_error->type = ApiError::ConnectionInvalidError;
            p_error->text = "dbapi::Subject has invalid connection, connection may be nullptr or it did not open";
        }

        return {};
    }

    QSqlQuery query(p_connection->database);
    query.prepare("select * from Subjects");

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

    QVector<Subject*> subjects;

    while(query.next())
    {
        auto p_subject = new Subject(Key{query.value(0).toInt()}, p_connection);
        p_subject->_name = query.value(1).toString();

        subjects.append(p_subject);
    }

    return QVector<Subject*>(std::move(subjects));
}

QVector<Subject*> Subject::makeAll(Connection* p_connection, ApiError* p_error) noexcept
{
    if(p_connection == nullptr)
    {
        if(p_error)
        {
            p_error->type = ApiError::ConnectionInvalidError;
            p_error->text = "dbapi::Subject has invalid connection, connection may be nullptr or it did not open";
        }

        return {};
    }

    if(not p_connection->valid())
    {
        if(p_error)
        {
            p_error->type = ApiError::ConnectionInvalidError;
            p_error->text = "dbapi::Subject has invalid connection, connection may be nullptr or it did not open";
        }

        return {};
    }

    QSqlQuery query(p_connection->database);
    query.prepare("select (id) from Subjects");

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

    QVector<Subject*> subjects;

    while(query.next())
    {
        auto p_subject = new Subject(Key{query.value(0).toInt()}, p_connection);
        subjects.append(p_subject);
    }

    return QVector<Subject*>(std::move(subjects));
}

const QString& Subject::name() const noexcept
{
    return this->_name;
}

void Subject::setName(const QString& name) noexcept
{
    this->_name = name;
}

bool Subject::updateName(const QString& name) noexcept
{
    this->_name = name;
    return this->update();
}

bool Subject::loadName() noexcept
{
    return this->load();
}

dbapi::BoolResult Subject::existsByKey() const noexcept
{
    if(not this->valid())
    {
        this->_error.type = ApiError::ConnectionInvalidError;
        this->_error.text = "dbapi::Subject has invalid connection, connection may be nullptr or it did not open";
        return ErrorResult;
    }

    QSqlQuery query(this->p_connection->database);
    query.prepare("select NULL from Subjects where id = :id");
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

dbapi::BoolResult Subject::existsByTuple() const noexcept
{
    if(not this->valid())
    {
        this->_error.type = ApiError::ConnectionInvalidError;
        this->_error.text = "dbapi::Subject has invalid connection, connection may be nullptr or it did not open";
        return ErrorResult;
    }

    QSqlQuery query(this->p_connection->database);
    query.prepare("select NULL from Subjects where id = :id and name = :name");

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
