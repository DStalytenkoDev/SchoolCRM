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

dbapi::ApiError JournalModel::store(const QModelIndex& index, int value)
{
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

    this->rowCount = rows;
    this->columnCount = columns;
}

void JournalModel::freeAll()
{
    for(auto& row : this->marksTable)
        for(auto mark : row)
            if(mark) delete mark;

    for(auto student : this->students)
        delete student;
}
