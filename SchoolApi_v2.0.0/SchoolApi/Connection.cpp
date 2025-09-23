#include "Connection.h"

using dbapi::Connection;

Connection::Connection(const QString& serverName, const QString& databaseName) noexcept : _databaseName(databaseName), _serverName(serverName)
{
}

Connection::Connection(const QString& serverName, const QString& userName, const QString& password, const QString& databaseName) noexcept : _databaseName(databaseName), _serverName(serverName)
{
    this->setUserName(userName);
    this->setPassword(password);
}

bool Connection::open() noexcept
{
    QString openTemplate("DRIVER={SQL SERVER};SERVER=%1;DATABASE=%2;Trusted_Connection=%3;");
    this->database.setDatabaseName(openTemplate.arg(this->_serverName, this->_databaseName, this->trustedConnection ? "Yes" : "No"));

    if(this->database.open())
        return true;

    this->_error = this->database.lastError();
    return false;
}

void Connection::close() noexcept
{
    this->database.close();
}

bool Connection::transaction() noexcept
{
    if(this->database.transaction())
        return true;

    this->_error = this->database.lastError();
    return false;
}

bool Connection::commit() noexcept
{
    if(this->database.commit())
        return true;

    this->_error = this->database.lastError();
    return false;
}

bool Connection::rollback() noexcept
{
    if(this->database.rollback())
        return true;

    this->_error = this->database.lastError();
    return false;
}

void Connection::setServerName(const QString& name) noexcept
{
    this->_serverName = name;
}

void Connection::setUserName(const QString& name) noexcept
{
    this->database.setUserName(name);
}

void Connection::setPassword(const QString& password) noexcept
{
    this->database.setPassword(password);
}

void Connection::setDatabaseName(const QString& name) noexcept
{
    this->_databaseName = name;
}

void Connection::setPort(int port) noexcept
{
    this->database.setPort(port);
}

void Connection::setConnectOptions(const QString& options) noexcept
{
    this->database.setConnectOptions(options);
}

void Connection::setTrustedConnection(bool trusted) noexcept
{
    this->trustedConnection = trusted;
}

const QString& Connection::serverName() const noexcept
{
    return this->_serverName;
}

QString Connection::userName() const noexcept
{
    return this->database.userName();
}

QString Connection::password() const noexcept
{
    return this->database.password();
}

const QString& Connection::databaseName() const noexcept
{
    return this->_databaseName;
}

int Connection::port() const noexcept
{
    return this->database.port();
}

QString Connection::connectionOptions() const noexcept
{
    return this->database.connectOptions();
}

bool Connection::valid() const noexcept
{
    return this->database.isOpen();
}

Connection::operator bool() const noexcept
{
    return this->valid();
}

void Connection::makeDefault() noexcept
{
    Connection::p_defaultConnection = this;
}

Connection* Connection::defaultConnection() noexcept
{
    return Connection::p_defaultConnection;
}

Connection* Connection::p_defaultConnection = nullptr;

const dbapi::ApiError& Connection::error() const noexcept
{
    return this->_error;
}

Connection::~Connection() noexcept
{
    this->close();

    if(Connection::p_defaultConnection == this)
        Connection::p_defaultConnection = nullptr;
}
