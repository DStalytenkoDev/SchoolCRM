#ifndef SUBJECTSOFTEACHERMODEL_H
#define SUBJECTSOFTEACHERMODEL_H

#include <SchoolApi/TeacherSubjectsList.h>
#include <QAbstractListModel>

class SubjectsOfTeacherModel : public QAbstractListModel
{
private:
    struct Item
    {
        QString text;
        dbapi::Subject::Key key;
    };

public:
    SubjectsOfTeacherModel(QObject *parent = nullptr);

    void setConnection(dbapi::Connection* connection);

    /// req: called setConnection() with a valid arg
    dbapi::ApiError loadSubjects(const dbapi::Person::Key& key);

    /// in case of any not valid index undefined behaviour
    dbapi::Subject::Key subject(const QModelIndex& index);
    /// in case of any not valid index undefined behaviour
    dbapi::Subject::Key subject(int row);

    virtual int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    virtual QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;

    void insertRow(const dbapi::Subject& subject);
    void removeRow(int row);
    void clear();

private:
    dbapi::Connection* connection;
    QList<Item> items;
};

#endif // SUBJECTSOFTEACHERMODEL_H
