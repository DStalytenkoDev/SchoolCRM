#ifndef ROLES_H
#define ROLES_H


#include <QVector>
#include "Connection.h"


namespace dbapi
{


class Role
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
    Role(Connection* p_connection = nullptr) noexcept;
    Role(const Key& key, Connection* p_connection = nullptr) noexcept;

    void setKey(const Key& key) noexcept;
    const Key& key() const noexcept;

    Connection* connection() const noexcept;
    void setConnection(Connection* p_connection = nullptr) noexcept;

    bool load() noexcept;
    bool store() noexcept;
    bool remove() noexcept;
    bool update() noexcept;

    static QVector<Role*> loadAll(Connection* p_connection, ApiError* p_error = nullptr) noexcept;
    static QVector<Role*> makeAll(Connection* p_connection, ApiError* p_error = nullptr) noexcept;

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


#endif // ROLES_H
