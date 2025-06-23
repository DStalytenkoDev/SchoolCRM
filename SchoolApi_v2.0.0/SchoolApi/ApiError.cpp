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
