import QtQuick 2.0
import qt.YG 1.0

Rectangle {
    id: idRoot
    width: 1024
    height: 768

    signal sigPayFinished(QtObject objDestroy);

    function f_handleMoneyIn(money)
    {
        var value = parseInt(textInputValue.text)
        value += money
        textInputValue.text = String(value)
    }

    GoodsManager {
        id: goodsManager
        Component.onCompleted: {
            onSigGetMoney.connect(f_handleMoneyIn)
        }
    }

    Image {
        id: rootBG
        anchors.fill: parent
        source: "photoes/photoes/payUIDownBG.jpg"
    }

    Rectangle {
        id: rectHint
        width: parent.width
        height: 150
        Text {
            z: 1
            text: "请投币"
            font.family: "WenQuanYi Micro Hei"
            font.pointSize: 46
            anchors.centerIn: parent
        }

        Image {
            id: imageHint
            fillMode: Image.TileVertically
            anchors.rightMargin: 0
            anchors.bottomMargin: 0
            anchors.leftMargin: 0
            anchors.topMargin: 0
            z: 0
            anchors.fill: parent
            source: "photoes/photoes/payUIUpBG.jpg"
        }
    }

    Column {
        id: coinColumn
        width: 500
        height: 300
        anchors.top: rectHint.bottom
        anchors.topMargin: 100
//        anchors.verticalCenter: parent.verticalCenter
        anchors.horizontalCenter: parent.horizontalCenter
        spacing: 20

        Item {
            id: rect1
            width: 300
            height: 80
            Text {
                id: textTotal
                text: "合计：<font color='#a22222'>¥</font>"
                font.pointSize: 24
                font.family: "WenQuanYi Micro Hei"
                anchors.verticalCenter: parent.verticalCenter
            }
            Text {
                id: textTotalValue
                anchors.left: textTotal.right
                anchors.leftMargin: 10
                anchors.verticalCenter: parent.verticalCenter
                text: ""
                font.pointSize: 24
            }
        }

        Item {
            id: rect2
            width: 300
            height: 80
            Text {
                id: textInput
                anchors.verticalCenter: parent.verticalCenter
                text: "已支付：<font color='#a22222'>¥</font>"
                font.pointSize: 24
                font.family: "WenQuanYi Micro Hei"
            }
            Text {
                id: textInputValue
                anchors.left: textInput.right
                anchors.leftMargin: 10
                anchors.verticalCenter: parent.verticalCenter
                text: "0"
                font.pointSize: 24
                font.family: "WenQuanYi Micro Hei"

                onTextChanged: {
                    if (parseInt(textInputValue.text) >= parseInt(textTotalValue.text)) {
                        rectOK.opacity = 1.0
                        rectOK.enabled = true
                        var output = parseInt(textInputValue.text) - parseInt(textTotalValue.text)
                        textOutputValue.text = String(output)
                    }
                }
            }
        }

        Item {
            id: rect3
            width: 300
            height: 80
            Text {
                id: textOutput
                anchors.verticalCenter: parent.verticalCenter
                text: "将找零：<font color='#a22222'>¥</font>"
                font.pointSize: 24
            }
            Text {
                id: textOutputValue
                anchors.left: textOutput.right
                anchors.leftMargin: 10
                anchors.verticalCenter: parent.verticalCenter
                text: "0"
                font.pointSize: 24
                font.family: "WenQuanYi Micro Hei"

                onTextChanged: {

                }
            }
        }
    }

    Rectangle {
        id: rectOK
        width: 200
        height: 60
        color: "red"
        anchors.top: coinColumn.bottom
        anchors.topMargin: 80
        anchors.left: parent.left
        anchors.leftMargin: 240
        opacity: 0.3
        enabled: false
        Text {
            text: "确认付款"
            font.pointSize: 30
            font.family: "WenQuanYi Micro Hei"
            color: "white"
            anchors.centerIn: parent
        }

        MouseArea {
            anchors.fill: parent
            onClicked: {
                /* 出货中和出货完毕界面展示... */
                var change = parseInt(textOutputValue.text)
                if (change > 0) {
                    goodsManager.startOddChange(change)
                }
                goodsManager.refreshSoldGoodsInfoToDB()
                goodsManager.stopToPay()
            }
        }
    }

    Rectangle {
        id: rectCancle
        width: 200
        height: 60
        color: "#0f8f20"
        anchors.top: coinColumn.bottom
        anchors.topMargin: 80
        anchors.left: rectOK.right
        anchors.leftMargin: 30
        Text {
            text: "取消"
            font.pointSize: 30
            font.family: "WenQuanYi Micro Hei"
            anchors.centerIn: parent
        }

        MouseArea {
            anchors.fill: parent
            onClicked: {
                /* 取消的相关提示并返回到播放主界面 */
                goodsManager.stopToPay()
                idRoot.sigPayFinished(idRoot)
            }
        }
    }

    Component.onCompleted: {
        goodsManager.setPayStyle(0)
        goodsManager.startToPay()
        textTotalValue.text = goodsManager.getTotalToPay()
    }
}

