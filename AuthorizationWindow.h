#ifndef AUTHORIZATIONWINDOW_H
#define AUTHORIZATIONWINDOW_H


#include <QWidget>
#include <QThread>
#include <QMutex>
#include <QTimer>
#include <QStateMachine>
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

    dbapi::Connection connection;

    QTimer* timer;
    int counter = 0;

    static const int TIMER_INTERVAL = 150;
    static const int TIMER_DURATION = TIMER_INTERVAL * 70;

    OpenConnectionWorker* openConnectionWorker;
    QMutex mutex;

    QStateMachine* stateMachine;

    QState* credentialsEdition;
    QState* connectionAttempt;
    QState* DBConnected;

    void enterCredentialsEdition();
    void enterConnectionAttempt();
    void enterDBConnected();

    void completeConnectionAttempt(bool isConnected);

    void updateProgressBar();
    void setupStateMachine();

private: signals:
    void connectedIs();
    void failedIs();
    void credentialsInvalidAre();
};


#endif // AUTHORIZATIONWINDOW_H
