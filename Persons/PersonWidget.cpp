#include "PersonWidget.h"
#include "ui_PersonWidget.h"

PersonWidget::PersonWidget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::PersonWidget)
{
    ui->setupUi(this);
}

PersonWidget::~PersonWidget()
{
    delete ui;
}
