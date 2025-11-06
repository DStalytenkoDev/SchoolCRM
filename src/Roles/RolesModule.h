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


#ifndef ROLESMODULE_H
#define ROLESMODULE_H


#include <QWidget>

#include <QItemSelection>
#include <QStateMachine>
#include <QSortFilterProxyModel>

#include <SchoolApi/Role.h>

#include "RolesModel.h"
#include "RoleCreationDialog.h"


namespace Ui {
class RolesModule;
}


class RolesModule : public QWidget
{
    Q_OBJECT

public:
    RolesModule(QWidget *parent = nullptr);

    void setConnection(dbapi::Connection* connection);

    ~RolesModule();

private:
    Ui::RolesModule *ui;

    RoleCreationDialog* roleCreationDialog = nullptr;

    RolesModel* model = nullptr;
    QSortFilterProxyModel* proxyModel;

    dbapi::Connection* connection = nullptr;

    QStateMachine* stateMachine;
    QState* updateGroup;
    QState* rolesNotLoaded;
    QState* rolesLoaded;
    QState* itemSelected;
    QState* searching;

    void enterRolesNotLoaded();
    void enterRolesLoaded();
    void enterItemSelected();
    void enterSearching();

    void handleRoleDeletion();

    void initRoleCreation();
    void completeRoleCreation();

    void completeSearching();
    void abortSearching();
    void handleSearching();

    void setupRoleFinder();
    void setupRolesList();
    void setupStateMachine();

    bool tryConnect();

    void showEvent(QShowEvent* event) override;

private: signals:
    void rolesLoadedAre();
};


#endif // ROLESMODULE_H
