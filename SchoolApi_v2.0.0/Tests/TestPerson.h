#ifndef TESTPERSON_H
#define TESTPERSON_H

#include <QTest>
#include <SchoolApi/Person.h>
#include "Config.h"

class TestPerson : public QObject
{
    Q_OBJECT

private:
    dbapi::Connection connection{Config::SERVER_NAME, Config::USERNAME, Config::PASSWORD, Config::DATABASE_NAME};
    dbapi::Person person;

    dbapi::Role role1;
    dbapi::Role role2;

private slots:
    void initTestCase();

    void testValid();

    void testStore();
    void testLoad();
    void testUpdate();

    void testLoadAll();
    void testMakeAll();

    void testLoadRole();
    void testLoadFirstName();
    void testLoadSecondName();
    void testLoadBirthday();

    void testUpdateRole();
    void testUpdateFirstName();
    void testUpdateSecondName();
    void testUpdateBirthday();

    void testExists();

    void testRemove();

    void cleanupTestCase();
};


#endif // TESTPERSON_H
