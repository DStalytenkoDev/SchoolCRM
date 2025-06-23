#include "TestStudent.h"

void TestStudent::initTestCase()
{
    if(not this->connection.open())
        QSKIP("Connection need to be opened");

    this->role.setConnection(&this->connection);
    this->role.setName("ROLE");
    if(not this->role.store())
        QSKIP("This test require role");

    this->person.setConnection(&this->connection);
    this->person.setFirstName("FNAME");
    this->person.setSecondName("SNAME");
    this->person.setRole(this->role.key());
    if(not this->person.store())
        QSKIP("This test require person");

    this->grade1.setConnection(&this->connection);
    this->grade2.setConnection(&this->connection);
    this->grade1.setName("NAME");
    this->grade2.setName("NAME");
    if(not this->grade1.store() or not this->grade2.store())
        QSKIP("This test require two grades");
}

void TestStudent::testValid()
{
    this->student.setConnection(nullptr);
    QVERIFY(not this->student.valid());

    this->connection.close();
    this->student.setConnection(&this->connection);
    QVERIFY(not this->student.valid());

    this->connection.open();
    QVERIFY(this->student.valid());
}

void TestStudent::testStore()
{
    this->student.setKey({this->person.key()});
    this->student.setGrade(this->grade1.key());
    QVERIFY(this->student.store());
}

void TestStudent::testLoad()
{
    this->student.setGrade({999999});
    QVERIFY(this->student.load());
    QCOMPARE(this->student.grade(), this->grade1.key());
}

void TestStudent::testUpdate()
{
    this->student.setGrade(this->grade2.key());
    QVERIFY(this->student.update());

    this->student.setGrade({99999});
    this->student.load();

    QCOMPARE(this->student.grade(), this->grade2.key());
}

void TestStudent::testLoadGrade()
{
    this->student.setGrade({999999});
    QVERIFY(this->student.loadGrade());
    QCOMPARE(this->student.grade(), this->grade2.key());
}

void TestStudent::testUpdateGrade()
{
    QVERIFY(this->student.updateGrade(this->grade1.key()));

    this->student.setGrade({999999});
    this->student.loadGrade();

    QCOMPARE(this->student.grade(), this->grade1.key());
}

void TestStudent::testLoadAll()
{
    dbapi::ApiError error;
    auto students = dbapi::Student::loadAll(&this->connection, &error);

    QCOMPARE(error.type, dbapi::ApiError::NoError);

    for(auto p_student : students)
    {
        if(p_student->key() == this->student.key())
        {
            QCOMPARE(p_student->grade(), this->student.grade());
            return;
        }

        delete p_student;
    }

    QVERIFY(false);
}

void TestStudent::testMakeAll()
{
    auto students = dbapi::Student::makeAll(&this->connection);

    for(auto p_student : students)
    {
        if(p_student->key() == this->student.key())
        {
            QVERIFY(true);
            return;
        }

        delete p_student;
    }

    QVERIFY(false);
}


void TestStudent::testExists()
{
    QCOMPARE(this->student.existsByKey(), dbapi::BoolResult::TrueResult);
    QCOMPARE(this->student.existsByTuple(), dbapi::BoolResult::TrueResult);

    this->student.setGrade({999999});
    QCOMPARE(this->student.existsByKey(), dbapi::BoolResult::TrueResult);
    QCOMPARE(this->student.existsByTuple(), dbapi::BoolResult::FalseResult);

    auto key = this->student.key();
    this->student.setKey({99999});

    QCOMPARE(this->student.existsByKey(), dbapi::BoolResult::FalseResult);
    QCOMPARE(this->student.existsByTuple(), dbapi::BoolResult::FalseResult);

    this->student.setKey(key);
}


void TestStudent::testRemove()
{
    QVERIFY(this->student.remove());
    QCOMPARE(this->student.existsByKey(), dbapi::FalseResult);
}

void TestStudent::cleanupTestCase()
{
    this->student.remove();
    this->person.remove();
    this->grade1.remove();
    this->grade2.remove();
    this->role.remove();

    this->connection.close();
}
