#ifndef TESTTEACHERSUBJECTSLIST_H
#define TESTTEACHERSUBJECTSLIST_H


#include <QTest>
#include <SchoolApi/TeacherSubjectsList.h>
#include "Config.h"


class TestTeacherSubjectsList : public QObject
{
    Q_OBJECT

private:
    dbapi::Connection connection{Config::SERVER_NAME, Config::USERNAME, Config::PASSWORD, Config::DATABASE_NAME};

    dbapi::Role role;
    dbapi::Person person;

    dbapi::Subject subject1;
    dbapi::Subject subject2;

    dbapi::TeacherSubjectsList teacherSubjectsList;

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


#endif // TESTTEACHERSUBJECTSLIST_H
