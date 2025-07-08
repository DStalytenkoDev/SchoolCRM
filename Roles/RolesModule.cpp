#include <QMessageBox>
#include "RolesModule.h"
#include "ui_RolesModule.h"
#include "TransitionFactrory.h"

RolesModule::RolesModule(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::RolesModule)
{
    ui->setupUi(this);

    this->model = new RolesModel(this);
    this->proxyModel = new QSortFilterProxyModel(this);

    this->setupRoleFinder();
    this->setupRolesList();
    this->setupStateMachine();
}

void RolesModule::setupRoleFinder()
{
    this->proxyModel->setSourceModel(this->model);
    this->proxyModel->setFilterCaseSensitivity(Qt::CaseInsensitive);
    this->proxyModel->setFilterKeyColumn(0);
}

void RolesModule::setupRolesList()
{
    this->ui->rolesList->setModel(this->model);
    this->ui->rolesList->setSelectionBehavior(QAbstractItemView::SelectRows);
    this->ui->rolesList->setSelectionMode(QAbstractItemView::SingleSelection);
}

void RolesModule::setupStateMachine()
{
    this->stateMachine = new QStateMachine(this);

    this->rolesLoaded = new QState(this->stateMachine);
    this->rolesNotLoaded = new QState(this->stateMachine);
    this->itemSelected = new QState(this->stateMachine);
    this->searching = new QState(this->stateMachine);

    this->stateMachine->setInitialState(this->rolesNotLoaded);

    connect(this->rolesNotLoaded, &QState::entered, this, &RolesModule::enterRolesNotLoaded);
    this->rolesNotLoaded->addTransition(this, &RolesModule::rolesLoadedAre, this->rolesLoaded);
    this->rolesNotLoaded->addTransition(this->ui->updateRoles, &QPushButton::clicked, this->rolesNotLoaded);

    connect(this->rolesLoaded, &QState::entered, this, &RolesModule::enterRolesLoaded);
    this->rolesLoaded->addTransition(this, &RolesModule::itemSelectedIs, this->itemSelected);
    this->rolesLoaded->addTransition(this->ui->updateRoles, &QPushButton::clicked, this->rolesNotLoaded);
    this->rolesLoaded->addTransition(this->ui->searchRole, &QLineEdit::textChanged, this->searching);
    transition(this->ui->rolesList->selectionModel(), &QItemSelectionModel::selectionChanged, this, &RolesModule::handleSelectedRole, this->rolesLoaded);
    transition(this->ui->createRole, &QPushButton::clicked, this, &RolesModule::initRoleCreation, this->rolesLoaded);

    connect(this->itemSelected, &QState::entered, this, &RolesModule::enterItemSelected);
    this->itemSelected->addTransition(this->ui->abort, &QPushButton::clicked, this->rolesLoaded);
    transition(this->ui->deleteRole, &QPushButton::clicked, this, &RolesModule::handleRoleDeletion, this->itemSelected, this->rolesLoaded);

    connect(this->searching, &QState::entered, this, &RolesModule::enterSearching);
    transition(this->ui->abort, &QPushButton::clicked, this, &RolesModule::abortSearching, this->searching, this->rolesLoaded);
    transition(this->ui->rolesList->selectionModel(), &QItemSelectionModel::selectionChanged, this, &RolesModule::completeSearching, this->searching, this->itemSelected);
    transition(this->ui->searchRole, &QLineEdit::textChanged, this, &RolesModule::handleSearching, this->searching);
}

bool RolesModule::tryConnect()
{
    if(this->connection->open())
        return true;

    UserError::connectionError("Connection", "be established 'cause something went wrong", "Check credentials or internet connection, server might be down").show(this);

    return false;
}

void RolesModule::showEvent(QShowEvent *event)
{
    if(not this->stateMachine->isRunning())
        this->stateMachine->start();

    return QWidget::showEvent(event);
}

void RolesModule::setConnection(dbapi::Connection *connection)
{
    this->connection = connection;
    this->model->setConnection(connection);
}

RolesModule::~RolesModule()
{
    delete ui;
}

void RolesModule::enterRolesNotLoaded()
{
    this->ui->deleteRole->hide();
    this->ui->abort->hide();
    this->ui->updateRoles->show();
    this->ui->createRole->hide();

    this->model->clear();

    if(not this->tryConnect())
        return;

    auto error = this->model->loadAll();
    this->connection->close();

    if(error.isError())
        return error.show(this);

    emit this->rolesLoadedAre();
}

void RolesModule::enterRolesLoaded()
{
    this->ui->deleteRole->hide();
    this->ui->abort->hide();
    this->ui->updateRoles->show();
    this->ui->createRole->show();

    this->ui->rolesList->clearSelection();
}

void RolesModule::enterItemSelected()
{
    this->ui->deleteRole->show();
    this->ui->abort->show();
    this->ui->updateRoles->hide();
    this->ui->createRole->hide();
}

void RolesModule::enterSearching()
{
    this->ui->deleteRole->hide();
    this->ui->abort->show();
    this->ui->updateRoles->hide();
    this->ui->createRole->hide();

    this->ui->rolesList->setModel(this->proxyModel);
}

void RolesModule::handleFoundRole(QModelIndex index)
{
    this->ui->rolesList->scrollTo(index);
}

void RolesModule::handleSelectedRole()
{
    auto& selection = this->ui->rolesList->selectionModel()->selection();

    if(selection.size() > 0)
        emit this->itemSelectedIs();
}

void RolesModule::handleRoleDeletion()
{
    if(not this->tryConnect())
        return;

    auto indexes = this->ui->rolesList->selectionModel()->selectedIndexes();

    assert((void("empty"), indexes.size() > 0));

    auto error = this->model->removeRole(indexes.front().row());
    this->connection->close();

    if(error.isError())
        return error.show(this);

    QMessageBox::information(this, "Info", "All selected roles have been deleted");
}

void RolesModule::initRoleCreation()
{
    if(this->roleCreationDialog)
        delete this->roleCreationDialog;

    this->roleCreationDialog = new RoleCreationDialog(this);
    this->roleCreationDialog->show();

    connect(this->roleCreationDialog, &QDialog::finished, this, &RolesModule::completeRoleCreation);
}

void RolesModule::completeRoleCreation()
{
    if(this->roleCreationDialog->result() == QDialog::Rejected)
        return;

    if(not this->tryConnect())
        return;

    auto error = this->model->createRole(this->roleCreationDialog->roleName());
    this->connection->close();

    if(error.isError())
        return error.show(this);

    QMessageBox::information(this, "Info", "Role is created");
}

void RolesModule::completeSearching()
{
    QPersistentModelIndex activeIndex = this->ui->rolesList->currentIndex();

    this->ui->rolesList->setModel(this->model);
    this->ui->rolesList->setCurrentIndex(activeIndex);
}

void RolesModule::abortSearching()
{
    this->ui->rolesList->setModel(this->model);
}

void RolesModule::handleSearching()
{
    this->proxyModel->setFilterWildcard(this->ui->searchRole->text());
}
