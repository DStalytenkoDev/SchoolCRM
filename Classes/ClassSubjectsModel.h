#ifndef CLASSSUBJECTSMODEL_H
#define CLASSSUBJECTSMODEL_H


#include <QAbstractListModel>
#include <SchoolApi/ClassSubjectsList.h>
#include "UserError.h"


class ClassSubjectsModel : public QAbstractListModel
{
public:
    ClassSubjectsModel(QObject *parent = nullptr);

    void setConnection(dbapi::Connection* connection);
    void setClass(const dbapi::Class::Key& key);

    /// req: called setConnection() with a valid arg
    UserError loadAll();
    UserError appendSubject(const dbapi::Subject::Key& key);
    UserError removeSubject(int index);

    /// in case of any not valid index undefined behaviour
    dbapi::Subject* subject(const QModelIndex& index);
    /// in case of any not valid index undefined behaviour
    dbapi::Subject* subject(int row);

    virtual int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    virtual QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;

    void clear();

private:
    dbapi::Connection* connection;
    QList<dbapi::Subject*> subjects;

    dbapi::Class::Key classKey;
};

#endif // CLASSSUBJECTSMODEL_H
