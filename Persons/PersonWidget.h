#ifndef PERSONWIDGET_H
#define PERSONWIDGET_H

#include <QWidget>

namespace Ui {
class PersonWidget;
}

class PersonWidget : public QWidget
{
    Q_OBJECT

public:
    explicit PersonWidget(QWidget *parent = nullptr);
    ~PersonWidget();

private:
    Ui::PersonWidget *ui;
};

#endif // PERSONWIDGET_H
