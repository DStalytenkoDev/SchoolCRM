#ifndef MAINWINDOW_H
#define MAINWINDOW_H


#include <QMainWindow>
#include <qtreewidget.h>
#include "AuthorizationDialog.h"
#include "SchoolDatabaseApi/Connection/Connection.h"
#include "Persons/PersonsWidget.h"


QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE


class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    AuthorizationDialog* authorizationDialog;

    /// modules
    PersonsWidget* persons;

    QWidget* lastMainWidget = nullptr;

    dbapi::Connection connection;

    void manageLeftBarActions(QTreeWidgetItem* item, int column);

    void showAuthorizationDialog();
    void completeAuthorization(int code);

    void swapMainWidget(QWidget* newWidget);
};


#endif // MAINWINDOW_H
