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


#ifndef SUBJECTSOFTEACHERMODEL_H
#define SUBJECTSOFTEACHERMODEL_H

#include <SchoolApi/TeacherSubjectsList.h>
#include <QAbstractListModel>
#include "UserError.h"

class SubjectsOfTeacherModel : public QAbstractListModel
{
public:
    SubjectsOfTeacherModel(QObject *parent = nullptr);

    void setConnection(dbapi::Connection* connection);
    void setTeacher(const dbapi::Person::Key& key);

    /// req: called setConnection() with a valid arg
    UserError loadSubjects();
    UserError appendSubject(const dbapi::Subject::Key& key);
    UserError removeSubject(int index);

    /// in case of any not valid index undefined behaviour
    dbapi::Subject* subject(const QModelIndex& index);
    /// in case of any not valid index undefined behaviour
    dbapi::Subject* subject(int row);

    virtual int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    virtual QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;

    void clear();
    ~SubjectsOfTeacherModel();

private:
    dbapi::Connection* connection;
    dbapi::Person::Key teacher;
    QList<dbapi::Subject*> subjects;
};

#endif // SUBJECTSOFTEACHERMODEL_H
