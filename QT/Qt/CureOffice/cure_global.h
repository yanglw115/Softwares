#ifndef CURE_GLOBAL
#define CURE_GLOBAL

#include <QString>
#include <QSqlDatabase>
#include <QSqlDriver>
#include <QSqlError>
#include <QSqlQuery>
#include <QSqlRecord>

#define SALARY_CHECKED 0x01
#define SALARY_SEND_OK 0x02

extern QSqlDatabase g_db;
extern const QString g_strDataDir;
extern const QString g_strDatabaseFile;
extern const QString g_strTableNameStructure;

#endif // CURE_GLOBAL

