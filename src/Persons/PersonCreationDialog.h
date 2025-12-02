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


#ifndef PERSONCREATIONDIALOG_H
#define PERSONCREATIONDIALOG_H


#include "PersonWidget.h"
#include <qpushbutton.h>
#include <QDialog>


/// this class does not operate the database, when finished does not disappear
class PersonCreationDialog : public PersonWidget
{
    Q_OBJECT

public:
    PersonCreationDialog(QWidget* parent = nullptr);

signals:
    void finished(QDialog::DialogCode code);

private:
    QPushButton* createBtn;
    QPushButton* cancelBtn;
};


#endif // PERSONCREATIONDIALOG_H
