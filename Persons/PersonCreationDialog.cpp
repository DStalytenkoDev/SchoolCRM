#include "PersonCreationDialog.h"


PersonCreationDialog::PersonCreationDialog(QWidget *parent) : PersonWidget(parent)
{
    this->createBtn = new QPushButton("Create", this);
    this->cancelBtn = new QPushButton("Cancel", this);

    this->menuLayout()->addWidget(this->createBtn);
    this->menuLayout()->addWidget(this->cancelBtn);

    connect(this->cancelBtn, &QPushButton::clicked, this, [this](){
        emit this->finished(QDialog::Rejected);
    });

    connect(this->createBtn, &QPushButton::clicked, this, [this](){
        emit this->finished(QDialog::Accepted);
    });

    this->setWindowModality(Qt::WindowModal);
}
