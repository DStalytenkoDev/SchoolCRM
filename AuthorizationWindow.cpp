#include <QMessageBox>
#include "UserError.h"
#include "AuthorizationWindow.h"
#include "ui_AuthorizationWindow.h"

AuthorizationWindow::AuthorizationWindow(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::AuthorizationWindow)
{
    ui->setupUi(this);
    this->setupStateMachine();

    this->setWindowModality(Qt::ApplicationModal);
    this->setWindowFlag(Qt::Window);
    this->setWindowTitle("Authorization");

    this->timer = new QTimer(this);
    this->timer->setInterval(TIMER_INTERVAL);
    connect(this->timer, &QTimer::timeout, this, &AuthorizationWindow::updateProgressBar);

    this->openConnectionWorker = new OpenConnectionWorker(&this->connection, &this->mutex, this);
    connect(this->openConnectionWorker, &OpenConnectionWorker::connected, this, &AuthorizationWindow::completeConnectionAttempt);

    connect(this->ui->abort, &QPushButton::clicked, this, &QWidget::close);

    this->ui->progressBar->setMinimum(1);
    this->ui->progressBar->setMaximum(TIMER_DURATION + 1);
}

dbapi::Connection *AuthorizationWindow::getConnection()
{
    this->mutex.lock();
    this->mutex.unlock();

    return &this->connection;
}

AuthorizationWindow::~AuthorizationWindow()
{
    delete ui;
}

void AuthorizationWindow::enterCredentialsEdition()
{
    this->ui->progressBar->hide();
    this->ui->serverConnectedLabel->hide();

    this->ui->abort->show();
    this->ui->abort->setText("Abort");
    this->ui->confirm->setDisabled(false);
    this->ui->confirm->setText("Connect");

    this->ui->settingsFrame->setDisabled(false);

    bool isLogin = this->ui->connectionType->currentIndex() == 0; // login

    this->ui->password->setEnabled(isLogin);
    this->ui->userName->setEnabled(isLogin);
}

void AuthorizationWindow::enterConnectionAttempt()
{
    this->counter = 0;

    if(this->ui->serverName->text().isEmpty())
    {
        UserError::validityError("Server name", "be valid 'cause its empty").show(this);
        emit this->credentialsInvalidAre();
        return;
    }

    if(this->ui->databaseName->text().isEmpty())
    {
        UserError::validityError("Database name", "be valid 'cause its empty").show(this);
        emit this->credentialsInvalidAre();
        return;
    }

    this->ui->settingsFrame->setDisabled(true);
    this->ui->progressBar->reset();
    this->ui->progressBar->show();
    this->ui->abort->hide();
    this->ui->confirm->setDisabled(true);

    if(this->ui->connectionType->currentIndex() == 0) // login
    {
        this->connection.setUserName(this->ui->userName->text());
        this->connection.setPassword(this->ui->password->text());
    }
    else
        this->connection.setTrustedConnection(true);

    this->connection.setServerName(this->ui->serverName->text());
    this->connection.setDatabaseName(this->ui->databaseName->text());

    this->timer->start();
    this->openConnectionWorker->start();
}

void AuthorizationWindow::enterDBConnected()
{
    this->ui->progressBar->hide();
    this->ui->abort->show();
    this->ui->abort->setText("Close");

    this->ui->confirm->setDisabled(false);
    this->ui->confirm->setText("Change");
    this->ui->serverConnectedLabel->show();
}

void AuthorizationWindow::completeConnectionAttempt(bool isConnected)
{
    this->openConnectionWorker->quit();
    this->openConnectionWorker->wait();

    this->timer->stop();
    this->connection.close();

    if(not isConnected)
    {
        UserError::connectionError("Authorization", "be done 'cause credentials might be wrong or server is down").show(this);
        emit this->failedIs();
    }
    else
        emit this->connectedIs();
}

void AuthorizationWindow::updateProgressBar()
{
    if(this->counter < TIMER_DURATION)
    {
        this->counter += TIMER_INTERVAL;
        this->ui->progressBar->setValue(this->counter);

        return;
    }

    this->counter /= 2;
}

void AuthorizationWindow::setupStateMachine()
{
    this->stateMachine = new QStateMachine(this);

    this->credentialsEdition = new QState(this->stateMachine);
    this->connectionAttempt = new QState(this->stateMachine);
    this->DBConnected = new QState(this->stateMachine);

    connect(this->credentialsEdition, &QState::entered, this, &AuthorizationWindow::enterCredentialsEdition);
    this->credentialsEdition->addTransition(this->ui->connectionType, &QComboBox::activated, this->credentialsEdition);
    this->credentialsEdition->addTransition(this->ui->confirm, &QPushButton::clicked, this->connectionAttempt);

    connect(this->connectionAttempt, &QState::entered, this, &AuthorizationWindow::enterConnectionAttempt);
    this->connectionAttempt->addTransition(this, &AuthorizationWindow::connectedIs, this->DBConnected);
    this->connectionAttempt->addTransition(this, &AuthorizationWindow::failedIs, this->credentialsEdition);
    this->connectionAttempt->addTransition(this, &AuthorizationWindow::credentialsInvalidAre, this->credentialsEdition);

    connect(this->DBConnected, &QState::entered, this, &AuthorizationWindow::enterDBConnected);
    this->DBConnected->addTransition(this->ui->confirm, &QPushButton::clicked, this->credentialsEdition);

    this->stateMachine->setInitialState(this->credentialsEdition);
    this->stateMachine->start();
}

OpenConnectionWorker::OpenConnectionWorker(dbapi::Connection *connection, QMutex *mutex, QObject *parent) : connection(connection), mutex(mutex), QThread(parent)
{
}

void OpenConnectionWorker::run()
{
    this->mutex->lock();

    bool isConnected = this->connection->open();

    this->mutex->unlock();

    emit this->connected(isConnected);
}
