#ifndef JOURNALMODEL_H
#define JOURNALMODEL_H


#include <QAbstractTableModel>
#include <SchoolDatabaseApi/StudentMark/StudentMark.h>
#include <SchoolDatabaseApi/Student/Student.h>


class JournalModel : public QAbstractTableModel
{
public:
    JournalModel(QObject *parent = nullptr);

    void setConnection(dbapi::Connection* connection);

    void setClass(const dbapi::Class::Key& key);
    void setSubject(const dbapi::Subject::Key& subejct);
    void setType(const dbapi::StudentMark::Type& type);
    void setDateRange(const QDate& begin, const QDate& end);

    dbapi::ApiError load();
    dbapi::ApiError store(const QModelIndex& index, int value);

    ~JournalModel();

private:
    QList<dbapi::Person*> students;
    QList<QList<dbapi::StudentMark*>> marksTable;

    int rowCount = 0;
    int columnCount = 0;

    QDate rangeBegin;
    QDate rangeEnd;

    dbapi::Class::Key classKey;
    dbapi::Subject::Key subjectKey;
    dbapi::StudentMark::Type type;

    dbapi::Connection* connection = nullptr;

    QList<dbapi::Person*> loadStudents(dbapi::ApiError* error);

    bool filterMark(dbapi::StudentMark* mark);

    void initTable(int rows, int columns);
    void freeAll();
};


#endif // JOURNALMODEL_H
