#ifndef CHOOSEANDFINDSTUDENT_H
#define CHOOSEANDFINDSTUDENT_H

#include "ChooseAndFindConcept.h"
#include <SchoolDatabaseApi/Student/Student.h>
#include <QSharedPointer>


class ChooseAndFindStudent : public ChooseAndFindConcept
{
    Q_OBJECT

public:
    ChooseAndFindStudent(dbapi::Connection* connection, dbapi::Class::Key classKey, QWidget* parent = nullptr);

    dbapi::Student* currentStudent() const noexcept;
    dbapi::ApiError sync() noexcept;

    ~ChooseAndFindStudent();

signals:
    void currentStudentChanged(dbapi::Student* student);

private:
    dbapi::Connection* connection;
    QVector<dbapi::Student*> students;
    dbapi::Class::Key classKey;

    void indexChanging(int index);
    void cleanItemsAndStudents();
};


#endif // CHOOSEANDFINDSTUDENT_H
