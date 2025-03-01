#include "AddAndFindWZC.h"

AddAndFindWZC::AddAndFindWZC(QWidget* parent) : WorkZoneConcept(parent)
{
    this->addAction = new QAction(this);
    this->addAction->setIcon(QIcon::fromTheme("QIcon::ThemeIcon::ListAdd"));
    connect(this->addAction, &QAction::triggered, this, &AddAndFindWZC::addButtonClicked);

    this->finder = new ChooseAndFindConcept(this);

    this->getRightToolBar()->addAction(this->addAction);
    this->getLeftToolBar()->addWidget(this->finder);
}

ChooseAndFindConcept *AddAndFindWZC::getFinder() const
{
    return this->finder;
}
