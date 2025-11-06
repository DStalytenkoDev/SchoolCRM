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


#ifndef SUBJECTSMODEL_H
#define SUBJECTSMODEL_H


#include <SchoolApi/Connection.h>
#include <QAbstractListModel>
#include "src/UserError.h"


class SubjectsModel : public QAbstractListModel
{
public:
    SubjectsModel(QObject *parent = nullptr);

    void setConnection(dbapi::Connection* connection);

    /// req: called setConnection() with a valid arg
    UserError loadAll();
    UserError removeSubject(int index);
    UserError createSubject(const QString& name);

    /// in case of any not valid index undefined behaviour
    dbapi::Subject* subject(const QModelIndex& index);
    /// in case of any not valid index undefined behaviour
    dbapi::Subject* subject(int row);

    virtual int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    virtual QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;

    void clear();

    ~SubjectsModel();

private:
    dbapi::Connection* connection = nullptr;
    QList<dbapi::Subject*> subjects;
};


#endif // SUBJECTSMODEL_H
