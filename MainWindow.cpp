#include "MainWindow.h"
#include "./ui_MainWindow.h"

#include "AuthorizationDialog.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);


    auto dialog = new AuthorizationDialog(this);
    dialog->exec();
}

MainWindow::~MainWindow()
{
    delete ui;
}
