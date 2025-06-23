#ifndef TESTSTUDENT_H
#define TESTSTUDENT_H


#include <QTest>
#include <SchoolApi/Student.h>
#include "Config.h"


class TestStudent : public QObject
{
    Q_OBJECT

private:
    dbapi::Connection connection{Config::SERVER_NAME, Config::USERNAME, Config::PASSWORD, Config::DATABASE_NAME};
    dbapi::Student student;

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



#endif // TESTSTUDENT_H
