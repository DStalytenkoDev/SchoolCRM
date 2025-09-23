#include "PersonWidget.h"
#include "ui_PersonWidget.h"


PersonWidget::PersonWidget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::PersonWidget)
{
    ui->setupUi(this);

    this->roleView = new ComboBoxFinderView(this);
    this->ui->roleLayout->addWidget(this->roleView);
}

QString PersonWidget::firstName()
{
    return this->ui->firstName->text();
}

QString PersonWidget::secondName()
{
    return this->ui->secondName->text();
}

QModelIndex PersonWidget::role()
{
    int index = this->roleView->currentIndex();

    return this->roleView->model()->index(index, 0);
}

QDate PersonWidget::date()
{
    return this->ui->date->date();
}

void PersonWidget::setFirstName(const QString &name)
{
    this->ui->firstName->setText(name);
}

void PersonWidget::setSecondName(const QString &name)
{
    this->ui->secondName->setText(name);
}

void PersonWidget::setDate(const QDate &date)
{
    this->ui->date->setDate(date);
}

void PersonWidget::setRole(const QModelIndex &role)
{
    this->roleView->setCurrentIndex(role.row());
}

void PersonWidget::setRoles(QAbstractItemModel *roles)
{
    this->roleView->setModel(roles);
}

void PersonWidget::clear()
{
    this->ui->firstName->clear();
    this->ui->secondName->clear();
    this->ui->date->clear();
    this->ui->description->clear();
}

PersonWidget::~PersonWidget()
{
    delete ui;
}

QHBoxLayout *PersonWidget::menuLayout()
{
    return this->ui->menuLayout;
}
