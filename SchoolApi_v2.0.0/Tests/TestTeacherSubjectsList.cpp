#include "TestTeacherSubjectsList.h"


void TestTeacherSubjectsList::initTestCase()
{
    this->connection.open();

    this->role.setConnection(&this->connection);
    this->role.setName("ROLE");
    if(not this->role.store())
        QSKIP("Test require some role");

    this->person.setConnection(&this->connection);
    this->person.setRole(this->role.key());
    this->person.setFirstName("FNAME");
    this->person.setSecondName("SNAME");
    this->person.setBirthday(103200);
    if(not this->person.store())
        QSKIP("Test require some person");


    this->subject1.setConnection(&this->connection);
    this->subject1.setName("SUBJECT1");

    this->subject2.setConnection(&this->connection);
    this->subject2.setName("SUBJECT2");

    if(not this->subject1.store() or not this->subject2.store())
        QSKIP("Test require two subjects");
}

void TestTeacherSubjectsList::testValid()
{
    this->teacherSubjectsList.setConnection(nullptr);
    QVERIFY(not this->teacherSubjectsList.valid());

    this->connection.close();
    this->teacherSubjectsList.setConnection(&this->connection);
    QVERIFY(not this->teacherSubjectsList.valid());

    this->connection.open();
    QVERIFY(this->teacherSubjectsList.valid());
}

void TestTeacherSubjectsList::testStore()
{
    this->teacherSubjectsList.setKey({this->person.key()});
    this->teacherSubjectsList.appendSubject(this->subject1.key());
    this->teacherSubjectsList.appendSubject(this->subject2.key());

    QVERIFY(this->teacherSubjectsList.store());
}

void TestTeacherSubjectsList::testLoad()
{
    this->teacherSubjectsList.clear();
    QVERIFY(this->teacherSubjectsList.load());

    QCOMPARE(this->teacherSubjectsList.subjects().at(0), this->subject1.key());
    QCOMPARE(this->teacherSubjectsList.subjects().at(1), this->subject2.key());
    QCOMPARE(this->teacherSubjectsList.subjects().size(), 2);
}

void TestTeacherSubjectsList::testUpdate()
{
    this->teacherSubjectsList.removeSubject(this->teacherSubjectsList.subjects().begin());
    QVERIFY(this->teacherSubjectsList.update());

    this->teacherSubjectsList.clear();
    this->teacherSubjectsList.load();

    QCOMPARE(this->teacherSubjectsList.subjects().size(), 1);
    QCOMPARE(this->teacherSubjectsList.subjects().at(0), this->subject2.key());
}

void TestTeacherSubjectsList::testLoadAll()
{
    dbapi::ApiError error;
    auto teacherSubjectsLists = dbapi::TeacherSubjectsList::loadAll(&this->connection, &error);

    QVERIFY(error.type == dbapi::ApiError::NoError);

    for(auto p_teacherSubjectsList : teacherSubjectsLists)
    {
        if(p_teacherSubjectsList->key() == this->teacherSubjectsList.key())
        {
            QCOMPARE(p_teacherSubjectsList->subjects().size(), 1);
            QCOMPARE(p_teacherSubjectsList->subjects().at(0), this->subject2.key());
            return;
        }

        delete p_teacherSubjectsList;
    }

    QVERIFY(false);
}

void TestTeacherSubjectsList::testMakeAll()
{
    dbapi::ApiError error;
    auto teacherSubjectsLists = dbapi::TeacherSubjectsList::makeAll(&this->connection, &error);

    QVERIFY(error.type == dbapi::ApiError::NoError);

    for(auto p_teacherSubjectsList : teacherSubjectsLists)
    {
        if(p_teacherSubjectsList->key() == this->teacherSubjectsList.key())
            return;

        delete p_teacherSubjectsList;
    }

    QVERIFY(false);
}

void TestTeacherSubjectsList::testExists()
{
    QCOMPARE(this->teacherSubjectsList.existsByKey(), dbapi::BoolResult::TrueResult);
    QCOMPARE(this->teacherSubjectsList.existsByTuple(), dbapi::BoolResult::TrueResult);

    this->teacherSubjectsList.appendSubject({99999999});
    QCOMPARE(this->teacherSubjectsList.existsByKey(), dbapi::BoolResult::TrueResult);
    QCOMPARE(this->teacherSubjectsList.existsByTuple(), dbapi::BoolResult::FalseResult);

    auto key = this->teacherSubjectsList.key();
    this->teacherSubjectsList.setKey({322});

    QCOMPARE(this->teacherSubjectsList.existsByKey(), dbapi::BoolResult::FalseResult);
    QCOMPARE(this->teacherSubjectsList.existsByTuple(), dbapi::BoolResult::FalseResult);

    this->teacherSubjectsList.setKey(key);
    this->teacherSubjectsList.removeSubject(this->teacherSubjectsList.subjects().begin() + 1);
}

void TestTeacherSubjectsList::testRemove()
{
    QVERIFY(this->teacherSubjectsList.remove());
    QCOMPARE(this->teacherSubjectsList.existsByKey(), dbapi::FalseResult);
}

void TestTeacherSubjectsList::cleanupTestCase()
{
    this->person.remove();
    this->role.remove();
    this->subject1.remove();
    this->subject2.remove();
}
