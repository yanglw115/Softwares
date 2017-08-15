#ifndef CPLAYLISTMANAGER_H
#define CPLAYLISTMANAGER_H

#include <QThread>
#include "YG_playlist.h"

class CPlaylistManager: public QObject
{
    Q_OBJECT
public:
    explicit CPlaylistManager(QObject *parent = NULL);
    ~CPlaylistManager();
    Q_INVOKABLE QString getNextPlayMediaPath();

signals:
    /* 当播放列表初始化完成之后发射该信号，只有收到该信号之后才可获取播放列表内容 */
    void sigInitFinished();

public slots:
    /* Used for QML, start to play video */
    /* 该信号与播放列表类进行绑定。播放列表管理类在QML中进行实例化。 */
    void slotInitFinished();

public:
    QString m_strPlayUrl; /* 下一个要播放的媒体内容 */

private:
    QThread *m_pThread; /* 将播放列表的同步操作放在线程中去实现 */
    CPlaylist *m_pCPlaylist; /* 播放列表类 */

};

#endif // CPLAYLISTMANAGER_H
