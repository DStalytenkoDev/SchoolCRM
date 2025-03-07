#ifndef PERSONSWIDGET_H
#define PERSONSWIDGET_H

#include <QWidget>

namespace Ui {
class PersonsWidget;
}

class PersonsWidget : public QWidget
{
    Q_OBJECT

public:
    explicit PersonsWidget(QWidget *parent = nullptr);
    ~PersonsWidget();

private:
    Ui::PersonsWidget *ui;
};

#endif // PERSONSWIDGET_H
