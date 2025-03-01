#ifndef MENUOFREGULARLISTITEM_H
#define MENUOFREGULARLISTITEM_H

#include <QToolBar>
#include <QToolButton>

class MenuOfRegularListItem : public QToolBar
{
    Q_OBJECT

public:
    MenuOfRegularListItem(QWidget* parent = nullptr);
    void setEtcOptions(QMenu* menu);
    QMenu* getEtcOptions();

signals:
    void removeClicked();

private:
    QToolButton* toolButton;
};

#endif // MENUOFREGULARLISTITEM_H
