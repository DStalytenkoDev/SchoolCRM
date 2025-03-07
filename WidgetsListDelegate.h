#ifndef WIDGETSLISTDELEGATE_H
#define WIDGETSLISTDELEGATE_H

#include <QStyledItemDelegate>

class WidgetsListDelegate : public QStyledItemDelegate
{
public:
    WidgetsListDelegate(QObject* parent  = nullptr);

    virtual void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const override;
    virtual QSize sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const override;

protected:
    /// user has to implement widget creation and return a widget
    virtual QWidget* initiateWidget(QWidget* parent, const QStyleOptionViewItem& option, const QModelIndex& index) = 0;

private:
    bool initiateOnce = false;
    QWidget* initiatedWidget = nullptr;
};

#endif // WIDGETSLISTDELEGATE_H
