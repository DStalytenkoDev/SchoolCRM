#include "ClassesModule.h"
#include "ui_ClassesModule.h"

ClassesModule::ClassesModule(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::ClassesModule)
{
    ui->setupUi(this);
}

ClassesModule::~ClassesModule()
{
    delete ui;
}
