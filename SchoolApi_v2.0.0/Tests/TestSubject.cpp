#include "TestSubject.h"

void TestSubject::testValid()
{
    this->subject.setConnection(nullptr);
    QVERIFY(not this->subject.valid());

    this->subject.setConnection(&this->connection);
    QVERIFY(not this->subject.valid());

    this->connection.open();
    QVERIFY(this->subject.valid());
}

void TestSubject::testStore()
{
    this->subject.setName("NAME");
    QVERIFY(this->subject.store());
}

void TestSubject::testLoad()
{
    this->subject.setName("");
    QVERIFY(this->subject.load());
    QCOMPARE(this->subject.name(), "NAME");
}

void TestSubject::testLoadName()
{
    this->subject.setName("");
    QVERIFY(this->subject.loadName());
    QCOMPARE(this->subject.name(), "NAME");
}

void TestSubject::testUpdateName()
{
    QVERIFY(this->subject.updateName("UPDATED NAME"));

    this->subject.setName("");
    this->subject.loadName();

    QCOMPARE(this->subject.name(), "UPDATED NAME");
}

void TestSubject::testUpdate()
{
    this->subject.setName("UPDATED 2 NAME");
    QVERIFY(this->subject.update());

    this->subject.setName("");
    this->subject.load();

    QCOMPARE(this->subject.name(), "UPDATED 2 NAME");
}

void TestSubject::testLoadAll()
{
    dbapi::ApiError error;
    auto subjects = dbapi::Subject::loadAll(&this->connection, &error);

    QVERIFY(error.type == dbapi::ApiError::NoError);

    for(auto p_subject : subjects)
    {
        if(p_subject->key() == this->subject.key())
        {
            QCOMPARE(p_subject->name(), "UPDATED 2 NAME");
            return;
        }

        delete p_subject;
    }

    QVERIFY(false);
}

void TestSubject::testMakeAll()
{
    dbapi::ApiError error;
    auto subjects = dbapi::Subject::makeAll(&this->connection);

    QVERIFY(error.type == dbapi::ApiError::NoError);

    for(auto p_subject : subjects)
    {
        if(p_subject->key() == this->subject.key())
        {
            QVERIFY(true);
            return;
        }

        delete p_subject;
    }

    QVERIFY(false);
}


void TestSubject::testExists()
{
    QCOMPARE(this->subject.existsByKey(), dbapi::BoolResult::TrueResult);
    QCOMPARE(this->subject.existsByTuple(), dbapi::BoolResult::TrueResult);

    this->subject.setName("OTHER");
    QCOMPARE(this->subject.existsByKey(), dbapi::BoolResult::TrueResult);
    QCOMPARE(this->subject.existsByTuple(), dbapi::BoolResult::FalseResult);

    auto key = this->subject.key();
    this->subject.setKey({999999});

    QCOMPARE(this->subject.existsByKey(), dbapi::BoolResult::FalseResult);
    QCOMPARE(this->subject.existsByTuple(), dbapi::BoolResult::FalseResult);

    this->subject.setKey(key);
}


void TestSubject::testRemove()
{
    QVERIFY(this->subject.remove());
    QCOMPARE(this->subject.existsByKey(), dbapi::FalseResult);
}

void TestSubject::cleanupTestCase()
{
    this->connection.close();
}
