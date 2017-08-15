#include <QtDebug>
#include <QSqlRecord>

#include "YLW_database_table_model.h"

#ifndef _HW_CHARSET_H_
#define _HW_CHARSET_H_

#if _MSC_VER >= 1600
#pragma execution_character_set("utf-8")
#endif

#endif // _HW_CHARSET_H_

QString DatabaseTableModel::m_strDatabasePath = QString("");
QSqlDatabase DatabaseTableModel::m_database;
DatabaseTableModel *DatabaseTableModel::m_pInstance = NULL;

const QString g_strSqlDriverName                    = "QSQLITE";
const QString g_strConnectName                      = "DES";
/* 用于保存所有播放列表的记录 */
static const QString g_strADInfoTableName           = "advinfo";
/* 用于记录播放列表的版本信息,前期仅使用hash值代表当前列表的版本,hash变化时才代表播放列表变化,默认播放文件在播放列表中的变化不影响hash值 */
static const QString g_strVersionTableName          = "version";

DatabaseTableModel::DatabaseTableModel(QObject *parent, QSqlDatabase db)
    : QSqlTableModel(parent, db)
{

}

DatabaseTableModel::~DatabaseTableModel()
{

}

DatabaseTableModel *DatabaseTableModel::getInstance()
{
    if (NULL == m_pInstance) {
        if (!m_strDatabasePath.isEmpty()) {
            m_database = QSqlDatabase::addDatabase(g_strSqlDriverName, g_strConnectName);
            m_database.setDatabaseName(m_strDatabasePath);
            m_pInstance = new DatabaseTableModel(0, m_database);
            m_pInstance->openDatabase();
        }
    }
    return m_pInstance;
}

void DatabaseTableModel::destroyInstance()
{
    if (m_pInstance) {
        delete m_pInstance;
        m_pInstance = NULL;
    }
}

void DatabaseTableModel::getFieldsTableVersion(QStringList &strList)
{
    strList.clear();
    this->setTable(g_strVersionTableName);
    QSqlRecord record = this->record();
    qDebug() << "Version fields count: " << record.count();
    for (int i = 0; i < record.count(); ++i) {
        strList.append(record.fieldName(i));
        qDebug() << strList[i];
    }
}

bool DatabaseTableModel::openDatabase()
{
    /* 打开数据库 */
    if (!m_database.open()) {
        qCritical() << "数据库文件打开失败!";
        if (m_database.isValid()) {
            m_database.removeDatabase(g_strConnectName);
        }
        return false;
    }
    qDebug() << "打开连接数据库成功！";
    return true;
}

void DatabaseTableModel::closeDatabase()
{
    qDebug() << "关闭数据库连接！";
    if (m_database.isOpen()) {
        m_database.close();
        /* 网上的方法，避免报connection is still in use的警告 */
        QString name;
        {
            name = QSqlDatabase::database().connectionName();
        } // 超出作用域，隐含对象QSqlDatabase::database()被删除。
        QSqlDatabase::removeDatabase(name);
    }
}

