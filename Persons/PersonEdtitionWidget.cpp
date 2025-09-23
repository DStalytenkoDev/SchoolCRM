#include "PersonEdtitionWidget.h"

PersonEditionWidget::PersonEditionWidget(QWidget *parent) : PersonWidget(parent)
{
    this->saveBtn = new QPushButton("Save", this);
    this->abortButton = new QPushButton("Abort", this);

    this->menuLayout()->addWidget(this->saveBtn);
    this->menuLayout()->addWidget(this->abortButton);

    connect(this->saveBtn, &QPushButton::clicked, this, &PersonEditionWidget::saveButtonClicked);
    connect(this->abortButton, &QPushButton::clicked, this, &PersonEditionWidget::abortButtonClicked);
}
