#ifndef CUSTOMLISTMODEL_H
#define CUSTOMLISTMODEL_H

#include <QAbstractListModel>

class CustomListModel : public QAbstractListModel
{
public:
    CustomListModel(QObject *parent = nullptr);

    int rowCount(const QModelIndex &parent = QModelIndex()) const;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;

    bool insertRows(int row, int count, const QModelIndex &parent = QModelIndex());
    bool removeRows(int row, int count, const QModelIndex &parent = QModelIndex());

private:

};

#endif // CUSTOMLISTMODEL_H
