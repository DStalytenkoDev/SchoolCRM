#ifndef CLASSESMODEL_H
#define CLASSESMODEL_H


#include <QAbstractListModel>
#include <SchoolApi/Class.h>
#include <SchoolApi/Person.h>
#include "UserError.h"


class ClassesModel : public QAbstractListModel
{
public:
    ClassesModel(QObject *parent = nullptr);

    void setConnection(dbapi::Connection* connection);

    /// req: called setConnection() with a valid arg
    UserError loadAll();
    UserError createClass(const QString& name, const dbapi::Person::Key& homeroomTeacher);
    UserError removeClass(int index);
    UserError changeHomeroomTeacher(int index, const dbapi::Person::Key& homeroomTeacher);
    std::variant<dbapi::Person::Key, UserError> getHomeroomTeacher(int index);

    /// in case of any not valid index undefined behaviour
    dbapi::Class* grade(const QModelIndex& index);
    /// in case of any not valid index undefined behaviour
    dbapi::Class* grade(int row);

    virtual int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    virtual QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;

    void clear();

    ~ClassesModel();

private:
    dbapi::Connection* connection = nullptr;
    QList<dbapi::Class*> classes;

    dbapi::Classmate* findTeacher(int index);
    bool removeTeacher(int index);
};

#endif // CLASSESMODEL_H
