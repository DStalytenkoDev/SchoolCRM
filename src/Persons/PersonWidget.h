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


#ifndef PERSONWIDGET_H
#define PERSONWIDGET_H

#include <QWidget>
#include <qabstractitemmodel.h>
#include <qboxlayout.h>
#include "src/ComboBoxFinderView.h"

namespace Ui {
class PersonWidget;
}

class PersonWidget : public QWidget
{
    Q_OBJECT

public:
    PersonWidget(QWidget *parent = nullptr);

    QString firstName();
    QString secondName();
    QModelIndex role();
    QDate date();

    void setFirstName(const QString& name);
    void setSecondName(const QString& name);
    void setDate(const QDate& date);

    /// set role from a model
    void setRole(const QModelIndex& role);

    /// set the whole model with roles
    void setRoles(QAbstractItemModel* roles);

    /// except for the roles model
    void clear();

    ~PersonWidget();

protected:
    QHBoxLayout* menuLayout();

private:
    Ui::PersonWidget *ui;

    ComboBoxFinderView* roleView;
};

#endif // PERSONWIDGET_H
