#ifndef SUBJECTSOFTEACHERMODEL_H
#define SUBJECTSOFTEACHERMODEL_H

#include <SchoolApi/TeacherSubjectsList.h>
#include <QAbstractListModel>
#include "UserError.h"

class SubjectsOfTeacherModel : public QAbstractListModel
{
public:
    SubjectsOfTeacherModel(QObject *parent = nullptr);

    void setConnection(dbapi::Connection* connection);
    void setTeacher(const dbapi::Person::Key& key);

    /// req: called setConnection() with a valid arg
    UserError loadSubjects();
    UserError appendSubject(const dbapi::Subject::Key& key);
    UserError removeSubject(int index);

    /// in case of any not valid index undefined behaviour
    dbapi::Subject::Key subject(const QModelIndex& index);
    /// in case of any not valid index undefined behaviour
    dbapi::Subject::Key subject(int row);

    virtual int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    virtual QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;

    void clear();
    ~SubjectsOfTeacherModel();

private:
    dbapi::Connection* connection;
    dbapi::Person::Key teacher;
    QList<dbapi::Subject*> subjects;
};

#endif // SUBJECTSOFTEACHERMODEL_H
