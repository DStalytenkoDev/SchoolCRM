// Copyright (C) 2025 AUTHORS of SchooliesCave
// SPDX-License-Identifier: GPL-3.0-only
//
// This file is part of SchooliesCave.
// 
// SchooliesCave is free software: you can redistribute it and/or modify it under the terms
// of the GNU General Public License as published by the Free Software Foundation, version 3
// of the License only.
// 
// SchooliesCave is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY;
// without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
// See the GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License along with SchooliesCave.
// If not, see <https://www.gnu.org/licenses/>.


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
