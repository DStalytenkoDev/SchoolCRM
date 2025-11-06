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


#include "PersonWidget.h"
#include "ui_PersonWidget.h"


PersonWidget::PersonWidget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::PersonWidget)
{
    ui->setupUi(this);

    this->roleView = new ComboBoxFinderView(this);
    this->ui->roleLayout->addWidget(this->roleView);
}

QString PersonWidget::firstName()
{
    return this->ui->firstName->text();
}

QString PersonWidget::secondName()
{
    return this->ui->secondName->text();
}

QModelIndex PersonWidget::role()
{
    int index = this->roleView->currentIndex();

    return this->roleView->model()->index(index, 0);
}

QDate PersonWidget::date()
{
    return this->ui->date->date();
}

void PersonWidget::setFirstName(const QString &name)
{
    this->ui->firstName->setText(name);
}

void PersonWidget::setSecondName(const QString &name)
{
    this->ui->secondName->setText(name);
}

void PersonWidget::setDate(const QDate &date)
{
    this->ui->date->setDate(date);
}

void PersonWidget::setRole(const QModelIndex &role)
{
    this->roleView->setCurrentIndex(role.row());
}

void PersonWidget::setRoles(QAbstractItemModel *roles)
{
    this->roleView->setModel(roles);
}

void PersonWidget::clear()
{
    this->ui->firstName->clear();
    this->ui->secondName->clear();
    this->ui->date->clear();
    this->ui->description->clear();
}

PersonWidget::~PersonWidget()
{
    delete ui;
}

QHBoxLayout *PersonWidget::menuLayout()
{
    return this->ui->menuLayout;
}
