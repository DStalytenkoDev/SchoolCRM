#ifndef CONNECTION_H
#define CONNECTION_H

#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlError>
#include "ApiBaseHeader.h"
#include "ApiError.h"


namespace dbapi {


class Connection
{
private:
    friend dbapi::Role;
    friend dbapi::Person;
    friend dbapi::Class;
    friend dbapi::Classmate;
    friend dbapi::Student;
    friend dbapi::Subject;
    friend dbapi::ClassSubjectsList;
    friend dbapi::StudentMark;
    friend dbapi::TeacherSubjectsList;

    QSqlDatabase database = QSqlDatabase::addDatabase("QODBC");
    QString _databaseName;
    QString _serverName;
    bool trustedConnection = false;

    static Connection* p_defaultConnection;

    ApiError _error;

public:
    /// No server request
    Connection() noexcept = default;

    /// No server request
    Connection(const QString& serverName, const QString& databaseName) noexcept;

    /// No server request
    Connection(const QString& serverName, const QString& userName, const QString& password, const QString& databaseName) noexcept;

    /*
     * Tries to open connection to database
     *
     * @retunrs true if success
     */
    bool open() noexcept;

    /*
     * Tries to close connection
     *
     * @retunrs true if success
     */
    void close() noexcept;

    /*
     * Tries to start transaction that could be undone if need or commited
     *
     * @retunrs true if success
     */
    bool transaction() noexcept;

    /*
     * Tries to commit transaction
     *
     * @retunrs true if success
     */
    bool commit() noexcept;

    /*
     * Tries to undone all changes after transaction was issued
     *
     * @retunrs true if success
     */
    bool rollback() noexcept;

    /// No server request, setter for server name
    void setServerName(const QString& name) noexcept;

    /// No server request, setter for user name
    void setUserName(const QString& name) noexcept;

    /// No server request, setter for password
    void setPassword(const QString& password) noexcept;

    /// No server request, setter for database name
    void setDatabaseName(const QString& name) noexcept;

    /// No server request, server for port
    void setPort(int port) noexcept;

    /// No server request, setter for additional options in form as need for ::QSqlDatabase
    void setConnectOptions(const QString& options = QString()) noexcept;

    /// No server request, setter for using trusted(secure) automactic connection
    void setTrustedConnection(bool trusted = true) noexcept;

    /// No server request, server name getter
    const QString& serverName() const noexcept;

    /// No server request, user name getter
    QString userName() const noexcept;

    /// No server request, password getter
    QString password() const noexcept;

    /// No server request, dataname getter
    const QString& databaseName() const noexcept;

    /// No server request, port getter
    int port() const noexcept;

    /// No server request, options getter
    QString connectionOptions() const noexcept;

    /// No server request
    bool valid() const noexcept;

    /*
     * Overload is an alias
     *
     * @returns ::valid()
     */
    operator bool() const noexcept;

    /*
     * Says if ::Connection loaded valid driver
     *
     * @returns validity
     */
    const ApiError& error() const noexcept;

    /*
     * Sets this ::Conection in default
     * The ::Connection instance can be deleted safely afterwards if need
     *
     * No server request
     */
    void makeDefault() noexcept;

    /*
     * Gets default ::Connection if has been set, otherwise nullptr
     * No server request
     *
     * @returns ::Connecton* | nullptr
     */
    static Connection* defaultConnection() noexcept;

    /// No server request
    ~Connection() noexcept;
};


}

#endif // CONNECTION_H
