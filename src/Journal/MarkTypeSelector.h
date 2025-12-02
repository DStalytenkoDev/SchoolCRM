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


#ifndef MARKTYPESELECTOR_H
#define MARKTYPESELECTOR_H

#include <QComboBox>
#include <SchoolApi/StudentMark.h>

class MarkTypeSelector : public QComboBox
{
    Q_OBJECT

public:
    MarkTypeSelector(QWidget* parent);

    void setType(dbapi::StudentMark::Type type);
    dbapi::StudentMark::Type getType();

signals:
    void typeChanged(dbapi::StudentMark::Type type);

private:
    void indexChanging();
};

#endif // MARKTYPESELECTOR_H
