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


#include "TestConnection.h"
#include "TestRole.h"
#include "TestPerson.h"
#include "TestClass.h"
#include "TestClassmate.h"
#include "TestStudent.h"
#include "TestSubject.h"
#include "TestStudentMark.h"
#include "TestClassSubjectsList.h"
#include "TestTeacherSubjectsList.h"

int main(int argc, char** args)
{
    QTest::qExec(new TestConnection, argc, args);
    QTest::qExec(new TestRole, argc, args);
    QTest::qExec(new TestPerson, argc, args);
    QTest::qExec(new TestClass, argc, args);
    QTest::qExec(new TestClassmate, argc, args);
    QTest::qExec(new TestStudent, argc, args);
    QTest::qExec(new TestSubject, argc, args);    
    QTest::qExec(new TestStudentMark, argc, args);
    QTest::qExec(new TestClassSubjectsList, argc, args);
    QTest::qExec(new TestTeacherSubjectsList, argc, args);

    return 0;
}
