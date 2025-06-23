#ifndef TEACHERSUBJECT_H
#define TEACHERSUBJECT_H


#include "QVector"
#include "Connection.h"
#include "Person.h"
#include "Subject.h"


namespace dbapi {


class TeacherSubjectsList
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

    Connection* p_connection = nullptr;

    mutable ApiError _error;

    QVector<Subject::Key> _subjects;
    QVector<Subject::Key> subjectsToRemove;
    QVector<Subject::Key> subjectsToStore;

public:
    TeacherSubjectsList(Connection* p_connection = nullptr) noexcept;
    TeacherSubjectsList(const Key& key, Connection* p_connection = nullptr) noexcept;

    void setKey(const Key& key) noexcept;
    const Key& key() const noexcept;

    Connection* connection() const noexcept;
    void setConnection(Connection* p_connection = nullptr) noexcept;

    bool load() noexcept;
    bool store() noexcept;
    bool remove() noexcept;

    bool update() noexcept;
    void removeSubject(QVector<Subject::Key>::ConstIterator iterator) noexcept;
    void appendSubject(const Subject::Key& subject) noexcept;

    void clearUpdate() noexcept;
    void clear() noexcept;

    const QVector<Subject::Key>& subjects() const noexcept;

    static QVector<TeacherSubjectsList*> loadAll(Connection* p_connection, ApiError* p_error = nullptr) noexcept;
    static QVector<TeacherSubjectsList*> makeAll(Connection* p_connection, ApiError* p_error = nullptr) noexcept;

    const ApiError& error() const noexcept;

    bool valid() const noexcept;

    BoolResult existsByKey() const noexcept;
    BoolResult existsByTuple() const noexcept;
};


}



#endif // TEACHERSUBJECT_H
