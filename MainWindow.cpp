#include <QMessageBox>
#include "Menu/AboutDialog.h"
#include "MainWindow.h"
#include "./ui_MainWindow.h"


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->setWindowTitle("School CRM");

    this->aboutDialog = new AboutDialog(this);

    // menu
    connect(this->ui->actionAbout, &QAction::triggered, this, &MainWindow::showAbout);
    connect(this->ui->actionHelp, &QAction::triggered, this, &MainWindow::showHelp);
    connect(this->ui->actionExit, &QAction::triggered, this, &MainWindow::close);

    // autorization dialog
    this->authorizationWindow = new AuthorizationWindow(this);

    // left menu
    connect(this->ui->treeWidget, &QTreeWidget::itemClicked, this, &MainWindow::manageLeftBarActions);

    // persons
    this->persons = new PersonsModule(this);
    this->persons->hide();
    this->persons->setConnection(this->authorizationWindow->getConnection());

    // roles
    this->roles = new RolesModule(this);
    this->roles->hide();
    this->roles->setConnection(this->authorizationWindow->getConnection());

    // subjects
    this->subjects = new SubjectsModule(this);
    this->subjects->hide();
    this->subjects->setConnection(this->authorizationWindow->getConnection());

    this->subjectsOfTeacherModule = new SubjectsOfTeacherModule(this);
    this->subjectsOfTeacherModule->hide();
    this->subjectsOfTeacherModule->setConnection(this->authorizationWindow->getConnection());

    // classes
    this->classes = new ClassesModule(this);
    this->classes->hide();
    this->classes->setConnection(this->authorizationWindow->getConnection());

    // journal
    this->journal = new JournalModule(this);
    this->journal->hide();
    this->journal->setConnection(this->authorizationWindow->getConnection());

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
    this->authorizationWindow->show();
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

void MainWindow::showAbout()
{
    this->aboutDialog->show();
}

void MainWindow::showHelp()
{
    QMessageBox::information(this, "Help", "Some help eg. link to git issues...");
}
