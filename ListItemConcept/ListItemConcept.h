#ifndef LISTITEMCONCEPT_H
#define LISTITEMCONCEPT_H

#include <QWidget>
#include <QHBoxLayout>
#include <QToolBar>

class ListItemConcept : public QWidget
{
    Q_OBJECT

public:
    ListItemConcept(QWidget* parent = nullptr) noexcept;

protected:
    void setLeftLayout(QHBoxLayout* layout) noexcept;
    QHBoxLayout* getLeftLayout() const noexcept;

    void setRightToolBar(QToolBar* toolBar) noexcept;
    QToolBar* getRightToolBar() const noexcept;

private:
    QHBoxLayout* mainLayout = nullptr;
    QHBoxLayout* leftLayout = nullptr;
    QToolBar* toolBar = nullptr;
};

#endif // LISTITEMCONCEPT_H
