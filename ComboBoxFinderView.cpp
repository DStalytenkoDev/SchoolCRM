#include "ComboBoxFinderView.h"
#include <QCompleter>


ComboBoxFinderView::ComboBoxFinderView(QWidget *parent) : QComboBox(parent)
{
    this->setEditable(true);
    this->setInsertPolicy(NoInsert);

    connect(this, &ComboBoxFinderView::editTextChanged, this, &ComboBoxFinderView::filter);
    connect(this, &ComboBoxFinderView::currentIndexChanged, this, &ComboBoxFinderView::tellFoundItem);

    this->proxyModel = new QSortFilterProxyModel(this);
    this->proxyModel->setSourceModel(this->model());
    this->proxyModel->setFilterCaseSensitivity(Qt::CaseInsensitive);
    this->proxyModel->setFilterKeyColumn(this->modelColumn());

    auto completer = new QCompleter(this);
    completer->setModel(this->proxyModel);
    completer->setCompletionMode(QCompleter::UnfilteredPopupCompletion);
    this->setCompleter(completer);
}

void ComboBoxFinderView::setModel(QAbstractItemModel *model)
{
    QComboBox::setModel(model);
    this->proxyModel->setSourceModel(model);
}

void ComboBoxFinderView::filter(const QString& text)
{
    this->proxyModel->setFilterFixedString(text);
}

void ComboBoxFinderView::tellFoundItem(int index)
{
    emit this->foundItem(this->model()->index(index, 0));
}
