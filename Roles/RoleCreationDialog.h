#ifndef ROLECREATIONDIALOG_H
#define ROLECREATIONDIALOG_H

#include <QDialog>

namespace Ui {
class RoleCreationDialog;
}

class RoleCreationDialog : public QDialog
{
    Q_OBJECT

public:
    RoleCreationDialog(QWidget *parent = nullptr);

    QString roleName() const;

    ~RoleCreationDialog();

private:
    Ui::RoleCreationDialog *ui;
};

#endif // ROLECREATIONDIALOG_H
