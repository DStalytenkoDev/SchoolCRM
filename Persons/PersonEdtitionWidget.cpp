#include "PersonEdtitionWidget.h"

PersonEditionWidget::PersonEditionWidget(QWidget *parent) : PersonWidget(parent)
{
    this->saveBtn = new QPushButton("Save", this);
    this->deleteBtn = new QPushButton("Delete", this);
    this->abortButton = new QPushButton("Abort", this);

    this->menuLayout()->addWidget(this->saveBtn);
    this->menuLayout()->addWidget(this->deleteBtn);
    this->menuLayout()->addWidget(this->abortButton);

    connect(this->saveBtn, &QPushButton::clicked, this, &PersonEditionWidget::saveButtonClicked);
    connect(this->deleteBtn, &QPushButton::clicked, this, &PersonEditionWidget::deleteButtonClicked);
    connect(this->abortButton, &QPushButton::clicked, this, &PersonEditionWidget::abortButtonClicked);
}
