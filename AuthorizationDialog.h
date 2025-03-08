#ifndef AUTHORIZATIONDIALOG_H
#define AUTHORIZATIONDIALOG_H

#include <QDialog>

namespace Ui {
class AuthorizationDialog;
}

class AuthorizationDialog : public QDialog
{
    Q_OBJECT

public:
    AuthorizationDialog(QWidget *parent = nullptr);

    QString serverName();
    QString databaseName();

    bool isTrustedConnection();

    QString userName();
    QString password();

    void setServerName(const QString& name);
    void setDatabaseName(const QString& name);

    void setTrustedConnection(bool enabled = true);

    void setUserName(const QString& name);
    void setPassword(const QString& password);

    ~AuthorizationDialog();

private:
    Ui::AuthorizationDialog *ui;

    void swapLoginMethod();
};

#endif // AUTHORIZATIONDIALOG_H
