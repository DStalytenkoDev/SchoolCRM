 #include <QtSql/QSqlQuery>
#include <QtSql/QSqlError>

#include "StudentMark.h"


using dbapi::StudentMark;


bool StudentMark::Key::operator ==(const Key& key) const noexcept
{
    return this->id == key.id;
}

bool StudentMark::Key::operator !=(const Key& key) const noexcept
{
    return not (this->id == key.id);
}

StudentMark::StudentMark(Connection* p_connection) noexcept : p_connection(p_connection)
{
}

StudentMark::StudentMark(const Key& key, Connection* p_connection) noexcept : _key(key), p_connection(p_connection)
{
}

void StudentMark::setKey(const Key& key) noexcept
{
    this->_key = key;
}

const StudentMark::Key& StudentMark::key() const noexcept
{
    return this->_key;
}

bool StudentMark::valid() const noexcept
{
    if(this->p_connection == nullptr)
        return false;

    if(not this->p_connection->valid())
        return false;

    return true;
}

const dbapi::ApiError& StudentMark::error() const noexcept
{
    return this->_error;
}

dbapi::Connection* StudentMark::connection() const noexcept
{
    return this->p_connection;
}

void StudentMark::setConnection(dbapi::Connection* p_connection) noexcept
{
    this->p_connection = p_connection;
}

bool StudentMark::load() noexcept
{
    if(not this->valid())
    {
        this->_error.type = ApiError::ConnectionInvalidError;
        this->_error.text = "dbapi::StudentMark has invalid connection, connection may be nullptr or it did not open";
        return false;
    }

    QSqlQuery query(this->p_connection->database);
    query.prepare("select teacher_id, student_id, subject_id, type, mark, date from StudentMarks where id = :id");

    query.bindValue(":id", this->_key.id);

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

    query.next();
    this->_teacher.id = query.value(0).toInt();
    this->_student.id = query.value(1).toInt();
    this->_subject.id = query.value(2).toInt();
    this->_type = static_cast<dbapi::StudentMark::Type>(query.value(3).toInt());
    this->_mark = query.value(4).toInt();
    this->_date = QDate::fromString(query.value(5).toString(), Qt::DateFormat::ISODate);

    this->_error.setNoError();
    return true;
}

bool StudentMark::store() noexcept
{
    if(not this->valid())
    {
        this->_error.type = ApiError::ConnectionInvalidError;
        this->_error.text = "dbapi::StudentMark has invalid connection, connection may be nullptr or it did not open";
        return false;
    }

    QSqlQuery query(this->p_connection->database);
    query.prepare("insert StudentMarks (teacher_id, student_id, subject_id, type, mark, date)\
values (:teacher_id, :student_id, :subject_id, :type, :mark, :date)");

    query.bindValue(":teacher_id", this->_teacher.id);
    query.bindValue(":student_id", this->_student.id);
    query.bindValue(":subject_id", this->_subject.id);
    query.bindValue(":mark", this->_mark);

    query.bindValue(":type", static_cast<int>(this->_type));
    query.bindValue(":date", this->_date.toString(Qt::DateFormat::ISODate));

    if(not query.exec())
    {
        this->_error = query.lastError();
        return false;
    }

    query.prepare("select IDENT_CURRENT('StudentMarks')");

    if(not query.exec())
    {
        this->_error = query.lastError();
        return false;
    }

    query.next();
    this->_key.id = query.value(0).toInt();

    this->_error.setNoError();
    return true;
}

bool StudentMark::remove() noexcept
{
    if(not this->valid())
    {
        this->_error.type = ApiError::ConnectionInvalidError;
        this->_error.text = "dbapi::StudentMark has invalid connection, connection may be nullptr or it did not open";
        return false;
    }

    QSqlQuery query(this->p_connection->database);
    query.prepare("delete from StudentMarks where id = :id");

    query.bindValue(":id", this->_key.id);

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

    this->_error.setNoError();
    return true;
}

bool StudentMark::update() noexcept
{
    if(not this->valid())
    {
        this->_error.type = ApiError::ConnectionInvalidError;
        this->_error.text = "dbapi::StudentMark has invalid connection, connection may be nullptr or it did not open";
        return false;
    }

    QSqlQuery query(this->p_connection->database);
    query.prepare("update StudentMarks set teacher_id=:teacher_id, student_id=:student_id, subject_id=:subject_id, mark=:mark, date=:date, type=:type where id=:id");

    query.bindValue(":id", this->_key.id);
    query.bindValue(":teacher_id", this->_teacher.id);
    query.bindValue(":student_id", this->_student.id);
    query.bindValue(":subject_id", this->_subject.id);
    query.bindValue(":mark", this->_mark);
    query.bindValue(":type", static_cast<int>(this->_type));
    query.bindValue(":date", this->_date.toString(Qt::DateFormat::ISODate));

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

    this->_error.setNoError();
    return true;
}

QVector<StudentMark*> StudentMark::loadAll(Connection* p_connection, ApiError* p_error) noexcept
{
    if(p_connection == nullptr)
    {
        if(p_error)
        {
            p_error->type = ApiError::ConnectionInvalidError;
            p_error->text = "dbapi::StudentMark has invalid connection, connection may be nullptr or it did not open";
        }

        return {};
    }

    if(not p_connection->valid())
    {
        if(p_error)
        {
            p_error->type = ApiError::ConnectionInvalidError;
            p_error->text = "dbapi::StudentMark has invalid connection, connection may be nullptr or it did not open";
        }

        return {};
    }

    QSqlQuery query(p_connection->database);
    query.prepare("select * from StudentMarks");

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

    QVector<StudentMark*> studentMarks;

    while(query.next())
    {
        auto p_studentMark = new StudentMark(Key{query.value(0).toInt()}, p_connection);

        p_studentMark->_teacher.id = query.value(1).toInt();
        p_studentMark->_student.id = query.value(2).toInt();
        p_studentMark->_subject.id = query.value(3).toInt();
        p_studentMark->_type = static_cast<Type>(query.value(4).toInt());
        p_studentMark->_mark = query.value(5).toInt();
        p_studentMark->_date = query.value(6).toDate();

        studentMarks.append(p_studentMark);
    }

    return QVector<StudentMark*>(std::move(studentMarks));
}

QVector<StudentMark*> StudentMark::makeAll(Connection* p_connection, ApiError* p_error) noexcept
{
    if(p_connection == nullptr)
    {
        if(p_error)
        {
            p_error->type = ApiError::ConnectionInvalidError;
            p_error->text = "dbapi::StudentMark has invalid connection, connection may be nullptr or it did not open";
        }

        return {};
    }

    if(not p_connection->valid())
    {
        if(p_error)
        {
            p_error->type = ApiError::ConnectionInvalidError;
            p_error->text = "dbapi::StudentMark has invalid connection, connection may be nullptr or it did not open";
        }

        return {};
    }

    QSqlQuery query(p_connection->database);
    query.prepare("select id from StudentMarks");

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

    QVector<StudentMark*> studentMarks;

    while(query.next())
    {
        auto p_studentMark = new StudentMark(Key{query.value(0).toInt()}, p_connection);
        studentMarks.append(p_studentMark);
    }

    return QVector<StudentMark*>(std::move(studentMarks));
}

const int& StudentMark::mark() const noexcept
{
    return this->_mark;
}

void StudentMark::setMark(const int& mark) noexcept
{
    this->_mark = mark;
}

bool StudentMark::updateMark(const int& mark) noexcept
{
    if(not this->valid())
    {
        this->_error.type = ApiError::ConnectionInvalidError;
        this->_error.text = "dbapi::StudentMark has invalid connection, connection may be nullptr or it did not open";
        return false;
    }

    QSqlQuery query(this->p_connection->database);
    query.prepare("update StudentMarks set mark = :mark where id = :id");

    query.bindValue(":id", this->_key.id);
    query.bindValue(":mark", mark);

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

    this->_mark = mark;
    this->_error.setNoError();
    return true;
}

bool StudentMark::loadMark() noexcept
{
    if(not this->valid())
    {
        this->_error.type = ApiError::ConnectionInvalidError;
        this->_error.text = "dbapi::StudentMarks has invalid connection, connection may be nullptr or it did not open";
        return false;
    }

    QSqlQuery query(this->p_connection->database);
    query.prepare("select (mark) from StudentMarks where id = :id");
    query.bindValue(":id", this->_key.id);

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

    query.next();
    this->_mark = query.value(0).toInt();

    this->_error.setNoError();
    return true;
}

const dbapi::Person::Key &StudentMark::teacher() const noexcept
{
    return this->_teacher;
}

void StudentMark::setTeacher(const Person::Key &teacher) noexcept
{
    this->_teacher = teacher;
}

bool StudentMark::updateTeacher(const Person::Key &teacher) noexcept
{
    if(not this->valid())
    {
        this->_error.type = ApiError::ConnectionInvalidError;
        this->_error.text = "dbapi::StudentMark has invalid connection, connection may be nullptr or it did not open";
        return false;
    }

    QSqlQuery query(this->p_connection->database);
    query.prepare("update StudentMarks set teacher_id = :teacher where id = :id");

    query.bindValue(":id", this->_key.id);
    query.bindValue(":teacher", teacher.id);

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

    this->_teacher = teacher;
    this->_error.setNoError();
    return true;
}

bool StudentMark::loadTeacher() noexcept
{
    if(not this->valid())
    {
        this->_error.type = ApiError::ConnectionInvalidError;
        this->_error.text = "dbapi::StudentMarks has invalid connection, connection may be nullptr or it did not open";
        return false;
    }

    QSqlQuery query(this->p_connection->database);
    query.prepare("select (teacher_id) from StudentMarks where id = :id");
    query.bindValue(":id", this->_key.id);

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

    query.next();
    this->_teacher.id = query.value(0).toInt();

    this->_error.setNoError();
    return true;
}

const dbapi::Person::Key &StudentMark::student() const noexcept
{
    return this->_student;
}

void StudentMark::setStudent(const Person::Key &student) noexcept
{
    this->_student = student;
}

bool StudentMark::updateStudent(const Person::Key &student) noexcept
{
    if(not this->valid())
    {
        this->_error.type = ApiError::ConnectionInvalidError;
        this->_error.text = "dbapi::StudentMark has invalid connection, connection may be nullptr or it did not open";
        return false;
    }

    QSqlQuery query(this->p_connection->database);
    query.prepare("update StudentMarks set student_id = :student where id = :id");

    query.bindValue(":id", this->_key.id);
    query.bindValue(":student", student.id);

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

    this->_student = student;
    this->_error.setNoError();
    return true;
}

bool StudentMark::loadStudent() noexcept
{
    if(not this->valid())
    {
        this->_error.type = ApiError::ConnectionInvalidError;
        this->_error.text = "dbapi::StudentMarks has invalid connection, connection may be nullptr or it did not open";
        return false;
    }

    QSqlQuery query(this->p_connection->database);
    query.prepare("select (student_id) from StudentMarks where id = :id");
    query.bindValue(":id", this->_key.id);

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

    query.next();
    this->_student.id = query.value(0).toInt();

    this->_error.setNoError();
    return true;

}

const QDate &StudentMark::date() const noexcept
{
    return this->_date;
}

void StudentMark::setDate(const QDate &date) noexcept
{
    this->_date = date;
}

bool StudentMark::updateDate(const QDate &date) noexcept
{
    if(not this->valid())
    {
        this->_error.type = ApiError::ConnectionInvalidError;
        this->_error.text = "dbapi::StudentMark has invalid connection, connection may be nullptr or it did not open";
        return false;
    }

    QSqlQuery query(this->p_connection->database);
    query.prepare("update StudentMarks set date = :date where id = :id");

    query.bindValue(":id", this->_key.id);
    query.bindValue(":date", date.toString(Qt::DateFormat::ISODate));

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

    this->_date = date;
    this->_error.setNoError();
    return true;
}

bool StudentMark::loadDate() noexcept
{
    if(not this->valid())
    {
        this->_error.type = ApiError::ConnectionInvalidError;
        this->_error.text = "dbapi::StudentMarks has invalid connection, connection may be nullptr or it did not open";
        return false;
    }

    QSqlQuery query(this->p_connection->database);
    query.prepare("select (date) from StudentMarks where id = :id");
    query.bindValue(":id", this->_key.id);

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

    query.next();
    this->_date = query.value(0).toDate();

    this->_error.setNoError();
    return true;
}

const StudentMark::Type &StudentMark::type() const noexcept
{
    return this->_type;
}

void StudentMark::setType(const Type &type) noexcept
{
    this->_type = type;
}

bool StudentMark::updateType(const Type &type) noexcept
{
    if(not this->valid())
    {
        this->_error.type = ApiError::ConnectionInvalidError;
        this->_error.text = "dbapi::StudentMark has invalid connection, connection may be nullptr or it did not open";
        return false;
    }

    QSqlQuery query(this->p_connection->database);
    query.prepare("update StudentMarks set type = :type where id = :id");

    query.bindValue(":id", this->_key.id);
    query.bindValue(":type", static_cast<int>(type));

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

    this->_type = type;
    this->_error.setNoError();
    return true;
}

bool StudentMark::loadType() noexcept
{
    if(not this->valid())
    {
        this->_error.type = ApiError::ConnectionInvalidError;
        this->_error.text = "dbapi::StudentMarks has invalid connection, connection may be nullptr or it did not open";
        return false;
    }

    QSqlQuery query(this->p_connection->database);
    query.prepare("select (type) from StudentMarks where id = :id");
    query.bindValue(":id", this->_key.id);

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

    query.next();
    this->_type = static_cast<Type>(query.value(0).toInt());

    this->_error.setNoError();
    return true;
}

const dbapi::Subject::Key &StudentMark::subject() const noexcept
{
    return this->_subject;
}

void StudentMark::setSubject(const Subject::Key &subject) noexcept
{
    this->_subject = subject;
}

bool StudentMark::updateSubject(const Subject::Key &subject) noexcept
{
    if(not this->valid())
    {
        this->_error.type = ApiError::ConnectionInvalidError;
        this->_error.text = "dbapi::StudentMark has invalid connection, connection may be nullptr or it did not open";
        return false;
    }

    QSqlQuery query(this->p_connection->database);
    query.prepare("update StudentMarks set subject_id = :subject where id = :id");

    query.bindValue(":id", this->_key.id);
    query.bindValue(":subject", subject.id);

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

    this->_subject = subject;
    this->_error.setNoError();
    return true;

}

bool StudentMark::loadSubject() noexcept
{
    if(not this->valid())
    {
        this->_error.type = ApiError::ConnectionInvalidError;
        this->_error.text = "dbapi::StudentMarks has invalid connection, connection may be nullptr or it did not open";
        return false;
    }

    QSqlQuery query(this->p_connection->database);
    query.prepare("select (subject_id) from StudentMarks where id = :id");
    query.bindValue(":id", this->_key.id);

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

    query.next();
    this->_subject.id = query.value(0).toInt();

    this->_error.setNoError();
    return true;

}

dbapi::BoolResult StudentMark::existsByKey() const noexcept
{
    if(not this->valid())
    {
        this->_error.type = ApiError::ConnectionInvalidError;
        this->_error.text = "dbapi::StudentMark has invalid connection, connection may be nullptr or it did not open";
        return ErrorResult;
    }

    QSqlQuery query(this->p_connection->database);
    query.prepare("select NULL from StudentMarks where id = :id");

    query.bindValue(":id", this->_key.id);

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

dbapi::BoolResult StudentMark::existsByTuple() const noexcept
{
    if(not this->valid())
    {
        this->_error.type = ApiError::ConnectionInvalidError;
        this->_error.text = "dbapi::StudentMark has invalid connection, connection may be nullptr or it did not open";
        return ErrorResult;
    }

    QSqlQuery query(this->p_connection->database);
    query.prepare("select NULL from StudentMarks where id = :id and \
teacher_id = :teacher and \
student_id = :student and \
subject_id = :subject and \
type = :type and \
mark = :mark and \
date = :date");

    query.bindValue(":id", this->_key.id);
    query.bindValue(":teacher", this->_teacher.id);
    query.bindValue(":student", this->_student.id);
    query.bindValue(":subject", this->_subject.id);
    query.bindValue(":mark", this->_mark);
    query.bindValue(":type", static_cast<int>(this->_type));
    query.bindValue(":date", this->_date.toString(Qt::DateFormat::ISODate));

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

