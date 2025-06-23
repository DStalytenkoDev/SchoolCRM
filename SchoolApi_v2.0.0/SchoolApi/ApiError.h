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
