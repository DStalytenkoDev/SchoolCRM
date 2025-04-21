#ifndef CREATEROLEDIALOG_H
#define CREATEROLEDIALOG_H

#include <QDialog>

namespace Ui {
class CreateRoleDialog;
}

class CreateRoleDialog : public QDialog
{
    Q_OBJECT

public:
    CreateRoleDialog(QWidget *parent = nullptr);

    QString roleName() const;

    ~CreateRoleDialog();

private:
    Ui::CreateRoleDialog *ui;
};

#endif // CREATEROLEDIALOG_H
