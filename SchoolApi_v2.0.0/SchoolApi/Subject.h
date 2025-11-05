// Copyright (C) 2025 AUTHORS of SchooliesCave
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


#ifndef SUBJECT_H
#define SUBJECT_H

#include <QVector>
#include "Connection.h"


namespace dbapi {


class Subject
{
public:
    struct Key
    {
        int id;

        bool operator ==(const Key& key) const noexcept;
        bool operator !=(const Key& key) const noexcept;
    };

private:
    QString _name;
    Key _key = {};

    Connection* p_connection = nullptr;

    mutable ApiError _error;

public:
    Subject(Connection* p_connection = nullptr) noexcept;
    Subject(const Key& key, Connection* p_connection = nullptr) noexcept;

    void setKey(const Key& key) noexcept;
    const Key& key() const noexcept;

    Connection* connection() const noexcept;
    void setConnection(Connection* p_connection = nullptr) noexcept;

    bool load() noexcept;
    bool store() noexcept;
    bool remove() noexcept;
    bool update() noexcept;

    static QVector<Subject*> loadAll(Connection* p_connection, ApiError* p_error = nullptr) noexcept;
    static QVector<Subject*> makeAll(Connection* p_connection, ApiError* p_error = nullptr) noexcept;

    const QString& name() const noexcept;
    void setName(const QString& name) noexcept;
    bool updateName(const QString& name) noexcept;
    bool loadName() noexcept;

    bool valid() const noexcept;

    BoolResult existsByKey() const noexcept;
    BoolResult existsByTuple() const noexcept;

    const ApiError& error() const noexcept;
};


}



#endif // SUBJECT_H
