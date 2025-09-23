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
    QSortFilterProxyModel* proxyModel;
    QCompleter* completer;

    void filter(const QString& text);
    void handleSelectedItem(const QModelIndex& index);
    void handleSelectedItem(int index);
    void handleChangedIndex(int index);
};


#endif // COMBOBOXFINDERVIEW_H
