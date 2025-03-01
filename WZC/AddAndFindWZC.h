#ifndef ADDANDFINDWZC_H
#define ADDANDFINDWZC_H

#include "WorkZoneConcept.h"
#include "../ChooseAndFindEntities/ChooseAndFindConcept.h"

class AddAndFindWZC : public WorkZoneConcept
{
    Q_OBJECT

public:
    AddAndFindWZC(QWidget* parent = nullptr);

protected:
    ChooseAndFindConcept* getFinder() const;

signals:
    void addButtonClicked();

private:
    ChooseAndFindConcept* finder;
    QAction* addAction;
};

#endif // ADDANDFINDWZC_H
