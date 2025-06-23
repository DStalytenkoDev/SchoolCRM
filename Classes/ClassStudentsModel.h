#ifndef CLASSSTUDENTSMODEL_H
#define CLASSSTUDENTSMODEL_H


#include <SchoolApi/Student.h>
#include <QAbstractListModel>


class ClassStudentsModel : public QAbstractListModel
{
public:
    explicit ClassStudentsModel(QObject *parent = nullptr);

    void setConnection(dbapi::Connection* connection);

    /// req: called setConnection() with a valid arg
    dbapi::ApiError loadStudents(const dbapi::Class::Key& key);

    /// in case of any not valid index undefined behaviour
    dbapi::Person* student(const QModelIndex& index);
    /// in case of any not valid index undefined behaviour
    dbapi::Person* student(int row);

    virtual int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    virtual QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;

    void insertRow(const dbapi::Person& person);
    void removeRow(int row);

private:
    dbapi::Connection* connection;
    QList<dbapi::Person> students;

    void clearStudents();
};

#endif // CLASSSTUDENTSMODEL_H
