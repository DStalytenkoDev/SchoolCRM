#ifndef SUBJECTSMODEL_H
#define SUBJECTSMODEL_H


#include <SchoolApi/Connection.h>
#include <QAbstractListModel>
#include "UserError.h"


class SubjectsModel : public QAbstractListModel
{
public:
    SubjectsModel(QObject *parent = nullptr);

    void setConnection(dbapi::Connection* connection);

    /// req: called setConnection() with a valid arg
    UserError loadAll();
    UserError removeSubject(int index);
    UserError createSubject(const QString& name);

    /// in case of any not valid index undefined behaviour
    dbapi::Subject* subject(const QModelIndex& index);
    /// in case of any not valid index undefined behaviour
    dbapi::Subject* subject(int row);

    virtual int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    virtual QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;

    void clear();

    ~SubjectsModel();

private:
    dbapi::Connection* connection = nullptr;
    QList<dbapi::Subject*> subjects;
};


#endif // SUBJECTSMODEL_H
