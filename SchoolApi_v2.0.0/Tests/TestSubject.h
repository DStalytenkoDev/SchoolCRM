#ifndef TESTSUBJECT_H
#define TESTSUBJECT_H


#include <QTest>
#include <SchoolApi/Subject.h>
#include "Config.h"


class TestSubject : public QObject
{
    Q_OBJECT

private:
    dbapi::Connection connection{Config::SERVER_NAME, Config::USERNAME, Config::PASSWORD, Config::DATABASE_NAME};
    dbapi::Subject subject;

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



#endif // TESTSUBJECT_H
