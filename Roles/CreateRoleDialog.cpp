#include "CreateRoleDialog.h"
#include "ui_CreateRoleDialog.h"

CreateRoleDialog::CreateRoleDialog(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::CreateRoleDialog)
{
    ui->setupUi(this);
}

CreateRoleDialog::~CreateRoleDialog()
{
    delete ui;
}
