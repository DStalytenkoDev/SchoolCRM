#ifndef CLASSMATE_H
#define CLASSMATE_H


#include "Person.h"
#include "Class.h"


namespace dbapi {


class Classmate
{
public:
    /// Contains the persons's key of an instance
    struct Key
    {
        Person::Key person;

        bool operator ==(const Key& key) const noexcept;
        bool operator !=(const Key& key) const noexcept;
    };

private:
    Class::Key _grade = {};
    Key _key = {};

    Connection* p_connection = nullptr;

    mutable ApiError _error;

public:
    /// No server request
    Classmate(Connection* p_connection = nullptr) noexcept;

    /// No server request
    Classmate(const Key& key, Connection* p_connection = nullptr) noexcept;

    /// Setter for the key, No server request
    void setKey(const Key& key) noexcept;

    /// Getter for the key, No server request
    const Key& key() const noexcept;

    /*
     * Getter of ::Connection if was set
     *
     * No server request
     *
     * @returns nullptr or ptr to a ::Connection
     */
    Connection* connection() const noexcept;

    /// Setter of ::Connection, no server request
    void setConnection(Connection* p_connection = nullptr) noexcept;

    /*
     * Fetches data by the key
     *
     * @returns request successfullness
     */
    bool load() noexcept;

    /*
     * Pushes new instance to the database
     *
     * @returns request successfullness
     */
    bool store() noexcept;

    /*
     * Removes the instance from the database by the key
     *
     * @returns request successfullness
     */
    bool remove() noexcept;

    /*
     * Updates the instance in the database
     * The policy is just to re-write all data including changed
     *
     * @returns request successfullness
     */
    bool update() noexcept;

    /*
     * Fetches items from the database and
     * creates a list of ::Classmates entities allocated on the heap
     * Each item contains valid key and all other data fields
     *
     * @param[in] p_connection The pointer to any ::Connection instance, if nullptr then ::InvalidConnection error
     * @param[out] p_error The pointer to any ::ApiError instance
     *
     * @returns QVector<Class*> uses a move semantic
     */
    static QVector<Classmate*> loadAll(Connection* p_connection, ApiError* p_error = nullptr) noexcept;

    /*
     * Fetches items from the database and
     * creates a list of ::Classmate entities allocated on the heap
     * Each item contains valid key ONLY, no other fields are loaded
     *
     * @param[in] p_connection The pointer to any ::Connection instance, if nullptr then ::InvalidConnection error
     * @param[out] p_error any ::ApiError instance
     *
     * @returns QVector<Class*>(uses a move semantic), the list is empty if error happend
     */
    static QVector<Classmate*> makeAll(Connection* p_connection, ApiError* p_error = nullptr) noexcept;

    /// getter for grade field that is a ::Classmate key, no server request
    const Class::Key& grade() const noexcept;

    /// Setter, No server request
    void setGrade(const Class::Key& grade) noexcept;

    /*
     * Updates only the grade field in the database
     *
     * @param grade is a new value for updation
     * @returns request successfullness
     */
    bool updateGrade(const Class::Key& grade) noexcept;

    /*
     * Fetches only the grade field
     *
     * @returns request successfullness
     */
    bool loadGrade() noexcept;

    /*
     * Says if instance has valid connection
     *
     * @retunrs validity of connection
     */
    bool valid() const noexcept;

    /*
     * Requests database if instance exists by the key
     *
     * @returns a enum of yes, no or error happend
     */
    BoolResult existsByKey() const noexcept;

    /*
     * Requests database if the instance exists by the key and ALL fields that are set
     * even if they are were not set (default)
     *
     * @returns a enum of yes, no or error happend
     */
    BoolResult existsByTuple() const noexcept;

    /// Getter for the error, No server request
    const ApiError& error() const noexcept;
};


}


#endif // CLASSMATE_H
