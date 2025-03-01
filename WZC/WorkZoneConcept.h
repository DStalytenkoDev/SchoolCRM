#ifndef WORKZONECONCEPT_H
#define WORKZONECONCEPT_H


#include <QWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QToolBar>


class WorkZoneConcept : public QWidget
{
    Q_OBJECT

public:
    WorkZoneConcept(QWidget* parent = nullptr);

protected:
    void setLayout(QLayout* layout);
    QLayout* layout() const;

    QToolBar* getLeftToolBar() const;
    QToolBar* getRightToolBar() const;

private:
    QVBoxLayout* mainLayout;
    QHBoxLayout* toolBarLayout;

    QToolBar* leftToolBar;
    QToolBar* rightToolBar;

    QLayout* childLayout;
};

#endif // WORKZONECONCEPT_H
