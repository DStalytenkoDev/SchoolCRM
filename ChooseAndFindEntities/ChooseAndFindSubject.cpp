#include "ChooseAndFindSubject.h"

ChooseAndFindSubject::ChooseAndFindSubject(dbapi::Connection* connection, QWidget* parent) noexcept
    : ChooseAndFindConcept(parent),
    connection(connection)
{
    connect(this, &ChooseAndFindSubject::currentIndexChanged, this, &ChooseAndFindSubject::indexChanging);
}

dbapi::ApiError ChooseAndFindSubject::sync() noexcept
{
    this->cleanItemsAndSubjects();

    dbapi::ApiError error;
    this->subjects = dbapi::Subject::loadAll(connection, &error);

    if(error.type != dbapi::ApiError::NoError)
        return error;

    for(auto subject : this->subjects)
        this->addItem(subject->name());

    return {};
}

ChooseAndFindSubject::~ChooseAndFindSubject()
{
    this->cleanItemsAndSubjects();
}

dbapi::Subject* ChooseAndFindSubject::currentSubject() const noexcept
{
    return this->subjects[this->currentIndex()];
}

void ChooseAndFindSubject::indexChanging(int index)
{
    emit this->currentSubjectChanged(this->subjects[this->currentIndex()]);
}

void ChooseAndFindSubject::cleanItemsAndSubjects()
{
    this->clear();
    for(auto subject : this->subjects)
        delete subject;
}

