#include "TestClassSubjectsList.h"


void TestClassSubjectsList::initTestCase()
{
    this->connection.open();

    this->grade.setConnection(&this->connection);
    this->grade.setName("GRADE");
    if(not this->grade.store())
        QSKIP("Test require some class");

    this->subject1.setConnection(&this->connection);
    this->subject1.setName("SUBJECT1");

    this->subject2.setConnection(&this->connection);
    this->subject2.setName("SUBJECT2");

    if(not this->subject1.store() or not this->subject2.store())
        QSKIP("Test require two subjects");
}

void TestClassSubjectsList::testValid()
{
    this->classSubjectsList.setConnection(nullptr);
    QVERIFY(not this->classSubjectsList.valid());

    this->connection.close();
    this->classSubjectsList.setConnection(&this->connection);
    QVERIFY(not this->classSubjectsList.valid());

    this->connection.open();
    QVERIFY(this->classSubjectsList.valid());
}

void TestClassSubjectsList::testStore()
{
    this->classSubjectsList.setKey({this->grade.key()});
    this->classSubjectsList.appendSubject(this->subject1.key());
    this->classSubjectsList.appendSubject(this->subject2.key());

    QVERIFY(this->classSubjectsList.store());
}

void TestClassSubjectsList::testLoad()
{
    this->classSubjectsList.clear();
    QVERIFY(this->classSubjectsList.load());

    QCOMPARE(this->classSubjectsList.subjects().at(0), this->subject1.key());
    QCOMPARE(this->classSubjectsList.subjects().at(1), this->subject2.key());
    QCOMPARE(this->classSubjectsList.subjects().size(), 2);
}

void TestClassSubjectsList::testUpdate()
{
    this->classSubjectsList.removeSubject(this->classSubjectsList.subjects().begin());
    QVERIFY(this->classSubjectsList.update());

    this->classSubjectsList.clear();
    this->classSubjectsList.load();

    QCOMPARE(this->classSubjectsList.subjects().size(), 1);
    QCOMPARE(this->classSubjectsList.subjects().at(0), this->subject2.key());
}

void TestClassSubjectsList::testLoadAll()
{
    dbapi::ApiError error;
    auto classSubjectsLists = dbapi::ClassSubjectsList::loadAll(&this->connection, &error);

    QVERIFY(error.type == dbapi::ApiError::NoError);

    for(auto p_classSubjectsList : classSubjectsLists)
    {
        if(p_classSubjectsList->key() == this->classSubjectsList.key())
        {
            QCOMPARE(p_classSubjectsList->subjects().size(), 1);
            QCOMPARE(p_classSubjectsList->subjects().at(0), this->subject2.key());
            return;
        }

        delete p_classSubjectsList;
    }

    QVERIFY(false);
}

void TestClassSubjectsList::testMakeAll()
{
    dbapi::ApiError error;
    auto classSubjectsLists = dbapi::ClassSubjectsList::makeAll(&this->connection, &error);

    QVERIFY(error.type == dbapi::ApiError::NoError);

    for(auto p_classSubjectsList : classSubjectsLists)
    {
        if(p_classSubjectsList->key() == this->classSubjectsList.key())
            return;

        delete p_classSubjectsList;
    }

    QVERIFY(false);
}

void TestClassSubjectsList::testExists()
{
    QCOMPARE(this->classSubjectsList.existsByKey(), dbapi::BoolResult::TrueResult);
    QCOMPARE(this->classSubjectsList.existsByTuple(), dbapi::BoolResult::TrueResult);

    this->classSubjectsList.appendSubject({99999999});
    QCOMPARE(this->classSubjectsList.existsByKey(), dbapi::BoolResult::TrueResult);
    QCOMPARE(this->classSubjectsList.existsByTuple(), dbapi::BoolResult::FalseResult);

    auto key = this->classSubjectsList.key();
    this->classSubjectsList.setKey({322});

    QCOMPARE(this->classSubjectsList.existsByKey(), dbapi::BoolResult::FalseResult);
    QCOMPARE(this->classSubjectsList.existsByTuple(), dbapi::BoolResult::FalseResult);

    this->classSubjectsList.setKey(key);
    this->classSubjectsList.removeSubject(this->classSubjectsList.subjects().begin() + 1);
}

void TestClassSubjectsList::testRemove()
{
    QVERIFY(this->classSubjectsList.remove());
    QCOMPARE(this->classSubjectsList.existsByKey(), dbapi::FalseResult);
}

void TestClassSubjectsList::cleanupTestCase()
{
    this->subject1.remove();
    this->subject2.remove();
    this->grade.remove();
}
