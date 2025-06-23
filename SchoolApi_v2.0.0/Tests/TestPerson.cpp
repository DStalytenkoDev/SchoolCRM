#include "TestPerson.h"

#define TEST_NAME "Jone"
#define TEST_NAME2 "Jone 2"
#define TEST_SECOND_NAME "Smith"
#define TEST_SECOND_NAME2 "Smith 2"
#define TEST_BIRTHDAY 100799
#define TEST_BIRTHDAY2 110799

#define TEST_ROLE_NAME "TEST Role"
#define TEST_ROLE_NAME2 "TEST Role 2"

#define FAKE_ROLE 999999

void TestPerson::initTestCase()
{
    this->connection.open();

    this->role1.setConnection(&this->connection);
    this->role1.setName(TEST_ROLE_NAME);
    this->role1.store();

    this->role2.setConnection(&this->connection);
    this->role2.setName(TEST_ROLE_NAME2);
    this->role2.store();
}

void TestPerson::testValid()
{
    QVERIFY(not this->person.valid());

    this->connection.close();
    this->person.setConnection(&this->connection);
    QVERIFY(not this->person.valid());

    this->connection.open();

    QVERIFY(this->person.valid());
}

void TestPerson::testStore()
{
    this->person.setFirstName(TEST_NAME);
    this->person.setSecondName(TEST_SECOND_NAME);
    this->person.setBirthday(TEST_BIRTHDAY);
    this->person.setRole({FAKE_ROLE});

    QVERIFY(not this->person.store());

    this->person.setRole(role1.key());

    QVERIFY(this->person.store());
}

void TestPerson::testLoad()
{
    this->person.setFirstName("");
    this->person.setSecondName("");
    this->person.setBirthday(0);
    this->person.setRole({FAKE_ROLE});

    QVERIFY(this->person.load());

    QVERIFY(this->person.role() == this->role1.key());
    QCOMPARE(this->person.firstName(), TEST_NAME);
    QCOMPARE(this->person.secondName(), TEST_SECOND_NAME);
    QCOMPARE(this->person.birthday(), TEST_BIRTHDAY);
}

void TestPerson::testUpdate()
{
    this->person.setFirstName(TEST_NAME2);
    this->person.setSecondName(TEST_SECOND_NAME2);
    this->person.setBirthday(TEST_BIRTHDAY2);
    this->person.setRole(this->role2.key());

    this->person.update();
    qDebug() << this->person.error().text;
    QVERIFY(this->person.error().type == dbapi::ApiError::NoError);

    this->person.setFirstName("");
    this->person.setSecondName("");
    this->person.setBirthday(0);
    this->person.setRole({FAKE_ROLE});

    this->person.load();

    QVERIFY(this->person.role() == this->role2.key());
    QCOMPARE(this->person.firstName(), TEST_NAME2);
    QCOMPARE(this->person.secondName(), TEST_SECOND_NAME2);
    QCOMPARE(this->person.birthday(), TEST_BIRTHDAY2);
}

void TestPerson::testLoadAll()
{
    dbapi::ApiError error;
    auto persons = dbapi::Person::loadAll(&this->connection, &error);

    QVERIFY(error.type == dbapi::ApiError::NoError);

    for(auto p_person : persons)
    {
        if(p_person->key() == this->person.key())
        {
            QVERIFY(p_person->role() == this->role2.key());
            QCOMPARE(p_person->firstName(), TEST_NAME2);
            QCOMPARE(p_person->secondName(), TEST_SECOND_NAME2);
            QCOMPARE(p_person->birthday(), TEST_BIRTHDAY2);

            return;
        }

        delete p_person;
    }

    QVERIFY(false);
}

void TestPerson::testMakeAll()
{
    dbapi::ApiError error;
    auto persons = dbapi::Person::loadAll(&this->connection, &error);

    QVERIFY(error.type == dbapi::ApiError::NoError);

    for(auto p_person : persons)
    {
        if(p_person->key() == this->person.key())
        {
            QVERIFY(true);
            return;
        }

        delete p_person;
    }

    QVERIFY(false);
}

void TestPerson::testLoadRole()
{
    this->person.setRole({FAKE_ROLE});
    QVERIFY(this->person.loadRole());
    QVERIFY(this->person.role() == this->role2.key());
}

void TestPerson::testUpdateRole()
{
    QVERIFY(this->person.updateRole(this->role1.key()));

    this->person.setRole({FAKE_ROLE});
    this->person.loadRole();

    QVERIFY(this->person.role() == this->role1.key());
}

void TestPerson::testLoadFirstName()
{
    this->person.setFirstName("");
    QVERIFY(this->person.loadFirstName());
    QCOMPARE(this->person.firstName(), TEST_NAME2);
}

void TestPerson::testUpdateFirstName()
{
    QVERIFY(this->person.updateFirstName(TEST_NAME));

    this->person.setFirstName("");
    this->person.loadFirstName();

    QCOMPARE(this->person.firstName(), TEST_NAME);
}

void TestPerson::testLoadSecondName()
{
    this->person.setSecondName("");
    QVERIFY(this->person.loadSecondName());
    QCOMPARE(this->person.secondName(), TEST_SECOND_NAME2);
}

void TestPerson::testUpdateSecondName()
{
    QVERIFY(this->person.updateSecondName(TEST_SECOND_NAME));

    this->person.setSecondName("");
    this->person.loadSecondName();

    QCOMPARE(this->person.secondName(), TEST_SECOND_NAME);
}

void TestPerson::testLoadBirthday()
{
    this->person.setBirthday(0);
    QVERIFY(this->person.loadBirthday());
    QCOMPARE(this->person.birthday(), TEST_BIRTHDAY2);
}

void TestPerson::testUpdateBirthday()
{
    QVERIFY(this->person.updateBirthday(TEST_BIRTHDAY));

    this->person.setBirthday(0);
    this->person.loadBirthday();

    QCOMPARE(this->person.birthday(), TEST_BIRTHDAY);
}

void TestPerson::testExists()
{
    QCOMPARE(this->person.existsByKey(), dbapi::BoolResult::TrueResult);
    QCOMPARE(this->person.existsByTuple(), dbapi::BoolResult::TrueResult);

    this->person.setSecondName("OTHER");

    QCOMPARE(this->person.existsByKey(), dbapi::BoolResult::TrueResult);
    QCOMPARE(this->person.existsByTuple(), dbapi::BoolResult::FalseResult);

    dbapi::Person invalidPerson({999999}, &this->connection);

    QCOMPARE(invalidPerson.existsByKey(), dbapi::BoolResult::FalseResult);
    QCOMPARE(invalidPerson.existsByTuple(), dbapi::BoolResult::FalseResult);
}

void TestPerson::testRemove()
{
    QVERIFY(this->person.remove());
    QVERIFY(this->person.existsByKey() == dbapi::BoolResult::FalseResult);
}

void TestPerson::cleanupTestCase()
{
    this->role1.remove();
    this->role2.remove();
    this->connection.close();
}
