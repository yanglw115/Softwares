#include "YG_playlist_manager.h"

CPlaylistManager::CPlaylistManager(QObject *parent)
    : QObject(parent)
    , m_pThread(NULL)
    , m_pCPlaylist(NULL)
{
    m_pThread = new QThread(this);
    m_pCPlaylist = new CPlaylist;

    m_pCPlaylist->moveToThread(m_pThread);
    this->connect(m_pThread, SIGNAL(started()), m_pCPlaylist, SLOT(slotStartPlaylistThread()));
    this->connect(m_pCPlaylist, SIGNAL(signalPlaylistInitFinished()), this, SLOT(slotInitFinished()));
    m_pThread->start();
}

CPlaylistManager::~CPlaylistManager()
{

}

/**************************************************************************
 * Function: getNextPlayMediaPath
 * Description: 获取下一个将要播放的视频内容
 * Input: 无
 * Output: 无
 * Return: 返回播放媒体所在路径
 *************************************************************************/
QString CPlaylistManager::getNextPlayMediaPath()
{
    QString strReturn = "file:///"; /* QML有时候需要使用该前缀来区分qrc资源 */
    strReturn += m_pCPlaylist->getNextValidMedia();
    qDebug() << "Next media to play:" << strReturn;
    return strReturn;
}

/**************************************************************************
 * Function: slotInitFinished
 * Description: 当播放列表初始化完成之后，调用该槽函数发射相关信号给QML界面
 * Input: 无
 * Output: 无
 * Return: 无
 *************************************************************************/
void CPlaylistManager::slotInitFinished()
{
    emit sigInitFinished();
}

