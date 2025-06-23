#ifndef ROLESMODEL_H
#define ROLESMODEL_H


#include <QAbstractListModel>
#include <SchoolApi/Role.h>


/// the class provides loading down the names of the roles, but not any changes in an actual API
class RolesModel : public QAbstractListModel
{
public:
    RolesModel(QObject *parent = nullptr);

    void setConnection(dbapi::Connection* connection);

    /// req: called setConnection() with a valid arg
    dbapi::ApiError loadAll();

    /// in case of any not valid index undefined behaviour
    dbapi::Role* role(const QModelIndex& index);
    /// in case of any not valid index undefined behaviour
    dbapi::Role* role(int row);

    virtual int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    virtual QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;

    bool insertRow(int rowBefore, const dbapi::Role& role, const QModelIndex &parent = QModelIndex());

    void clear();

    ~RolesModel();

private:
    dbapi::Connection* connection = nullptr;
    QList<dbapi::Role*> roles;

    void cleanRoles();
};


#endif // ROLESMODEL_H
