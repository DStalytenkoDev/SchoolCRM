#include <SchoolApi/Subject.h>
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
    connect(this->ui->deleteSubject, &QPushButton::clicked, this, &SubjectsModule::handleSubjectDeletion);

    // handle creation
    connect(this->ui->createSubject, &QPushButton::clicked, this, &SubjectsModule::initSubjectCreation);

    // handle load all
    connect(this->ui->loadAllButton, &QPushButton::clicked, this, &SubjectsModule::handleSubjectsLoading);
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

void SubjectsModule::handleSelectedSubject()
{
    auto& selection = this->ui->subjectsList->selectionModel()->selection();

    if(selection.empty())
        this->ui->deleteSubject->hide();
    else
        this->ui->deleteSubject->show();
}

void SubjectsModule::handleSubjectDeletion()
{
    if(not this->tryConnect())
        return;

    auto indexes = this->ui->subjectsList->selectionModel()->selectedIndexes();

    auto error = this->model->removeSubject(indexes.front().row());

    if(error.isError())
        return error.show(this);

    QMessageBox::information(this, "Info", "All selected subjects have been deleted");

    this->connection->close();
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

    if(not this->tryConnect())
        return;

    auto error = this->model->createSubject(this->subjectCreationDialog->subject());
    this->connection->close();

    if(error.isError())
        return error.show(this);

    QMessageBox::information(this, "Info", "Subject is created");
}

void SubjectsModule::handleSubjectsLoading()
{
    if(not this->tryConnect())
        return;

    auto error = this->model->loadAll();
    this->connection->close();

    if(error.isError())
        return error.show(this);
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
    this->ui->subjectsList->setSelectionMode(QAbstractItemView::SingleSelection);

    // handle selection
    connect(this->ui->subjectsList->selectionModel(), &QItemSelectionModel::selectionChanged, this, &SubjectsModule::handleSelectedSubject);
}

bool SubjectsModule::tryConnect()
{
    if(this->connection->open())
        return true;

    UserError::connectionError("Connection", "be established 'cause something went wrong", "Check credentials or internet connection, server might be down").show(this);

    return false;
}
