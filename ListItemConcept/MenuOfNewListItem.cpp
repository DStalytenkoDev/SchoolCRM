#include "MenuOfNewListItem.h"

MenuOfNewListItem::MenuOfNewListItem(QWidget* parent) : QToolBar(parent)
{
    auto loadUpAction = new QAction(this);
    auto cancelAction = new QAction(this);

    loadUpAction->setIcon(QIcon::fromTheme("QIcon::ThemeIcon::GoUp"));
    cancelAction->setIcon(QIcon::fromTheme("QIcon::ThemeIcon::ClearEdit"));

    connect(loadUpAction, &QAction::triggered, this, &MenuOfNewListItem::loadUpClicked);
    connect(cancelAction, &QAction::triggered, this, &MenuOfNewListItem::cancelClicked);

    this->addActions({loadUpAction, cancelAction});
}
