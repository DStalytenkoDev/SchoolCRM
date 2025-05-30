#ifndef PERSONSMODEL_H
#define PERSONSMODEL_H


#include <QAbstractTableModel>
#include <SchoolDatabaseApi/Person/Person.h>


/// provides person's name as a solid string
class PersonsModel : public QAbstractListModel
{
public:
    PersonsModel(QObject *parent = nullptr);

    void setConnection(dbapi::Connection* connection);

    /// req: called setConnection() with a valid arg
    dbapi::ApiError loadAll();

    /// in case of any not valid index undefined behaviour
    dbapi::Person* person(const QModelIndex& index);
    /// in case of any not valid index undefined behaviour
    dbapi::Person* person(int row);

    /// must be called when dbapi::Person object is edited (first or second persons's name)
    void personNameEdited(const QModelIndex& index);

    virtual int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    virtual QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;

    bool insertRow(int rowBefore, const dbapi::Person& person, const QModelIndex &parent = QModelIndex());

    void clear();

    ~PersonsModel();

private:
    dbapi::Connection* connection = nullptr;
    QList<dbapi::Person*> persons;

    void cleanPersons();
};


#endif // PERSONSMODEL_H
