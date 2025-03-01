#include "MenuOfRegularListItem.h"
#include <QToolButton>

MenuOfRegularListItem::MenuOfRegularListItem(QWidget* parent) : QToolBar(parent)
{
    auto removeAction = new QAction(this);
    this->toolButton = new QToolButton(this);

    removeAction->setIcon(QIcon::fromTheme("QIcon::ThemeIcon::EditDelete"));
    connect(removeAction, &QAction::triggered, this, &MenuOfRegularListItem::removeClicked);

    this->addAction(removeAction);
    this->addWidget(this->toolButton);
}

void MenuOfRegularListItem::setEtcOptions(QMenu *menu)
{
    this->toolButton->setMenu(menu);
}

QMenu* MenuOfRegularListItem::getEtcOptions()
{
    return this->toolButton->menu();
}
