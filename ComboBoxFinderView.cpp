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


#include "ComboBoxFinderView.h"
#include <QCompleter>
#include <QEvent>
#include <QAbstractItemView>
#include <qlineedit.h>


ComboBoxFinderView::ComboBoxFinderView(QWidget *parent) : QComboBox(parent)
{
    this->setInsertPolicy(NoInsert);

    connect(this, &ComboBoxFinderView::editTextChanged, this, &ComboBoxFinderView::filter);
    connect(this, &ComboBoxFinderView::activated, this, QOverload<int>::of(&ComboBoxFinderView::handleSelectedItem));
    connect(this, &ComboBoxFinderView::currentIndexChanged, this, &ComboBoxFinderView::handleChangedIndex);

    this->proxyModel = new QSortFilterProxyModel(this);
    this->proxyModel->setFilterCaseSensitivity(Qt::CaseInsensitive);
    this->proxyModel->setFilterKeyColumn(this->modelColumn());

    this->completer = new QCompleter(this->proxyModel, this);
    this->completer->setCompletionMode(QCompleter::UnfilteredPopupCompletion);

    connect(this->completer, QOverload<const QModelIndex&>::of(&QCompleter::activated), this, QOverload<const QModelIndex&>::of(&ComboBoxFinderView::handleSelectedItem));

    this->completer->setWidget(this);

    this->setEditable(true);

    this->setMinimumWidth(150);
}

void ComboBoxFinderView::setModel(QAbstractItemModel *model)
{
    QComboBox::setModel(model);
    this->proxyModel->setSourceModel(model);
}

void ComboBoxFinderView::filter(const QString& text)
{
    this->proxyModel->setFilterWildcard(text);
    this->completer->complete();
}

void ComboBoxFinderView::handleSelectedItem(const QModelIndex &index)
{
    QModelIndex mainModelIndex = this->proxyModel->mapToSource(this->proxyModel->index(index.row(), 0));

    this->setCurrentIndex(mainModelIndex.row());
    emit this->foundItem(mainModelIndex);
}

void ComboBoxFinderView::handleSelectedItem(int index)
{
    QModelIndex modelIndex = this->model()->index(index, 0);
    emit this->foundItem(modelIndex);
}

void ComboBoxFinderView::handleChangedIndex(int index)
{
    QModelIndex modelIndex = this->model()->index(index, 0);
    this->lineEdit()->setPlaceholderText(this->model()->data(modelIndex).toString());
}

