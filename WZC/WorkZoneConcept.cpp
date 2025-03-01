#include "WorkZoneConcept.h"

WorkZoneConcept::WorkZoneConcept(QWidget *parent)
    : QWidget{parent}
{
    this->mainLayout = new QVBoxLayout(this);
    this->toolBarLayout = new QHBoxLayout(this);
    this->childLayout = new QVBoxLayout(this);

    this->leftToolBar = new QToolBar(this);
    this->rightToolBar = new QToolBar(this);

    QWidget::setLayout(this->mainLayout);

    this->mainLayout->addLayout(this->toolBarLayout);
    this->mainLayout->addLayout(this->childLayout);

    this->toolBarLayout->addWidget(this->leftToolBar);
    this->toolBarLayout->addSpacerItem(new QSpacerItem(20, 10, QSizePolicy::Fixed, QSizePolicy::Ignored));
    this->toolBarLayout->addWidget(this->rightToolBar);
}
void WorkZoneConcept::setLayout(QLayout *layout)
{
    this->mainLayout->removeItem(this->childLayout);
    delete this->childLayout;

    this->mainLayout->addLayout(layout);
    this->childLayout = layout;
}

QLayout *WorkZoneConcept::layout() const
{
    return this->childLayout;
}

QToolBar *WorkZoneConcept::getLeftToolBar() const
{
    return this->leftToolBar;
}

QToolBar *WorkZoneConcept::getRightToolBar() const
{
    return this->rightToolBar;
}
