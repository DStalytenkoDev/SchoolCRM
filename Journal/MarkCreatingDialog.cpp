#include "MarkCreatingDialog.h"
#include "ui_MarkCreatingDialog.h"

MarkCreatingDialog::MarkCreatingDialog(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::MarkCreatingDialog)
{
    ui->setupUi(this);

    this->teacherFinder = new ComboBoxFinderView(this);

    this->ui->verticalLayout->insertWidget(0, this->teacherFinder);
}

void MarkCreatingDialog::setTeachersModel(QAbstractItemModel *model)
{
    this->teacherFinder->setModel(model);
}

QModelIndex MarkCreatingDialog::currentTeacher()
{
    return this->teacherFinder->model()->index(this->teacherFinder->currentIndex(), 0);
}

int MarkCreatingDialog::value()
{
    return this->ui->markValue->text().toInt();
}

void MarkCreatingDialog::clear()
{
    this->teacherFinder->setCurrentIndex(-1);
    this->ui->markValue->clear();
}

MarkCreatingDialog::~MarkCreatingDialog()
{
    delete ui;
}
