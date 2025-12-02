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


#include "ApiError.h"

using dbapi::ApiError;

static inline ApiError::Type translateSqlErrorTypeToOur(QSqlError::ErrorType type) noexcept;

ApiError::ApiError(Type type, const QString& text) noexcept : type(type), text(text)
{
}

ApiError::ApiError(const QSqlError& error) noexcept
{
    this->type = translateSqlErrorTypeToOur(error.type());
    this->text = error.text();
}

ApiError& ApiError::operator =(const QSqlError& error) noexcept
{
    this->type = translateSqlErrorTypeToOur(error.type());
    this->text = error.text();
    
    return *this;
}

void ApiError::setNoError() noexcept
{
    this->type = NoError;
    this->text = "No Errors =)";
}

static inline ApiError::Type translateSqlErrorTypeToOur(QSqlError::ErrorType type) noexcept
{
    switch (type)
    {
    case QSqlError::ErrorType::NoError:
        return ApiError::Type::NoError;

    case QSqlError::ErrorType::ConnectionError:
        return ApiError::Type::ConnectionOpenError;

    case QSqlError::ErrorType::TransactionError:
        return ApiError::Type::TransactionError;

    default:
        return ApiError::Type::UnknownError;
    }
}
