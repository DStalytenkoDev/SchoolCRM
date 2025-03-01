#ifndef REGULARSUBJECTLISTITEM_H
#define REGULARSUBJECTLISTITEM_H


#include "ListItemConcept.h"
#include "MenuOfRegularListItem.h"
#include <SchoolDatabaseApi/Subject/Subject.h>
#include <QLabel>


class RegularSubjectListItem : public ListItemConcept
{
    Q_OBJECT

public:
    RegularSubjectListItem(dbapi::Subject* subject, QWidget* parent = nullptr) noexcept;
    ~RegularSubjectListItem();

signals:
    void removed();

private:
    dbapi::Subject* subject;

    QLabel* title;
    MenuOfRegularListItem* toolBar;

    void tryRemove();
};

#endif // REGULARSUBJECTLISTITEM_H
