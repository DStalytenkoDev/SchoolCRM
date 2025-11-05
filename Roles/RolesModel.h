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


#ifndef ROLESMODEL_H
#define ROLESMODEL_H


#include <QAbstractListModel>
#include <SchoolApi/Role.h>
#include "UserError.h"


/// the class provides loading down the names of the roles, but not any changes in an actual API
class RolesModel : public QAbstractListModel
{
public:
    RolesModel(QObject *parent = nullptr);

    void setConnection(dbapi::Connection* connection);

    UserError loadAll();
    UserError removeRole(int index);
    UserError createRole(const QString& name);

    /// in case of any not valid index undefined behaviour
    dbapi::Role* role(const QModelIndex& index);
    /// in case of any not valid index undefined behaviour
    dbapi::Role* role(int row);

    virtual int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    virtual QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;

    void clear();

    ~RolesModel();

private:
    dbapi::Connection* connection = nullptr;
    QList<dbapi::Role*> roles;
};


#endif // ROLESMODEL_H
