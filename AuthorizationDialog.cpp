#include "AuthorizationDialog.h"
#include "ui_AuthorizationDialog.h"

AuthorizationDialog::AuthorizationDialog(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::AuthorizationDialog)
{
    ui->setupUi(this);

    connect(this->ui->connectionType, &QComboBox::currentIndexChanged, this, &AuthorizationDialog::swapLoginMethod);
}

QString AuthorizationDialog::serverName()
{
    return this->ui->serverName->text();
}

QString AuthorizationDialog::databaseName()
{
    return this->ui->databaseName->text();
}

bool AuthorizationDialog::isTrustedConnection()
{
    if(this->ui->connectionType->currentIndex() == 0) // user login
        return false;

    // trusted login
    return true;
}

QString AuthorizationDialog::userName()
{
    return this->ui->userName->text();
}

QString AuthorizationDialog::password()
{
    return this->ui->password->text();
}

void AuthorizationDialog::setServerName(const QString &name)
{
    this->ui->serverName->setText(name);
}

void AuthorizationDialog::setDatabaseName(const QString &name)
{
    this->ui->databaseName->setText(name);
}

void AuthorizationDialog::setTrustedConnection(bool enabled)
{
    this->ui->connectionType->setCurrentIndex(1);
    this->swapLoginMethod();
}

void AuthorizationDialog::setUserName(const QString &name)
{
    this->ui->userName->setText(name);
}

void AuthorizationDialog::setPassword(const QString &password)
{
    this->ui->password->setText(password);
}

AuthorizationDialog::~AuthorizationDialog()
{
    delete ui;
}

void AuthorizationDialog::swapLoginMethod()
{
    if(this->ui->connectionType->currentIndex() == 0) // user login
    {
        this->ui->loginFrame->setHidden(false);
        return;
    }

    // trusted login
    this->ui->loginFrame->hide();
}
