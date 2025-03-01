#include "SmartEdit.h"

SmartEdit::SmartEdit(QWidget *parent)
    : QWidget(parent)
{
    this->labelButton = new QPushButton(this);
    this->editField = new QLineEdit(this);
    this->layout = new QHBoxLayout(this);

    this->layout->addWidget(this->labelButton);
    this->layout->addWidget(this->editField);
    this->setLayout(this->layout);

    this->editField->hide();

    connect(this->labelButton, &QPushButton::clicked, this, &SmartEdit::swapUi);
    connect(this->editField, &QLineEdit::editingFinished, this, &SmartEdit::swapUi);
}

void SmartEdit::setText(const QString &text)
{
    if(this->stateSolid)
        this->labelButton->setText(text);
    else
        this->editField->setText(text);
}

QString SmartEdit::text() const
{
    return this->labelButton->text();
}

void SmartEdit::swapUi()
{
    this->stateSolid = not this->stateSolid;

    if(this->stateSolid)
    {
        this->labelButton->setText(this->editField->text());

        this->editField->hide();
        this->labelButton->setHidden(false);
    }
    else
    {
        this->editField->setText(this->labelButton->text());

        this->labelButton->hide();
        this->editField->setHidden(false);
    }
}
