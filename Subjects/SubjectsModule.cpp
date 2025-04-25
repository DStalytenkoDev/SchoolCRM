#include <SchoolDatabaseApi/Subject/Subject.h>
#include <QMessageBox>
#include "SubjectsModule.h"
#include "ui_SubjectsModule.h"


SubjectsModule::SubjectsModule(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::SubjectsModule)
{
    ui->setupUi(this);

    this->ui->deleteSubject->hide();

    this->subjectFinder = new ComboBoxFinderView(this);

    auto layout = this->ui->menuLayout->replaceWidget(this->ui->lineEdit, this->subjectFinder);
    this->ui->lineEdit->hide();

    delete layout;

    this->model = new SubjectsModel(this);

    this->ui->subjectsList->setModel(this->model);
    this->subjectFinder->setModel(this->model);

    // handle selection
    connect(this->ui->subjectsList->selectionModel(), &QItemSelectionModel::selectionChanged, this, &SubjectsModule::handleSelectedSubjects);

    // handle searching
    connect(this->subjectFinder, &ComboBoxFinderView::foundItem, this, &SubjectsModule::handleFoundSubject);

    // handle subject clicking
    connect(this->ui->subjectsList, &QListView::clicked, this, &SubjectsModule::handleClickedSubject);

    // handle deleting
    connect(this->ui->deleteSubject, &QPushButton::clicked, this, &SubjectsModule::deleteSubject);

    // handle creation
    connect(this->ui->createSubject, &QPushButton::clicked, this, &SubjectsModule::initSubjectCreation);
}

void SubjectsModule::setConnection(dbapi::Connection *connection)
{
    this->connection = connection;
    this->model->setConnection(connection);
}

SubjectsModule::~SubjectsModule()
{
    delete ui;
}

void SubjectsModule::handleFoundSubject(QModelIndex index)
{
    this->ui->subjectsList->scrollTo(index);
}

void SubjectsModule::handleClickedSubject(const QModelIndex &index)
{
    this->selectedSubjects.clear();
    this->selectedSubjects.select(index, index);

    this->ui->deleteSubject->setHidden(false);
}

void SubjectsModule::handleSelectedSubjects(const QItemSelection &selected, const QItemSelection &deselected)
{
    this->selectedSubjects = selected;

    if(selected.empty())
        this->ui->deleteSubject->hide();
    else
        this->ui->deleteSubject->setHidden(false);
}

void SubjectsModule::deleteSubject()
{
    bool errorFlag = false;

    for(QModelIndex& index : this->selectedSubjects.indexes())
        if(not this->model->subject(index)->remove()) // API calling for deletion
            errorFlag = true; // flag any request failed

    if(errorFlag)    {
        QString msg("Deletion of these subjects is failed:\n");

        for(QModelIndex& index : this->selectedSubjects.indexes())
        {
            auto subject = this->model->subject(index);

            if(subject->error().type != dbapi::ApiError::NoError)
                msg.append(QString("%1 %2\n").arg(subject->name(), subject->error().text));
            else
                this->ui->subjectsList->setRowHidden(index.row(), true); // hide deleted subjects
        }

        QMessageBox::warning(this, "Deletion", msg);
    }
    else
        QMessageBox::information(this, "Deletion", "All selected subjects had been deleted"); // report a success
}

void SubjectsModule::initSubjectCreation()
{
    if(this->subjectCreationDialog)
        delete this->subjectCreationDialog;

    this->subjectCreationDialog = new CreateSubjectDialog(this);
    this->subjectCreationDialog->show();

    connect(this->subjectCreationDialog, &QDialog::finished, this, &SubjectsModule::completeSubjectCreation);
}

void SubjectsModule::completeSubjectCreation()
{
    if(this->subjectCreationDialog->result() == QDialog::Rejected)
        return;

    dbapi::Subject subject(this->connection);

    subject.setName(this->subjectCreationDialog->subject());

    if(not subject.store())
    {
        QMessageBox::warning(this, "Creation",
                            QString("The creation of the new subject: %1 is failed due to: %2").arg(subject.name(), subject.error().text));
        return;
    }

    this->model->insertRow(this->model->rowCount(), subject);
    QMessageBox::information(this, "Creation", QString("The creation of the new subject: %1 is succeed").arg(subject.name()));
}

void SubjectsModule::loadSubjects()
{
    auto error = this->model->loadAll();

    if(error.type != dbapi::ApiError::NoError)
        QMessageBox::warning(this, "Loading", QString("Loading of all subjects is failed due to: %1").arg(error.text));
}
