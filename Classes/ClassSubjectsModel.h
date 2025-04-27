#ifndef CLASSSUBJECTSMODEL_H
#define CLASSSUBJECTSMODEL_H


#include <QAbstractListModel>
#include <SchoolDatabaseApi/ClassSubjectsList/ClassSubjectsList.h>


class ClassSubjectsModel : public QAbstractListModel
{
private:
    struct Item
    {
        QString text;
        dbapi::Subject::Key key;
    };

public:
    ClassSubjectsModel(QObject *parent = nullptr);

    void setConnection(dbapi::Connection* connection);

    /// req: called setConnection() with a valid arg
    dbapi::ApiError loadSubjects(const dbapi::Class::Key& key);

    /// in case of any not valid index undefined behaviour
    dbapi::Subject::Key subject(const QModelIndex& index);
    /// in case of any not valid index undefined behaviour
    dbapi::Subject::Key subject(int row);

    virtual int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    virtual QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;

    void insertRow(const dbapi::Subject& subject);
    void removeRow(int row);

private:
    dbapi::Connection* connection;
    QList<Item> items;
};

#endif // CLASSSUBJECTSMODEL_H
