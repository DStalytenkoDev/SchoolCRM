#include <QtSql/QSqlQuery>
#include <QtSql/QSqlError>

#include "ClassSubjectsList.h"


using dbapi::ClassSubjectsList;


bool ClassSubjectsList::Key::operator ==(const Key& key) const noexcept
{
    return this->grade == key.grade;
}

bool ClassSubjectsList::Key::operator !=(const Key& key) const noexcept
{
    return not (this->grade == key.grade);
}

ClassSubjectsList::ClassSubjectsList(Connection* p_connection) noexcept : p_connection(p_connection)
{
}

ClassSubjectsList::ClassSubjectsList(const Key& key, Connection* p_connection) noexcept : _key(key), p_connection(p_connection)
{
}

void ClassSubjectsList::setKey(const Key& key) noexcept
{
    this->_key = key;
}

const ClassSubjectsList::Key& ClassSubjectsList::key() const noexcept
{
    return this->_key;
}

bool ClassSubjectsList::valid() const noexcept
{
    if(this->p_connection == nullptr)
        return false;

    if(not this->p_connection->valid())
        return false;

    return true;
}

const dbapi::ApiError& ClassSubjectsList::error() const noexcept
{
    return this->_error;
}

dbapi::Connection* ClassSubjectsList::connection() const noexcept
{
    return this->p_connection;
}

void ClassSubjectsList::setConnection(dbapi::Connection* p_connection) noexcept
{
    this->p_connection = p_connection;
}

bool ClassSubjectsList::load() noexcept
{
    if(not this->valid())
    {
        this->_error.type = ApiError::ConnectionInvalidError;
        this->_error.text = "dbapi::ClassSubjectsList has invalid connection, connection may be nullptr or it did not open";
        return false;
    }

    QSqlQuery query(this->p_connection->database);
    query.prepare("select (subject_id) from ClassesSubjects where class_id = :class_id");

    query.bindValue(":class_id", this->_key.grade.id);

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

bool ClassSubjectsList::store() noexcept
{
    if(not this->valid())
    {
        this->_error.type = ApiError::ConnectionInvalidError;
        this->_error.text = "dbapi::ClassSubjectsList has invalid connection, connection may be nullptr or it did not open";
        return false;
    }

    QSqlQuery query(this->p_connection->database);

    for(auto& subject : this->_subjects)
    {
        query.prepare("insert ClassesSubjects (class_id, subject_id) values (:class_id, :subject_id)");
        query.bindValue(":class_id", this->_key.grade.id);
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

bool ClassSubjectsList::remove() noexcept
{
    if(not this->valid())
    {
        this->_error.type = ApiError::ConnectionInvalidError;
        this->_error.text = "dbapi::ClassSubjectsList has invalid connection, connection may be nullptr or it did not open";
        return false;
    }

    QSqlQuery query(this->p_connection->database);

    for(auto& subject : this->_subjects)
    {
        query.prepare("delete from ClassesSubjects where class_id = :class_id and subject_id = :subject_id");
        query.bindValue(":class_id", this->_key.grade.id);
        query.bindValue(":subject_id", subject.id);

        if(not query.exec())
        {
            this->_error = query.lastError();
            return false;
        }

        if(not query.numRowsAffected())
        {
            this->_error.type = ApiError::KeyError;
            this->_error.text = "Invalid key, person";
            return false;
        }
    }

    this->clearUpdate();

    this->_error.setNoError();
    return true;
}

bool ClassSubjectsList::update() noexcept
{
    if(not this->valid())
    {
        this->_error.type = ApiError::ConnectionInvalidError;
        this->_error.text = "dbapi::ClassSubjectsList has invalid connection, connection may be nullptr or it did not open";
        return false;
    }

    QSqlQuery query(this->p_connection->database);

    for(const auto& subject : this->subjectsToStore)
    {
        query.prepare("insert ClassesSubjects (class_id, subject_id) values (:class_id, :subject_id)");
        query.bindValue(":class_id", this->_key.grade.id);
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
        query.prepare("delete from ClassesSubjects where class_id = :class_id and subject_id = :subject_id");
        query.bindValue(":class_id", this->_key.grade.id);
        query.bindValue(":subject_id", subject.id);

        if(not query.exec())
        {
            this->_error = query.lastError();
            return false;
        }

        if(not query.numRowsAffected())
        {
            this->_error.type = ApiError::KeyError;
            this->_error.text = "Invalid key, person";
            return false;
        }
    }

    this->subjectsToRemove.clear();

    this->_error.setNoError();
    return true;
}

void ClassSubjectsList::removeSubject(QVector<Subject::Key>::ConstIterator iterator) noexcept
{
    this->subjectsToRemove.append(*iterator);
    this->_subjects.erase(iterator);
}

void ClassSubjectsList::appendSubject(const Subject::Key& subject) noexcept
{
    this->subjectsToStore.append(subject);
    this->_subjects.append(subject);
}

void ClassSubjectsList::clearUpdate() noexcept
{
    this->subjectsToRemove.clear();
    this->subjectsToStore.clear();
}

void ClassSubjectsList::clear() noexcept
{
    this->clearUpdate();
    this->_subjects.clear();
}

const QVector<dbapi::Subject::Key>& ClassSubjectsList::subjects() const noexcept
{
    return this->_subjects;
}

QVector<ClassSubjectsList*> ClassSubjectsList::loadAll(Connection* p_connection, ApiError* p_error) noexcept
{
    if(p_connection == nullptr)
    {
        if(p_error)
        {
            p_error->type = ApiError::ConnectionInvalidError;
            p_error->text = "dbapi::ClassSubjectsList has invalid connection, connection may be nullptr or it did not open";
        }

        return {};
    }

    if(not p_connection->valid())
    {
        if(p_error)
        {
            p_error->type = ApiError::ConnectionInvalidError;
            p_error->text = "dbapi::ClassSubjectsList has invalid connection, connection may be nullptr or it did not open";
        }

        return {};
    }

    QSqlQuery query(p_connection->database);
    query.prepare("select distinct (class_id) from ClassesSubjects");

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

    QVector<ClassSubjectsList*> classSubjectsLists;

    while(query.next())
    {
        auto p_classSubjectsList = new ClassSubjectsList(Key{query.value(0).toInt()}, p_connection);

        if(not p_classSubjectsList->load())
        {
            if(p_error)
                *p_error = p_classSubjectsList->error();

            delete p_classSubjectsList;

            for(auto p_classSubjectsList : classSubjectsLists)
                delete p_classSubjectsList;

            return {};
        }

        classSubjectsLists.append(p_classSubjectsList);
    }

    return QVector<ClassSubjectsList*>(std::move(classSubjectsLists));
}

QVector<ClassSubjectsList*> ClassSubjectsList::makeAll(Connection* p_connection, ApiError* p_error) noexcept
{
    if(p_connection == nullptr)
    {
        if(p_error)
        {
            p_error->type = ApiError::ConnectionInvalidError;
            p_error->text = "dbapi::ClassSubjectsList has invalid connection, connection may be nullptr or it did not open";
        }

        return {};
    }

    if(not p_connection->valid())
    {
        if(p_error)
        {
            p_error->type = ApiError::ConnectionInvalidError;
            p_error->text = "dbapi::ClassSubjectsList has invalid connection, connection may be nullptr or it did not open";
        }

        return {};
    }

    QSqlQuery query(p_connection->database);
    query.prepare("select distinct (class_id) from ClassesSubjects");

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

    QVector<ClassSubjectsList*> classSubjectsLists;

    while(query.next())
    {
        auto p_classSubjectsList = new ClassSubjectsList(Key{query.value(0).toInt()}, p_connection);
        classSubjectsLists.append(p_classSubjectsList);
    }

    return QVector<ClassSubjectsList*>(std::move(classSubjectsLists));
}

dbapi::BoolResult ClassSubjectsList::existsByKey() const noexcept
{
    if(not this->valid())
    {
        this->_error.type = ApiError::ConnectionInvalidError;
        this->_error.text = "dbapi::ClassSubjectsList has invalid connection, connection may be nullptr or it did not open";
        return ErrorResult;
    }

    QSqlQuery query(this->p_connection->database);
    query.prepare("select top(1) NULL from ClassesSubjects where class_id = :class_id");

    query.bindValue(":class_id", this->_key.grade.id);

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

dbapi::BoolResult ClassSubjectsList::existsByTuple() const noexcept
{
    if(not this->valid())
    {
        this->_error.type = ApiError::ConnectionInvalidError;
        this->_error.text = "dbapi::ClassSubjectsList has invalid connection, connection may be nullptr or it did not open";
        return ErrorResult;
    }

    if(not this->_subjects.size())
        return this->existsByKey();

    QSqlQuery query(this->p_connection->database);
    query.prepare("select NULL from ClassesSubjects where class_id = :class_id");
    query.bindValue(":class_id", this->_key.grade.id);

    if(not query.exec())
    {
        this->_error = query.lastError();
        return ErrorResult;
    }

    if(query.numRowsAffected() != this->_subjects.size())
        return FalseResult;

    for(const auto& subject : this->_subjects)
    {
        query.prepare("select NULL from ClassesSubjects where class_id = :class_id and subject_id = :subject_id");

        query.bindValue(":class_id", this->_key.grade.id);
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

