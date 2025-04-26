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

    this->model = new SubjectsModel(this);

    this->setupSubjectFinder();
    this->setupSubjectList();

    // handle deleting
    connect(this->ui->deleteSubject, &QPushButton::clicked, this, &SubjectsModule::deleteSubject);

    // handle creation
    connect(this->ui->createSubject, &QPushButton::clicked, this, &SubjectsModule::initSubjectCreation);

    // handle load all
    connect(this->ui->loadAllButton, &QPushButton::clicked, this, &SubjectsModule::loadSubjects);
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
    if(not this->connection->open())
    {
        QMessageBox::critical(this, "Internal error",
                              QString("Deletion could not be performed due to: %1").arg(this->connection->error().text));
        return;
    }

    bool errorFlag = false;

    for(QModelIndex& index : this->selectedSubjects.indexes())
    {
        if(not this->model->subject(index)->remove()) // API calling for deletion
            errorFlag = true; // flag any request failed

        this->ui->subjectsList->setRowHidden(index.row(), true); // hide deleted subject
    }

    if(errorFlag)
    {
        QString msg("Deletion of these subjects is failed:\n");

        for(QModelIndex& index : this->selectedSubjects.indexes())
        {
            auto subject = this->model->subject(index);

            if(subject->error().type != dbapi::ApiError::NoError)
            {
                msg.append(QString("%1 %2\n").arg(subject->name(), subject->error().text));
                this->ui->subjectsList->setRowHidden(index.row(), false); // unhide not deleted subjects
            }
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

    if(not this->connection->open())
    {
        QMessageBox::critical(this, "Internal error",
                              QString("Creation could not be performed due to: %1").arg(this->connection->error().text));
        return;
    }

    dbapi::Subject subject(this->connection);

    subject.setName(this->subjectCreationDialog->subject());

    if(not subject.store())
    {
        QMessageBox::warning(this, "Creation",
                            QString("The creation of the new subject: %1 is failed due to: %2").arg(subject.name(), subject.error().text));

        this->connection->close();
        return;
    }

    this->connection->close();

    this->model->insertRow(this->model->rowCount(), subject);
    QMessageBox::information(this, "Creation", QString("The creation of the new subject: %1 is succeed").arg(subject.name()));
}

void SubjectsModule::loadSubjects()
{
    if(not this->connection->open())
    {
        QMessageBox::critical(this, "Internal error",
                              QString("Loading could not be performed due to: %1").arg(this->connection->error().text));
        return;
    }

    auto error = this->model->loadAll();

    if(error.type != dbapi::ApiError::NoError)
        QMessageBox::warning(this, "Loading", QString("Loading of all subjects is failed due to: %1").arg(error.text));

    this->connection->close();
}

void SubjectsModule::setupSubjectFinder()
{
    this->subjectFinder = new ComboBoxFinderView(this);

    auto layout = this->ui->menuLayout->replaceWidget(this->ui->lineEdit, this->subjectFinder);
    this->ui->lineEdit->hide();

    delete layout;

    this->subjectFinder->setModel(this->model);

    // handle searching
    connect(this->subjectFinder, &ComboBoxFinderView::foundItem, this, &SubjectsModule::handleFoundSubject);
}

void SubjectsModule::setupSubjectList()
{
    this->ui->subjectsList->setModel(this->model);
    this->ui->subjectsList->setSelectionBehavior(QAbstractItemView::SelectRows);
    this->ui->subjectsList->setSelectionMode(QAbstractItemView::MultiSelection);

    // handle selection
    connect(this->ui->subjectsList->selectionModel(), &QItemSelectionModel::selectionChanged, this, &SubjectsModule::handleSelectedSubjects);

    // handle subject clicking
    connect(this->ui->subjectsList, &QListView::clicked, this, &SubjectsModule::handleClickedSubject);
}
