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


#ifndef APIERROR_H
#define APIERROR_H


#include <QString>
#include <QtSql/QSqlError>

#include "ApiBaseHeader.h"


class dbapi::ApiError
{
public:
    enum Type
    {
        /// if no error was
        NoError,
        /// if error was not classified
        UnknownError,
        /// if connection failed to open
        ConnectionOpenError,
        /// if transaction failed
        TransactionError,

        /// if an instance doesnt exist in the database
        KeyError,
        /// if connection nullptr or invalid by the QSqlDatabase's judgement
        ConnectionInvalidError,
        /// by the QSqlDatabase's judgement
        PolicyError
    };

    /// contains type of the error
    Type type = NoError;
    /// contains text message 'bout error
    QString text;

    /// by deafult the instance is no error
    ApiError() noexcept = default;

    /// by deafult the instance is no error
    ApiError(Type type, const QString& text) noexcept;

    /// creates an instance based on QSqlError
    ApiError(const QSqlError& error) noexcept;

    ApiError& operator =(const QSqlError& error) noexcept;

    /// sets the instance to no error state
    void setNoError() noexcept;
};


#endif // APIERROR_H
