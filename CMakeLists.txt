cmake_minimum_required(VERSION 3.16)

project(SchoolCRM VERSION 0.1 LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets Sql)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets Sql)

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
        Tools/MarkTypeSelector.h
        Tools/MarkTypeSelector.cpp
        Tools/FuzzySearchTools.h

        ChooseAndFindEntities/ChooseAndFindConcept.h
        ChooseAndFindEntities/ChooseAndFindConcept.cpp

        ChooseAndFindEntities/ChooseAndFindPerson.h
        ChooseAndFindEntities/ChooseAndFindPerson.cpp
        ChooseAndFindEntities/ChooseAndFindSubject.h
        ChooseAndFindEntities/ChooseAndFindSubject.cpp
        ChooseAndFindEntities/ChooseAndFindClass.h
        ChooseAndFindEntities/ChooseAndFindClass.cpp
        ChooseAndFindEntities/ChooseAndFindStudent.h
        ChooseAndFindEntities/ChooseAndFindStudent.cpp
        ChooseAndFindEntities/ChooseAndFindRole.h
        ChooseAndFindEntities/ChooseAndFindRole.cpp

        ListItemConcept/ListItemConcept.h ListItemConcept/ListItemConcept.cpp
        ListItemConcept/MenuOfNewListItem.h ListItemConcept/MenuOfNewListItem.cpp
        ListItemConcept/MenuOfRegularListItem.h ListItemConcept/MenuOfRegularListItem.cpp
        ListItemConcept/NewSubjectListItem.h ListItemConcept/NewSubjectListItem.cpp
        ListItemConcept/NewRoleListItem.h ListItemConcept/NewRoleListItem.cpp
        ListItemConcept/NewTeacherSubjectListItem.h ListItemConcept/NewTeacherSubjectListItem.cpp
        ListItemConcept/RegularSubjectListItem.h ListItemConcept/RegularSubjectListItem.cpp

        PersonCardConcept/PersonCardConcept.h PersonCardConcept/PersonCardConcept.cpp
        PersonCardConcept/NewPersonCard.h PersonCardConcept/NewPersonCard.cpp
        PersonCardConcept/RegularPersonCard.h PersonCardConcept/RegularPersonCard.cpp
        Tools/SmartEdit.h Tools/SmartEdit.cpp
        ListItemConcept/RegularRoleListItem.h ListItemConcept/RegularRoleListItem.cpp
        WZC/WorkZoneConcept.h WZC/WorkZoneConcept.cpp
        WZC/AddAndFindWZC.h WZC/AddAndFindWZC.cpp
        WZC/ListedAddAndFindWZC.h WZC/ListedAddAndFindWZC.cpp
        WZC/RolesWidget.h WZC/RolesWidget.cpp
        WZC/CustomListModel.h WZC/CustomListModel.cpp











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

if(NOT DEFINED ENV{SchoolDatabaseApiLibPath})
    message(FATAL_ERROR "you have to set a [SchoolDatabaseApiLibPath] variable in project settings to specify the path to compiled library")
endif()

target_link_libraries(SchoolCRM PRIVATE Qt${QT_VERSION_MAJOR}::Widgets $ENV{SchoolDatabaseApiLibPath} Qt${QT_VERSION_MAJOR}::Sql)

if(NOT DEFINED ENV{SchoolDatabaseApiHeadersPath})
    message(FATAL_ERROR "you have to set a [SchoolDatabaseApiHeadersPath] variable in project settings to specify the path to headers of the library")
else()
    target_include_directories(SchoolCRM PRIVATE $ENV{SchoolDatabaseApiHeadersPath})
endif()

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
