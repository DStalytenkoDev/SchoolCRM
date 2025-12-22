# Copyright (C) 2025 Davyd Stalytenko
# SPDX-License-Identifier: GPL-3.0-only
#
# This file is part of SchooliesCave.
# 
# SchooliesCave is free software: you can redistribute it and/or modify it under the terms
# of the GNU General Public License as published by the Free Software Foundation, version 3
# of the License only.
# 
# SchooliesCave is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY;
# without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
# See the GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License along with SchooliesCave.
# If not, see <https://www.gnu.org/licenses/>.


cmake_minimum_required(VERSION 3.16)

project(SchooliesCave VERSION 0.1 LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(cmake/InitQt6.cmake)
include(cmake/InitLibreSSL.cmake)
include(cmake/Libb64.cmake)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets StateMachine)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets StateMachine)

add_subdirectory(./src/SchoolApi_v2.0.0)

set(PROJECT_SOURCES
        src/main.cpp
        src/MainWindow.cpp
        src/MainWindow.h
        src/MainWindow.ui
)

if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(SchooliesCave
        MANUAL_FINALIZATION
        ${PROJECT_SOURCES}
        src/Journal/MarkTypeSelector.h
        src/Journal/MarkTypeSelector.cpp

        src/ComboBoxFinderView.h
        src/ComboBoxFinderView.cpp



        src/Persons/PersonsModule.h src/Persons/PersonsModule.cpp src/Persons/PersonsModule.ui
        src/Persons/PersonWidget.h src/Persons/PersonWidget.cpp src/Persons/PersonWidget.ui
        src/Persons/PersonCreationDialog.h src/Persons/PersonCreationDialog.cpp
        src/Persons/PersonsModel.h src/Persons/PersonsModel.cpp
        src/Persons/PersonEdtitionWidget.h src/Persons/PersonEdtitionWidget.cpp

        src/Roles/RolesModel.h src/Roles/RolesModel.cpp
        src/Roles/RolesModule.h src/Roles/RolesModule.cpp src/Roles/RolesModule.ui
        src/Roles/RoleCreationDialog.h src/Roles/RoleCreationDialog.cpp src/Roles/RoleCreationDialog.ui

        src/Subjects/SubjectsModel.h src/Subjects/SubjectsModel.cpp
        src/Subjects/SubjectsModule.h src/Subjects/SubjectsModule.cpp src/Subjects/SubjectsModule.ui
        src/Subjects/CreateSubjectDialog.h src/Subjects/CreateSubjectDialog.cpp src/Subjects/CreateSubjectDialog.ui

        src/SubjectsOfTeacher/SubjectsOfTeacherModule.h src/SubjectsOfTeacher/SubjectsOfTeacherModule.cpp src/SubjectsOfTeacher/SubjectsOfTeacherModule.ui
        src/SubjectsOfTeacher/SubjectsOfTeacherModel.h src/SubjectsOfTeacher/SubjectsOfTeacherModel.cpp

        src/Classes/ClassSubjectsModel.h src/Classes/ClassSubjectsModel.cpp
        src/Classes/ClassStudentsModel.h src/Classes/ClassStudentsModel.cpp
        src/Classes/ClassesModel.h src/Classes/ClassesModel.cpp
        src/Classes/ClassesModule.h src/Classes/ClassesModule.cpp src/Classes/ClassesModule.ui
        src/Classes/ClassCreationDialog.h src/Classes/ClassCreationDialog.cpp src/Classes/ClassCreationDialog.ui

        src/Journal/JournalModel.h src/Journal/JournalModel.cpp
        src/Journal/JournalModule.h src/Journal/JournalModule.cpp src/Journal/JournalModule.ui
        src/Journal/MarkCreatingDialog.h src/Journal/MarkCreatingDialog.cpp src/Journal/MarkCreatingDialog.ui
        src/Journal/FakePersonsModel.h src/Journal/FakePersonsModel.cpp

        src/TransitionFactrory.h

        readme.md

        src/UserError.h
        src/UserError.cpp
        src/AuthorizationWindow.h
	src/AuthorizationWindow.cpp
	src/AuthorizationWindow.ui
        src/Menu/AboutDialog.h
	src/Menu/AboutDialog.cpp
	src/Menu/AboutDialog.ui

        assets/MEDIA.md
    )
# Define target properties for Android with Qt 6 as:
#    set_property(TARGET SchooliesCave APPEND PROPERTY QT_ANDROID_PACKAGE_SOURCE_DIR
#                 ${CMAKE_CURRENT_SOURCE_DIR}/android)
# For more information, see https://doc.qt.io/qt-6/qt-add-executable.html#target-creation
else()
    if(ANDROID)
        add_library(SchooliesCave SHARED
            ${PROJECT_SOURCES}
        )
# Define properties for Android with Qt 5 after find_package() calls as:
#    set(ANDROID_PACKAGE_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/android")
    else()
        add_executable(SchooliesCave
            ${PROJECT_SOURCES}
        )
    endif()
endif()

target_link_libraries(SchooliesCave PRIVATE Qt${QT_VERSION_MAJOR}::Widgets Qt${QT_VERSION_MAJOR}::StateMachine SchoolDatabaseApi)

target_include_directories(SchooliesCave PRIVATE ./)

# Qt for iOS sets MACOSX_BUNDLE_GUI_IDENTIFIER automatically since Qt 6.1.
# If you are developing for iOS or macOS you should consider setting an
# explicit, fixed bundle identifier manually though.
if(${QT_VERSION} VERSION_LESS 6.1.0)
  set(BUNDLE_ID_OPTION MACOSX_BUNDLE_GUI_IDENTIFIER com.example.SchooliesCave)
endif()
set_target_properties(SchooliesCave PROPERTIES
    ${BUNDLE_ID_OPTION}
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

include(GNUInstallDirs)
install(TARGETS SchooliesCave
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(SchooliesCave)
endif()
