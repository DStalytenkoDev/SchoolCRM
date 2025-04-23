#include "CreateSubjectDialog.h"
#include "ui_CreateSubjectDialog.h"

CreateSubjectDialog::CreateSubjectDialog(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::CreateSubjectDialog)
{
    ui->setupUi(this);
}

QString CreateSubjectDialog::subject() const
{
    return this->ui->lineEdit->text();
}

CreateSubjectDialog::~CreateSubjectDialog()
{
    delete ui;
}
