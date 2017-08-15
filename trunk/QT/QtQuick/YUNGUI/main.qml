import QtQuick 2.4
import QtQuick.Window 2.2

Window {
    id: idRoot
    width: 1024
    height: 768
    visible: true

    /* 从视频播放进入商品列表购买界面 */
    function f_showShopping()
    {
        var component = Qt.createComponent("GoodsList.qml")
        if (component.status === Component.Error) {
            console.log("Error: ", component.errorString())
            return
        }
        var goodsList = component.createObject(idRoot)
        if (goodsList === null) {
            console.log("Create object failed:", component.errorString())
            return
        }
        goodsList.sigStartToPay.connect(f_showPayStyle)
        goodsList.sigGoToPlay.connect(f_goodsListToPlay)
    }

    function f_goodsListToPlay()
    {
        idPlayer.f_showAndPlay()
    }

    function f_showPayStyle()
    {
        console.log("start to pay ...")
        payStyleSelect.visible = true
    }

    /* 视频播放界面 */
    VideoPlay {
        id: idPlayer
        Component.onCompleted: {
            idPlayer.sigGotoShopping.connect(f_showShopping)
        }
    }

    /* 支付方式选择 */
    PayStyleSelection {
        id: payStyleSelect
        visible: false

        onSigReturnToPlayVideo: {
            payStyleSelect.visible = false
            idPlayer.f_showAndPlay()
        }
    }

    Component.onCompleted: {
//        idRoot.showFullScreen()
    }
}
