import QtQuick 2.0
import QtQuick.Controls 1.2
import qt.YG 1.0

Rectangle {
    id: idRoot
    width: 412
    height: 600
    color: sysPalette.window

    property alias myModel: idListView.model

    signal sigStartToPay()

    /* 将选购的商品信息添加到C++中的数据缓冲中，便入后续记录入数据库或上传给服务器 */
    function f_saveSelectedInfo()
    {
        goodsManager.setSelectedGoods(-1, 0, true, false) // start

        var index = 0
        var selCount = 0
        for (var i = 0; i < idListView.model.count; ++i) {
            index = idListView.model.get(i).index
            selCount = parseInt(idListView.model.get(i).selectCount)
            if (selCount > 0) {
                goodsManager.setSelectedGoods(index, selCount, false, false)
            }
        }

        goodsManager.setSelectedGoods(-1, 0, false, true) //end
    }

    /* 计算总价，以刷新总计的显示 */
    function f_setTotalPrice()
    {

        var index = 0
        var selCount = 0
        var price = 0
        var valueTotal = 0
        for (var i = 0; i < idListView.model.count; ++i) {
            index = idListView.model.get(i).index
            selCount = parseInt(idListView.model.get(i).selectCount)
            if (selCount > 0) {
                price = parseInt(idListView.model.get(i).price)
                valueTotal += selCount * price
            }
        }

        if (valueTotal > 0) {
            textTotalPriceValue.text = String(valueTotal)
            if (!rectTotalPrice.visible) {
                rectTotalPrice.visible = true
            }
            if (!buttonBuy.visible) {
                buttonBuy.visible = true
            }
        } else {
            textTotalPriceValue.text = String(0)
            rectTotalPrice.visible = false
            buttonBuy.visible = false
        }
    }

    SystemPalette {
        id: sysPalette
    }

    GoodsManager {
        id: goodsManager
    }

    Text {
        id: textShoppingCart
        width: 120
        text: "我的购物车"
        font.family: "WenQuanYi Micro Hei"
        font.pointSize: 16
        horizontalAlignment: Text.AlignHCenter
    }

    Rectangle {
        id: redLine
        width: parent.width
        height: 2
        anchors.top: textShoppingCart.bottom
        anchors.topMargin: 2
        color: "red"
    }

    /* 购物车中商品详细信息标题 */
    Rectangle {
        id: rectTile
        width: parent.width
        anchors.top: redLine.bottom
        anchors.topMargin: 2
        height: 24
        Row {
            Text {
                width: 120
                font.pointSize: 14
                font.family: "WenQuanYi Micro Hei"
                text: "商品"
                horizontalAlignment: Text.AlignHCenter
            }

            Rectangle {
                width: 1
                height: 20
                color: "red"
            }

            Text {
                width: 100
                font.pointSize: 14
                font.family: "WenQuanYi Micro Hei"
                text: "单价"
                horizontalAlignment: Text.AlignHCenter
            }

            Rectangle {
                width: 1
                height: 20
                color: "red"
            }

            Text {
                width: 120
                font.pointSize: 14
                font.family: "WenQuanYi Micro Hei"
                text: "购买数量"
                horizontalAlignment: Text.AlignHCenter
            }

            Rectangle {
                width: 1
                height: 20
                color: "red"
            }

            Text {
                width: 60
                font.pointSize: 14
                font.family: "WenQuanYi Micro Hei"
                text: "操作"
                horizontalAlignment: Text.AlignHCenter
            }
        }
    }

    /* 在ListView中展示加入购物车中的商品信息 */
    ListView {
        id: idListView
        width: parent.width
        anchors.top: rectTile.bottom
        anchors.topMargin: 1
        height: 460
        visible: true

        /* 这里定义的model，直接用ListModel，并没有使用已经定义好的model，因为这里model中的元素是使用append动态生成的 */
        model: ListModel { id: idModel }
        /* 这里使用GoodsInCart.qml中定义的对象作为代理 */
        delegate:  GoodsInCart {
            id: idDelegate
            source: model.image
            name: model.name
            price: model.price
            selCount: model.selectCount
            maxCount: model.leftCount

            onSigRemoveMe: {
                idModel.remove(ListView.currentIndex)
                if (idModel.count === 0) {
                    textTotalPriceValue.text = String(0)
                    rectTotalPrice.visible = false
                    buttonBuy.visible = false
                }
            }

            onSelCountChanged: {
                /* 如果在内部通过+/-去操作，则model的值反而没有变，所以才有sigAddOne和sigDelOne */
                f_setTotalPrice()
            }

            onSigAddOne: {
                var count = parseInt(idModel.get(ListView.currentIndex).selectCount) + 1
                idModel.get(ListView.currentIndex).selectCount = count
                selCount = count
                /* 如果不使用f_setTotalPrice接口，上面model值改变后delegate并不会触发selCountChanged信号
                    但是调用onSigAddOne之后，在GoodsList中设置model的值时，delegate的值却又不变了，这里有些矛盾
                    为了解决问题，暂时在GoodsList中屏蔽掉第二次设置同样model item的值，同时不调用f_setTotalPrice*/
//                f_setTotalPrice()
            }

            onSigDelOne: {
                var count = parseInt(idModel.get(ListView.currentIndex).selectCount) - 1
                idModel.get(ListView.currentIndex).selectCount = count
                selCount = count
//                f_setTotalPrice()
            }
        }
    }

    /* 总计的展示 */
    Rectangle {
        id: rectTotalPrice
        width: parent.width
        height: 30
        anchors.top: idListView.bottom
        anchors.topMargin: 20
        color: "#cfcfcf"
        visible: false

        Text {
            id: textTotalPrice
            anchors.right: rectTotalPrice.right
            anchors.rightMargin: 80
            font.bold: true
            font.pointSize: 16
            font.family: "WenQuanYi Micro Hei"
            text: qsTr("总计: <font color='#a22222'>¥</font>")
            anchors.verticalCenter: parent.verticalCenter
        }

        Text {
            id: textTotalPriceValue
            color: "#ff0707"
            anchors.left: textTotalPrice.right
            anchors.leftMargin: 2
            font.bold: true
            font.pointSize: 14
            font.family: "WenQuanYi Micro Hei"
            text: "0.0"
            anchors.verticalCenter: parent.verticalCenter
        }
    }

    /* 开始付款按钮 */
    Rectangle {
        id: buttonBuy
        width: 120
        height: 40
        anchors.top: rectTotalPrice.bottom
        anchors.topMargin: 20
        anchors.right: parent.right
        anchors.rightMargin: 10
        border.color: "#b3b3b3"
        color: "#368f15"
        radius: 10
        visible: false

        Text {
            color: "white"
            text: "开始付款"
            font.pointSize: 14
            font.bold: true
            font.family: "WenQuanYi Micro Hei"
            anchors.centerIn: parent
        }

        MouseArea {
            anchors.fill: parent
            onClicked: {
                idRoot.sigStartToPay()
                f_saveSelectedInfo()
            }
        }
    }

}

