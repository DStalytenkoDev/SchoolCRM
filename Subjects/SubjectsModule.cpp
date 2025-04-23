#include "SubjectsModule.h"
#include "ui_SubjectsModule.h"

SubjectsModule::SubjectsModule(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::SubjectsModule)
{
    ui->setupUi(this);
}

SubjectsModule::~SubjectsModule()
{
    delete ui;
}
