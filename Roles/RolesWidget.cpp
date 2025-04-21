#include "RolesWidget.h"
#include "ui_RolesWidget.h"

RolesWidget::RolesWidget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::RolesWidget)
{
    ui->setupUi(this);

    this->ui->deleteRole->hide();

    this->roleCreatationDialog = new CreateRoleDialog(this);
    this->roleFinder = new ComboBoxFinderView(this);

    auto layout = this->ui->menuLayout->replaceWidget(this->ui->searchRole, this->roleFinder);
    this->ui->searchRole->hide();

    delete layout;

    this->model = new RolesModel(this);

    this->ui->rolesList->setModel(this->model);
    this->roleFinder->setModel(this->model);
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
}

void RolesWidget::handleClickedRole(const QModelIndex &index)
{
    this->selectedRoles.clear();
    this->selectedRoles.select(index, index);
}

void RolesWidget::deleteRole()
{

}

void RolesWidget::createRole()
{

}

void RolesWidget::loadRoles()
{

}
