#include "PersonCardConcept.h"
#include <QPixmap>
#include <QSpacerItem>

PersonCardConcept::PersonCardConcept(QWidget* parent) noexcept
{
    this->setLayout(new QHBoxLayout(this));

    this->mainLittleLayout = new QHBoxLayout(this);
    this->personName = new QLabel(this);
    this->toolBar = new QToolBar(this);

    this->mainLittleLayout->addWidget(this->personName);
    this->mainLittleLayout->addWidget(this->toolBar);
    this->layout()->addItem(this->mainLittleLayout);
}

void PersonCardConcept::swapUi() noexcept
{
    this->layout()->removeWidget(this->toolBar);

    if(this->opened)
    {
        this->layout()->removeItem(this->mainBigLayout);
        this->layout()->addItem(this->mainLittleLayout);

        this->mainLittleLayout->addWidget(this->toolBar);
    }
    else
    {
        this->layout()->removeItem(this->mainLittleLayout);
        this->layout()->addItem(this->mainBigLayout);

        this->primaryControlsLayout->addWidget(this->toolBar);
    }

    opened = not opened;
}

void PersonCardConcept::setToolBar(QToolBar *toolBar) noexcept
{
    this->layout()->replaceWidget(this->toolBar, toolBar);

    delete this->toolBar;

    this->toolBar = toolBar;
}

void PersonCardConcept::setFirstNameWidget(QWidget *firstName) noexcept
{
    this->layout()->replaceWidget(this->firstName, firstName);

    delete this->firstName;

    this->firstName = firstName;
}

void PersonCardConcept::setSecondNameWidget(QWidget *secondName) noexcept
{
    this->layout()->replaceWidget(this->secondName, secondName);

    delete this->secondName;

    this->secondName = secondName;
}

void PersonCardConcept::setRolesWidget(ChooseAndFindRole *roles) noexcept
{
    this->layout()->replaceWidget(this->roleChooser, roles);

    delete this->roleChooser;

    this->roleChooser = roles;
}

QDateEdit *PersonCardConcept::dateWidget() const noexcept
{
    return this->dateChooser;
}

QTextEdit *PersonCardConcept::descriptionWidget() const noexcept
{
    return this->description;
}

void PersonCardConcept::initUi() noexcept
{
    this->mainBigLayout = new QHBoxLayout(this);

    this->fakePicture = new QLabel(this);
    this->fakePicture->setStyleSheet("background: grey;");

    this->subLayout = new QVBoxLayout(this);

    this->mainBigLayout->addWidget(this->fakePicture);
    this->mainBigLayout->addLayout(this->subLayout);

    this->primaryControlsLayout = new QHBoxLayout(this);

    this->firstName = new QLabel(this);
    this->secondName = new QLabel(this);

    this->toolBar = new QToolBar(this);

    this->primaryControlsLayout->addWidget(this->firstName);
    this->primaryControlsLayout->addWidget(this->secondName);
    this->primaryControlsLayout->addSpacerItem(new QSpacerItem(20,
                                                               10,
                                                               QSizePolicy::Fixed,
                                                               QSizePolicy::Ignored));

    this->subLayout->addLayout(this->primaryControlsLayout);

    this->secondairyControlsLayout = new QHBoxLayout(this);

    this->roleChooser = new ChooseAndFindRole((dbapi::Connection*)nullptr, this);
    this->dateChooser = new QDateEdit(this);

    this->secondairyControlsLayout->addWidget(this->roleChooser);
    this->secondairyControlsLayout->addWidget(this->dateChooser);
    this->secondairyControlsLayout->addSpacerItem(new QSpacerItem(20,
                                                                  10,
                                                                  QSizePolicy::Fixed,
                                                                  QSizePolicy::Ignored));

    this->subLayout->addLayout(this->secondairyControlsLayout);

    this->description = new QTextEdit(this);

    this->subLayout->addWidget(this->description);
}
