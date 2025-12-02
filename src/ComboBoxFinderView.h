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


#ifndef COMBOBOXFINDERVIEW_H
#define COMBOBOXFINDERVIEW_H


#include <QComboBox>
#include <QSortFilterProxyModel>


class ComboBoxFinderView : public QComboBox
{
    Q_OBJECT

public:
    ComboBoxFinderView(QWidget* parent = nullptr);
    virtual void setModel(QAbstractItemModel *model) override;

signals:
    void foundItem(QModelIndex);

private:
    QSortFilterProxyModel* proxyModel;
    QCompleter* completer;

    void filter(const QString& text);
    void handleSelectedItem(const QModelIndex& index);
    void handleSelectedItem(int index);
    void handleChangedIndex(int index);
};


#endif // COMBOBOXFINDERVIEW_H
