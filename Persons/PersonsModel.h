#ifndef PERSONSMODEL_H
#define PERSONSMODEL_H


#include <QAbstractTableModel>
#include <SchoolApi/Person.h>
#include "UserError.h"


/// provides person's name as a solid string
class PersonsModel : public QAbstractListModel
{
public:
    PersonsModel(QObject *parent = nullptr);

    void setConnection(dbapi::Connection* connection);

    /// req: called setConnection() with a valid arg
    UserError loadAll();
    UserError removePerson(int index);
    UserError createPerson(const QString& firstName,
                           const QString& secondName,
                           const QDate& birthday,
                           const dbapi::Role::Key& role);

    UserError editPerson(int index,
                         const QString& firstName,
                         const QString& secondName,
                         const QDate& birthday,
                         const dbapi::Role::Key& role);

    /// in case of any not valid index undefined behaviour
    dbapi::Person* person(const QModelIndex& index);
    /// in case of any not valid index undefined behaviour
    dbapi::Person* person(int row);

    virtual int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    virtual QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;

    void clear();

    ~PersonsModel();

private:
    dbapi::Connection* connection = nullptr;
    QList<dbapi::Person*> persons;
};


#endif // PERSONSMODEL_H
