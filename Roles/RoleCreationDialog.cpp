#include "RoleCreationDialog.h"
#include "ui_RoleCreationDialog.h"

RoleCreationDialog::RoleCreationDialog(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::RoleCreationDialog)
{
    ui->setupUi(this);
}

QString RoleCreationDialog::roleName() const
{
    return this->ui->lineEdit->text();
}

RoleCreationDialog::~RoleCreationDialog()
{
    delete ui;
}
