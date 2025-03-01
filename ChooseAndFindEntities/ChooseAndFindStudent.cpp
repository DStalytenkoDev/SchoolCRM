#include "ChooseAndFindStudent.h"

ChooseAndFindStudent::ChooseAndFindStudent(dbapi::Connection* connection, dbapi::Class::Key classKey, QWidget* parent)
    : ChooseAndFindConcept(parent),
    connection(connection),
    classKey(classKey)
{
    connect(this, &ChooseAndFindStudent::currentIndexChanged, this, &ChooseAndFindStudent::indexChanging);
}

dbapi::ApiError ChooseAndFindStudent::sync() noexcept
{
    this->cleanItemsAndStudents();

    dbapi::ApiError error;
    auto tempStudents = dbapi::Student::loadAll(connection, &error);

    if(error.type != dbapi::ApiError::NoError)
        return error;

    this->students.reserve(tempStudents.size());

    for(auto student : tempStudents)
    {
        if(student->grade() == this->classKey)
            this->students.push_back(student);
        else
            delete student;
    }

    this->students.squeeze();

    for(auto student : this->students)
    {
        dbapi::Person personOfStudent(student->key().person, this->connection);
        bool success = personOfStudent.loadFirstName() and personOfStudent.loadSecondName();

        if(not success)
        {
            this->cleanItemsAndStudents();
            return personOfStudent.error();
        }

        QString fullName = QString("%1 %2").arg(personOfStudent.firstName(), personOfStudent.secondName());
        this->addItem(fullName);
    }

    return {};
}

ChooseAndFindStudent::~ChooseAndFindStudent()
{
    this->cleanItemsAndStudents();
}

dbapi::Student* ChooseAndFindStudent::currentStudent() const noexcept
{
    return this->students[this->currentIndex()];
}

void ChooseAndFindStudent::indexChanging(int index)
{
    emit this->currentStudentChanged(this->students[this->currentIndex()]);
}

void ChooseAndFindStudent::cleanItemsAndStudents()
{
    this->clear();
    for(auto student : this->students)
        delete student;
}
