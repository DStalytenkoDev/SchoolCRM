#include <QMessageBox>
#include "UserError.h"
#include "AuthorizationWindow.h"
#include "ui_AuthorizationWindow.h"

AuthorizationWindow::AuthorizationWindow(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::AuthorizationWindow)
{
    ui->setupUi(this);

    this->setWindowModality(Qt::ApplicationModal);
    this->setWindowFlag(Qt::Window);
    this->setWindowTitle("Authorization");

    this->hide();

    this->timer = new QTimer(this);
    this->timer->setInterval(TIMER_INTERVAL);
    connect(this->timer, &QTimer::timeout, this, &AuthorizationWindow::updateProgressBar);

    connect(this->ui->connect, &QPushButton::clicked, this, &AuthorizationWindow::initConnection);
    connect(this->ui->abort, &QPushButton::clicked, this, &AuthorizationWindow::close);

    connect(this->ui->connectionType, &QComboBox::activated, this, &AuthorizationWindow::updateAuthUi);
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

void AuthorizationWindow::initConnection()
{
    this->counter = 0;

    if(this->ui->connectionType->currentIndex() == 0) // login
    {
        this->connection.setUserName(this->ui->userName->text());
        this->connection.setPassword(this->ui->password->text());
    }
    else
        this->connection.setTrustedConnection(true);

    this->connection.setServerName(this->ui->serverName->text());
    this->connection.setDatabaseName(this->ui->databaseName->text());

    this->openConnectionWorker = new OpenConnectionWorker(&this->connection, &this->mutex, this);
    connect(this->openConnectionWorker, &OpenConnectionWorker::connected, this, &AuthorizationWindow::completeConnection);

    this->progressDialog = new QProgressDialog(this);
    this->progressDialog->setWindowModality(Qt::ApplicationModal);
    this->progressDialog->setWindowFlag(Qt::Window);
    this->progressDialog->setMinimum(1);
    this->progressDialog->setMaximum(TIMER_DURATION + 1);
    this->progressDialog->setWindowTitle("Info");
    this->progressDialog->setLabelText("Connecting...");

    this->timer->start();
    this->progressDialog->show();
    this->openConnectionWorker->start();
}

void AuthorizationWindow::completeConnection(bool isConnected)
{
    this->openConnectionWorker->quit();
    this->openConnectionWorker->wait();

    this->progressDialog->close();

    delete this->progressDialog;
    delete this->openConnectionWorker;

    this->timer->stop();

    this->connection.close();

    if(isConnected)
    {
        QMessageBox::information(this, "Info", "Server is connected, my congrants))");
        return;
    }

    UserError::connectionError("Authorization", "be done 'cause credentials might be wrong or server is down").show(this);
}

void AuthorizationWindow::updateProgressBar()
{
    if(this->counter < TIMER_DURATION)
    {
        this->counter += TIMER_INTERVAL;
        this->progressDialog->setValue(this->counter);

        return;
    }

    this->counter /= 2;
}

void AuthorizationWindow::updateAuthUi()
{
    bool isLogin = this->ui->connectionType->currentIndex() == 0; // login

    this->ui->password->setEnabled(isLogin);
    this->ui->userName->setEnabled(isLogin);
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
