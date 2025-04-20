#include "ShowPerson.h"

ShowPerson::ShowPerson(QWidget *parent) : PersonWidget(parent)
{
    this->updateBtn = new QPushButton("Update", this);

    this->menuLayout()->addWidget(this->updateBtn);

    connect(this->updateBtn, &QPushButton::clicked, this, &ShowPerson::updatePerson);
}
