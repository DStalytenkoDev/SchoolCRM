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


#ifndef CLASSSTUDENTSMODEL_H
#define CLASSSTUDENTSMODEL_H


#include <QAbstractListModel>
#include <SchoolApi/Class.h>
#include <SchoolApi/Person.h>
#include "UserError.h"


class ClassStudentsModel : public QAbstractListModel
{
public:
    explicit ClassStudentsModel(QObject *parent = nullptr);

    void setConnection(dbapi::Connection* connection);
    void setClass(const dbapi::Class::Key& key);

    /// req: called setConnection() with a valid arg
    UserError loadAll();
    UserError appendStudent(const dbapi::Person::Key& key);
    UserError removeStudent(int index);

    /// in case of any not valid index undefined behaviour
    dbapi::Person* student(const QModelIndex& index);
    /// in case of any not valid index undefined behaviour
    dbapi::Person* student(int row);

    virtual int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    virtual QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;

    void clear();

private:
    dbapi::Connection* connection;
    dbapi::Class::Key classKey;

    QList<dbapi::Person*> students;
};

#endif // CLASSSTUDENTSMODEL_H
