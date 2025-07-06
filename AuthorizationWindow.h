#ifndef AUTHORIZATIONWINDOW_H
#define AUTHORIZATIONWINDOW_H


#include <QWidget>
#include <QProgressDialog>
#include <QThread>
#include <QMutex>
#include <QTimer>
#include <SchoolApi/Connection.h>


namespace Ui {
class AuthorizationWindow;
}


class OpenConnectionWorker : public QThread
{
    Q_OBJECT

public:
    OpenConnectionWorker(dbapi::Connection* connection, QMutex* mutex, QObject* parent = nullptr);

    virtual void run() override;

private:
    dbapi::Connection* connection;
    QMutex* mutex;

signals:
    void connected(bool isConnected);
};


class AuthorizationWindow : public QWidget
{
    Q_OBJECT

public:
    AuthorizationWindow(QWidget *parent = nullptr);

    dbapi::Connection* getConnection();

    ~AuthorizationWindow();

private:
    Ui::AuthorizationWindow *ui;

    QProgressDialog* progressDialog;
    QTimer* timer;
    int counter = 0;

    OpenConnectionWorker* openConnectionWorker;
    QMutex mutex;

    static const int TIMER_INTERVAL = 150;
    static const int TIMER_DURATION = TIMER_INTERVAL * 70;

    void initConnection();
    void completeConnection(bool isConnected);

    void updateProgressBar();

    void updateAuthUi();

    dbapi::Connection connection;
};


#endif // AUTHORIZATIONWINDOW_H
