cmake_minimum_required(VERSION 3.14)

project(SchoolDatabaseApi LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Core Sql Test)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Core Sql Test)

add_library(SchoolDatabaseApi STATIC
    SchoolApi/Connection.h
    SchoolApi/Connection.cpp
    
    SchoolApi/ApiBaseHeader.h
    
    SchoolApi/Role.h
    SchoolApi/Role.cpp
    
    SchoolApi/ApiError.h
    SchoolApi/ApiError.cpp
    
    SchoolApi/Person.h
    SchoolApi/Person.cpp

    SchoolApi/Class.h
    SchoolApi/Class.cpp

    SchoolApi/Classmate.h
    SchoolApi/Classmate.cpp

    SchoolApi/Student.h
    SchoolApi/Student.cpp

    SchoolApi/Subject.h
    SchoolApi/Subject.cpp

    SchoolApi/ClassSubjectsList.h
    SchoolApi/ClassSubjectsList.cpp

    SchoolApi/StudentMark.h
    SchoolApi/StudentMark.cpp

    SchoolApi/TeacherSubjectsList.h
    SchoolApi/TeacherSubjectsList.cpp
)

add_executable(Tests
   Tests/main.cpp
   Tests/TestConnection.h
   Tests/TestConnection.cpp
   Tests/TestRole.cpp
   Tests/TestRole.h
   Tests/Config.h
   Tests/Config.cpp
   Tests/TestPerson.h
   Tests/TestPerson.cpp
   Tests/TestClass.h
   Tests/TestClass.cpp
   Tests/TestClassmate.h
   Tests/TestClassmate.cpp
   Tests/TestStudent.h
   Tests/TestStudent.cpp
   Tests/TestSubject.h
   Tests/TestSubject.cpp
   Tests/TestStudentMark.h
   Tests/TestStudentMark.cpp
   Tests/TestClassSubjectsList.h
   Tests/TestClassSubjectsList.cpp
   Tests/TestTeacherSubjectsList.h
   Tests/TestTeacherSubjectsList.cpp)

target_link_libraries(SchoolDatabaseApi PUBLIC Qt${QT_VERSION_MAJOR}::Core Qt${QT_VERSION_MAJOR}::Sql)
target_link_libraries(Tests PRIVATE Qt${QT_VERSION_MAJOR}::Test SchoolDatabaseApi)

target_compile_definitions(SchoolDatabaseApi PRIVATE SCHOOLDATABASEAPI_LIBRARY)

target_include_directories(SchoolDatabaseApi PUBLIC ./)
