#include "RegularSubjectListItem.h"
#include <QMessageBox>

RegularSubjectListItem::RegularSubjectListItem(dbapi::Subject* subject, QWidget* parent) noexcept
    : ListItemConcept(parent),
    subject(subject)
{
    this->title = new QLabel(this);
    this->title->setText(this->subject->name());
    this->getLeftLayout()->addWidget(this->title);

    this->toolBar = new MenuOfRegularListItem(this);
    this->setRightToolBar(this->toolBar);

    connect(this->toolBar, &MenuOfRegularListItem::removeClicked, this, &RegularSubjectListItem::tryRemove);
}

RegularSubjectListItem::~RegularSubjectListItem()
{
    delete this->subject;
}

void RegularSubjectListItem::tryRemove()
{
    auto message = QMessageBox(this);
    message.setStandardButtons(QMessageBox::Ok);
    message.setDefaultButton(QMessageBox::Ok);
    message.setIcon(QMessageBox::Critical);
    message.setText("Error while tryna remove the subject");

    if(not this->subject->remove())
    {
        message.exec();
        return;
    }

    emit this->removed();
    this->setEnabled(false);
}
