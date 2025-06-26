#ifndef ROLESMODEL_H
#define ROLESMODEL_H


#include <QAbstractListModel>
#include <SchoolApi/Role.h>
#include "UserError.h"


/// the class provides loading down the names of the roles, but not any changes in an actual API
class RolesModel : public QAbstractListModel
{
public:
    RolesModel(QObject *parent = nullptr);

    void setConnection(dbapi::Connection* connection);

    UserError loadAll();
    UserError removeRoles(int start, int end);
    UserError createRole(const QString& name);

    /// in case of any not valid index undefined behaviour
    dbapi::Role* role(const QModelIndex& index);
    /// in case of any not valid index undefined behaviour
    dbapi::Role* role(int row);

    virtual int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    virtual QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;

    void clear();

    ~RolesModel();

private:
    dbapi::Connection* connection = nullptr;
    QList<dbapi::Role*> roles;
};


#endif // ROLESMODEL_H
