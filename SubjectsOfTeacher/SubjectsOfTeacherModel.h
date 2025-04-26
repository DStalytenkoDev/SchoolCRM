#ifndef SUBJECTSOFTEACHERMODEL_H
#define SUBJECTSOFTEACHERMODEL_H

#include <SchoolDatabaseApi/TeacherSubjectsList/TeacherSubjectsList.h>
#include <QAbstractListModel>

class SubjectsOfTeacherModel : public QAbstractListModel
{
public:
    SubjectsOfTeacherModel(QObject *parent = nullptr);

    void setConnection(dbapi::Connection* connection);
    void setTeacher(const dbapi::Person::Key& key);

    /// req: called setConnection() with a valid arg
    dbapi::ApiError loadSubjects();

    /// in case of any not valid index undefined behaviour
    dbapi::Subject* subject(const QModelIndex& index);
    /// in case of any not valid index undefined behaviour
    dbapi::Subject* subject(int row);

    virtual int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    virtual QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;

    void insertRow(int rowBefore, const dbapi::Subject& subject);
    void removeRow(int row);

private:
    dbapi::TeacherSubjectsList teacherSubjectsList;
    QList<dbapi::Subject> subjects;
};

#endif // SUBJECTSOFTEACHERMODEL_H
