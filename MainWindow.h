#ifndef MAINWINDOW_H
#define MAINWINDOW_H


#include <QMainWindow>
#include <QTreeWidget>
#include "AuthorizationWindow.h"
#include "Menu/AboutDialog.h"
#include "Roles/RolesModule.h"
#include "Subjects/SubjectsModule.h"
#include "Persons/PersonsModule.h"
#include "SubjectsOfTeacher/SubjectsOfTeacherModule.h"
#include "Classes/ClassesModule.h"
#include "Journal/JournalModule.h"


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
    AuthorizationWindow* authorizationWindow;

    // menu
    AboutDialog* aboutDialog;


    // modules
    RolesModule* roles;
    SubjectsModule* subjects;
    PersonsModule* persons;
    SubjectsOfTeacherModule* subjectsOfTeacherModule;
    ClassesModule* classes;
    JournalModule* journal;

    QWidget* lastModule = nullptr;

    void manageLeftBarActions(QTreeWidgetItem* item, int column);

    void initAuthorization();

    void swapMainWidget(QWidget* newWidget);

    void showAbout();
    void showHelp();
};


#endif // MAINWINDOW_H
