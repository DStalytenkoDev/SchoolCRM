#ifndef PERSONCARDCONCEPT_H
#define PERSONCARDCONCEPT_H

#include <QLabel>
#include <QLineEdit>
#include <QDateEdit>
#include <QTextEdit>
#include <QHBoxLayout>
#include <QToolBar>
#include "../ChooseAndFindEntities/ChooseAndFindRole.h"


class PersonCardConcept : public QWidget
{
    Q_OBJECT

public:
    PersonCardConcept(QWidget* parent = nullptr) noexcept;

protected:
    void swapUi() noexcept;

    void setToolBar(QToolBar* toolBar) noexcept;
    void setFirstNameWidget(QWidget* firstName) noexcept;
    void setSecondNameWidget(QWidget* secondName) noexcept;
    void setRolesWidget(ChooseAndFindRole* roles) noexcept;

    QDateEdit* dateWidget() const noexcept;
    QTextEdit* descriptionWidget() const noexcept;

private:
    void initUi() noexcept;

    // opened state
    QHBoxLayout* mainBigLayout = nullptr;
    QLabel* fakePicture;
    QVBoxLayout* subLayout;

    QHBoxLayout* primaryControlsLayout;
    QWidget* firstName;
    QWidget* secondName;
    QToolBar* toolBar;

    QHBoxLayout* secondairyControlsLayout;
    ChooseAndFindRole* roleChooser;
    QDateEdit* dateChooser;

    QTextEdit* description;

    // closed state
    QHBoxLayout* mainLittleLayout;
    QLabel* personName;

    bool opened = false;
};


#endif // PERSONCARDCONCEPT_H
