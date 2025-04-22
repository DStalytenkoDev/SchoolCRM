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

    connect(this->ui->rolesList->selectionModel(), &QItemSelectionModel::selectionChanged, this, &RolesWidget::handleSelectedRoles);
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
        }

        QMessageBox::warning(this, "Deletion", msg);
    }
    else
        QMessageBox::information(this, "Deletion", "All selected roles had been deleted");
}

void RolesWidget::initRoleCreation()
{
    this->roleCreatationDialog = new CreateRoleDialog(this);
    this->roleCreatationDialog->show();

    connect(this->roleCreatationDialog, &QDialog::finished, this, &RolesWidget::completeRoleCreation);
}

void RolesWidget::completeRoleCreation()
{
    dbapi::Role role(this->connection);

    role.setName(this->roleCreatationDialog->roleName());

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
