#include "ListItemConcept.h"


ListItemConcept::ListItemConcept(QWidget *parent) noexcept
    : QWidget{parent}
{
    this->toolBar = new QToolBar(this);
    this->leftLayout = new QHBoxLayout(this);

    this->mainLayout = new QHBoxLayout(this);

    this->mainLayout->addLayout(this->leftLayout);
    this->mainLayout->addWidget(this->toolBar, 0, Qt::AlignRight);

    this->setLayout(this->mainLayout);
}

void ListItemConcept::setLeftLayout(QHBoxLayout *layout) noexcept
{
    for (QLayoutItem* child = this->leftLayout->takeAt(0); child != nullptr; child = this->leftLayout->takeAt(0))
    {
        delete child->widget();
        delete child;
    }

    this->mainLayout->removeItem(this->leftLayout);
    delete this->leftLayout;

    this->mainLayout->removeWidget(this->toolBar);

    this->mainLayout->addItem(layout);
    this->mainLayout->addWidget(this->toolBar);

    this->leftLayout = layout;
}

QHBoxLayout *ListItemConcept::getLeftLayout() const noexcept
{
    return this->leftLayout;
}

void ListItemConcept::setRightToolBar(QToolBar *toolBar) noexcept
{
    this->mainLayout->replaceWidget(this->toolBar, toolBar);

    delete this->toolBar;
    this->toolBar = toolBar;
}

QToolBar *ListItemConcept::getRightToolBar() const noexcept
{
    return this->toolBar;
}
