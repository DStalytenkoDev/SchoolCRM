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


#include "ClassCreationDialog.h"
#include "ui_ClassCreationDialog.h"


ClassCreationDialog::ClassCreationDialog(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::ClassCreationDialog)
{
    ui->setupUi(this);

    this->teacherFinder = new ComboBoxFinderView(this);

    this->ui->teacherLayout->addWidget(this->teacherFinder);

    this->ui->teacherLayout->setStretch(0, 1);
    this->ui->teacherLayout->setStretch(1, 2);
}

QString ClassCreationDialog::name() const
{
    return this->ui->className->text();
}

QModelIndex ClassCreationDialog::currentIndex() const
{
    return this->teacherFinder->model()->index(this->teacherFinder->currentIndex(), 0);
}

void ClassCreationDialog::setModel(QAbstractItemModel *model)
{
    this->teacherFinder->setModel(model);
}

ClassCreationDialog::~ClassCreationDialog()
{
    delete ui;
}
