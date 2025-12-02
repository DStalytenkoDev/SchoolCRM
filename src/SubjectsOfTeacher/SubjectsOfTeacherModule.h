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


#ifndef SUBJECTSOFTEACHERMODULE_H
#define SUBJECTSOFTEACHERMODULE_H


#include <QWidget>
#include <QItemSelection>
#include "../ComboBoxFinderView.h"
#include "SubjectsOfTeacherModel.h"
#include "../Persons/PersonsModel.h"
#include "../Subjects/SubjectsModel.h"


namespace Ui {
class SubjectsOfTeacherModule;
}


class SubjectsOfTeacherModule : public QWidget
{
    Q_OBJECT

public:
    SubjectsOfTeacherModule(QWidget *parent = nullptr);

    void setConnection(dbapi::Connection* connection);
    void prepare();

    ~SubjectsOfTeacherModule();

private:
    Ui::SubjectsOfTeacherModule *ui;

    ComboBoxFinderView* personFinder;
    ComboBoxFinderView* subjectFinder;

    PersonsModel* personsModel;
    SubjectsModel* subjectsModel;
    SubjectsOfTeacherModel* model;

    dbapi::Connection* connection = nullptr;

    void handleSelectedTeacher();
    void handleSelectedSubject();

    void handleSubjectDeleting();

    void initSubjectAddition();
    void abortSubjectAddition();
    void completeSubjectAddition();

    void loadSubjects();

    void setupFinders();
    void setupSubjectsList();
    void setupToolBar();

    bool tryConnect();

    /// if not selected returns nullptr
    dbapi::Person* currentTeacher();
};

#endif // SUBJECTSOFTEACHERMODULE_H
