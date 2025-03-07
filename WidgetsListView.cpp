#include "WidgetsListView.h"

WidgetsListView::WidgetsListView(QWidget *parent)
{
    this->setMouseTracking(true);
    this->setEditTriggers(NoEditTriggers);
}

void WidgetsListView::itemEntered(const QModelIndex &item)
{
    if(this->lastEnteredIndex.isValid())
        this->setIndexWidget(this->lastEnteredIndex, nullptr);

    auto delegate = this->itemDelegateForIndex(item);

    delegate->
}
