#include "RolesWidget.h"
#include "ui_RolesWidget.h"
#include <qmessagebox.h>

RolesWidget::RolesWidget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::RolesWidget)
{
    ui->setupUi(this);

    this->ui->deleteRole->hide();

    this->roleFinder = new ComboBoxFinderView(this);

    auto layout = this->ui->menuLayout->replaceWidget(this->ui->searchRole, this->roleFinder);
    this->ui->searchRole->hide();

    delete layout;

    this->model = new RolesModel(this);

    this->ui->rolesList->setModel(this->model);
    this->roleFinder->setModel(this->model);

    // handle selection
    connect(this->ui->rolesList->selectionModel(), &QItemSelectionModel::selectionChanged, this, &RolesWidget::handleSelectedRoles);

    // handle searching
    connect(this->roleFinder, &ComboBoxFinderView::foundItem, this, &RolesWidget::handleFoundRole);

    // handle role clicking
    connect(this->ui->rolesList, &QListView::clicked, this, &RolesWidget::handleClickedRole);

    // handle deleting
    connect(this->ui->deleteRole, &QPushButton::clicked, this, &RolesWidget::deleteRole);

    // handle creation
    connect(this->ui->addRole, &QPushButton::clicked, this, &RolesWidget::initRoleCreation);
}

void RolesWidget::setConnection(dbapi::Connection *connection)
{
    this->connection = connection;
    this->model->setConnection(connection);
}

RolesWidget::~RolesWidget()
{
    delete ui;
}

void RolesWidget::handleFoundRole(QModelIndex index)
{
    this->ui->rolesList->scrollTo(index);
}

void RolesWidget::handleSelectedRoles(const QItemSelection &selected, const QItemSelection &deselected)
{
    this->selectedRoles = selected;

    if(selected.empty())
        this->ui->deleteRole->hide();
    else
        this->ui->deleteRole->setHidden(false);
}

void RolesWidget::handleClickedRole(const QModelIndex &index)
{
    this->selectedRoles.clear();
    this->selectedRoles.select(index, index);

    this->ui->deleteRole->setHidden(false);
}

void RolesWidget::deleteRole()
{
    bool errorFlag = false;

    for(QModelIndex& index : this->selectedRoles.indexes())
        if(not this->model->role(index)->remove()) // API calling for deletion
            errorFlag = true; // flag any request failed

    if(errorFlag)
    {
        QString msg("Deletion of these roles is failed:\n");

        for(QModelIndex& index : this->selectedRoles.indexes())
        {
            auto role = this->model->role(index);

            if(role->error().type != dbapi::ApiError::NoError)
                msg.append(QString("%1 %2\n").arg(role->name(), role->error().text));
            else
                this->ui->rolesList->setRowHidden(index.row(), true); // hide deleted roles
        }

        QMessageBox::warning(this, "Deletion", msg);
    }
    else
        QMessageBox::information(this, "Deletion", "All selected roles had been deleted");
}

void RolesWidget::initRoleCreation()
{
    if(this->roleCreationDialog)
        delete this->roleCreationDialog;

    this->roleCreationDialog = new CreateRoleDialog(this);
    this->roleCreationDialog->show();

    connect(this->roleCreationDialog, &QDialog::finished, this, &RolesWidget::completeRoleCreation);
}

void RolesWidget::completeRoleCreation()
{
    if(this->roleCreationDialog->result() == QDialog::Rejected)
        return;

    dbapi::Role role(this->connection);

    role.setName(this->roleCreationDialog->roleName());

    if(not role.store())
    {
        QMessageBox::warning(this, "Creation",
                            QString("The creation of the new role: %1 is failed due to: %2").arg(role.name(), role.error().text));
        return;
    }

    this->model->insertRow(this->model->rowCount(), role);
    QMessageBox::information(this, "Creation", QString("The creation of the new role: %1 is succeed").arg(role.name()));
}

void RolesWidget::loadRoles()
{
    auto error = this->model->loadAll();

    if(error.type != dbapi::ApiError::NoError)
        QMessageBox::warning(this, "Loading", QString("Loading of all roles is failed due to: %1").arg(error.text));
}
