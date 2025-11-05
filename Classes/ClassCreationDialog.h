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


#ifndef CLASSCREATIONDIALOG_H
#define CLASSCREATIONDIALOG_H


#include <QDialog>
#include <QAbstractItemModel>
#include "../ComboBoxFinderView.h"


namespace Ui {
class ClassCreationDialog;
}

class ClassCreationDialog : public QDialog
{
    Q_OBJECT

public:
    ClassCreationDialog(QWidget *parent = nullptr);

    QString name() const;
    QModelIndex currentIndex() const;

    void setModel(QAbstractItemModel* model);

    ~ClassCreationDialog();

private:
    Ui::ClassCreationDialog *ui;

    ComboBoxFinderView* teacherFinder;
};


#endif // CLASSCREATIONDIALOG_H
