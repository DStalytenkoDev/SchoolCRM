#ifndef CLASSSUBJECTSLIST_H
#define CLASSSUBJECTSLIST_H


#include "QVector"
#include "Connection.h"
#include "Class.h"
#include "Subject.h"


namespace dbapi {


class ClassSubjectsList
{
public:
    /// Contains the dbapi::Class key
    struct Key
    {
        Class::Key grade;

        bool operator ==(const Key& key) const noexcept;
        bool operator !=(const Key& key) const noexcept;
    };

private:
    Key _key = {};

    Connection* p_connection = nullptr;

    mutable ApiError _error;

    QVector<Subject::Key> _subjects;
    QVector<Subject::Key> subjectsToRemove;
    QVector<Subject::Key> subjectsToStore;

public:
    /// No server request
    ClassSubjectsList(Connection* p_connection = nullptr) noexcept;

    /// No server request
    ClassSubjectsList(const Key& key, Connection* p_connection = nullptr) noexcept;

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
     * If there are no items, it means the list doesnt exist and
     * it gives the KeyError
     *
     * @returns request successfullness
     */
    bool load() noexcept;

    /*
     * Pushes all subjects from the very only main list of them to
     * the database as new entities
     *
     * @returns request successfullness
     */
    bool store() noexcept;

    /*
     * Removes all subjects from the very only main list of them
     *
     * @returns request successfullness
     */
    bool remove() noexcept;

    /*
     * Updates all subjects that were affected by ::removeSubject(), ::appendSubject()
     * The policy is just to re-write all data including changed
     *
     * @returns request successfullness
     */
    bool update() noexcept;

    /*
     * Adds subject into the list for removing and removes it from main list
     *
     * Then could be applied the ::update() to request changes
     * or any other edit method, but the other will only go by the main list
     *
     * No server request
     */
    void removeSubject(QVector<Subject::Key>::ConstIterator iterator) noexcept;

    /*
     * Adds subject into the list for appending and main list
     *
     * Then could be applied the ::update() to request changes
     * or any other edit method, but the other will only go by the main list
     *
     * No server request
     */
    void appendSubject(const Subject::Key& subject) noexcept;

    /*
     * Clears all subjects that were affected by ::removeSubject(), ::appendSubject()
     *
     * does not affect the main list
     *
     * No server request
     */
    void clearUpdate() noexcept;

    /*
     * Clears the main list of subjects
     *
     * No server request
     */
    void clear() noexcept;

    /// Getter of main list of subejcts
    const QVector<Subject::Key>& subjects() const noexcept;

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
    static QVector<ClassSubjectsList*> loadAll(Connection* p_connection, ApiError* p_error = nullptr) noexcept;

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
    static QVector<ClassSubjectsList*> makeAll(Connection* p_connection, ApiError* p_error = nullptr) noexcept;

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


#endif // CLASSSUBJECT_H
