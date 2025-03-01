#include "NewTeacherSubjectListItem.h"
#include <QMessageBox>

NewTeacherSubjectListItem::NewTeacherSubjectListItem(dbapi::Connection* connection, const dbapi::TeacherSubjectsList::Key& teacherSubjectsListKey, QWidget* parent) noexcept
    : ListItemConcept(parent),
    subjects(teacherSubjectsListKey, connection)
{
    this->toolBar = new MenuOfNewListItem(this);
    this->choosenSubject = new ChooseAndFindSubject(connection, this);

    this->getLeftLayout()->addWidget(this->choosenSubject);

    connect(this->toolBar, &MenuOfNewListItem::loadUpClicked, this, &NewTeacherSubjectListItem::tryStore);
    connect(this->toolBar, &MenuOfNewListItem::cancelClicked, this, &NewTeacherSubjectListItem::canceled);
}

dbapi::ApiError NewTeacherSubjectListItem::sync() noexcept
{
    return this->choosenSubject->sync();
}

void NewTeacherSubjectListItem::tryStore()
{
    if(not this->choosenSubject->currentSubject())
    {
        auto message = QMessageBox(this);
        message.setStandardButtons(QMessageBox::Ok);
        message.setDefaultButton(QMessageBox::Ok);
        message.setIcon(QMessageBox::Warning);
        message.setText("please choose subject to be added");
        message.exec();
        return;
    }

    this->subjects.appendSubject(this->choosenSubject->currentSubject()->key());

    auto message = QMessageBox(this);
    message.setStandardButtons(QMessageBox::Retry | QMessageBox::Abort | QMessageBox::Ok);
    message.setDefaultButton(QMessageBox::Abort);
    message.setIcon(QMessageBox::Critical);
    message.setText("Error while trying to add a subject to list of a teacher");
    message.setDetailedText("Wanna try again?");

    while(true)
    {
        if(this->subjects.update())
        {
            emit NewTeacherSubjectListItem::stored(this->choosenSubject->currentSubject());
            return;
        }

        int res = message.exec();

        switch(res)
        {
        case QMessageBox::Retry:
            break;
        case QMessageBox::Abort:
            emit NewTeacherSubjectListItem::canceled();
            return;
        case QMessageBox::Ok:
            return;
        default:
            return;
        };
    }
}
