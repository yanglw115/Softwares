#include <QtDebug>

#include "YLW_database_model.h"

DatabaseModel::DatabaseModel(QObject *parent)
    : QObject(parent)
    , m_pTableModelInstance(NULL)
{

}

DatabaseModel::~DatabaseModel()
{

}

void DatabaseModel::setDatabasePath(QString strPath)
{
    qDebug() << "Set database path: " << strPath;
    if (!DatabaseTableModel::m_strDatabasePath.isEmpty()) {
        m_pTableModelInstance = NULL;
        DatabaseTableModel::destroyInstance();
    }

    DatabaseTableModel::m_strDatabasePath = strPath;
    m_pTableModelInstance = DatabaseTableModel::getInstance();
}

QStringList DatabaseModel::getRolesTableVersion()
{
    QStringList strList;
    if (m_pTableModelInstance) {
        m_pTableModelInstance->getFieldsTableVersion(strList);
    }
    qDebug() << "Table version fields: " << strList;
    return strList;
}

