#include <QDateTime>
#include <QFile>
#include <QMutex>
#include <QSqlError>
#include <QSqlRecord>
#include <QSqlQuery>

#include "YG_playlist.h"
#include "YG_utils.h"

static const QString g_strKeyDES                    = "%U(^f6@R";
static const QString g_strDatabaseName              = "./share/adconfig";
static const QString g_strSqlDriverName             = "QSQLITE";
static const QString g_strConnectName               = "YG_APP_AD";
static const QString g_strUserName                  = "";
static const QString g_strPassword                  = "";


/* 用于保存所有播放列表的记录 */
static const QString g_strADInfoTableName           = "advinfo";
/* 用于记录播放列表的版本信息,前期仅使用hash值代表当前列表的版本,hash变化时才代表播放列表变化,默认播放文件在播放列表中的变化不影响hash值 */
static const QString g_strVersionTableName          = "version";
/* 更新同步数据库中播放列表的定时器,一般数据库24小时更新一次,但是考虑与升级程序同步的情况,所以这里设置间隔小一些 */
static ulong g_timerPeriod                          = 600 * 1000;
static int g_hourForceUpdate                        = 0; // 强制进行播放列表刷新的时间点(考虑播放列表不会更新的场景,将在凌晨时分进行强制刷新播放列表)
static int g_criticalValue                          = 55;

QMutex m_mutexPlaylist;
QMediaPlaylist *g_pPlaylistMain = NULL;
/* 当前默认播放媒体也可以下发多个,与上面两个播放列表的下发流程一样,发布的时候可以发布多个,所以需要用PLAYLIST来管理 */
QMediaPlaylist *g_pPlaylistMainDefault = NULL;

/* 下面的QMap用于记录当天所有播放列表文件的MD5值,供播放文件MD5校验查询使用 */
QMap<QString, QString> g_mapMainPlaylistMD5;

CPlaylist::CPlaylist(QObject *parent)
    : QObject(parent)
    , m_timer(NULL)
    , m_lastUpdateTimestamp(0)
    , m_hashVersion("")
    , m_timerUpdatePlaylist(NULL)
    , m_pCurrentPlaylist(NULL)
    , m_indexNextPlay(0)
    , m_indexNextPlayDefault(0)
    , m_strNextPlayMediaPathDefault("")
{
    g_pPlaylistMain = new QMediaPlaylist[24];
    g_pPlaylistMainDefault = new QMediaPlaylist(this);

    if ((NULL == g_pPlaylistMain) || (NULL == g_pPlaylistMainDefault)) {
        qDebug() << "New QMediaPlaylist failed!";
    }

    m_timer = new QTimer(this);
    m_timerUpdatePlaylist = new QTimer(this);

    m_timer->setSingleShot(true);
    connect(m_timer, SIGNAL(timeout()), this, SLOT(slotTimeoutHandler()));
    connect(m_timerUpdatePlaylist, SIGNAL(timeout()), this, SLOT(slotUpdateCurrentPlaylist()));

}

CPlaylist::~CPlaylist()
{
    if (g_pPlaylistMain) {
        delete []g_pPlaylistMain;
    }
    if (g_pPlaylistMainDefault) {
        delete g_pPlaylistMainDefault;
    }
}

void CPlaylist::slotStartPlaylistThread()
{
    initGlobalPlaylist();
    initDatabase();
    setTimerForParseDB();
    parsePlaylistFromDB();
    /* 播放列表初始化完成之后才可进行播放功能的初始化 */
    emit signalPlaylistInitFinished();
}

void CPlaylist::slotUpdateCurrentPlaylist()
{

    if ((NULL != m_timerUpdatePlaylist) && (m_timerUpdatePlaylist->isActive())) {
        m_timerUpdatePlaylist->stop();
    }

//    m_listMD5FailedMedia.clear();
//    m_listPlayFailedMedia.clear();

    setCurrentHourPlaylist();
    setUpdatePlaylistTimer();
}

void CPlaylist::slotTimeoutHandler()
{
    qDebug() << "\n";
    qDebug() << "解析数据库的定时器定时结束，准备解析数据库。";
    stopTimer();
    setTimerForParseDB();
    parsePlaylistFromDB();
}

void CPlaylist::initGlobalPlaylist()
{
    /* 只有在程序初始化或从数据库同步新的播放列表时才清空 */
    for (int i = 0; i < 24; ++i) {
        g_pPlaylistMain[i].clear();
        /* 设置单次播放 */
        g_pPlaylistMain[i].setPlaybackMode(QMediaPlaylist::CurrentItemOnce);
    }

    g_pPlaylistMainDefault->clear();
    g_pPlaylistMainDefault->setPlaybackMode(QMediaPlaylist::CurrentItemOnce);

    g_mapMainPlaylistMD5.clear();
}

bool CPlaylist::initDatabase()
{
    /* 数据库只在程序初始化的时候初始化一次,涉及到后续要打开的数据库的名称等 */
    qDebug() << "init database...";
    /* 检查数据库文件是否存在 */
    QString strDatabasePath = CUtils::getFileFullPath(g_strDatabaseName);
    m_database = QSqlDatabase::addDatabase(g_strSqlDriverName, g_strConnectName);
    m_database.setDatabaseName(strDatabasePath);
    m_database.setUserName(g_strUserName);
    m_database.setPassword(g_strPassword);

    if (!QFile::exists(strDatabasePath)) {
        qCritical() << "Cannot find the database file!";
        return false;
    }

    if (!openDatabase()) {
        return false;
    } else {
        closeDatabase();
    }
    return true;
}

bool CPlaylist::openDatabase()
{
    /* 打开数据库 */
    if (!m_database.open()) {
        qCritical() << "Open database failed!";
        printSqlError();
        if (m_database.isValid()) {
            m_database.removeDatabase(g_strConnectName);
        }
        return false;
    }
    qDebug() << "Open database success!";
    return true;
}

void CPlaylist::closeDatabase()
{
    qDebug() << "关闭数据库连接！";
    if (m_database.isOpen()) {
        m_database.close();
        /* Avoid run warning"connection is still in use" */
        QString name;
        {
            name = QSqlDatabase::database().connectionName();
        } /* QSqlDatabase::database() will be deleted */
        QSqlDatabase::removeDatabase(name);
    }
}

void CPlaylist::printSqlError()
{
    if (m_database.isOpen()) {
        QSqlError sqlError = m_database.lastError();
        if (sqlError.isValid()) {
            qDebug() << "SQL error type:" << sqlError.type();
            qDebug() << "SQL error:" << sqlError.text();
        }
    } else {
        qDebug() << "SQL query error, the database is not open.";
    }
}

void CPlaylist::setTimerForParseDB()
{
    if (NULL != m_timer) {
        qDebug() << "Current time:" << QDateTime::currentDateTime().toString();
        qDebug() << "Set timer interval(minutes):" << g_timerPeriod / 1000 / 60;
        m_timer->start(g_timerPeriod);
    }
}

void CPlaylist::parsePlaylistFromDB()
{
    /* 如果version表中的HASH值没有变化，则只用解析默认播放文件 */
    if (didNeedUpdate()) {
        /* 解析数据库的整个过程一直加锁，避免播放模块同步异常 */
        /* 线程中访问共享变量，加解锁 */
        m_mutexPlaylist.lock();
        initGlobalPlaylist();
        parseDefaultList();
        parseDownloadedPlaylist();
        printParseResult();
        m_mutexPlaylist.unlock();

        setCurrentHourPlaylist();
        setUpdatePlaylistTimer();
        clearExpiredData();
    }
}

bool CPlaylist::didNeedUpdate()
{
    if (!openDatabase()) {
        return false;
    }

    bool bDidNeed = false;

    QSqlQuery query(m_database);
    QString strExec = QString("SELECT hash_ad FROM %1").arg(g_strVersionTableName);
    qDebug() << "Query:" << strExec;

    if (query.exec(strExec)) {
        QSqlRecord record = query.record();
        int indexHash = record.indexOf("hash_ad");
        qDebug() << "indexHash:" << indexHash;
        if (query.first()) {
            QString strHash = query.value(indexHash).toString();
            qDebug() << "Current hash:" << strHash;
            qDebug() << "Old hash:" << m_hashVersion;
            if (m_hashVersion == strHash) {
                qDebug() << "hash is the same!";
                closeDatabase();
            } else {
                qDebug() << "Parse database again!";
                m_hashVersion = strHash;
                bDidNeed = true;
            }
        } else {
            qWarning() << "The query result is empty!";
        }
    } else {
        printSqlError();
    }
    closeDatabase();

    if (!bDidNeed) {
        int currentHour = QDateTime::currentDateTime().time().hour();
        /* 需要在每天的0:00以后强制更新当天的播放列表并检查清理过期数据 */
        qDebug() << "Current time hour:" << currentHour << ":00";
        qDebug() << "Update time hour:" << g_hourForceUpdate << ":00";
        if (g_hourForceUpdate == currentHour) {
            qDebug() << "Last sync time:" << QDateTime::fromTime_t(m_lastUpdateTimestamp).toString();
            qDebug() << "Force to sync database time and clear the expired data!";
            bDidNeed = true;
        }
    }
    if (bDidNeed) {
        m_lastUpdateTimestamp = QDateTime::currentMSecsSinceEpoch() / 1000;
    }
    return bDidNeed;
}

void CPlaylist::stopTimer()
{
    if (NULL != m_timer) {
        if (m_timer->isActive()) {
            m_timer->stop();
        }
    }
}


bool CPlaylist::parseDefaultList()
{
    qDebug() << "Start to parse default playlist from database:";

    uint currentDayTimestamp = QDateTime(QDate::currentDate(), QTime(0, 0)).toTime_t();
    qDebug() << "Current time:" << QDateTime::currentDateTime().toString();
    qDebug() << "Current day timestamp:" << currentDayTimestamp;

    QString strCurrentDayTimestamp = "";
    QString strNextDayTimestamp = "";
    /* QString::setNum有多个重构函数，可以对uint进行转换 */
    strCurrentDayTimestamp = strCurrentDayTimestamp.setNum(currentDayTimestamp);
    strNextDayTimestamp = strNextDayTimestamp.setNum(currentDayTimestamp + 24 * 3600);

    if (!openDatabase()) {
        return false;
    }

    QSqlQuery query(m_database);
    /** 查询播放开始时间小于明天的起始时间,并且播放结束时间大于当天的起始时间的记录,这些记录被视为当天需要播放的记录
     * type=2代表主窗口的默认播放文件,type=3代表子窗口的默认播放文件 **/
    QString strExec = QString("SELECT md5, savepath, type, playtimestart, playtimeend "
                              "FROM %1 WHERE type in(2,3) "
                              "and playtimestart<%2 and playtimeend>%3")
            .arg(g_strADInfoTableName).arg(strNextDayTimestamp)
            .arg(strCurrentDayTimestamp);
    qDebug() << "Query:" << strExec;

    /* 取出需要当天需要播放的记录 */
    if (query.exec(strExec)) {

        QSqlRecord record;
        record = query.record();

        int indexStartTime = record.indexOf("playtimestart");
        int indexEndTime = record.indexOf("playtimeend");
        int indexSavePath = record.indexOf("savepath");
        int indexType = record.indexOf("type");
        int indexMD5 = record.indexOf("md5");
        qDebug() << "indexStartTime:" << indexStartTime;
        qDebug() << "indexEndTime:" << indexEndTime;
        qDebug() << "indexSavePath:" << indexSavePath;
        qDebug() << "indexType:" << indexType;
        qDebug() << "indexMD5:" << indexMD5;


        if (query.first()) {
            do {
                /* 播放开始时间 */
                uint startTime = query.value(indexStartTime).toUInt();
                /* 播放结束时间*/
                uint endTime = query.value(indexEndTime).toUInt();
                int type = query.value(indexType).toInt();
                /* 播放媒体路径 */
                QString strSavePath = query.value(indexSavePath).toString();

                /* 将时间戳转换为字符串输出 */
                QDateTime dateStartTime, dateEndTime;
                dateStartTime.setTime_t(startTime);
                dateEndTime.setTime_t(endTime);

                if (0 == query.at()) {
                    qDebug() << "The valid default play records of the day:";
                }
                qDebug() << strSavePath << ":" << type << ":" << startTime << "to" << endTime << ":"
                         << dateStartTime.toString() << "to" << dateEndTime.toString();

                /** the MD5 of the database is encrypted via DES **/
                QString strDESMD5 = query.value(indexMD5).toString();
                QByteArray byteArrayMD5 = "";
                CUtils::decryptDataByDES(QByteArray::fromHex(strDESMD5.toLocal8Bit()), g_strKeyDES, byteArrayMD5);
                QString strMD5Hex = byteArrayMD5.toHex();

                if (TYPE_MAIN_DEFAULT_PLAY == type) {
                    if (!g_mapMainPlaylistMD5.values().contains(strMD5Hex)) {
                        g_pPlaylistMainDefault->addMedia(QUrl::fromLocalFile(strSavePath));
                        g_mapMainPlaylistMD5.insert(strSavePath, strMD5Hex);
                    }
                }

            } while (query.next());
        } else {
            qWarning() << "The query result is empty.";
        }
        closeDatabase();
        return true;
    } else {
        closeDatabase();
        printSqlError();
        return false;
    }
}

bool CPlaylist::parseDownloadedPlaylist()
{
    qDebug() << "\n";
    qDebug() << "开始解析从服务器同步的播放列表";

    uint currentDayTimestamp = QDateTime(QDate::currentDate(), QTime(0, 0)).toTime_t();
    qDebug() << "Current time:" << QDateTime::currentDateTime().toString();
    qDebug() << "Current day timestamp:" << currentDayTimestamp;

    QString strCurrentDayTimestamp = "";
    QString strNextDayTimestamp = "";
    strCurrentDayTimestamp = strCurrentDayTimestamp.setNum(currentDayTimestamp);
    strNextDayTimestamp = strNextDayTimestamp.setNum(currentDayTimestamp + 24 * 3600);

    if (!openDatabase()) {
        return false;
    }

    QSqlQuery query(m_database);
    /** 查询播放开始时间小于明天的起始时间,并且播放结束时间大于当天的起始时间的记录,这些记录被视为当天需要播放的记录
     * type=0代表主窗口播放文件,type=1代表子窗口播放文件**/
    QString strExec = QString("SELECT md5, savepath, type, playtimestart, playtimeend "
                              "FROM %1 WHERE type=0 or type=1 and playtimestart<%2 and playtimeend>%3")
            .arg(g_strADInfoTableName).arg(strNextDayTimestamp)
            .arg(strCurrentDayTimestamp);
    qDebug() << "Query:" << strExec;

    /* 取出需要当天需要播放的记录 */
    if (query.exec(strExec)) {

        QSqlRecord record;
        record = query.record();

        int indexStartTime = record.indexOf("playtimestart");
        int indexEndTime = record.indexOf("playtimeend");
        int indexSavePath = record.indexOf("savepath");
        int indexType = record.indexOf("type");
        int indexMD5 = record.indexOf("md5");
        qDebug() << "indexStartTime:" << indexStartTime;
        qDebug() << "indexEndTime:" << indexEndTime;
        qDebug() << "indexSavePath:" << indexSavePath;
        qDebug() << "indexType:" << indexType;
        qDebug() << "indexMD5:" << indexMD5;

        /* 按照1天24小时,分24个时间段将播放媒体插入对应时间段的播放列表中 */
        for (int i = 0; i < 24; ++i) {
            if (query.first()) {
                do {
                    /* 播放开始时间 */
                    uint startTime = query.value(indexStartTime).toUInt();
                    /* 播放结束时间*/
                    uint endTime = query.value(indexEndTime).toUInt();
                    /* 主窗口还是子窗口的播放 */
                    int type = query.value(indexType).toInt();
                    /* 播放媒体路径 */
                    QString strSavePath = query.value(indexSavePath).toString();

                    /* 将时间戳转换为字符串输出 */
                    QDateTime dateStartTime, dateEndTime;
                    dateStartTime.setTime_t(startTime);
                    dateEndTime.setTime_t(endTime);

                    /* 只需要打印一遍即可 */
                    if (0 == i) {
                        if (0 == query.at()) {
                            qDebug() << "The valid playlist record of the day:";
                        }
                        qDebug() << strSavePath << ":" << type << ":" << startTime << "to" << endTime << ":"
                                 << dateStartTime.toString() << "to" << dateEndTime.toString();
                    }
                    /* 结束时间大于当前时间段的开始时间，并且开始时间小于当前时间段的结束时间 */
                    if ((endTime > (currentDayTimestamp + i * 3600))
                            && (startTime < (currentDayTimestamp + (i + 1) * 3600))) {

                        /** 从数据库取出来的MD5是通过DES加密后的数据,需要先解密 **/
                        QString strDESMD5 = query.value(indexMD5).toString();
                        QByteArray byteArrayMD5 = "";
                        CUtils::decryptDataByDES(QByteArray::fromHex(strDESMD5.toLocal8Bit()), g_strKeyDES, byteArrayMD5);

                        if (TYPE_MAIN_PLAY == type) {
                            g_pPlaylistMain[i].addMedia(QUrl::fromLocalFile(strSavePath));
                            if (!g_mapMainPlaylistMD5.contains(strSavePath)) {
                                g_mapMainPlaylistMD5.insert(strSavePath, QString(byteArrayMD5.toHex()));
                            }
                        }

                    }
                } while (query.next());
            } else {
                qWarning() << "查询到的结果为空！";
            }
        }
        closeDatabase();
        return true;
    } else {
        closeDatabase();
        qDebug() << "查询命令执行失败！";
        printSqlError();
        return false;
    }
}

void CPlaylist::printParseResult()
{
    if (!g_pPlaylistMainDefault->isEmpty()) {
        for (int j = 0; j < g_pPlaylistMainDefault->mediaCount(); ++j) {
            qDebug() << g_pPlaylistMainDefault->media(j).canonicalUrl().path();
        }
    } else {
    }

    for (int i = 0; i < 24; ++i) {
        if (!g_pPlaylistMain[i].isEmpty()) {
            for (int j = 0; j < g_pPlaylistMain[i].mediaCount(); ++j) {
                qDebug() << g_pPlaylistMain[i].media(j).canonicalUrl().path();
            }
        }
    }

    QMap<QString, QString>::const_iterator i;
    for (i = g_mapMainPlaylistMD5.constBegin(); i != g_mapMainPlaylistMD5.constEnd(); ++i) {
        qDebug() << i.key() << ":" << i.value();
    }
}

//void CPlaylist::syncPlayNewList()
//{
//    /* 发射信号给播放类,使其同步更新后的播放列表 */
//    emit signalPlaylistUpdated();
//}

void CPlaylist::clearExpiredData()
{
    qDebug() << "\n";
    qDebug() << "准备删除过期的数据";

    uint currentDayTimestamp = QDateTime(QDate::currentDate(), QTime(0, 0)).toTime_t();
    qDebug() << "Current time:" << QDateTime::currentDateTime().toString();
    qDebug() << "Current time of today:" << currentDayTimestamp;

    QString strCurrentDayTimestamp = "";
    strCurrentDayTimestamp = strCurrentDayTimestamp.setNum(currentDayTimestamp);

    if (!openDatabase()) {
        return;
    }

    QSqlQuery query(m_database);
    QString strExec = QString("SELECT gid, md5, savepath, symlink FROM %1 WHERE playtimeend<%2")
            .arg(g_strADInfoTableName).arg(strCurrentDayTimestamp);
    qDebug() << "Query:" << strExec;

    /* 取出已经过时的记录 */
    if (query.exec(strExec)) {
        QSqlRecord record;
        record = query.record();

        int indexGid = record.indexOf("gid");
        int indexSavePath = record.indexOf("savepath");
        int indexMD5 = record.indexOf("md5");
        int indexSymlink = record.indexOf("symlink");
        qDebug() << "IndexGid:" << indexGid;
        qDebug() << "indexSavePath:" << indexSavePath;
        qDebug() << "indexMD5:" << indexMD5;
        qDebug() << "indexSymlink:" << indexSymlink;

        while (query.next()) {
            QString strGid = query.value(indexGid).toString();
            QString strSavePath = query.value(indexSavePath).toString();
            QString strDESMD5 = query.value(indexMD5).toString();
            /* 0是软链接,1不是软链接 */
            int bIsSymlink = query.value(indexSymlink).toInt();

            bool bDidNeedDelete = true;

            if (!bIsSymlink) {
                /* 如果是实体文件，而不是符号链接，则对实体文件进行判断是否还有用 */
                QSqlQuery queryTemp(m_database);
                QString strExecTemp = QString("SELECT gid FROM %1 WHERE md5='%2' and playtimeend>%3 and symlink=1")
                        .arg(g_strADInfoTableName).arg(strDESMD5).arg(strCurrentDayTimestamp);
                qDebug() << strExecTemp;

                if (queryTemp.exec(strExecTemp)) {
                    if (queryTemp.first()) {
                        /* 实体文件还有作用，不能删除文件及记录 */
                        qDebug() << "记录中的实体文件需要保留使用，暂不能删除gid："
                                 << strGid << ", savepath:" << strSavePath;
                        bDidNeedDelete = false;
                    } else {
                        /* 实体文件已经不再被使用，可以删除文件及记录 */
                        qDebug() << "记录中的实体文件不再使用，可以删除gid："
                                 << strGid << ", savepath:" << strSavePath;;
                    }
                } else {
                    qDebug() << "查询有效播放记录失败！gid："
                             << strGid << ", savepath:" << strSavePath;
                    bDidNeedDelete = false;
                    printSqlError();
                }
            } else {
                /* 如果是符号链接，则直接删除记录即可 */
                qDebug() << "当前过期记录的文件是符号链接文件，将被删除gid："
                         << strGid << ", savepath:" << strSavePath;
            }

            if (bDidNeedDelete) {
                if (!QFile::remove(strSavePath)) {
                    qWarning() << "文件删除失败:" << strSavePath;
                    if (bIsSymlink) {
                        /** QFile::exsits()或file.exists()都不能判断符号链接文件是否存在，所以只好用间接的方法：
                         * 如果符号链接文件不存在，则QFile::symLinkTarget()返回的是空；存在返回实体文件的路径。
                         * 由于这里只考虑不存在的情况，所以存在的时候删除不成功时只能以后再尝试删除。*/
                        bool bIsNotExist = QFile::symLinkTarget(strSavePath).isEmpty();
                        if (bIsNotExist) {
                            qDebug() << "符号链接文件已经损坏或不存在，将删除其记录！";
                        } else {
                            continue;
                        }
                    } else {
                        continue;
                    }
                } else {
                    qDebug() << "删除文件成功！" << strSavePath;
                }

                QString strExecDelete = QString("DELETE FROM %1 WHERE savepath='%2'")\
                        .arg(g_strADInfoTableName).arg(strSavePath);
                qDebug() << "Query:" << strExecDelete;

                QSqlQuery queryDel(m_database);
                if (queryDel.exec(strExecDelete)) {

                } else {
                    printSqlError();
                }
            } else {
                ;
            }
        }
    } else {
        printSqlError();
    }
    closeDatabase();
}

QString CPlaylist::getSavePathFromRecord(QSqlQuery &query, QString &strDESMD5)
{
    QString strReturn = "";
    if (!query.isActive()) {
        return strReturn;
    }

    QSqlRecord record;
    record = query.record();
    int indexSavePath = record.indexOf("savepath");
    int indexMD5 = record.indexOf("md5");
    qDebug() << "indexSavePath:" << indexSavePath;
    qDebug() << "indexMD5:" << indexMD5;

    /* query.first等同第一次query.next */
    if (query.first()) {
        QString filePath = query.value(indexSavePath).toString();
        strDESMD5 = query.value(indexMD5).toString();
        qDebug() << "表中记录的播放文件路径为：" << filePath;
        qDebug() << "表中记录的DES MD5值为：" << strDESMD5;
        strReturn = filePath;
    } else {
        qWarning() << "查询到的结果为空！";
    }

    return strReturn;
}


void CPlaylist::setUpdatePlaylistTimer()
{
    if (NULL != m_timerUpdatePlaylist) {

        QTime dateTime = QDateTime::currentDateTime().time();
        int passedTimeMinutes = dateTime.msecsSinceStartOfDay() / 1000 / 60;
        int sleepSeconds = 0;

        if ((passedTimeMinutes % 60) > g_criticalValue) {
            sleepSeconds = (60 - passedTimeMinutes % 60 + 60) * 60;
        } else {
            sleepSeconds = (60 - passedTimeMinutes % 60) * 60;
        }

        qDebug() << "Current time" << QDateTime::currentDateTime().toString();
        qDebug() << "The time from now to next timer start:" << sleepSeconds / 60;

        m_timerUpdatePlaylist->start(sleepSeconds * 1000);
    }
}

void CPlaylist::setCurrentHourPlaylist()
{
    QTime dateTime = QDateTime::currentDateTime().time();
    int passedTimeMinutes = dateTime.msecsSinceStartOfDay() / 1000 / 60;

    qDebug() << "Current time:" << QDateTime::currentDateTime().toString();
    int playlistIndex = 0;

    // 23 = 24 - 1,24=24hours(one day)
    if (((passedTimeMinutes % 60) > g_criticalValue)
            && ((passedTimeMinutes / 60) < 23)) {
        playlistIndex = passedTimeMinutes / 60 + 1;
    } else {
        playlistIndex = passedTimeMinutes / 60;
    }
    qDebug("Current play time period:%d:00 ~ %d:00", playlistIndex, playlistIndex + 1);

    m_mutexPlaylist.lock();
    m_pCurrentPlaylist = &g_pPlaylistMain[playlistIndex];
    m_indexNextPlay = 0;
    m_indexNextPlayDefault = 0;
//    getNextValidMedia();
    m_mutexPlaylist.unlock();
}

QString CPlaylist::getNextValidMedia()
{
    m_mutexPlaylist.lock();

    int mediaCount = m_pCurrentPlaylist->mediaCount();
    bool bHasValidMedia = false;
    m_strNextPlayMediaPath = "";

    if (m_indexNextPlay >= m_pCurrentPlaylist->mediaCount()) {
        m_indexNextPlay = 0;
    }

    qDebug() << "The current playlist count:" << mediaCount;
    qDebug() << "The next play index to be searched:" << m_indexNextPlay;

    if (mediaCount > 0) {
        for (int i = m_indexNextPlay; i < mediaCount; ++i) {
            QString strMediaPath = m_pCurrentPlaylist->media(i).canonicalUrl().path();
            if (checkMediaValid(strMediaPath)) {
                m_indexNextPlay = i + 1;
                bHasValidMedia = true;
                m_strNextPlayMediaPath = strMediaPath;

//                int indexPosition = m_listMD5FailedMedia.indexOf(strMediaPath);
//                if (-1 != indexPosition) {
//                    m_listMD5FailedMedia.removeAt(indexPosition);
//                }
                break;
            } else {
//                m_listMD5FailedMedia.append(strMediaPath);
            }
        }

        if (!bHasValidMedia) {
            if (m_indexNextPlay > 0) {
                for (int j = 0; j < m_indexNextPlay; ++j) {
                    QString strMediaPath = m_pCurrentPlaylist->media(j).canonicalUrl().path();
                    if (checkMediaValid(strMediaPath)) {
                        m_indexNextPlay = j + 1;
                        bHasValidMedia = true;
                        m_strNextPlayMediaPath = strMediaPath;

//                        int indexPosition = m_listMD5FailedMedia.indexOf(strMediaPath);
//                        if (-1 != indexPosition) {
//                            m_listMD5FailedMedia.removeAt(indexPosition);
//                        }
                        break;
                    } else {
//                        m_listMD5FailedMedia.append(strMediaPath);
                    }
                }
            } else {

            }
        }
    }

    if (!bHasValidMedia) {
        getNextValidMediaDefault();
        if (!m_strNextPlayMediaPathDefault.isEmpty()) {
            m_strNextPlayMediaPath = m_strNextPlayMediaPathDefault;
         } else {
         }
    } else {
    }

    m_mutexPlaylist.unlock();
    return m_strNextPlayMediaPath;
}

void CPlaylist::getNextValidMediaDefault()
{
    int mediaCount = g_pPlaylistMainDefault->mediaCount();
    bool bHasValidMediaDefault = false;
    m_strNextPlayMediaPathDefault = "";

    if (m_indexNextPlayDefault >= mediaCount) {
        m_indexNextPlayDefault = 0;
    }

    if (mediaCount > 0) {
        for (int i = m_indexNextPlayDefault; i < mediaCount; ++i) {
            QString strMediaPath = g_pPlaylistMainDefault->media(i).canonicalUrl().path();
            if (checkMediaValid(strMediaPath)) {
                m_indexNextPlayDefault = i + 1;
                bHasValidMediaDefault = true;
                m_strNextPlayMediaPathDefault = strMediaPath;

//                int indexPosition = m_listMD5FailedMedia.indexOf(strMediaPath);
//                if (-1 != indexPosition) {
//                    /* MD5校验成功以后,将该媒体从MD5校验失败的列表中移除 */
//                    m_listMD5FailedMedia.removeAt(indexPosition);
//                }
                break;
            } else {
//                m_listMD5FailedMedia.append(strMediaPath);
            }
        }

        if (!bHasValidMediaDefault) {
            if (m_indexNextPlayDefault > 0) {
                for (int j = 0; j < m_indexNextPlayDefault; ++j) {
                    QString strMediaPath = g_pPlaylistMainDefault->media(j).canonicalUrl().path();
                    if (checkMediaValid(strMediaPath)) {
                        m_indexNextPlayDefault = j + 1;
                        bHasValidMediaDefault = true;
                        m_strNextPlayMediaPathDefault = strMediaPath;

//                        int indexPosition = m_listMD5FailedMedia.indexOf(strMediaPath);
//                        if (-1 != indexPosition) {
//                            m_listMD5FailedMedia.removeAt(indexPosition);
//                        }
                        break;
                    } else {
//                        m_listMD5FailedMedia.append(strMediaPath);
                    }
                }
            } else {

            }
        }
    }

    if (!bHasValidMediaDefault) {

    } else {

    }
}

bool CPlaylist::checkMediaValid(const QString &strMediaPath)
{
    return true;

    bool bReturn = false;

    if (strMediaPath.isEmpty()) {
        return false;
    }

    if (g_mapMainPlaylistMD5.contains(strMediaPath)) {

        QString strMD5Value = g_mapMainPlaylistMD5.find(strMediaPath).value();
        QString strMD5Calculated = CUtils::calculateMD5ForFile(strMediaPath);
        qDebug() << "MD5 from database:" << strMD5Value.toUpper();
        qDebug() << "Calculated MD5:" << strMD5Calculated.toUpper();

        if (!strMD5Value.isEmpty()) {
            if (strMD5Value.toUpper() == strMD5Calculated.toUpper()) {
                qDebug() << "MD5 check success!";
                bReturn = true;
            } else {
                qDebug() << "MD5 check failed!";
            }
        } else {
            qWarning() << "MD5 is empty!";
        }
    } else {
        qWarning() << "No MD5 of the file!";
    }
    return bReturn;
}


