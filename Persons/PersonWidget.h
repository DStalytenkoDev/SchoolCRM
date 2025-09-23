#ifndef PERSONWIDGET_H
#define PERSONWIDGET_H

#include <QWidget>
#include <qabstractitemmodel.h>
#include <qboxlayout.h>
#include "../ComboBoxFinderView.h"

namespace Ui {
class PersonWidget;
}

class PersonWidget : public QWidget
{
    Q_OBJECT

public:
    PersonWidget(QWidget *parent = nullptr);

    QString firstName();
    QString secondName();
    QModelIndex role();
    QDate date();

    void setFirstName(const QString& name);
    void setSecondName(const QString& name);
    void setDate(const QDate& date);

    /// set role from a model
    void setRole(const QModelIndex& role);

    /// set the whole model with roles
    void setRoles(QAbstractItemModel* roles);

    /// except for the roles model
    void clear();

    ~PersonWidget();

protected:
    QHBoxLayout* menuLayout();

private:
    Ui::PersonWidget *ui;

    ComboBoxFinderView* roleView;
};

#endif // PERSONWIDGET_H
