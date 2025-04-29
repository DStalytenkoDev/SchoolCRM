#include "ClassCreationDialog.h"
#include "ui_ClassCreationDialog.h"


ClassCreationDialog::ClassCreationDialog(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::ClassCreationDialog)
{
    ui->setupUi(this);

    this->teacherFinder = new ComboBoxFinderView(this);

    this->ui->teacherLayout->addWidget(this->teacherFinder);

    this->ui->teacherLayout->setStretch(0, 1);
    this->ui->teacherLayout->setStretch(1, 2);
}

QString ClassCreationDialog::name() const
{
    return this->ui->className->text();
}

QModelIndex ClassCreationDialog::currentIndex() const
{
    return this->teacherFinder->model()->index(this->teacherFinder->currentIndex(), 0);
}

void ClassCreationDialog::setModel(QAbstractItemModel *model)
{
    this->teacherFinder->setModel(model);
}

ClassCreationDialog::~ClassCreationDialog()
{
    delete ui;
}
