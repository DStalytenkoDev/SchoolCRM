#ifndef MAINWINDOW_H
#define MAINWINDOW_H


#include <QMainWindow>
#include <QTreeWidget>
#include "AuthorizationDialog.h"
#include "Roles/RolesModule.h"
#include "Subjects/SubjectsModule.h"
#include "Persons/PersonsModule.h"
#include "SubjectsOfTeacher/SubjectsOfTeacherModule.h"


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
    RolesModule* roles;
    SubjectsModule* subjects;
    PersonsModule* persons;
    SubjectsOfTeacherModule* subjectsOfTeacherModule;

    QWidget* lastModule = nullptr;

    dbapi::Connection connection;

    void manageLeftBarActions(QTreeWidgetItem* item, int column);

    void initAuthorization();
    void completeAuthorization(int code);

    void swapMainWidget(QWidget* newWidget);
};


#endif // MAINWINDOW_H
