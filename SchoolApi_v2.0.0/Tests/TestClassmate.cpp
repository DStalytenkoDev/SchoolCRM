#include "TestClassmate.h"

void TestClassmate::initTestCase()
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

void TestClassmate::testValid()
{
    this->classmate.setConnection(nullptr);
    QVERIFY(not this->classmate.valid());

    this->connection.close();
    this->classmate.setConnection(&this->connection);
    QVERIFY(not this->classmate.valid());

    this->connection.open();
    QVERIFY(this->classmate.valid());
}

void TestClassmate::testStore()
{
    this->classmate.setKey({this->person.key()});
    this->classmate.setGrade(this->grade1.key());
    QVERIFY(this->classmate.store());
}

void TestClassmate::testLoad()
{
    this->classmate.setGrade({999999});
    QVERIFY(this->classmate.load());
    QCOMPARE(this->classmate.grade(), this->grade1.key());
}

void TestClassmate::testUpdate()
{
    this->classmate.setGrade(this->grade2.key());
    qDebug() << "grade 2 key" << this->grade2.key().id;
    QVERIFY(this->classmate.update());

    this->classmate.setGrade({99999});
    this->classmate.load();

    QCOMPARE(this->classmate.grade(), this->grade2.key());
}

void TestClassmate::testLoadGrade()
{
    this->classmate.setGrade({999999});
    QVERIFY(this->classmate.loadGrade());
    QCOMPARE(this->classmate.grade(), this->grade2.key());
}

void TestClassmate::testUpdateGrade()
{
    QVERIFY(this->classmate.updateGrade(this->grade1.key()));

    this->classmate.setGrade({999999});
    this->classmate.loadGrade();

    QCOMPARE(this->classmate.grade(), this->grade1.key());
}

void TestClassmate::testLoadAll()
{
    dbapi::ApiError error;
    auto classmates = dbapi::Classmate::loadAll(&this->connection, &error);

    QCOMPARE(error.type, dbapi::ApiError::NoError);

    for(auto p_classmate : classmates)
    {
        if(p_classmate->key() == this->classmate.key())
        {
            QCOMPARE(p_classmate->grade(), this->classmate.grade());
            return;
        }

        delete p_classmate;
    }

    QVERIFY(false);
}

void TestClassmate::testMakeAll()
{
    auto classmates = dbapi::Classmate::makeAll(&this->connection);

    for(auto p_classmate : classmates)
    {
        if(p_classmate->key() == this->classmate.key())
        {
            QVERIFY(true);
            return;
        }

        delete p_classmate;
    }

    QVERIFY(false);
}


void TestClassmate::testExists()
{
    QCOMPARE(this->classmate.existsByKey(), dbapi::BoolResult::TrueResult);
    QCOMPARE(this->classmate.existsByTuple(), dbapi::BoolResult::TrueResult);

    this->classmate.setGrade({999999});
    QCOMPARE(this->classmate.existsByKey(), dbapi::BoolResult::TrueResult);
    QCOMPARE(this->classmate.existsByTuple(), dbapi::BoolResult::FalseResult);

    auto key = this->classmate.key();
    this->classmate.setKey({99999});

    QCOMPARE(this->classmate.existsByKey(), dbapi::BoolResult::FalseResult);
    QCOMPARE(this->classmate.existsByTuple(), dbapi::BoolResult::FalseResult);

    this->classmate.setKey(key);
}


void TestClassmate::testRemove()
{
    QVERIFY(this->classmate.remove());
    QCOMPARE(this->classmate.existsByKey(), dbapi::FalseResult);
}

void TestClassmate::cleanupTestCase()
{
    this->classmate.remove();
    this->person.remove();
    this->grade1.remove();
    this->grade2.remove();
    this->role.remove();

    this->connection.close();
}
