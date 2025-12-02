// Copyright (C) 2025 Davyd Stalytenko
// SPDX-License-Identifier: GPL-3.0-only
//
// This file is part of SchooliesCave.
// 
// SchooliesCave is free software: you can redistribute it and/or modify it under the terms
// of the GNU General Public License as published by the Free Software Foundation, version 3
// of the License only.
// 
// SchooliesCave is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY;
// without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
// See the GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License along with SchooliesCave.
// If not, see <https://www.gnu.org/licenses/>.


#include "Connection.h"

using dbapi::Connection;

Connection::Connection(const QString& serverName, const QString& databaseName) noexcept
{
    this->database.setDatabaseName(databaseName);
    this->database.setHostName(serverName);
}

Connection::Connection(const QString& serverName, const QString& userName, const QString& password, const QString& databaseName) noexcept
{
    this->setUserName(userName);
    this->setPassword(password);
}

bool Connection::open() noexcept
{
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
    this->database.setHostName(name);
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
    this->database.setDatabaseName(name);
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

QString Connection::serverName() const noexcept
{
    return this->database.hostName();
}

QString Connection::userName() const noexcept
{
    return this->database.userName();
}

QString Connection::password() const noexcept
{
    return this->database.password();
}

QString Connection::databaseName() const noexcept
{
    return this->database.databaseName();
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
    return this->database.isValid();
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
