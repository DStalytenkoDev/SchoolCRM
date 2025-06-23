#ifndef TESTROLE_H
#define TESTROLE_H


#include <QTest>
#include <SchoolApi/Connection.h>
#include <SchoolApi/Role.h>
#include "Config.h"


class TestRole : public QObject
{
    Q_OBJECT

private:
    dbapi::Connection connection{Config::SERVER_NAME, Config::USERNAME, Config::PASSWORD, Config::DATABASE_NAME};
    dbapi::Role role;

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


#endif // TESTROLE_H
