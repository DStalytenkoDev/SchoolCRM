#ifndef TESTCLASSSUBJECTSLIST_H
#define TESTCLASSSUBJECTSLIST_H


#include <QTest>
#include <SchoolApi/ClassSubjectsList.h>
#include "Config.h"


class TestClassSubjectsList : public QObject
{
    Q_OBJECT

private:
    dbapi::Connection connection{Config::SERVER_NAME, Config::USERNAME, Config::PASSWORD, Config::DATABASE_NAME};
    dbapi::Class grade;

    dbapi::Subject subject1;
    dbapi::Subject subject2;

    dbapi::ClassSubjectsList classSubjectsList;

private slots:
    void initTestCase();

    void testValid();
    void testStore();
    void testLoad();
    void testUpdate();
    void testLoadAll();
    void testMakeAll();
    void testExists();
    void testRemove();

    void cleanupTestCase();
};


#endif // TESTCLASSSUBJECTSLIST_H
