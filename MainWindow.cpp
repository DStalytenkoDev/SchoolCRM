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
    this->persons = new PersonsModule(this);
    this->persons->hide();
    this->persons->setConnection(&this->connection);

    // roles
    this->roles = new RolesModule(this);
    this->roles->hide();
    this->roles->setConnection(&this->connection);

    // subjects
    this->subjects = new SubjectsModule(this);
    this->subjects->hide();
    this->subjects->setConnection(&this->connection);

    this->subjectsOfTeacherModule = new SubjectsOfTeacherModule(this);
    this->subjectsOfTeacherModule->hide();
    this->subjectsOfTeacherModule->setConnection(&this->connection);

    // classes
    this->classes = new ClassesModule(this);
    this->classes->hide();
    this->classes->setConnection(&this->connection);

    // journal
    this->journal = new JournalModule(this);
    this->journal->hide();
    this->journal->setConnection(&this->connection);

    // home page (installed by the designer)
    this->lastModule = this->ui->homeFrame;
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::manageLeftBarActions(QTreeWidgetItem *item, int column)
{
    auto text = item->text(0);

    if(text == "Authorization")
        this->initAuthorization();

    if(text == "Persons")
    {
        this->swapMainWidget(this->persons);
        this->persons->prepare();
    }

    if(text == "Roles")
        this->swapMainWidget(this->roles);

    if(text == "Subjects")
        this->swapMainWidget(this->subjects);

    if(text == "Subjects of teacher")
    {
        this->swapMainWidget(this->subjectsOfTeacherModule);
        this->subjectsOfTeacherModule->prepare();
    }

    if(text == "Classes")
    {
        this->swapMainWidget(this->classes);
        this->classes->prepare();
    }

    if(text == "Journal")
    {
        this->swapMainWidget(this->journal);
        this->journal->prepare();
    }
}

void MainWindow::initAuthorization()
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

    this->ui->statusbar->showMessage("Connecting in progress...");

    bool opened = this->connection.open();

    this->ui->statusbar->clearMessage();

    if(not opened)
    {
        this->ui->statusbar->showMessage("Connecting failed");
        return;
    }

    this->ui->statusbar->showMessage("Server is connected");
    this->connection.close();
}

void MainWindow::swapMainWidget(QWidget *newWidget)
{
    if(newWidget == nullptr)
        newWidget = this->ui->homeFrame;

    if(this->lastModule)
        this->lastModule->hide();

    QLayoutItem* item = this->ui->centralLayout->replaceWidget(this->lastModule, newWidget);

    if(item)
        delete item;

    newWidget->setHidden(false);
    this->lastModule = newWidget;
}
