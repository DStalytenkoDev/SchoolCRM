#ifndef TESTCLASSMATE_H
#define TESTCLASSMATE_H


#include <QTest>
#include <SchoolApi/Classmate.h>
#include "Config.h"


class TestClassmate : public QObject
{
    Q_OBJECT

private:
    dbapi::Connection connection{Config::SERVER_NAME, Config::USERNAME, Config::PASSWORD, Config::DATABASE_NAME};
    dbapi::Classmate classmate;

    dbapi::Person person;
    dbapi::Role role;

    dbapi::Class grade1;
    dbapi::Class grade2;

private slots:
    void initTestCase();

    void testValid();
    void testStore();
    void testLoad();
    void testUpdate();

    void testLoadGrade();
    void testUpdateGrade();

    void testLoadAll();
    void testMakeAll();

    void testExists();
    void testRemove();

    void cleanupTestCase();
};


#endif // TESTCLASSMATE_H
