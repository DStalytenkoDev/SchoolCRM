#ifndef CLASSESMODEL_H
#define CLASSESMODEL_H


#include <QAbstractListModel>
#include <SchoolDatabaseApi/Class/Class.h>


class ClassesModel : public QAbstractListModel
{
public:
    ClassesModel(QObject *parent = nullptr);

    void setConnection(dbapi::Connection* connection);

    /// req: called setConnection() with a valid arg
    dbapi::ApiError loadAll();

    /// in case of any not valid index undefined behaviour
    dbapi::Class* grade(const QModelIndex& index);
    /// in case of any not valid index undefined behaviour
    dbapi::Class* grade(int row);

    virtual int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    virtual QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;

    void insertRow(const dbapi::Class& grade);
    void removeRow(int row);

    ~ClassesModel();

private:
    dbapi::Connection* connection = nullptr;
    QList<dbapi::Class*> classes;
};

#endif // CLASSESMODEL_H
