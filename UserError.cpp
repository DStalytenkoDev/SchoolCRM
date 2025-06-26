#include "UserError.h"

UserError::UserError(const QString &shortText, Type type) : _title("Error"), _shortText(shortText), _type(type) {}

UserError::UserError(const QString &title, const QString &shortText, Type type) : _title(title), _shortText(shortText), _type(type) {}

UserError::UserError(const QString &title, const QString &shortText, const QString &longText, Type type) : _title(title), _shortText(shortText), _longText(longText), _type(type) {}

bool UserError::isError() const { return this->_type != None; }

const QString &UserError::shortText() const { return this->_shortText; }

const QString &UserError::longText() const { return this->_longText; }

const QString &UserError::title() const { return this->_title; }

UserError::Type UserError::type() const { return this->_type; }

void UserError::show(QWidget *parent) const
{
    auto box = this->createMessageBox(parent);

    box->show();

    delete box;
}

QMessageBox *UserError::formMessage(QWidget *parent) const
{
    return this->createMessageBox(parent);
}

QMessageBox *UserError::createMessageBox(QWidget *parent) const
{
    auto box = new QMessageBox(parent);

    box->setText(this->_title.isEmpty() ? this->_title : "Error");
    box->setInformativeText(this->_shortText);
    box->setDetailedText(this->_longText);

    switch(this->_type)
    {
    case None:
        box->setIcon(QMessageBox::Information);
    case Warning:
        box->setIcon(QMessageBox::Warning);
    case Critical:
        box->setIcon(QMessageBox::Critical);
    }

    return box;
}
