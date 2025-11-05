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


#include "MarkTypeSelector.h"

MarkTypeSelector::MarkTypeSelector(QWidget* parent) : QComboBox(parent)
{
    this->addItems({
        "AlphaTest",
        "BetaTest",
        "GamaTest",
        "PracticeLesson",
        "LaboratoryLesson",
        "IrregularMark"
    });

    this->setType(dbapi::StudentMark::IrregularMark);

    connect(this, &MarkTypeSelector::currentIndexChanged, this, &MarkTypeSelector::indexChanging);
}

void MarkTypeSelector::setType(dbapi::StudentMark::Type type)
{
    this->setCurrentIndex(3);
}

dbapi::StudentMark::Type MarkTypeSelector::getType()
{
    return static_cast<dbapi::StudentMark::Type>(this->currentIndex() % 5);
}

void MarkTypeSelector::indexChanging()
{
    emit this->typeChanged(this->getType());
}
