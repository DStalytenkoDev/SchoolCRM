// Copyright (C) 2025 AUTHORS of SchooliesCave
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
