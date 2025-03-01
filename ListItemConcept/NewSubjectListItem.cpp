#include "NewSubjectListItem.h"
#include <QMessageBox>


NewSubjectListItem::NewSubjectListItem(dbapi::Connection* connection, QWidget* parent) noexcept
    : ListItemConcept(parent),
    subject(connection)
{
    this->lineEditOfSubjectName = new QLineEdit(this);
    this->toolBar = new MenuOfNewListItem(this);

    this->setRightToolBar(this->toolBar);
    this->getLeftLayout()->addWidget(this->lineEditOfSubjectName);

    connect(this->toolBar, &MenuOfNewListItem::loadUpClicked, this, &NewSubjectListItem::tryStore);
    connect(this->toolBar, &MenuOfNewListItem::cancelClicked, this, &NewSubjectListItem::canceled);
}

void NewSubjectListItem::tryStore()
{
    this->subject.setName(this->lineEditOfSubjectName->text());

    auto message = QMessageBox(this);
    message.setStandardButtons(QMessageBox::Retry | QMessageBox::Abort | QMessageBox::Ok);
    message.setDefaultButton(QMessageBox::Abort);
    message.setIcon(QMessageBox::Critical);
    message.setText("Error while trying to store new subject");
    message.setDetailedText("Wanna try again?");

    while(true)
    {
        if(this->subject.store())
        {
            emit this->stored(&this->subject);
            return;
        }

        int res = message.exec();

        switch(res)
        {
        case QMessageBox::Retry:
            break;
        case QMessageBox::Abort:
            emit this->canceled();
            return;
        case QMessageBox::Ok:
            return;
        default:
            return;
        };
    }
}
