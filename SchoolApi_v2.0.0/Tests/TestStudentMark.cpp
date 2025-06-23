#include "TestStudentMark.h"


#define TEST_MARK 10
#define TEST_MARK2 12


void TestStudentMark::initTestCase()
{
    if(not this->connection.open())
        QSKIP("Connection need to be opened");

    this->roleForTeacher.setConnection(&this->connection);
    this->roleForTeacher.setName("TEACHER");
    if(not this->roleForTeacher.store())
        QSKIP("This test require a role for teacher");

    this->roleForStudent.setConnection(&this->connection);
    this->roleForStudent.setName("STUDENT");
    if(not this->roleForStudent.store())
        QSKIP("This test require a role for student");

    this->teacher.setConnection(&this->connection);
    this->teacher.setFirstName("FNAME");
    this->teacher.setSecondName("SNAME");
    this->teacher.setRole(this->roleForTeacher.key());

    if(not this->teacher.store())
        QSKIP("This test require a teacher");

    this->student.setConnection(&this->connection);
    this->student.setFirstName("FNAME");
    this->student.setSecondName("SNAME");
    this->student.setRole(this->roleForStudent.key());

    if(not this->student.store())
        QSKIP("This test require a student");

    this->teacher1.setConnection(&this->connection);
    this->teacher1.setFirstName("FNAME");
    this->teacher1.setSecondName("SNAME");
    this->teacher1.setRole(this->roleForTeacher.key());

    if(not this->teacher1.store())
        QSKIP("This test require a teacher 1");

    this->student1.setConnection(&this->connection);
    this->student1.setFirstName("FNAME");
    this->student1.setSecondName("SNAME");
    this->student1.setRole(this->roleForStudent.key());

    if(not this->student1.store())
        QSKIP("This test require a student 1");

    this->subject.setConnection(&this->connection);
    this->subject.setName("SUBJECT");

    if(not this->subject.store())
        QSKIP("This test require a subject");

    this->subject1.setConnection(&this->connection);
    this->subject1.setName("SUBJECT");

    if(not this->subject1.store())
        QSKIP("This test require a subject 1");
}

void TestStudentMark::testValid()
{
    this->mark.setConnection(nullptr);
    QVERIFY(not this->mark.valid());

    this->connection.close();
    this->mark.setConnection(&this->connection);
    QVERIFY(not this->mark.valid());

    this->connection.open();
    QVERIFY(this->mark.valid());
}

void TestStudentMark::testStore()
{
    this->mark.setTeacher(this->teacher.key());
    this->mark.setStudent(this->student.key());
    this->mark.setSubject(this->subject.key());
    this->mark.setMark(TEST_MARK);
    this->mark.setType(dbapi::StudentMark::Type::AlphaTest);
    this->mark.setDate(QDate::currentDate());

    QVERIFY(this->mark.store());
}

void TestStudentMark::testLoad()
{
    this->mark.setTeacher({this->teacher.key().id + 1});
    this->mark.setStudent({this->student.key().id + 1});
    this->mark.setSubject({this->subject.key().id + 1});
    this->mark.setMark(TEST_MARK + 1);
    this->mark.setType(dbapi::StudentMark::Type::BetaTest);
    this->mark.setDate({});

    QVERIFY(this->mark.load());

    QCOMPARE(this->mark.teacher(), this->teacher.key());
    QCOMPARE(this->mark.student(), this->student.key());
    QCOMPARE(this->mark.subject(), this->subject.key());
    QCOMPARE(this->mark.date(), QDate::currentDate());
    QCOMPARE(this->mark.type(), dbapi::StudentMark::Type::AlphaTest);
    QCOMPARE(this->mark.mark(), TEST_MARK);
}

void TestStudentMark::testUpdate()
{
    this->mark.setTeacher(this->teacher1.key());
    this->mark.setStudent(this->student1.key());
    this->mark.setSubject(this->subject1.key());
    this->mark.setMark(TEST_MARK2);
    this->mark.setType(dbapi::StudentMark::Type::GamaTest);
    this->mark.setDate(QDate::currentDate().addDays(1));

    QVERIFY(this->mark.update());

    this->mark.setTeacher({this->teacher1.key().id + 1});
    this->mark.setStudent({this->student1.key().id + 1});
    this->mark.setSubject({this->subject1.key().id + 1});
    this->mark.setMark(TEST_MARK);
    this->mark.setType(dbapi::StudentMark::Type::BetaTest);
    this->mark.setDate({});

    this->mark.load();

    QCOMPARE(this->mark.teacher(), this->teacher1.key());
    QCOMPARE(this->mark.student(), this->student1.key());
    QCOMPARE(this->mark.subject(), this->subject1.key());
    QCOMPARE(this->mark.date(), QDate::currentDate().addDays(1));
    QCOMPARE(this->mark.type(), dbapi::StudentMark::Type::GamaTest);
    QCOMPARE(this->mark.mark(), TEST_MARK2);
}

void TestStudentMark::testLoadMark()
{
    this->mark.setMark({99});
    QVERIFY(this->mark.loadMark());
    QCOMPARE(this->mark.mark(), TEST_MARK2);
}

void TestStudentMark::testUpdateMark()
{
    QVERIFY(this->mark.updateMark(TEST_MARK));

    this->mark.setMark({99});
    this->mark.loadMark();

    QCOMPARE(this->mark.mark(), TEST_MARK);
}

void TestStudentMark::testLoadTeacher()
{
    this->mark.setTeacher(this->teacher.key());
    QVERIFY(this->mark.loadTeacher());
    QCOMPARE(this->mark.teacher(), this->teacher1.key());
}

void TestStudentMark::testUpdateTeacher()
{
    QVERIFY(this->mark.updateTeacher(this->teacher.key()));

    this->mark.setTeacher(this->teacher1.key());
    this->mark.loadTeacher();

    QCOMPARE(this->mark.teacher(), this->teacher.key());
}

void TestStudentMark::testLoadStudent()
{
    this->mark.setStudent(this->student.key());
    QVERIFY(this->mark.loadStudent());
    QCOMPARE(this->mark.student(), this->student1.key());
}

void TestStudentMark::testUpdateStudent()
{
    QVERIFY(this->mark.updateStudent(this->student.key()));

    this->mark.setStudent(this->student1.key());
    this->mark.loadStudent();

    QCOMPARE(this->mark.student(), this->student.key());
}

void TestStudentMark::testLoadDate()
{
    this->mark.setDate(QDate::currentDate());
    QVERIFY(this->mark.loadDate());
    QCOMPARE(this->mark.date(), QDate::currentDate().addDays(1));
}

void TestStudentMark::testUpdateDate()
{
    QVERIFY(this->mark.updateDate(QDate::currentDate()));

    this->mark.setDate(QDate::currentDate().addDays(1));
    this->mark.loadDate();

    QCOMPARE(this->mark.date(), QDate::currentDate());
}

void TestStudentMark::testLoadType()
{
    this->mark.setType(dbapi::StudentMark::Type::AlphaTest);
    QVERIFY(this->mark.loadType());
    QCOMPARE(this->mark.type(), dbapi::StudentMark::Type::GamaTest);
}

void TestStudentMark::testUpdateType()
{
    QVERIFY(this->mark.updateType(dbapi::StudentMark::Type::AlphaTest));

    this->mark.setType(dbapi::StudentMark::Type::GamaTest);
    this->mark.loadType();

    QCOMPARE(this->mark.type(), dbapi::StudentMark::Type::AlphaTest);
}

void TestStudentMark::testLoadSubject()
{
    this->mark.setSubject(this->subject.key());
    QVERIFY(this->mark.loadSubject());
    QCOMPARE(this->mark.subject(), this->subject1.key());
}

void TestStudentMark::testUpdateSubject()
{
    QVERIFY(this->mark.updateSubject(this->subject.key()));

    this->mark.setSubject(this->subject1.key());
    this->mark.loadSubject();

    QCOMPARE(this->mark.subject(), this->subject.key());
}

void TestStudentMark::testLoadAll()
{
    dbapi::ApiError error;
    auto marks = dbapi::StudentMark::loadAll(&this->connection, &error);

    QCOMPARE(error.type, dbapi::ApiError::NoError);

    for(auto p_mark : marks)
    {
        if(p_mark->key() == this->mark.key())
        {
            QCOMPARE(p_mark->mark(), this->mark.mark());
            QCOMPARE(p_mark->teacher(), this->mark.teacher());
            QCOMPARE(p_mark->student(), this->mark.student());
            QCOMPARE(p_mark->subject(), this->mark.subject());
            QCOMPARE(p_mark->type(), this->mark.type());
            QCOMPARE(p_mark->date(), this->mark.date());
            return;
        }

        delete p_mark;
    }

    QVERIFY(false);
}

void TestStudentMark::testMakeAll()
{
    auto marks = dbapi::StudentMark::makeAll(&this->connection);

    for(auto p_mark : marks)
    {
        if(p_mark->key() == this->mark.key())
        {
            QVERIFY(true);
            return;
        }

        delete p_mark;
    }

    QVERIFY(false);
}


void TestStudentMark::testExists()
{
    QCOMPARE(this->mark.existsByKey(), dbapi::BoolResult::TrueResult);
    QCOMPARE(this->mark.existsByTuple(), dbapi::BoolResult::TrueResult);

    this->mark.setMark(999999);
    QCOMPARE(this->mark.existsByKey(), dbapi::BoolResult::TrueResult);
    QCOMPARE(this->mark.existsByTuple(), dbapi::BoolResult::FalseResult);

    auto key = this->mark.key();
    this->mark.setKey({999999999});

    QCOMPARE(this->mark.existsByKey(), dbapi::BoolResult::FalseResult);
    QCOMPARE(this->mark.existsByTuple(), dbapi::BoolResult::FalseResult);

    this->mark.setKey(key);
}


void TestStudentMark::testRemove()
{
    QVERIFY(this->mark.remove());
    QCOMPARE(this->mark.existsByKey(), dbapi::FalseResult);
}

void TestStudentMark::cleanupTestCase()
{
    this->student.remove();
    this->student1.remove();

    this->teacher.remove();
    this->teacher1.remove();

    this->roleForStudent.remove();
    this->roleForTeacher.remove();

    this->subject.remove();
    this->subject1.remove();

    this->connection.close();
}
