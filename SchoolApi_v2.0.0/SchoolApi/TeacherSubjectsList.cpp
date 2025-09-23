#include <QtSql/QSqlQuery>

#include "TeacherSubjectsList.h"

using dbapi::TeacherSubjectsList;

bool TeacherSubjectsList::Key::operator ==(const Key& key) const noexcept
{
    return this->person == key.person;
}

bool TeacherSubjectsList::Key::operator !=(const Key& key) const noexcept
{
    return not (this->person == key.person);
}

TeacherSubjectsList::TeacherSubjectsList(Connection* p_connection) noexcept : p_connection(p_connection)
{
}

TeacherSubjectsList::TeacherSubjectsList(const Key& key, Connection* p_connection) noexcept : _key(key), p_connection(p_connection)
{
}

void TeacherSubjectsList::setKey(const Key& key) noexcept
{
    this->_key = key;
}

const TeacherSubjectsList::Key& TeacherSubjectsList::key() const noexcept
{
    return this->_key;
}

bool TeacherSubjectsList::valid() const noexcept
{
    if(this->p_connection == nullptr)
        return false;

    if(not this->p_connection->valid())
        return false;

    return true;
}

const dbapi::ApiError& TeacherSubjectsList::error() const noexcept
{
    return this->_error;
}

dbapi::Connection* TeacherSubjectsList::connection() const noexcept
{
    return this->p_connection;
}

void TeacherSubjectsList::setConnection(dbapi::Connection* p_connection) noexcept
{
    this->p_connection = p_connection;
}

bool TeacherSubjectsList::load() noexcept
{
    if(not this->valid())
    {
        this->_error.type = ApiError::ConnectionInvalidError;
        this->_error.text = "dbapi::TeacherSubjectsList has invalid connection, connection may be nullptr or it did not open";
        return false;
    }

    QSqlQuery query(this->p_connection->database);
    query.prepare("select (subject_id) from TeachersSubjects where person_id = :person_id");
    query.bindValue(":person_id", this->_key.person.id);

    if(not query.exec())
    {
        this->_error = query.lastError();
        return false;
    }

    if(not query.numRowsAffected())
    {
        this->_error.type = ApiError::KeyError;
        this->_error.text = "Invalid key, it does not exists";
        return false;
    }

    this->clear();

    while(query.next())
        this->_subjects.append(Subject::Key{query.value(0).toInt()});

    this->_error.setNoError();
    return true;
}

bool TeacherSubjectsList::store() noexcept
{
    if(not this->valid())
    {
        this->_error.type = ApiError::ConnectionInvalidError;
        this->_error.text = "dbapi::TeacherSubjectsList has invalid connection, connection may be nullptr or it did not open";
        return false;
    }

    QSqlQuery query(this->p_connection->database);

    for(auto& subject : this->_subjects)
    {
        query.prepare("insert TeachersSubjects (person_id, subject_id) values (:person_id, :subject_id)");

        query.bindValue(":person_id", this->_key.person.id);
        query.bindValue(":subject_id", subject.id);

        if(not query.exec())
        {
            this->_error = query.lastError();
            return false;
        }
    }

    this->clearUpdate();

    this->_error.setNoError();
    return true;
}

bool TeacherSubjectsList::remove() noexcept
{
    if(not this->valid())
    {
        this->_error.type = ApiError::ConnectionInvalidError;
        this->_error.text = "dbapi::TeacherSubjectsList has invalid connection, connection may be nullptr or it did not open";
        return false;
    }

    QSqlQuery query(this->p_connection->database);

    for(auto& subject : this->_subjects)
    {
        query.prepare("delete from TeachersSubjects where person_id = :person_id and subject_id = :subject_id");

        query.bindValue(":person_id", this->_key.person.id);
        query.bindValue(":subject_id", subject.id);

        if(not query.exec())
        {
            this->_error = query.lastError();
            return false;
        }

        if(not query.numRowsAffected())
        {
            this->_error.type = ApiError::KeyError;
            this->_error.text = "Invalid key, it does not exists";
            return false;
        }
    }

    this->clearUpdate();

    this->_error.setNoError();
    return true;
}

bool TeacherSubjectsList::update() noexcept
{
    if(not this->valid())
    {
        this->_error.type = ApiError::ConnectionInvalidError;
        this->_error.text = "dbapi::TeacherSubjectsList has invalid connection, connection may be nullptr or it did not open";
        return false;
    }

    QSqlQuery query(this->p_connection->database);

    for(const auto& subject : this->subjectsToStore)
    {
        query.prepare("insert TeachersSubjects (person_id, subject_id) values (:person_id, :subject_id)");

        query.bindValue(":person_id", this->_key.person.id);
        query.bindValue(":subject_id", subject.id);

        if(not query.exec())
        {
            this->_error = query.lastError();
            return false;
        }
    }

    this->subjectsToStore.clear();

    for(const auto& subject : this->subjectsToRemove)
    {
        query.prepare("delete from TeachersSubjects where person_id = :person_id and subject_id = :subject_id");

        query.bindValue(":person_id", this->_key.person.id);
        query.bindValue(":subject_id", subject.id);

        if(not query.exec())
        {
            this->_error = query.lastError();
            return false;
        }

        if(not query.numRowsAffected())
        {
            this->_error.type = ApiError::KeyError;
            this->_error.text = "Invalid key, it does not exists";
            return false;
        }
    }

    this->subjectsToRemove.clear();

    this->_error.setNoError();
    return true;
}

void TeacherSubjectsList::removeSubject(QVector<Subject::Key>::ConstIterator iterator) noexcept
{
    this->subjectsToRemove.append(*iterator);
    this->_subjects.erase(iterator);
}

void TeacherSubjectsList::appendSubject(const Subject::Key& subject) noexcept
{
    this->subjectsToStore.append(subject);
    this->_subjects.append(subject);
}

void TeacherSubjectsList::clearUpdate() noexcept
{
    this->subjectsToRemove.clear();
    this->subjectsToStore.clear();
}

void TeacherSubjectsList::clear() noexcept
{
    this->clearUpdate();
    this->_subjects.clear();
}

const QVector<dbapi::Subject::Key>& TeacherSubjectsList::subjects() const noexcept
{
    return this->_subjects;
}

QVector<TeacherSubjectsList*> TeacherSubjectsList::loadAll(Connection* p_connection, ApiError* p_error) noexcept
{
    if(p_connection == nullptr)
    {
        if(p_error)
        {
            p_error->type = ApiError::ConnectionInvalidError;
            p_error->text = "dbapi::TeacherSubjectsList has invalid connection, connection may be nullptr or it did not open";
        }

        return {};
    }

    if(not p_connection->valid())
    {
        if(p_error)
        {
            p_error->type = ApiError::ConnectionInvalidError;
            p_error->text = "dbapi::TeacherSubjectsList has invalid connection, connection may be nullptr or it did not open";
        }

        return {};
    }

    QSqlQuery query(p_connection->database);
    query.prepare("select distinct (person_id) from TeachersSubjects");

    if(not query.exec())
    {
        if(p_error)
            *p_error = query.lastError();

        return {};
    }

    if(p_error)
        p_error->setNoError();

    if(not query.numRowsAffected())
        return {};

    QVector<TeacherSubjectsList*> teacherSubjectsLists;

    while(query.next())
    {
        auto p_teacherSubjectsList = new TeacherSubjectsList(Key{query.value(0).toInt()}, p_connection);

        if(not p_teacherSubjectsList->load())
        {
            if(p_error)
                *p_error = p_teacherSubjectsList->error();

            delete p_teacherSubjectsList;

            for(auto p_teacherSubjectsList : teacherSubjectsLists)
                delete p_teacherSubjectsList;

            return {};
        }

        teacherSubjectsLists.append(p_teacherSubjectsList);
    }

    return QVector<TeacherSubjectsList*>(std::move(teacherSubjectsLists));
}

QVector<TeacherSubjectsList*> TeacherSubjectsList::makeAll(Connection* p_connection, ApiError* p_error) noexcept
{
    if(p_connection == nullptr)
    {
        if(p_error)
        {
            p_error->type = ApiError::ConnectionInvalidError;
            p_error->text = "dbapi::TeacherSubjectsList has invalid connection, connection may be nullptr or it did not open";
        }

        return {};
    }

    if(not p_connection->valid())
    {
        if(p_error)
        {
            p_error->type = ApiError::ConnectionInvalidError;
            p_error->text = "dbapi::TeacherSubjectsList has invalid connection, connection may be nullptr or it did not open";
        }

        return {};
    }

    QSqlQuery query(p_connection->database);
    query.prepare("select distinct (person_id) from TeachersSubjects");

    if(not query.exec())
    {
        if(p_error)
            *p_error = query.lastError();

        return {};
    }

    if(p_error)
        p_error->setNoError();

    if(not query.numRowsAffected())
        return {};

    QVector<TeacherSubjectsList*> teacherSubjectsLists;

    while(query.next())
    {
        auto p_teacherSubjectsList = new TeacherSubjectsList(Key{query.value(0).toInt()}, p_connection);
        teacherSubjectsLists.append(p_teacherSubjectsList);
    }

    return QVector<TeacherSubjectsList*>(std::move(teacherSubjectsLists));
}

dbapi::BoolResult TeacherSubjectsList::existsByKey() const noexcept
{
    if(not this->valid())
    {
        this->_error.type = ApiError::ConnectionInvalidError;
        this->_error.text = "dbapi::TeacherSubjectsList has invalid connection, connection may be nullptr or it did not open";
        return ErrorResult;
    }

    QSqlQuery query(this->p_connection->database);
    query.prepare("select top(1) NULL from TeachersSubjects where person_id = :person_id");
    query.bindValue(":person_id", this->_key.person.id);

    if(not query.exec())
    {
        this->_error = query.lastError();
        return ErrorResult;
    }

    this->_error.setNoError();

    if(not query.numRowsAffected())
        return FalseResult;

    return TrueResult;
}

dbapi::BoolResult TeacherSubjectsList::existsByTuple() const noexcept
{
    if(not this->valid())
    {
        this->_error.type = ApiError::ConnectionInvalidError;
        this->_error.text = "dbapi::TeacherSubjectsList has invalid connection, connection may be nullptr or it did not open";
        return ErrorResult;
    }

    if(not this->_subjects.size())
        return this->existsByKey();

    QSqlQuery query(this->p_connection->database);
    query.prepare("select NULL from TeachersSubjects where person_id = :person_id");
    query.bindValue(":person_id", this->_key.person.id);

    if(not query.exec())
    {
        this->_error = query.lastError();
        return ErrorResult;
    }

    if(query.numRowsAffected() != this->_subjects.size())
        return FalseResult;

    for(const auto& subject : this->_subjects)
    {
        query.prepare("select NULL from TeachersSubjects where person_id = :person_id and subject_id = :subject_id");

        query.bindValue(":person_id", this->_key.person.id);
        query.bindValue(":subject_id", subject.id);

        if(not query.exec())
        {
            this->_error = query.lastError();
            return ErrorResult;
        }

        if(not query.numRowsAffected())
            return FalseResult;
    }

    this->_error.setNoError();
    return TrueResult;
}

