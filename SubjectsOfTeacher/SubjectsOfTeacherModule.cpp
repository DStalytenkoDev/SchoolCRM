#include "SubjectsOfTeacherModule.h"
#include "ui_SubjectsOfTeacherModule.h"

SubjectsOfTeacherModule::SubjectsOfTeacherModule(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::SubjectsOfTeacherModule)
{
    ui->setupUi(this);
}

SubjectsOfTeacherModule::~SubjectsOfTeacherModule()
{
    delete ui;
}
