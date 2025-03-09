#include "PersonsWidget.h"
#include "ui_PersonsWidget.h"

PersonsWidget::PersonsWidget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::PersonsWidget)
{
    ui->setupUi(this);
}

void PersonsWidget::setConnection(dbapi::Connection *connection)
{
    this->connection = connection;
}

PersonsWidget::~PersonsWidget()
{
    delete ui;
}
