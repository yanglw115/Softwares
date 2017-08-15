#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QtQml>

#include "YG_goods_manager.h"
#include "YG_goods_info.h"
#include "YG_playlist_manager.h"

#if _MSC_VER >= 1600
#pragma execution_character_set("utf-8")
#endif

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);

    /* 将商品信息类的实现放在子线程中去实现，避免主线程的方法阻塞 */
    if (!createGoodsInfoInstance()) {
        return -1;
    }

    /* 注册两个QML组件类型，以在QML中进行实例化使用。一个是商品管理，另一个是播放列表的管理 */
    qmlRegisterType<CPlaylistManager>("qt.YG", 1, 0, "PlaylistManager");
    qmlRegisterType<CGoodsManager>("qt.YG", 1, 0, "GoodsManager");

    QQmlApplicationEngine engine;
    engine.clearComponentCache();
    /* 通过QQmlApplicationEngine加载QML的主界面 */
    engine.load(QUrl(QStringLiteral("qrc:/main.qml")));

    return app.exec();
}
