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


#ifndef CLASSESMODEL_H
#define CLASSESMODEL_H


#include <QAbstractListModel>
#include <SchoolApi/Class.h>
#include <SchoolApi/Person.h>
#include "src/UserError.h"


class ClassesModel : public QAbstractListModel
{
public:
    ClassesModel(QObject *parent = nullptr);

    void setConnection(dbapi::Connection* connection);

    /// req: called setConnection() with a valid arg
    UserError loadAll();
    UserError createClass(const QString& name, const dbapi::Person::Key& homeroomTeacher);
    UserError removeClass(int index);
    UserError changeHomeroomTeacher(int index, const dbapi::Person::Key& homeroomTeacher);
    std::variant<dbapi::Person::Key, UserError> getHomeroomTeacher(int index);

    /// in case of any not valid index undefined behaviour
    dbapi::Class* grade(const QModelIndex& index);
    /// in case of any not valid index undefined behaviour
    dbapi::Class* grade(int row);

    virtual int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    virtual QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;

    void clear();

    ~ClassesModel();

private:
    dbapi::Connection* connection = nullptr;
    QList<dbapi::Class*> classes;

    dbapi::Classmate* findTeacher(int index);
    bool removeTeacher(int index);
};

#endif // CLASSESMODEL_H
