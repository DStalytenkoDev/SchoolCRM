#ifndef SMARTEDIT_H
#define SMARTEDIT_H

#include <QWidget>
#include <QHBoxLayout>
#include <QLineEdit>
#include <QPushButton>

class SmartEdit : public QWidget
{
    Q_OBJECT

public:
    SmartEdit(QWidget *parent = nullptr);

    void setText(const QString& text);
    QString text() const;

private:
    bool stateSolid = true;

    QHBoxLayout* layout;
    QPushButton* labelButton;
    QLineEdit* editField;

    void swapUi();
};

#endif // SMARTEDIT_H
