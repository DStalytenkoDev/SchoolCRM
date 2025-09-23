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

