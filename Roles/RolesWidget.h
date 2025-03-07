#ifndef ROLESWIDGET_H
#define ROLESWIDGET_H

#include <QWidget>

namespace Ui {
class RolesWidget;
}

class RolesWidget : public QWidget
{
    Q_OBJECT

public:
    explicit RolesWidget(QWidget *parent = nullptr);
    ~RolesWidget();

private:
    Ui::RolesWidget *ui;
};

#endif // ROLESWIDGET_H
