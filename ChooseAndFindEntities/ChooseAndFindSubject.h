#ifndef CHOOSEANDFINDSUBJECT_H
#define CHOOSEANDFINDSUBJECT_H


#include "ChooseAndFindConcept.h"
#include <SchoolDatabaseApi/Subject/Subject.h>
#include <QSharedPointer>


class ChooseAndFindSubject : public ChooseAndFindConcept
{
    Q_OBJECT

public:
    ChooseAndFindSubject(dbapi::Connection* connection, QWidget* parent = nullptr) noexcept;

    dbapi::Subject* currentSubject() const noexcept;
    dbapi::ApiError sync() noexcept;

    ~ChooseAndFindSubject();

signals:
    void currentSubjectChanged(dbapi::Subject* subject);

private:
    dbapi::Connection* connection;
    QVector<dbapi::Subject*> subjects;

    void indexChanging(int index);
    void cleanItemsAndSubjects();
};


#endif // CHOOSEANDFINDSUBJECT_H
