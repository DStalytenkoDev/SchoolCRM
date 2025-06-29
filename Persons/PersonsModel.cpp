#include <QDate>
#include "PersonsModel.h"

PersonsModel::PersonsModel(QObject *parent) : QAbstractListModel(parent)
{
}

void PersonsModel::setConnection(dbapi::Connection *connection)
{
    assert((void("nullptr"), connection != nullptr));

    this->connection = connection;
}

UserError PersonsModel::loadAll()
{
    dbapi::ApiError error;

    this->beginResetModel();
    this->clear();

    this->persons = dbapi::Person::loadAll(this->connection, &error);

    this->endResetModel();

    if(error.type != dbapi::ApiError::NoError)
        return UserError::internalError("Persons", "be loaded 'cause an unknown error", "Try again or contact support");

    return {};
}

UserError PersonsModel::removePerson(int index)
{
    assert((void("out of range"), index > 0 && index < this->persons.size()));

    auto person = this->persons[index];

    if(not person->remove())
    {
        UserError userError;

        if(person->error().type == dbapi::ApiError::PolicyError)
            return UserError::referenceError("Person", "be removed 'cause its related", "Try first removing objects are using certain person");
        else
            return UserError::internalError("Person", "be removed 'cause an unknown error", "Try again or contact support");
    }

    this->beginRemoveRows({}, index, index);

    delete person;
    this->persons.removeAt(index);

    this->endRemoveRows();

    return {};
}

UserError PersonsModel::createPerson(const QString &firstName, const QString &secondName, const QDate &birthday, const dbapi::Role::Key &role)
{
    auto error = this->loadAll();

    if(error.isError())
        return UserError::internalError("Person", "be created 'cause an unknown error", "Try again or contact support");

    QString trimmedFirstName = firstName.trimmed();
    QString trimmedSecondName = secondName.trimmed();

    if(trimmedFirstName.isEmpty() or trimmedSecondName.isEmpty())
        return UserError::validityError("Person", "be created 'cause it has empty first or second name");

    for(auto person: this->persons)
        if(person->firstName() == trimmedFirstName and person->secondName() == trimmedSecondName)
            return UserError::validityError("Person", "be created 'cause it already exists");

    auto person = new dbapi::Person(this->connection);

    person->setFirstName(trimmedFirstName);
    person->setSecondName(trimmedSecondName);
    person->setBirthday(birthday.toJulianDay());
    person->setRole(role);

    if(not person->store())
    {
        delete person;
        return UserError::internalError("Person", "be created 'cause an unknown error", "Try again or contact support");
    }

    this->beginInsertRows({}, this->persons.size(), this->persons.size());
    this->persons.append(person);
    this->endInsertRows();

    return {};
}

UserError PersonsModel::editPerson(int index, const QString &firstName, const QString &secondName, const QDate &birthday, const dbapi::Role::Key &role)
{
    assert((void("out of range"), index > 0 && index < this->persons.size()));

    auto person = this->persons[index];

    auto error = this->loadAll();

    if(error.isError())
        return UserError::internalError("Person", "be edited 'cause an unknown error", "Try again or contact support");

    QString trimmedFirstName = firstName.trimmed();
    QString trimmedSecondName = secondName.trimmed();

    if(trimmedFirstName.isEmpty() or trimmedSecondName.isEmpty())
        return UserError::validityError("Person", "be edited 'cause it has empty first or second name");

    for(auto person: this->persons)
        if(person->firstName() == trimmedFirstName and person->secondName() == trimmedSecondName)
            return UserError::validityError("Person", "be edited 'cause same one already exists");

    person->setFirstName(trimmedFirstName);
    person->setSecondName(trimmedSecondName);
    person->setBirthday(birthday.toJulianDay());
    person->setRole(role);

    if(not person->update())
        return UserError::internalError("Person", "be edited 'cause an unknown error", "Try again or contact support");

    QModelIndex modelIndex = this->index(index);
    emit this->dataChanged(modelIndex, modelIndex, {Qt::DisplayRole});

    return {};
}

dbapi::Person *PersonsModel::person(const QModelIndex &index)
{
    assert((void("out of range"), index.row() > 0 && index.row() < this->persons.size()));

    return this->persons[index.row()];
}

dbapi::Person *PersonsModel::person(int row)
{
    assert((void("out of range"), row > 0 && row < this->persons.size()));

    return this->persons[row];
}

int PersonsModel::rowCount(const QModelIndex &parent) const
{
    return this->persons.size();
}

QVariant PersonsModel::data(const QModelIndex &index, int role) const
{
    if(role != Qt::DisplayRole)
        return {};

    if(not index.isValid())
        return {};

    if(index.row() >= this->persons.count())
        return {};

    QString fullName;
    auto person = this->persons[index.row()];

    fullName.append(person->firstName());
    fullName.append(' ');
    fullName.append(person->secondName());

    return fullName;
}

void PersonsModel::clear()
{
    if(this->persons.size() == 0)
        return;

    this->beginResetModel();

    for(auto person : this->persons)
        delete person;

    this->persons.clear();

    this->endResetModel();
}

PersonsModel::~PersonsModel()
{
    for(auto person : this->persons)
        delete person;
}
