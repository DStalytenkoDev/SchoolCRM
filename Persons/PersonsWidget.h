#ifndef PERSONSWIDGET_H
#define PERSONSWIDGET_H


#include <SchoolDatabaseApi/Connection/Connection.h>
#include <QWidget>


namespace Ui {
class PersonsWidget;
}


class PersonsWidget : public QWidget
{
    Q_OBJECT

public:
    PersonsWidget(QWidget *parent = nullptr);

    void setConnection(dbapi::Connection* connection);

    ~PersonsWidget();

private:
    Ui::PersonsWidget *ui;
    dbapi::Connection* connection = nullptr;
};


#endif // PERSONSWIDGET_H
