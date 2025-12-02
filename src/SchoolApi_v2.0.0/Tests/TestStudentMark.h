// Copyright (C) 2025 Davyd Stalytenko
// SPDX-License-Identifier: GPL-3.0-only
//
// This file is part of SchooliesCave.
// 
// SchooliesCave is free software: you can redistribute it and/or modify it under the terms
// of the GNU General Public License as published by the Free Software Foundation, version 3
// of the License only.
// 
// SchooliesCave is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY;
// without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
// See the GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License along with SchooliesCave.
// If not, see <https://www.gnu.org/licenses/>.


#ifndef TESTSTUDENTMARK_H
#define TESTSTUDENTMARK_H


#include <QTest>
#include <SchoolApi/StudentMark.h>
#include <SchoolApi/Person.h>
#include <SchoolApi/Subject.h>

#include "Config.h"


class TestStudentMark : public QObject
{
    Q_OBJECT

private:
    dbapi::Connection connection{Config::SERVER_NAME, Config::USERNAME, Config::PASSWORD, Config::DATABASE_NAME};
    dbapi::Role roleForTeacher;
    dbapi::Role roleForStudent;

    dbapi::Person teacher;
    dbapi::Person student;
    dbapi::Subject subject;

    dbapi::Person teacher1;
    dbapi::Person student1;
    dbapi::Subject subject1;

    dbapi::StudentMark mark;

private slots:
    void initTestCase();

    void testValid();
    void testStore();
    void testLoad();
    void testUpdate();

    void testLoadMark();
    void testUpdateMark();

    void testLoadTeacher();
    void testUpdateTeacher();

    void testLoadStudent();
    void testUpdateStudent();

    void testLoadDate();
    void testUpdateDate();

    void testLoadType();
    void testUpdateType();

    void testLoadSubject();
    void testUpdateSubject();

    void testLoadAll();
    void testMakeAll();

    void testExists();
    void testRemove();

    void cleanupTestCase();
};



#endif // TESTSTUDENTMARK_H
