#ifndef TESTCLASS_H
#define TESTCLASS_H

#include <QTest>
#include <SchoolApi/Class.h>
#include "Config.h"


class TestClass : public QObject
{
    Q_OBJECT

private:
    dbapi::Connection connection{Config::SERVER_NAME, Config::USERNAME, Config::PASSWORD, Config::DATABASE_NAME};
    dbapi::Class grade;

private slots:
    void testValid();
    void testStore();
    void testLoad();
    void testLoadName();
    void testUpdateName();
    void testUpdate();
    void testLoadAll();
    void testMakeAll();
    void testExists();
    void testRemove();

    void cleanupTestCase();
};


#endif // TESTCLASS_H
