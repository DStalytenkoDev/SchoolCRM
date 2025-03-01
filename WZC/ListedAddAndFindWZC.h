#ifndef LISTEDADDANDFINDWZC_H
#define LISTEDADDANDFINDWZC_H

#include <QListWidget>
#include "AddAndFindWZC.h"

class ListedAddAndFindWZC : public AddAndFindWZC
{
    Q_OBJECT

public:
    ListedAddAndFindWZC(QWidget* parent = nullptr);

protected:
    QListWidget* getListWidget() const;

private:
    QListWidget* listWidget;
};

#endif // LISTEDADDANDFINDWZC_H
