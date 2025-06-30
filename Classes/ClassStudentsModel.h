#ifndef CLASSSTUDENTSMODEL_H
#define CLASSSTUDENTSMODEL_H


#include <QAbstractListModel>
#include <SchoolApi/Class.h>
#include <SchoolApi/Person.h>
#include "UserError.h"


class ClassStudentsModel : public QAbstractListModel
{
public:
    explicit ClassStudentsModel(QObject *parent = nullptr);

    void setConnection(dbapi::Connection* connection);
    void setClass(const dbapi::Class::Key& key);

    /// req: called setConnection() with a valid arg
    UserError loadAll();
    UserError appendStudent(const dbapi::Person::Key& key);
    UserError removeStudent(int index);

    /// in case of any not valid index undefined behaviour
    dbapi::Person* student(const QModelIndex& index);
    /// in case of any not valid index undefined behaviour
    dbapi::Person* student(int row);

    virtual int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    virtual QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;

    void clear();

private:
    dbapi::Connection* connection;
    dbapi::Class::Key classKey;

    QList<dbapi::Person*> students;
};

#endif // CLASSSTUDENTSMODEL_H
