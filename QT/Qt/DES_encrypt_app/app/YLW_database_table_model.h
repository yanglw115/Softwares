#ifndef DATABASETABLEMODEL_H
#define DATABASETABLEMODEL_H

#include <QSqlTableModel>
#include <QString>

class DatabaseTableModel : public QSqlTableModel
{
private:
    DatabaseTableModel(QObject * parent = 0, QSqlDatabase db = QSqlDatabase());
    virtual ~DatabaseTableModel();

public:
    static DatabaseTableModel *getInstance();
    static void destroyInstance();
    void getFieldsTableVersion(QStringList &);

private:
    bool openDatabase();
    void closeDatabase();

public:
    static QString m_strDatabasePath;
private:
    static DatabaseTableModel *m_pInstance;
    static QSqlDatabase m_database;
};

#endif // DATABASETABLEMODEL_H
