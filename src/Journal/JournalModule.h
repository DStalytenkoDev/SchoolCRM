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


#ifndef JOURNALMODULE_H
#define JOURNALMODULE_H


#include <QWidget>
#include <QStateMachine>
#include <QDateEdit>
#include <QSpacerItem>

#include "src/Classes/ClassStudentsModel.h"
#include "src/Classes/ClassSubjectsModel.h"
#include "src/Classes/ClassesModel.h"
#include "FakePersonsModel.h"

#include "src/ComboBoxFinderView.h"
#include "MarkCreatingDialog.h"

#include "MarkTypeSelector.h"
#include "JournalModel.h"


namespace Ui {
class JournalModule;
}


class JournalModule : public QWidget
{
    Q_OBJECT

public:
    JournalModule(QWidget *parent = nullptr);

    void setConnection(dbapi::Connection* connection);
    void prepare();

    ~JournalModule();

private:
    Ui::JournalModule *ui;
    QSpacerItem* journalSpacer;

    ComboBoxFinderView* classFinder;
    ComboBoxFinderView* subjectFinder;
    MarkTypeSelector* markTypeFinder;

    JournalModel* journalModel;

    ClassesModel* classesModel;
    ClassStudentsModel* classStudentsModel;
    ClassSubjectsModel* classSubjectsModel;
    FakePersonsModel* personsModel;

    QStateMachine* stateMachine;
    QState* classesNotLoaded;
    QState* classesLoaded;
    QState* classSelected;
    QState* keySelected;
    QState* emptyCellSelected;
    QState* rangeSelected;

    QState* resetGroupState;

    dbapi::Connection* connection = nullptr;

    MarkCreatingDialog* markCreatingDialog = nullptr;
    QPersistentModelIndex selectedMark;

    void enterClassesNotLoadedState();
    void enterClassesLoadedState();
    void enterClassSelectedState();
    void enterKeySelectedState();
    void enterEmptyCellSelectedState();
    void enterRangeSelectedState();

    void handleSelectedMarks();
    void handleMarksDeleting();

    void handleJournalLoading();
    void handleJournalAndTeachersLoading();

    void initMarkCreating();
    void completeMarkCreating();

    void setupFinders();
    void setupJournal();
    void setupToolBar();
    void setupStateMachine();

    /// trys to open the connection, otherwise shows error
    bool tryConnect();
    /// shows generall error
    void showInternalError();
    /// if not selected returns nullptr
    dbapi::Class* currentClass();

    bool loadCompatibleTeachers();

private: signals:
    void setClassesNotLoadedState();
    void setClassesLoadedState();
    void setKeySelectedState();
    void setEmptyCellSelectedState();
    void setRangeSelectedState();
};


#endif // JOURNALMODULE_H
