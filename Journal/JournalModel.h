#ifndef JOURNALMODEL_H
#define JOURNALMODEL_H


#include <QAbstractTableModel>
#include <SchoolApi/StudentMark.h>
#include <SchoolApi/Student.h>
#include <QItemSelection>


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
    dbapi::ApiError store(const QModelIndex& index, int value, const dbapi::Person::Key& teacher);
    dbapi::ApiError remove(const QItemSelection& selection);

    dbapi::StudentMark* mark(const QModelIndex& index);
    dbapi::StudentMark* mark(int row, int column);

    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    int columnCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;

    ~JournalModel();

private:
    QList<dbapi::Person*> students;
    QList<QList<dbapi::StudentMark*>> marksTable;

    int _rowCount = 0;
    int _columnCount = 0;

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

    QVariant verticalHeader(int row) const;
    QVariant horizontalHeader(int column) const;
};


#endif // JOURNALMODEL_H
