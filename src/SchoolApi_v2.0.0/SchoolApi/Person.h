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


#ifndef PERSON_H
#define PERSON_H


#include "Connection.h"
#include "Role.h"


namespace dbapi
{


class Person
{
public:
    struct Key
    {
        int id = {};

        bool operator ==(const Key& key) const noexcept;
        bool operator !=(const Key& key) const noexcept;
    };

private:
    Key _key = {};
    Role::Key _role = {};
    QString _firstName;
    QString _secondName;
    int _birthday = {};

    Connection* p_connection = nullptr;

    mutable ApiError _error;

public:
    Person(Connection* p_connection = nullptr) noexcept;
    Person(const Key& key, Connection* p_connection = nullptr) noexcept;

    void setKey(const Key& key) noexcept;
    const Key& key() const noexcept;

    Connection* connection() const noexcept;
    void setConnection(Connection* p_connection = nullptr) noexcept;

    bool load() noexcept;
    bool store() noexcept;
    bool remove() noexcept;
    bool update() noexcept;

    static QVector<Person*> loadAll(Connection* p_connection, ApiError* p_error = nullptr) noexcept;
    static QVector<Person*> makeAll(Connection* p_connection, ApiError* p_error = nullptr) noexcept;

    const Role::Key& role() const noexcept;
    void setRole(const Role::Key& role) noexcept;
    bool updateRole(const Role::Key& role) noexcept;
    bool loadRole() noexcept;

    const QString& firstName() const noexcept;
    void setFirstName(const QString& name) noexcept;
    bool updateFirstName(const QString& name) noexcept;
    bool loadFirstName() noexcept;

    const QString& secondName() const noexcept;
    void setSecondName(const QString& name) noexcept;
    bool updateSecondName(const QString& name) noexcept;
    bool loadSecondName() noexcept;

    const int& birthday() const noexcept;
    void setBirthday(const int& birthday) noexcept;
    bool updateBirthday(const int& birthday) noexcept;
    bool loadBirthday() noexcept;

    bool valid() const noexcept;

    BoolResult existsByKey() const noexcept;
    BoolResult existsByTuple() const noexcept;

    const ApiError& error() const noexcept;
};


}


#endif // PERSON_H
