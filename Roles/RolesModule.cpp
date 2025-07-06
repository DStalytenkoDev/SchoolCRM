#include "RolesModule.h"
#include "ui_RolesModule.h"
#include <QMessageBox>

RolesModule::RolesModule(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::RolesModule)
{
    ui->setupUi(this);

    this->ui->deleteRole->hide();

    this->model = new RolesModel(this);

    this->setupRoleFinder();
    this->setupRolesList();

    // handle deleting
    connect(this->ui->deleteRole, &QPushButton::clicked, this, &RolesModule::handleRoleDeletion);

    // handle creation
    connect(this->ui->addRole, &QPushButton::clicked, this, &RolesModule::initRoleCreation);

    // handle load all
    connect(this->ui->loadAllButton, &QPushButton::clicked, this, &RolesModule::handleRolesLoading);
}

void RolesModule::setupRoleFinder()
{
    this->roleFinder = new ComboBoxFinderView(this);

    auto layout = this->ui->menuLayout->replaceWidget(this->ui->searchRole, this->roleFinder);
    this->ui->searchRole->hide();

    delete layout;

    this->roleFinder->setModel(this->model);

    // handle searching
    connect(this->roleFinder, &ComboBoxFinderView::foundItem, this, &RolesModule::handleFoundRole);
}

void RolesModule::setupRolesList()
{
    this->ui->rolesList->setModel(this->model);
    this->ui->rolesList->setSelectionBehavior(QAbstractItemView::SelectRows);
    this->ui->rolesList->setSelectionMode(QAbstractItemView::SingleSelection);

    // handle selection
    connect(this->ui->rolesList->selectionModel(), &QItemSelectionModel::selectionChanged, this, &RolesModule::handleSelectedRole);
}

bool RolesModule::tryConnect()
{
    if(this->connection->open())
        return true;

    UserError::connectionError("Connection", "be established 'cause something went wrong", "Check credentials or internet connection, server might be down").show(this);

    return false;
}

void RolesModule::setConnection(dbapi::Connection *connection)
{
    this->connection = connection;
    this->model->setConnection(connection);
}

RolesModule::~RolesModule()
{
    delete ui;
}

void RolesModule::handleFoundRole(QModelIndex index)
{
    this->ui->rolesList->scrollTo(index);
}

void RolesModule::handleSelectedRole()
{
    auto& selection = this->ui->rolesList->selectionModel()->selection();

    if(selection.empty())
        this->ui->deleteRole->hide();
    else
        this->ui->deleteRole->show();
}

void RolesModule::handleRoleDeletion()
{
    if(not this->tryConnect())
        return;

    auto indexes = this->ui->rolesList->selectionModel()->selectedIndexes();

    auto error = this->model->removeRole(indexes.front().row());

    if(error.isError())
        return error.show(this);

    QMessageBox::information(this, "Info", "All selected roles have been deleted");

    this->connection->close();
}

void RolesModule::initRoleCreation()
{
    if(this->roleCreationDialog)
        delete this->roleCreationDialog;

    this->roleCreationDialog = new RoleCreationDialog(this);
    this->roleCreationDialog->show();

    connect(this->roleCreationDialog, &QDialog::finished, this, &RolesModule::completeRoleCreation);
}

void RolesModule::completeRoleCreation()
{
    if(this->roleCreationDialog->result() == QDialog::Rejected)
        return;

    if(not this->tryConnect())
        return;

    auto error = this->model->createRole(this->roleCreationDialog->roleName());
    this->connection->close();

    if(error.isError())
        return error.show(this);

    QMessageBox::information(this, "Info", "Role is created");
}

void RolesModule::handleRolesLoading()
{
    if(not this->tryConnect())
        return;

    auto error = this->model->loadAll();
    this->connection->close();

    if(error.isError())
        return error.show(this);
}
