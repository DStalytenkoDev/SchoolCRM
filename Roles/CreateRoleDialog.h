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
    explicit CreateRoleDialog(QWidget *parent = nullptr);
    ~CreateRoleDialog();

private:
    Ui::CreateRoleDialog *ui;
};

#endif // CREATEROLEDIALOG_H
