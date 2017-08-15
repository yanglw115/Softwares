/******************************************************************************
 * Copyright(c) 2016 Daboo Technology Co.,Ltd.
 *
 * @FileName: YG_playlist.h
 * @Author: yangliwei
 * @Version: 1.0
 * @Date: 2016.3.14
 * @Description: 播放列表同步机制。最新的播放列表由升级进程周期性从服务器同步，然后写入数据
 * 库，应用程序周期性从数据库中同步最新的播放列表，然后触发播放器同步。
 * @Others:
 * @History:
 *     1、Date: 2016.3.14
 *        Author: yangliwei
 *        Modification: 实现基本功能
*****************************************************************************/

#ifndef _YG_PLAYLIST_H_
#define _YG_PLAYLIST_H_

//#include <QList>
#include <QMediaPlaylist>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QTimer>
#include <QMutex>

class CPlaylist: public QObject
{
    Q_OBJECT
    enum enumPlayType {
        TYPE_MAIN_PLAY = 0, /* 商家发布的播放媒体 */
        TYPE_MAIN_DEFAULT_PLAY = 2, /* 默认播放媒体 */
    };
public:
    explicit CPlaylist(QObject *parent = 0);
    ~CPlaylist();
    QString getNextValidMedia();

signals:
//    void signalPlaylistUpdated();
    void signalPlaylistInitFinished();

public slots:
    void slotStartPlaylistThread();
    void slotUpdateCurrentPlaylist();

private slots:
    /**************************************************************************
     * Function: slotTimeoutHandler
     * Description: 定时器槽函数，由此槽函数处理数据库的播放列表同步
     * Input: 无
     * Output: 无
     * Return: 空
     *************************************************************************/
    void slotTimeoutHandler();

private:
    /**************************************************************************
     * Function: initGlobalPlaylist
     * Description: 初始化全局播放列表，主要是清空所有的播放列表，设置播放列表的播放模式等
     * Input: 无
     * Output: 无
     * Return: 空
     *************************************************************************/
    void initGlobalPlaylist();

    /**************************************************************************
     * Function: initDatabase
     * Description: 初始化数据库，查询数据库文件是否存在，设置数据库的名称及连接用户名和密码
     * Input: 无
     * Output: 无
     * Return: 成功返回真； 失败返回假
     *************************************************************************/
    bool initDatabase();

    /**************************************************************************
     * Function: openDatabase
     * Description: 打开数据库。对于每个sql语句使用前都尽可能打开一次，之后立即关闭
     * Input: 无
     * Output: 无
     * Return: 成功返回真； 失败返回假
     *************************************************************************/
    bool openDatabase();

    /**************************************************************************
     * Function: closeDatabase
     * Description: 关闭数据库的连接
     * Input: 无
     * Output: 无
     * Return: 空
     *************************************************************************/
    void closeDatabase();

    /**************************************************************************
     * Function: printSqlError
     * Description: 输出sql语句执行出错时的内容
     * Input: 无
     * Output: 无
     * Return: 空
     *************************************************************************/
    void printSqlError();

    /**************************************************************************
     * Function: didNeedUpdate
     * Description: 在解析从服务器获取的播放列表前，先判断播放列表是否更新过，如果没有更新
     * 则不需要解析数据库，否则进行解析
     * Input: 无
     * Output: 无
     * Return: 数据库中播放列表更新过，则返回为真；否则返回假
     *************************************************************************/
    bool didNeedUpdate();

    /**************************************************************************
     * Function: setDefaultPlaylist
     * Description: 在数据库文件不存在或无法打开的情况下，设置播放内容为最原始默认的两个播放
     * 文件
     * Input: 无
     * Output: 无
     * Return: 设置成功返回真； 失败返回假
     *************************************************************************/
    bool setDefaultPlaylist(enumPlayType type);

    void stopTimer();

    /**************************************************************************
     * Function: setTimerForParseDB
     * Description: 设置定时器，用于计算何时下一次进行数据库播放列表的解析
     * Input: 无
     * Output: 无
     * Return: 无
     *************************************************************************/
    void setTimerForParseDB();

    /**************************************************************************
     * Function: parsePlaylistFromDB
     * Description: 从数据库中解析播放列表的整体控制方法
     * Input: 无
     * Output: 无
     * Return: 无
     *************************************************************************/
    void parsePlaylistFromDB();

    /**************************************************************************
     * Function: parseDefaultlist
     * Description: 从数据库中解析默认播放列表
     * Input: 无
     * Output: 无
     * Return: 成功则返回真；失败则返回假
     *************************************************************************/
    bool parseDefaultList();

    /**************************************************************************
     * Function: parseDownloadedList
     * Description: 从数据库中解析从服务器同步的播放列表
     * Input: 无
     * Output: 无
     * Return: 成功则返回真；失败则返回假
     *************************************************************************/
    bool parseDownloadedPlaylist();

    /**************************************************************************
     * Function: printParseResult
     * Description: 输出所有的解析结果
     * Input: 无
     * Output: 无
     * Return: 无
     *************************************************************************/
    void printParseResult();

    /**************************************************************************
     * Function: syncPlayNewList
     * Description: 向播放模块发射同步最新播放列表的信号
     * Input: 无
     * Output: 无
     * Return: 无
     *************************************************************************/
//    void syncPlayNewList();

    /**************************************************************************
     * Function: clearExpiredData
     * Description: 删除数据库中过期的播放记录及其对应的播放文件
     * Input: 无
     * Output: 无
     * Return: 无
     *************************************************************************/
    void clearExpiredData();

    /**************************************************************************
     * Function: getSavePathFromRecord
     * Description: 从查询结果中获取文件保存路径
     * Input: 查询对象
     * Output: 从数据库中查询到的对应文件MD5值
     * Return: 文件保存路径
     *************************************************************************/
    QString getSavePathFromRecord(QSqlQuery &, QString &strMd5);

    void setCurrentHourPlaylist();
    void setUpdatePlaylistTimer();
    void getNextValidMediaDefault();
    bool checkMediaValid(const QString &strMediaPath);

public:
    QString m_strNextPlayMediaPath;

private:
    QSqlDatabase m_database;        /* sql数据库对象 */
    QTimer *m_timer;                /* 用于周期性同步的定时器 */
    ulong m_lastUpdateTimestamp;    /* 上次同步的时间戳 */
    QString m_hashVersion;          /* version表中的hash值记录 */

    QMutex m_mutexPlaylist;
    QTimer *m_timerUpdatePlaylist;
    QMediaPlaylist *m_pCurrentPlaylist;
    int m_indexNextPlay;
    int m_indexNextPlayDefault;
    QString m_strNextPlayMediaPathDefault;
    QList<QString> m_listMD5FailedMedia;
    QList<QString> m_pMapMD5;
};

#endif // _YG_PLAYLIST_H_
