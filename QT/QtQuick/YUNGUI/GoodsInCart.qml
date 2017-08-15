import QtQuick 2.0

/* 在购物车的单个商品信息 */
Rectangle {
    id: idRoot
    width: 416
    height: 40
    color: "#ffe8e8"
    border.color: "#979090"
    visible: true

    property alias source: goodsImage.source
    property alias name: goodsName.text
    property alias price: goodsPrice.text
    property alias selCount: countNumber.text
    property string maxCount: "0"

    /* 从购物车中移除 */
    signal sigRemoveMe()
    /* 在购物车中增加/减少选购数量 */
    signal sigAddOne()
    signal sigDelOne()

    /* 商品图片 */
    Image {
        id: goodsImage
        width: 30
        height: 30
        anchors.left: parent.left
        anchors.leftMargin: 10
        anchors.verticalCenter: parent.verticalCenter
        source: ""
    }

    /* 商品名称 */
    Text {
        id: goodsName
        width: 80
        anchors.left: goodsImage.right
        anchors.verticalCenter: goodsImage.verticalCenter
        text: "my name"
        font.family: "WenQuanYi Micro Hei"
        horizontalAlignment: Text.AlignHCenter
    }

    Rectangle {
        width: 1
        anchors.left: goodsName.right
        height: 40
        color: "red"
    }

    /* 商品价格 */
    Text {
        id: goodsPrice
        width: 100
        anchors.left: goodsName.right
        anchors.verticalCenter: goodsName.verticalCenter
        text: ""
        horizontalAlignment: Text.AlignHCenter
        font.pointSize: 14
        font.family: "WenQuanYi Micro Hei"
    }

    Rectangle {
        anchors.left: goodsPrice.right
        width: 1
        height: 40
        color: "red"
    }

    /* 商品购物数量，可以通过+/-进行修改 */
    Rectangle {
        id: shoppingCount
        width: 120
        anchors.left: goodsPrice.right
        anchors.verticalCenter: goodsPrice.verticalCenter
        Row {
            anchors.verticalCenter: parent.verticalCenter
            anchors.horizontalCenter: parent.horizontalCenter
            spacing: 3
            Rectangle {
                width: 30
                height: 30
                border.color: "black"
                Text {
                    id: countDec
                    anchors.verticalCenter: parent.verticalCenter
                    anchors.horizontalCenter: parent.horizontalCenter
                    text: qsTr("-")
                    font.family: "WenQuanYi Micro Hei"
                    font.pointSize: 18
                }
                MouseArea {
                    anchors.fill: parent
                    onClicked: {
                        var count = parseInt(countNumber.text)
                        if (count > 0) {
//                            countNumber.text = String(count - 1)
                            sigDelOne()
                        }
                    }
                }
            }
            Rectangle {
                width: 40
                height: 30
                border.color: "black"
                Text {
                    id: countNumber
                    font.pointSize: 18
                    font.family: "WenQuanYi Micro Hei"
                    anchors.verticalCenter: parent.verticalCenter
                    anchors.horizontalCenter: parent.horizontalCenter
                    text: qsTr("0")
                }
            }
            Rectangle {
                width: 30
                height: 30
                border.color: "black"
                Text {
                    id: countInc
                    anchors.verticalCenter: parent.verticalCenter
                    anchors.horizontalCenter: parent.horizontalCenter
                    text: qsTr("+")
                    font.pointSize: 18
                    font.family: "WenQuanYi Micro Hei"
                }

                MouseArea {
                    anchors.fill: parent
                    onClicked: {
                        var count = parseInt(countNumber.text)
                        if (count < parseInt(maxCount)) {
//                            countNumber.text = String(count + 1)
                            sigAddOne()
                        }
                    }
                }
            }
        }
    }

    Rectangle {
        width: 1
        anchors.left: shoppingCount.right
        height: 40
        color: "red"
    }

    /* 删除操作，从购物车中删除选购商品 */
    Text {
        id: shoppingOperate
        width: 60
        anchors.left: shoppingCount.right
        anchors.verticalCenter: shoppingCount.verticalCenter
        text: "删除"
        font.pointSize: 14
        font.family: "WenQuanYi Micro Hei"
        horizontalAlignment: Text.AlignHCenter

        MouseArea {
            anchors.fill: parent
            onClicked: {
                sigRemoveMe()
            }
        }
    }

}
