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


#ifndef MAINWINDOW_H
#define MAINWINDOW_H


#include <QMainWindow>
#include <QTreeWidget>
#include "AuthorizationWindow.h"
#include "Menu/AboutDialog.h"
#include "Roles/RolesModule.h"
#include "Subjects/SubjectsModule.h"
#include "Persons/PersonsModule.h"
#include "SubjectsOfTeacher/SubjectsOfTeacherModule.h"
#include "Classes/ClassesModule.h"
#include "Journal/JournalModule.h"


QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE


class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    AuthorizationWindow* authorizationWindow;

    // menu
    AboutDialog* aboutDialog;


    // modules
    RolesModule* roles;
    SubjectsModule* subjects;
    PersonsModule* persons;
    SubjectsOfTeacherModule* subjectsOfTeacherModule;
    ClassesModule* classes;
    JournalModule* journal;

    QWidget* lastModule = nullptr;

    void manageLeftBarActions(QTreeWidgetItem* item, int column);

    void initAuthorization();

    void swapMainWidget(QWidget* newWidget);

    void showAbout();
    void showHelp();
};


#endif // MAINWINDOW_H
