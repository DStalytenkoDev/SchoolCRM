#ifndef NEWTEACHERSUBJECTLISTITEM_H
#define NEWTEACHERSUBJECTLISTITEM_H

#include "ListItemConcept.h"
#include "MenuOfNewListItem.h"
#include <SchoolDatabaseApi/TeacherSubjectsList/TeacherSubjectsList.h>
#include "../ChooseAndFindEntities/ChooseAndFindSubject.h"

class NewTeacherSubjectListItem : public ListItemConcept
{
    Q_OBJECT

public:
    NewTeacherSubjectListItem(dbapi::Connection* connection, const dbapi::TeacherSubjectsList::Key& teacherSubjectsListKey, QWidget* parent = nullptr) noexcept;

    dbapi::ApiError sync() noexcept;

signals:
    void canceled();
    void stored(dbapi::Subject* subject);

private:
    dbapi::TeacherSubjectsList subjects;

    ChooseAndFindSubject* choosenSubject;
    MenuOfNewListItem* toolBar;

    void tryStore();
};

#endif // NEWTEACHERSUBJECTLISTITEM_H
