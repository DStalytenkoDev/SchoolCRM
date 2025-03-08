#include "CreatePersonDialog.h"


CreatePersonDialog::CreatePersonDialog(QWidget *parent) : PersonWidget(parent)
{
    this->createBtn = new QPushButton("Create", this);
    this->cancelBtn = new QPushButton("Cancel", this);

    this->menuLayout()->addWidget(this->createBtn);
    this->menuLayout()->addWidget(this->cancelBtn);

    connect(this->cancelBtn, &QPushButton::clicked, this, &CreatePersonDialog::canceled);
    connect(this->createBtn, &QPushButton::clicked, this, &CreatePersonDialog::created);

    this->setWindowFlag(Qt::WindowType::Dialog);
}
