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


#ifndef APIBASEHEADER_H
#define APIBASEHEADER_H

#include <QString>

/// namespace of the API
namespace dbapi {

/*
 * Connection class provides all the work 'bout the database connection
 * Should be used with no copy policy, but refs or pointers
 */
class Connection;

/*
 * Provides creation, edition or delition of a persons's role
 */
class Role;

/*
 * Provides management of Persons
 *
 * Gives firstName, secondName, role and birthday
 */
class Person;

/*
 * Provides management of Class
 *
 * Just creates a class id
 */
class Class;

/*
 * Provides management of Homeroom teacher
 * (was made naming mistake, actually it meant HomeroomTeacher
 *
 * Relates person to a class
 */
class Classmate;

/*
 * Provides management of Student
 * Relates person to class
 */
class Student;

/*
 * Provides subject managment
 */
class Subject;

/*
 * Provides a list of subjects related to a certain class
 */
class ClassSubjectsList;

/*
 * Provides a mark entity
 */
class StudentMark;

/*
 * Provides a list of subjects related to a certain teacher
 */
class TeacherSubjectsList;

/*
 * This class provides info about the error
 * An instance appears ussualy by ::error() in any API's entity
 * by deafult the instance is no error
 */
class ApiError;

enum BoolResult
{
    TrueResult,
    FalseResult,
    ErrorResult
};

}


#endif // APIBASEHEADER_H
