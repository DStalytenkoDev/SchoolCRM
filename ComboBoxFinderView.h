#ifndef COMBOBOXFINDERVIEW_H
#define COMBOBOXFINDERVIEW_H


#include <QComboBox>
#include <QSortFilterProxyModel>


class ComboBoxFinderView : public QComboBox
{
    Q_OBJECT

public:
    ComboBoxFinderView(QWidget* parent = nullptr);
    virtual void setModel(QAbstractItemModel *model) override;

signals:
    void foundItem(QModelIndex);

private:
    void filter(const QString& text);
    void tellFoundItem(int index);
    QSortFilterProxyModel* proxyModel;
};


#endif // COMBOBOXFINDERVIEW_H
