#include "JournalModel.h"
#include <qitemselectionmodel.h>

JournalModel::JournalModel(QObject *parent)
    : QAbstractTableModel{parent}
{
    this->studentsModel = new ClassStudentsModel(this);
}

void JournalModel::setConnection(dbapi::Connection *connection)
{
    assert((void("nullptr"), connection));

    this->connection = connection;
    this->studentsModel->setConnection(connection);
}

void JournalModel::setClass(const dbapi::Class::Key& key)
{
    this->classKey = key;
    this->studentsModel->setClass(key);
}

void JournalModel::setSubject(const dbapi::Subject::Key& key)
{
    this->subjectKey = key;
}

void JournalModel::setType(const dbapi::StudentMark::Type &type)
{
    this->type = type;
}

void JournalModel::setDateRange(const QDate &begin, const QDate &end)
{
    this->rangeBegin = begin;

    if(end <= begin)
        this->rangeEnd = begin.addDays(1);
    else
        this->rangeEnd = end;
}

UserError JournalModel::load()
{
    if(this->studentsModel->loadAll().isError())
        return UserError::internalError("Journal", "be loadded 'cause something went wrong", "Try again or contact support");

    dbapi::ApiError error;
    auto marks = dbapi::StudentMark::loadAll(this->connection, &error);

    if(error.type != dbapi::ApiError::NoError)
        return UserError::internalError("Journal", "be loadded 'cause something went wrong", "Try again or contact support");

    this->beginResetModel();
    this->freeAll();

    int futureColumnCount = this->rangeBegin.daysTo(this->rangeEnd);
    int futureRowCount = this->studentsModel->rowCount();

    if(futureRowCount == 0)
        futureColumnCount = 0;

    this->initTable(futureRowCount, futureColumnCount);

    // assign indexies for each student by key
    QMap<int, int> grade;

    for(int idx = 0; idx < futureRowCount; idx++)
        grade.insert(this->studentsModel->student(idx)->key().id, idx);

    // filter
    for(auto mark : marks)
    {
        auto keyIterator = grade.find(mark->student().id);

        if(keyIterator == grade.end() or not this->filterMark(mark))
        {
            delete mark;
            continue;
        }

        int targetRow = keyIterator.value();
        int targetColumn = this->rangeBegin.daysTo(mark->date());

        this->marksTable[targetRow][targetColumn] = mark;
    }

    this->endResetModel();
    return {};
}

UserError JournalModel::store(const QModelIndex& index, int value, const dbapi::Person::Key &teacher)
{
    assert((void("index validity"), index.isValid()));
    assert((void("index validity"), index.row() < this->_rowCount));
    assert((void("index validity"), index.column() < this->_columnCount));

    dbapi::StudentMark* & cell = this->marksTable[index.row()][index.column()];

    assert((void("cell freedom"), not cell));

    dbapi::StudentMark mark(this->connection);
    mark.setStudent(this->studentsModel->student(index.row())->key());
    mark.setDate(this->rangeBegin.addDays(index.column()));
    mark.setMark(value);
    mark.setTeacher(teacher);
    mark.setSubject(this->subjectKey);
    mark.setType(this->type);

    if(not mark.store())
        return UserError::internalError("Mark", "be craeted 'cause something went wrong", "Try again or contact support");

    cell = new dbapi::StudentMark(mark);
    emit this->dataChanged(index, index, {Qt::DisplayRole});

    return {};
}

UserError JournalModel::remove(const QItemSelection& selection)
{
    this->connection->transaction();
    dbapi::ApiError error;

    for(auto index : selection.indexes())
    {
        assert((void("index validity"), index.isValid()));
        assert((void("index validity"), index.row() < this->_rowCount));
        assert((void("index validity"), index.column() < this->_columnCount));

        auto mark = this->marksTable[index.row()][index.column()];

        if(not mark)
            continue;

        if(not mark->remove())
        {
            error = mark->error();
            break;
        }
    }

    if(error.type != dbapi::ApiError::NoError)
    {
        this->connection->rollback();
        return UserError::internalError("Marks", "be removed 'cause something went wrong", "Try again or contact support");
    }

    this->connection->commit();

    this->beginResetModel();

    for(auto index : selection.indexes())
    {
        auto& mark = this->marksTable[index.row()][index.column()];

        if(not mark)
            continue;

        delete mark;
        mark = nullptr;
    }

    this->endResetModel();

    return {};
}

dbapi::StudentMark *JournalModel::mark(const QModelIndex &index)
{
    assert((void("index validity"), index.isValid()));
    assert((void("index validity"), index.row() < this->_rowCount));
    assert((void("index validity"), index.column() < this->_columnCount));

    return this->marksTable[index.row()][index.column()];
}

dbapi::StudentMark *JournalModel::mark(int row, int column)
{
    assert((void("index validity"), row < this->_rowCount));
    assert((void("index validity"), column < this->_columnCount));
    assert((void("index validity"), row >= 0 and column >= 0));

    return this->marksTable[row][column];
}

int JournalModel::rowCount(const QModelIndex &parent) const
{
    return this->_rowCount;
}

int JournalModel::columnCount(const QModelIndex &parent) const
{
    return this->_columnCount;
}

QVariant JournalModel::data(const QModelIndex &index, int role) const
{
    if(role != Qt::DisplayRole)
        return {};

    if(not index.isValid())
        return {};

    if(index.row() >= this->_rowCount)
        return {};

    if(index.column() >= this->_columnCount)
        return {};

    auto mark = this->marksTable[index.row()][index.column()];

    if(not mark)
        return {};

    return mark->mark();
}

QVariant JournalModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if(role != Qt::DisplayRole)
        return {};

    switch(orientation)
    {
    case Qt::Horizontal:
        return this->horizontalHeader(section);
    case Qt::Vertical:
        return this->verticalHeader(section);
    }

    return {};
}

JournalModel::~JournalModel()
{
    this->freeAll();
}

bool JournalModel::filterMark(dbapi::StudentMark *mark)
{
    if(mark->subject() != this->subjectKey) return false;

    if(mark->type() != this->type) 			return false;

    if(mark->date() >= this->rangeEnd or mark->date() < this->rangeBegin) return false;

    return true;
}

void JournalModel::initTable(int rows, int columns)
{
    this->marksTable.clear();

    this->marksTable.resize(rows);

    for(auto& row : this->marksTable)
        row.resize(columns);

    this->_rowCount = rows;
    this->_columnCount = columns;
}

void JournalModel::freeAll()
{
    for(auto& row : this->marksTable)
        for(auto mark : row)
            if(mark) delete mark;
}

QVariant JournalModel::verticalHeader(int row) const
{
    if(row < 0 or row >= this->_rowCount)
        return {};

    auto student = this->studentsModel->student(row);

    return QString("%1 %2").arg(student->firstName(), student->secondName());
}

QVariant JournalModel::horizontalHeader(int column) const
{
    if(column < 0 or column >= this->_columnCount)
        return {};

    return this->rangeBegin.addDays(column).toString("dd/MM/yy");
}
