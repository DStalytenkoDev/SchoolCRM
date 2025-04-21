#include "CreateRoleDialog.h"
#include "ui_CreateRoleDialog.h"

CreateRoleDialog::CreateRoleDialog(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::CreateRoleDialog)
{
    ui->setupUi(this);
}

QString CreateRoleDialog::roleName() const
{
    return this->ui->lineEdit->text();
}

CreateRoleDialog::~CreateRoleDialog()
{
    delete ui;
}
