#ifndef WIDGETSLISTVIEW_H
#define WIDGETSLISTVIEW_H


#include <QListView>


class WidgetsListView : public QListView
{
    Q_OBJECT

public:
    WidgetsListView(QWidget* parent = nullptr);

private:
    QModelIndex lastEnteredIndex;

    void itemEntered(const QModelIndex& item);

    // set no edit
};


#endif // WIDGETSLISTVIEW_H
