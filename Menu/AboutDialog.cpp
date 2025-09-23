#include "AboutDialog.h"
#include "ui_AboutDialog.h"

AboutDialog::AboutDialog(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::AboutDialog)
{
    ui->setupUi(this);

    this->setWindowTitle("About");
    this->setWindowModality(Qt::ApplicationModal);
    this->setWindowFlag(Qt::Window);
    this->hide();
}

AboutDialog::~AboutDialog()
{
    delete ui;
}
