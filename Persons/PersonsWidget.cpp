#include "PersonsWidget.h"
#include "ui_PersonsWidget.h"

PersonsWidget::PersonsWidget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::PersonsWidget)
{
    ui->setupUi(this);
}

PersonsWidget::~PersonsWidget()
{
    delete ui;
}
