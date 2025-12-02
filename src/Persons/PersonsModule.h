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


#ifndef PERSONSMODULE_H
#define PERSONSMODULE_H


#include <QWidget>
#include <QItemSelection>
#include <QStateMachine>
#include "PersonCreationDialog.h"
#include "PersonEdtitionWidget.h"
#include "PersonsModel.h"
#include "src/ComboBoxFinderView.h"
#include "src/Roles/RolesModel.h"


namespace Ui {
class PersonsModule;
}


class PersonsModule : public QWidget
{
    Q_OBJECT

public:
    PersonsModule(QWidget *parent = nullptr);

    void setConnection(dbapi::Connection* connection);

    ~PersonsModule();

private:
    Ui::PersonsModule *ui;
    PersonEditionWidget* personWidget = nullptr;
    PersonCreationDialog* personDialog = nullptr;

    ComboBoxFinderView* personFinder = nullptr;

    PersonsModel* model = nullptr;
    RolesModel* rolesModel = nullptr;

    QStateMachine* stateMachine;
    QState* resetState;
    QState* personsNotLoaded;
    QState* personsLoaded;
    QState* itemSelected;

    dbapi::Connection* connection = nullptr;

    void enterPersonsNotLoaded();
    void enterPersonsLoaded();
    void enterItemSelected();

    void handleFoundPerson(QModelIndex index);
    void handleSelectedItem();

    void handlePersonDeletion();

    void initPersonCreation();
    void completePersonCreation(QDialog::DialogCode result);

    void completePersonEdition();

    void setupPersonFinder();
    void setupPersonsList();
    void setupPersonEditionWidget();
    void setupStateMachine();

    /// trys to open the connection, otherwise shows error
    bool tryConnect();

    void showEvent(QShowEvent* event) override;

private: signals:
    void personFoundIs();
    void personsLoadedAre();
    void dataError();
};


#endif // PERSONSMODULE_H
