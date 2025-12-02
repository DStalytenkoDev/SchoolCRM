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


#ifndef STUDENT_H
#define STUDENT_H


#include "QVector"
#include "Connection.h"
#include "Person.h"
#include "Class.h"


namespace dbapi {


class Student
{
public:
    struct Key
    {
        Person::Key person = {};

        bool operator ==(const Key& key) const noexcept;
        bool operator !=(const Key& key) const noexcept;
    };

private:
    Key _key = {};
    Class::Key _grade = {};

    Connection* p_connection = nullptr;

    mutable ApiError _error;

public:
    Student(Connection* p_connection = nullptr) noexcept;
    Student(const Key& key, Connection* p_connection = nullptr) noexcept;

    void setKey(const Key& key) noexcept;
    const Key& key() const noexcept;

    Connection* connection() const noexcept;
    void setConnection(Connection* p_connection = nullptr) noexcept;

    bool load() noexcept;
    bool store() noexcept;
    bool remove() noexcept;
    bool update() noexcept;

    static QVector<Student*> loadAll(Connection* p_connection, ApiError* p_error = nullptr) noexcept;
    static QVector<Student*> makeAll(Connection* p_connection, ApiError* p_error = nullptr) noexcept;

    const Class::Key& grade() const noexcept;
    void setGrade(const Class::Key& grade) noexcept;
    bool updateGrade(const Class::Key& grade) noexcept;
    bool loadGrade() noexcept;

    bool valid() const noexcept;

    BoolResult existsByKey() const noexcept;
    BoolResult existsByTuple() const noexcept;

    const ApiError& error() const noexcept;
};


}


#endif // STUDENT_H
