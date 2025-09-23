#ifndef STUDENTSUBJECT_H
#define STUDENTSUBJECT_H


#include <QVector>
#include <QDate>
#include "Connection.h"
#include "Person.h"
#include "Subject.h"


namespace dbapi {


class StudentMark
{
public:
    struct Key
    {
        int id;

        bool operator ==(const Key& key) const noexcept;
        bool operator !=(const Key& key) const noexcept;
    };

    enum Type
    {
        AlphaTest,
        BetaTest,
        GamaTest,
        PracticeLesson,
        LaboratoryLesson,
        IrregularMark
    };

private:
    Key _key = {};
    int _mark = {};
    Person::Key _teacher = {};
    Person::Key _student = {};
    Type _type = IrregularMark;
    QDate _date = {};
    Subject::Key _subject = {};

    Connection* p_connection = nullptr;

    mutable ApiError _error;

public:
    StudentMark(Connection* p_connection = nullptr) noexcept;
    StudentMark(const Key& key, Connection* p_connection = nullptr) noexcept;

    void setKey(const Key& key) noexcept;
    const Key& key() const noexcept;

    Connection* connection() const noexcept;
    void setConnection(Connection* p_connection = nullptr) noexcept;

    bool load() noexcept;
    bool store() noexcept;
    bool remove() noexcept;
    bool update() noexcept;

    static QVector<StudentMark*> loadAll(Connection* p_connection, ApiError* p_error = nullptr) noexcept;
    static QVector<StudentMark*> makeAll(Connection* p_connection, ApiError* p_error = nullptr) noexcept;

    const int& mark() const noexcept;
    void setMark(const int& mark) noexcept;
    bool updateMark(const int& mark) noexcept;
    bool loadMark() noexcept;

    const Person::Key& teacher() const noexcept;
    void setTeacher(const Person::Key& teacher) noexcept;
    bool updateTeacher(const Person::Key& teacher) noexcept;
    bool loadTeacher() noexcept;

    const Person::Key& student() const noexcept;
    void setStudent(const Person::Key& student) noexcept;
    bool updateStudent(const Person::Key& student) noexcept;
    bool loadStudent() noexcept;

    const QDate& date() const noexcept;
    void setDate(const QDate& date) noexcept;
    bool updateDate(const QDate& date) noexcept;
    bool loadDate() noexcept;

    const Type& type() const noexcept;
    void setType(const Type& type) noexcept;
    bool updateType(const Type& type) noexcept;
    bool loadType() noexcept;

    const Subject::Key& subject() const noexcept;
    void setSubject(const Subject::Key& subject) noexcept;
    bool updateSubject(const Subject::Key& subject) noexcept;
    bool loadSubject() noexcept;

    bool valid() const noexcept;

    BoolResult existsByKey() const noexcept;
    BoolResult existsByTuple() const noexcept;

    const ApiError& error() const noexcept;
};


}


#endif // STUDENTSUBJECT_H
