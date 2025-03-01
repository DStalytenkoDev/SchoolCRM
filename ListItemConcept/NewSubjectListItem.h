#ifndef NEWSUBJECTLISTITEM_H
#define NEWSUBJECTLISTITEM_H

#include "ListItemConcept.h"
#include "MenuOfNewListItem.h"
#include <SchoolDatabaseApi/Subject/Subject.h>
#include <QLineEdit>

class NewSubjectListItem : public ListItemConcept
{
    Q_OBJECT

public:
    NewSubjectListItem(dbapi::Connection* connection, QWidget* parent = nullptr) noexcept;

signals:
    void canceled();
    void stored(dbapi::Subject* subject);

private:
    dbapi::Subject subject;

    QLineEdit* lineEditOfSubjectName;
    MenuOfNewListItem* toolBar;

    void tryStore();
};

#endif // NEWSUBJECTLISTITEM_H
