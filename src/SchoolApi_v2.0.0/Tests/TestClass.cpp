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


#include "TestClass.h"

void TestClass::testValid()
{
    this->grade.setConnection(nullptr);
    QVERIFY(not this->grade.valid());

    this->grade.setConnection(&this->connection);
    QVERIFY(not this->grade.valid());

    this->connection.open();
    QVERIFY(this->grade.valid());
}

void TestClass::testStore()
{
    this->grade.setName("NAME");
    QVERIFY(this->grade.store());
}

void TestClass::testLoad()
{
    this->grade.setName("");
    QVERIFY(this->grade.load());
    QCOMPARE(this->grade.name(), "NAME");
}

void TestClass::testLoadName()
{
    this->grade.setName("");
    QVERIFY(this->grade.load());
    QCOMPARE(this->grade.name(), "NAME");
}

void TestClass::testUpdateName()
{
    QVERIFY(this->grade.updateName("UPDATED NAME"));
    this->grade.setName("");

    QVERIFY(this->grade.loadName());
    QCOMPARE(this->grade.name(), "UPDATED NAME");
}

void TestClass::testUpdate()
{
    this->grade.setName("UPDATED 2 NAME");
    QVERIFY(this->grade.update());

    this->grade.setName("");
    QVERIFY(this->grade.load());
    QCOMPARE(this->grade.name(), "UPDATED 2 NAME");
}

void TestClass::testLoadAll()
{
    dbapi::ApiError error;
    auto grades = dbapi::Class::loadAll(&this->connection, &error);

    QVERIFY(error.type == dbapi::ApiError::NoError);

    for(auto p_grade : grades)
    {
        if(p_grade->key() == this->grade.key())
        {
            QCOMPARE(p_grade->name(), "UPDATED 2 NAME");
            return;
        }

        delete p_grade;
    }

    QVERIFY(false);
}

void TestClass::testMakeAll()
{
    auto grades = dbapi::Class::makeAll(&this->connection);

    for(auto p_grade : grades)
    {
        if(p_grade->key() == this->grade.key())
        {
            QVERIFY(true);
            return;
        }

        delete p_grade;
    }

    QVERIFY(false);
}


void TestClass::testExists()
{
    QCOMPARE(this->grade.existsByKey(), dbapi::BoolResult::TrueResult);
    QCOMPARE(this->grade.existsByTuple(), dbapi::BoolResult::TrueResult);

    this->grade.setName("OTHER");
    QCOMPARE(this->grade.existsByKey(), dbapi::BoolResult::TrueResult);
    QCOMPARE(this->grade.existsByTuple(), dbapi::BoolResult::FalseResult);

    auto key = this->grade.key();
    this->grade.setKey({322});

    QCOMPARE(this->grade.existsByKey(), dbapi::BoolResult::FalseResult);
    QCOMPARE(this->grade.existsByTuple(), dbapi::BoolResult::FalseResult);

    this->grade.setKey(key);
}


void TestClass::testRemove()
{
    QVERIFY(this->grade.remove());
    QCOMPARE(this->grade.existsByKey(), dbapi::FalseResult);
}

void TestClass::cleanupTestCase()
{
    this->connection.close();
}
