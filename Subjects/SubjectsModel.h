#ifndef SUBJECTSMODEL_H
#define SUBJECTSMODEL_H


#include <SchoolDatabaseApi/Connection/Connection.h>
#include <QAbstractListModel>


class SubjectsModel : public QAbstractListModel
{
public:
    SubjectsModel(QObject *parent = nullptr);

    void setConnection(dbapi::Connection* connection);

    /// req: called setConnection() with a valid arg
    dbapi::ApiError loadAll();

    /// in case of any not valid index undefined behaviour
    dbapi::Subject* subject(const QModelIndex& index);
    /// in case of any not valid index undefined behaviour
    dbapi::Subject* subject(int row);

    virtual int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    virtual QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;

    bool insertRow(int rowBefore, const dbapi::Subject& subject, const QModelIndex &parent = QModelIndex());

    ~SubjectsModel();

private:
    dbapi::Connection* connection = nullptr;
    QList<dbapi::Subject*> subjects;

    void cleanSubjects();

};


#endif // SUBJECTSMODEL_H
