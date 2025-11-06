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


#ifndef PERSONSMODEL_H
#define PERSONSMODEL_H


#include <QAbstractTableModel>
#include <SchoolApi/Person.h>
#include "src/UserError.h"


/// provides person's name as a solid string
class PersonsModel : public QAbstractListModel
{
public:
    PersonsModel(QObject *parent = nullptr);

    void setConnection(dbapi::Connection* connection);

    /// req: called setConnection() with a valid arg
    UserError loadAll();
    UserError removePerson(int index);
    UserError createPerson(const QString& firstName,
                           const QString& secondName,
                           const QDate& birthday,
                           const dbapi::Role::Key& role);

    UserError editPerson(int index,
                         const QString& firstName,
                         const QString& secondName,
                         const QDate& birthday,
                         const dbapi::Role::Key& role);

    /// in case of any not valid index undefined behaviour
    dbapi::Person* person(const QModelIndex& index);
    /// in case of any not valid index undefined behaviour
    dbapi::Person* person(int row);

    virtual int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    virtual QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;

    void clear();

    ~PersonsModel();

private:
    dbapi::Connection* connection = nullptr;
    QList<dbapi::Person*> persons;
};


#endif // PERSONSMODEL_H
