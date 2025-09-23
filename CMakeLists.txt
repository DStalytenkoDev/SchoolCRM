cmake_minimum_required(VERSION 3.16)

project(SchoolCRM VERSION 0.1 LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets StateMachine)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets StateMachine)

add_subdirectory(./SchoolApi_v2.0.0)

set(PROJECT_SOURCES
        main.cpp
        MainWindow.cpp
        MainWindow.h
        MainWindow.ui
)

if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(SchoolCRM
        MANUAL_FINALIZATION
        ${PROJECT_SOURCES}
        Journal/MarkTypeSelector.h
        Journal/MarkTypeSelector.cpp

        ComboBoxFinderView.h
        ComboBoxFinderView.cpp



        Persons/PersonsModule.h Persons/PersonsModule.cpp Persons/PersonsModule.ui
        Persons/PersonWidget.h Persons/PersonWidget.cpp Persons/PersonWidget.ui
        Persons/PersonCreationDialog.h Persons/PersonCreationDialog.cpp
        Persons/PersonsModel.h Persons/PersonsModel.cpp
        Persons/PersonEdtitionWidget.h Persons/PersonEdtitionWidget.cpp

        Roles/RolesModel.h Roles/RolesModel.cpp
        Roles/RolesModule.h Roles/RolesModule.cpp Roles/RolesModule.ui
        Roles/RoleCreationDialog.h Roles/RoleCreationDialog.cpp Roles/RoleCreationDialog.ui

        Subjects/SubjectsModel.h Subjects/SubjectsModel.cpp
        Subjects/SubjectsModule.h Subjects/SubjectsModule.cpp Subjects/SubjectsModule.ui
        Subjects/CreateSubjectDialog.h Subjects/CreateSubjectDialog.cpp Subjects/CreateSubjectDialog.ui

        SubjectsOfTeacher/SubjectsOfTeacherModule.h SubjectsOfTeacher/SubjectsOfTeacherModule.cpp SubjectsOfTeacher/SubjectsOfTeacherModule.ui
        SubjectsOfTeacher/SubjectsOfTeacherModel.h SubjectsOfTeacher/SubjectsOfTeacherModel.cpp

        Classes/ClassSubjectsModel.h Classes/ClassSubjectsModel.cpp
        Classes/ClassStudentsModel.h Classes/ClassStudentsModel.cpp
        Classes/ClassesModel.h Classes/ClassesModel.cpp
        Classes/ClassesModule.h Classes/ClassesModule.cpp Classes/ClassesModule.ui
        Classes/ClassCreationDialog.h Classes/ClassCreationDialog.cpp Classes/ClassCreationDialog.ui

        Journal/JournalModel.h Journal/JournalModel.cpp
        Journal/JournalModule.h Journal/JournalModule.cpp Journal/JournalModule.ui
        Journal/MarkCreatingDialog.h Journal/MarkCreatingDialog.cpp Journal/MarkCreatingDialog.ui
        Journal/FakePersonsModel.h Journal/FakePersonsModel.cpp

        TransitionFactrory.h
        readme.md

        UserError.h
        UserError.cpp
        AuthorizationWindow.h AuthorizationWindow.cpp AuthorizationWindow.ui
        Menu/AboutDialog.h Menu/AboutDialog.cpp Menu/AboutDialog.ui
    )
# Define target properties for Android with Qt 6 as:
#    set_property(TARGET SchoolCRM APPEND PROPERTY QT_ANDROID_PACKAGE_SOURCE_DIR
#                 ${CMAKE_CURRENT_SOURCE_DIR}/android)
# For more information, see https://doc.qt.io/qt-6/qt-add-executable.html#target-creation
else()
    if(ANDROID)
        add_library(SchoolCRM SHARED
            ${PROJECT_SOURCES}
        )
# Define properties for Android with Qt 5 after find_package() calls as:
#    set(ANDROID_PACKAGE_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/android")
    else()
        add_executable(SchoolCRM
            ${PROJECT_SOURCES}
        )
    endif()
endif()

target_link_libraries(SchoolCRM PRIVATE Qt${QT_VERSION_MAJOR}::Widgets $ENV{SchoolDatabaseApiLibPath} Qt${QT_VERSION_MAJOR}::StateMachine SchoolDatabaseApi)

target_include_directories(SchoolCRM PRIVATE ./)

# Qt for iOS sets MACOSX_BUNDLE_GUI_IDENTIFIER automatically since Qt 6.1.
# If you are developing for iOS or macOS you should consider setting an
# explicit, fixed bundle identifier manually though.
if(${QT_VERSION} VERSION_LESS 6.1.0)
  set(BUNDLE_ID_OPTION MACOSX_BUNDLE_GUI_IDENTIFIER com.example.SchoolCRM)
endif()
set_target_properties(SchoolCRM PROPERTIES
    ${BUNDLE_ID_OPTION}
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

include(GNUInstallDirs)
install(TARGETS SchoolCRM
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(SchoolCRM)
endif()
