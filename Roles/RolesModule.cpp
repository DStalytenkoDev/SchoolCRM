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
    this->ui->rolesList->setSelectionMode(QAbstractItemView::ExtendedSelection);

    // handle selection
    connect(this->ui->rolesList->selectionModel(), &QItemSelectionModel::selectionChanged, this, &RolesModule::handleSelectedRoles);
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

void RolesModule::handleSelectedRoles()
{
    auto& selection = this->ui->rolesList->selectionModel()->selection();

    if(selection.empty())
        this->ui->deleteRole->hide();
    else
        this->ui->deleteRole->show();
}

void RolesModule::handleRoleDeletion()
{
    if(not this->connection->open())
    {
        QMessageBox::critical(this, "Internal error",
                              QString("Deletion could not be performed due to: %1").arg(this->connection->error().text));
        return;
    }

    bool errorFlag = false;

    for(QModelIndex& index : this->ui->rolesList->selectionModel()->selectedIndexes())
    {
        if(not this->model->role(index)->remove()) // API calling for deletion
            errorFlag = true; // flag any request failed

        this->ui->rolesList->setRowHidden(index.row(), true); // hide deleted role
    }

    if(errorFlag)
    {
        QString msg("Deletion of these roles is failed:\n");

        for(QModelIndex& index : this->ui->rolesList->selectionModel()->selectedIndexes())
        {
            auto role = this->model->role(index);

            if(role->error().type != dbapi::ApiError::NoError)
            {
                msg.append(QString("%1 %2\n").arg(role->name(), role->error().text));
                this->ui->rolesList->setRowHidden(index.row(), false); // unhide not deleted roles
            }
        }

        QMessageBox::warning(this, "Deletion", msg);
    }
    else
        QMessageBox::information(this, "Deletion", "All selected roles had been deleted");

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

    if(not this->connection->open())
    {
        QMessageBox::critical(this, "Internal error",
                              QString("Creation could not be performed due to: %1").arg(this->connection->error().text));
        return;
    }

    dbapi::Role role(this->connection);

    role.setName(this->roleCreationDialog->roleName());

    if(not role.store())
    {
        QMessageBox::warning(this, "Creation",
                            QString("The creation of the new role: %1 is failed due to: %2").arg(role.name(), role.error().text));

        this->connection->close();
        return;
    }

    this->connection->close();

    this->model->insertRow(this->model->rowCount(), role);
    QMessageBox::information(this, "Creation", QString("The creation of the new role: %1 is succeed").arg(role.name()));
}

void RolesModule::handleRolesLoading()
{
    if(not this->connection->open())
    {
        QMessageBox::critical(this, "Internal error",
                              QString("Loading could not be performed due to: %1").arg(this->connection->error().text));
        return;
    }

    this->model->clear();
    auto error = this->model->loadAll();

    if(error.type != dbapi::ApiError::NoError)
        QMessageBox::warning(this, "Loading", QString("Loading of all roles is failed due to: %1").arg(error.text));

    this->connection->close();
}
