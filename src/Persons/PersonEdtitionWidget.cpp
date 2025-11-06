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


#include "PersonEdtitionWidget.h"

PersonEditionWidget::PersonEditionWidget(QWidget *parent) : PersonWidget(parent)
{
    this->saveBtn = new QPushButton("Save", this);
    this->abortButton = new QPushButton("Abort", this);

    this->menuLayout()->addWidget(this->saveBtn);
    this->menuLayout()->addWidget(this->abortButton);

    connect(this->saveBtn, &QPushButton::clicked, this, &PersonEditionWidget::saveButtonClicked);
    connect(this->abortButton, &QPushButton::clicked, this, &PersonEditionWidget::abortButtonClicked);
}
