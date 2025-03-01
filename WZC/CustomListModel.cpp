#include "CustomListModel.h"

CustomListModel::CustomListModel(QObject *parent)
    : QAbstractListModel{parent}
{}

int CustomListModel::rowCount(const QModelIndex &parent) const
{
    parent.row();
}

QVariant CustomListModel::data(const QModelIndex &index, int role) const
{

}

bool CustomListModel::insertRows(int row, int count, const QModelIndex &parent)
{

}

bool CustomListModel::removeRows(int row, int count, const QModelIndex &parent)
{

}
