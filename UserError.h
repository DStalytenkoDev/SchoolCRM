#ifndef USERERROR_H
#define USERERROR_H


#include <QMessageBox>


class UserError
{
public:
    enum Type
    {
        None,
        Warning,
        Critical
    };

    UserError() = default;
    UserError(const QString& shortText, Type type = Warning);
    UserError(const QString& title, const QString& shortText, Type type = Warning);
    UserError(const QString& title, const QString& shortText, const QString& longText, Type type = Warning);

    bool isError() const;

    const QString& shortText() const;
    const QString& longText() const;
    const QString& title() const;

    Type type() const;

    void show(QWidget* parent) const;
    QMessageBox* formMessage(QWidget* parent) const;

private:
    Type _type = None;

    QString _shortText;
    QString _longText;
    QString _title;

    QMessageBox* createMessageBox(QWidget* parent) const;
};


#endif // USERERROR_H
