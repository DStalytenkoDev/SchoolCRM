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


#ifndef FAKEPERSONSMODEL_H
#define FAKEPERSONSMODEL_H


#include <QPair>
#include <QAbstractListModel>
#include <SchoolApi/Person.h>


class FakePersonsModel : public QAbstractListModel
{
public:
    FakePersonsModel(QObject *parent = nullptr);

    void createPerson(const dbapi::Person::Key& key, const QString& firstName, const QString& secondName);
    void removePerson(int index);
    void clear();

    const dbapi::Person::Key& person(int index);

    virtual int rowCount(const QModelIndex& index = {}) const override;
    virtual QVariant data(const QModelIndex& index, int role) const override;

private:
    QList<QPair<dbapi::Person::Key, QString>> persons;
};


#endif // FAKEPERSONSMODEL_H
