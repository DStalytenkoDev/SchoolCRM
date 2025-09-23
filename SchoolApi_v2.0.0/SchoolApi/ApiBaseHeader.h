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
