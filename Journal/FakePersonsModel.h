#ifndef FAKEPERSONSMODEL_H
#define FAKEPERSONSMODEL_H


#include <QPair>
#include <QAbstractListModel>
#include <SchoolApi/Person.h>


class FakePersonsModel : public QAbstractListModel
{
public:
    FakePersonsModel(QObject *parent = nullptr);

    void createPerson(const dbapi::Person::Key& key, const QString& firstName, const QString& secondName);
    void removePerson(int index);
    void clear();

    const dbapi::Person::Key& person(int index);

    virtual int rowCount(const QModelIndex& index = {}) const override;
    virtual QVariant data(const QModelIndex& index, int role) const override;

private:
    QList<QPair<dbapi::Person::Key, QString>> persons;
};


#endif // FAKEPERSONSMODEL_H
