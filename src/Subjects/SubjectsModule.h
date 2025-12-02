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


#ifndef SUBJECTSMODULE_H
#define SUBJECTSMODULE_H


#include <QWidget>
#include <QSortFilterProxyModel>
#include <QStateMachine>
#include "SubjectsModel.h"
#include "CreateSubjectDialog.h"


namespace Ui {
class SubjectsModule;
}


class SubjectsModule : public QWidget
{
    Q_OBJECT

public:
    SubjectsModule(QWidget *parent = nullptr);

    void setConnection(dbapi::Connection* connection);

    ~SubjectsModule();

private:
    Ui::SubjectsModule *ui;
    CreateSubjectDialog* subjectCreationDialog = nullptr;

    SubjectsModel* model = nullptr;
    QSortFilterProxyModel* proxyModel;

    dbapi::Connection* connection = nullptr;

    QStateMachine* stateMachine;
    QState* updateGroup;
    QState* subjectsNotLoaded;
    QState* subjectsLoaded;
    QState* itemSelected;
    QState* searching;

    void enterSubjectsNotLoaded();
    void enterSubjectsLoaded();
    void enterItemSelected();
    void enterSearching();

    void handleSubjectDeletion();

    void initSubjectCreation();
    void completeSubjectCreation();

    void completeSearching();
    void abortSearching();
    void handleSearching();

    void setupSubjectFinder();
    void setupSubjectsList();
    void setupStateMachine();

    bool tryConnect();

    void showEvent(QShowEvent* event) override;

private: signals:
    void subjectsLoadedAre();
};


#endif // SUBJECTSMODULE_H
