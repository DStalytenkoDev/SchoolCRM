#ifndef CHOOSEANDFINDCLASS_H
#define CHOOSEANDFINDCLASS_H

#include <SchoolDatabaseApi/Class/Class.h>
#include "ChooseAndFindConcept.h"


class ChooseAndFindClass : public ChooseAndFindConcept
{
    Q_OBJECT

public:
    ChooseAndFindClass(dbapi::Connection* connection, QWidget* parent = nullptr) noexcept;

    dbapi::Class* currentClass() const noexcept;
    dbapi::ApiError sync() noexcept;

    ~ChooseAndFindClass();

signals:
    void currentClassChanged(dbapi::Class* newClass);

private:
    dbapi::Connection* connection;
    QVector<dbapi::Class*> classes;

    void indexChanging(int index);
    void cleanItemsAndClasses();
};


#endif // CHOOSEANDFINDCLASS_H
