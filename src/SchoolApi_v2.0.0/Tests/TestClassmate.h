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
