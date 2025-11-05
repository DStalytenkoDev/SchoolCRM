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


#include "TestRole.h"

void TestRole::testValid()
{
    this->role.setConnection(nullptr);
    QVERIFY(not this->role.valid());

    this->role.setConnection(&this->connection);
    QVERIFY(not this->role.valid());

    this->connection.open();
    this->role.setConnection(&this->connection);
    QVERIFY(this->role.valid());
}

void TestRole::testStore()
{
    this->role.setName("NAME");
    QVERIFY(this->role.store());
}

void TestRole::testLoad()
{
    this->role.setName("");
    QVERIFY(this->role.load());
    QCOMPARE(this->role.name(), "NAME");
}

void TestRole::testLoadName()
{
    this->role.setName("");
    QVERIFY(this->role.load());
    QCOMPARE(this->role.name(), "NAME");
}

void TestRole::testUpdateName()
{
    QVERIFY(this->role.updateName("UPDATED NAME"));
    this->role.setName("");

    QVERIFY(this->role.loadName());
    QCOMPARE(this->role.name(), "UPDATED NAME");
}

void TestRole::testUpdate()
{
    this->role.setName("UPDATED 2 NAME");
    QVERIFY(this->role.update());

    this->role.setName("");
    QVERIFY(this->role.load());
    QCOMPARE(this->role.name(), "UPDATED 2 NAME");
}

void TestRole::testLoadAll()
{
    dbapi::ApiError error;
    auto roles = dbapi::Role::loadAll(&this->connection, &error);

    QVERIFY(error.type == dbapi::ApiError::NoError);

    for(auto p_role : roles)
    {
        if(p_role->key() == this->role.key())
        {
            QCOMPARE(p_role->name(), "UPDATED 2 NAME");
            return;
        }

        delete p_role;
    }

    QVERIFY(false);
}

void TestRole::testMakeAll()
{
    auto roles = dbapi::Role::makeAll(&this->connection);

    for(auto p_role : roles)
    {
        if(p_role->key() == this->role.key())
        {
            QVERIFY(true);
            return;
        }

        delete p_role;
    }

    QVERIFY(false);
}


void TestRole::testExists()
{
    QVERIFY(this->role.existsByKey() == dbapi::BoolResult::TrueResult);
    QVERIFY(this->role.existsByTuple() == dbapi::BoolResult::TrueResult);

    this->role.setName("OTHER");
    QVERIFY(this->role.existsByTuple() == dbapi::BoolResult::FalseResult);

    auto key = this->role.key();
    this->role.setKey({322});

    QVERIFY(this->role.existsByKey() == dbapi::BoolResult::FalseResult);

    this->role.setKey(key);
}


void TestRole::testRemove()
{
    QVERIFY(this->role.remove());
    QVERIFY(not this->role.load());
}

void TestRole::cleanupTestCase()
{
    this->connection.close();
}
