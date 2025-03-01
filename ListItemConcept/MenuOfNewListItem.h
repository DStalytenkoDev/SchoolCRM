#ifndef MENUOFNEWLISTITEM_H
#define MENUOFNEWLISTITEM_H

#include <QToolBar>

class MenuOfNewListItem : public QToolBar
{
    Q_OBJECT

public:
    MenuOfNewListItem(QWidget* parent = nullptr);

signals:
    void loadUpClicked();
    void cancelClicked();
};

#endif // MENUOFNEWLISTITEM_H
