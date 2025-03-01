#include "ListedAddAndFindWZC.h"

ListedAddAndFindWZC::ListedAddAndFindWZC(QWidget *parent) : AddAndFindWZC(parent)
{
    this->listWidget = new QListWidget(this);

    this->layout()->addWidget(this->listWidget);
}

QListWidget *ListedAddAndFindWZC::getListWidget() const
{
    return this->listWidget;
}
