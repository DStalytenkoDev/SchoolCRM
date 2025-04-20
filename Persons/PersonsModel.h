#ifndef PERSONSMODEL_H
#define PERSONSMODEL_H


#include <QAbstractTableModel>
#include <SchoolDatabaseApi/Person/Person.h>

/// provides person's name as a solid string
class PersonsModel : public QAbstractListModel
{
private:
    struct Item
    {
        QString name;
        dbapi::Person::Key key;

        Item(const QString& name, const dbapi::Person::Key& key);
    };

public:
    PersonsModel(QObject *parent = nullptr);

    void setConnection(dbapi::Connection* connection);

    dbapi::ApiError loadAll();

    /// returns -1 in case of invalid index
    dbapi::Person::Key personKeyByIndex(const QModelIndex& index);

    virtual int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    virtual QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;

    /// does not affect the database
    bool insertRow(int rowBefore, const dbapi::Person& person, const QModelIndex &parent = QModelIndex());

private:
    dbapi::Connection* connection = nullptr;
    QList<Item> items;
};


#endif // PERSONSMODEL_H
