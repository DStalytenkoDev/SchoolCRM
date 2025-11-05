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


#ifndef TESTCONNECTION_H
#define TESTCONNECTION_H


#include <QtTest>
#include <SchoolApi/Connection.h>


class TestConnection : public QObject
{
    Q_OBJECT

public:
    TestConnection();

private slots:
    void testData();
    void testTrustedConnection();
    void testSecureConnection();
    void testValid();
};


#endif // TESTCONNECTION_H
