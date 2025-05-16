#include "JournalModel.h"

JournalModel::JournalModel(QObject *parent)
    : QAbstractTableModel{parent}
{}

void JournalModel::setConnection(dbapi::Connection *connection)
{
    this->connection = connection;
}

void JournalModel::setClass(const dbapi::Class::Key& key)
{
    this->classKey = key;
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
        this->rangeEnd = end.addDays(1);
    else
        this->rangeBegin = end;
}

dbapi::ApiError JournalModel::load()
{
    dbapi::ApiError error;
    auto students = this->loadStudents(&error);

    if(error.type != dbapi::ApiError::NoError)
        return error;

    auto marks = dbapi::StudentMark::loadAll(this->connection, &error);

    if(error.type != dbapi::ApiError::NoError)
    {
        for(auto student : students) delete student;
        return error;
    }

    this->beginResetModel();
    this->freeAll();

    int futureColumnCount = this->rangeBegin.daysTo(this->rangeEnd);
    int futureRowCount = students.size();

    this->initTable(futureRowCount, futureColumnCount);

    QMap<int, int> grade;

    for(int idx = 0; idx < students.size(); idx++)
        grade.insert(students[idx]->key().id, idx);

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

    this->students = std::move(students);
    this->endResetModel();
    return {};
}

dbapi::ApiError JournalModel::store(const QModelIndex& index, int value, const dbapi::Person::Key &teacher)
{
    if(not index.isValid())
        return {dbapi::ApiError::KeyError, "inavlid index in Journal::store()"};

    if(index.row() >= this->_rowCount)
        return {dbapi::ApiError::KeyError, "inavlid index in Journal::store()"};

    if(index.column() >= this->_columnCount)
        return {dbapi::ApiError::KeyError, "inavlid index in Journal::store()"};

    dbapi::StudentMark* & cell = this->marksTable[index.row()][index.column()];

    if(cell)
        return {dbapi::ApiError::KeyError, "mark already exists Journal::store()"};

    dbapi::StudentMark mark(this->connection);
    mark.setStudent(this->students[index.row()]->key());
    mark.setDate(this->rangeBegin.addDays(index.column()));
    mark.setMark(value);
    mark.setTeacher(teacher);
    mark.setSubject(this->subjectKey);
    mark.setType(this->type);

    if(not mark.store())
        return mark.error();

    cell = new dbapi::StudentMark(mark);
    emit this->dataChanged(index, index, {Qt::DisplayRole});

    return {};
}

dbapi::ApiError JournalModel::remove(const QModelIndex &topLeft, const QModelIndex &bottomRight)
{
    if(not topLeft.isValid() or not bottomRight.isValid())
         return {dbapi::ApiError::KeyError, "invalid indexes in Journal::remove()"};

    if(topLeft.row() >= this->_rowCount)
        return {dbapi::ApiError::KeyError, "invalid indexes in Journal::remove()"};

    if(topLeft.column() >= this->_columnCount)
        return {dbapi::ApiError::KeyError, "invalid indexes in Journal::remove()"};

    if(bottomRight.row() >= this->_rowCount)
        return {dbapi::ApiError::KeyError, "invalid indexes in Journal::remove()"};

    if(bottomRight.column() >= this->_columnCount)
        return {dbapi::ApiError::KeyError, "invalid indexes in Journal::remove()"};

    if(bottomRight.row() < topLeft.row())
        return {dbapi::ApiError::KeyError, "invalid indexes in Journal::remove()"};

    if(bottomRight.column() < topLeft.column())
        return {dbapi::ApiError::KeyError, "invalid indexes in Journal::remove()"};

    this->connection->transaction();
    dbapi::ApiError error;

    for(int row = topLeft.row(); row <= bottomRight.row(); row++)
    {
        for(int column = topLeft.column(); column <= bottomRight.column(); column++)
        {
            auto mark = this->marksTable[row][column];

            if(not mark)
                continue;

            if(not mark->remove())
            {
                error = mark->error();
                break;
            }
        }
    }

    if(error.type != dbapi::ApiError::NoError)
    {
        this->connection->rollback();
        return error;
    }

    this->connection->commit();

    this->beginResetModel();

    for(int row = topLeft.row(); row <= bottomRight.row(); row++)
    {
        for(int column = topLeft.column(); column <= bottomRight.column(); column++)
        {
            auto& mark = this->marksTable[row][column];

            if(not mark)
                continue;

            delete mark;
            mark = nullptr;
        }
    }

    this->endResetModel();

    return {};
}

dbapi::StudentMark *JournalModel::mark(const QModelIndex &index)
{
    if(index.isValid())
        return nullptr;

    if(index.row() >= this->_rowCount)
        return nullptr;

    if(index.column() >= this->_columnCount)
        return nullptr;

    return this->marksTable[index.row()][index.column()];
}

dbapi::StudentMark *JournalModel::mark(int row, int column)
{
    if(row >= this->_rowCount)
        return nullptr;

    if(column >= this->_columnCount)
        return nullptr;

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

    if(index.isValid())
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

QList<dbapi::Person*> JournalModel::loadStudents(dbapi::ApiError* error)
{
    auto students = dbapi::Student::loadAll(this->connection, error);

    if(error->type != dbapi::ApiError::NoError)
        return {};

    QList<dbapi::Person*> persons;
    persons.reserve(100);

    for(auto student : students)
    {
        if(student->grade() == this->classKey)
        {
            auto person = new dbapi::Person(student->key().person, this->connection);

            if(not person->load())
            {
                delete person;
                delete student;
                continue;
            }

            persons.append(person);
        }

        delete student;
    }

    persons.squeeze();
    return std::move(persons);
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

    for(auto student : this->students)
        delete student;
}

QVariant JournalModel::verticalHeader(int row) const
{
    if(row < 0 or row >= this->_columnCount)
        return {};

    return this->rangeBegin.addDays(row).toString();
}

QVariant JournalModel::horizontalHeader(int column) const
{
    if(column < 0 or column >= this->_columnCount)
        return {};

    auto student = this->students[column];

    return QString("%1 %2").arg(student->firstName(), student->secondName());
}
