#include <QMessageBox>
#include <QTimer>

#include "MainWindow.h"
#include "./ui_MainWindow.h"
#include "AuthorizationDialog.h"


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // autorization dialog
    this->authorizationDialog = new AuthorizationDialog(this);
    this->authorizationDialog->setTrustedConnection(true);
    connect(this->authorizationDialog, &QDialog::finished, this, &MainWindow::completeAuthorization);

    // left menu
    connect(this->ui->treeWidget, &QTreeWidget::itemClicked, this, &MainWindow::manageLeftBarActions);

    // persons
    this->persons = new PersonsWidget(this);
    this->persons->hide();
    this->persons->setConnection(&this->connection);

    // home page (installed by the designer)
    this->lastMainWidget = this->ui->homeFrame;
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::manageLeftBarActions(QTreeWidgetItem *item, int column)
{
    auto text = item->text(0);

    if(text == "Authorization")
        this->showAuthorizationDialog();

    if(text == "Persons")
        this->swapMainWidget(this->persons);
}

void MainWindow::showAuthorizationDialog()
{
    this->authorizationDialog->show();
}

void MainWindow::completeAuthorization(int code)
{
    if(code == QDialog::Rejected)
        return;

    this->connection.setDatabaseName(this->authorizationDialog->databaseName());
    this->connection.setServerName(this->authorizationDialog->serverName());

    this->connection.setTrustedConnection(this->authorizationDialog->isTrustedConnection());

    if(this->authorizationDialog->isTrustedConnection())
    {
        this->connection.setUserName(this->authorizationDialog->userName());
        this->connection.setPassword(this->authorizationDialog->password());
    }

    emit this->ui->statusbar->showMessage("Connecting in progress...");

    bool opened = this->connection.open();

    this->ui->statusbar->clearMessage();

    if(not opened)
    {
        emit this->ui->statusbar->showMessage("Connecting failed");
        return;
    }

    emit this->ui->statusbar->showMessage("Server is connected");
    this->connection.close();
}

void MainWindow::swapMainWidget(QWidget *newWidget)
{
    if(newWidget == nullptr)
        newWidget = this->ui->homeFrame;

    if(this->lastMainWidget)
        this->lastMainWidget->hide();

    QLayoutItem* item = this->ui->centralLayout->replaceWidget(this->lastMainWidget, newWidget);

    if(item)
        delete item;

    newWidget->setHidden(false);
    this->lastMainWidget = newWidget;
}
