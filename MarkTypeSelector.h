#ifndef MARKTYPESELECTOR_H
#define MARKTYPESELECTOR_H

#include <QComboBox>
#include <SchoolDatabaseApi/StudentMark/StudentMark.h>

class MarkTypeSelector : public QComboBox
{
    Q_OBJECT

public:
    MarkTypeSelector(QWidget* parent);

    void setType(dbapi::StudentMark::Type type);
    dbapi::StudentMark::Type getType();

signals:
    void typeChanged(dbapi::StudentMark::Type type);

private:
    void indexChanging();
};

#endif // MARKTYPESELECTOR_H
