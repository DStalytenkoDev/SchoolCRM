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


#include "MarkCreatingDialog.h"
#include "ui_MarkCreatingDialog.h"

MarkCreatingDialog::MarkCreatingDialog(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::MarkCreatingDialog)
{
    ui->setupUi(this);

    this->teacherFinder = new ComboBoxFinderView(this);

    this->ui->verticalLayout->insertWidget(0, this->teacherFinder);
}

void MarkCreatingDialog::setTeachersModel(QAbstractItemModel *model)
{
    this->teacherFinder->setModel(model);
}

QModelIndex MarkCreatingDialog::currentTeacher()
{
    return this->teacherFinder->model()->index(this->teacherFinder->currentIndex(), 0);
}

int MarkCreatingDialog::value()
{
    return this->ui->markValue->text().toInt();
}

void MarkCreatingDialog::clear()
{
    this->teacherFinder->setCurrentIndex(-1);
    this->ui->markValue->clear();
}

MarkCreatingDialog::~MarkCreatingDialog()
{
    delete ui;
}
