#ifndef DATABASEMODEL_H
#define DATABASEMODEL_H

#include <QObject>
#include <QStringList>

#include "YLW_database_table_model.h"

class DatabaseModel : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QStringList rolesTableVersion READ getRolesTableVersion CONSTANT)
public:
    explicit DatabaseModel(QObject *parent = 0);
    virtual ~DatabaseModel();

    Q_INVOKABLE void setDatabasePath(QString strPath);
    Q_INVOKABLE QStringList getRolesTableVersion();

signals:

public slots:
private:
    DatabaseTableModel *m_pTableModelInstance;
};

#endif // DATABASEMODEL_H
