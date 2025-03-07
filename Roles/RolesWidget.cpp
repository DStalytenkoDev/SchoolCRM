#include "RolesWidget.h"
#include "ui_RolesWidget.h"

RolesWidget::RolesWidget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::RolesWidget)
{
    ui->setupUi(this);
}

RolesWidget::~RolesWidget()
{
    delete ui;
}
