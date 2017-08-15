import QtQuick 2.0
import qt.YG 1.0

Rectangle {
    id: idRoot
    width: 1024
    height: 768

    signal sigPayFinished()

    function handleCardAcount(acount) /* acount: double */
    {
        if (acount < idTotalToPay.totalToPay) {
            idAcount.noteNotEnough = "(余额不足)"
        } else {
            idAcount.acount = acount
        }
    }

    function handleCardIn(bIsIn)
    {
    }

    GoodsManager {
        id: goodsManager
        Component.onCompleted: {
            sigGetCardMoney.connect(handleCardAcount)
            sigCardIn.connect(handleCardIn)
            idTotalToPay.totalToPay = parseFloat(getTotalToPay())
        }
    }

    Image {
        id: idImageLeft
        width: parent.width * 0.4
        height: parent.height
        //        source: "photoes/photoes/insertCard.jpg"
        source: "file:///D:/Qt/projects/YunGuiApplication/YunGuiApplication/photoes/insertCard.jpg"
        fillMode: Image.PreserveAspectCrop
    }

    Rectangle {
        id: idRectPay
        width: parent.width * 0.6
        height: parent.height
        anchors.left: idImageLeft.right
        Image {
            z: 0
            anchors.fill: parent
            //            source: "photoes/photoes/payUIDownBG.jpg"
            source: "file:///D:/Qt/projects/YunGuiApplication/YunGuiApplication/photoes/payUIDownBG.jpg"
            fillMode: Image.PreserveAspectCrop
        }

        Text {
            id: idAcount
            anchors.top: parent.top
            anchors.topMargin: 160
            anchors.left: parent.left
            anchors.leftMargin: 50
            property real acount: 0.00
            property string noteNotEnough: ""

            font.pointSize: 34
            text: "卡中余额: ¥" + acount + " " + noteNotEnough
        }

        Text {
            id: idTotalToPay
            property real totalToPay: 0.00
            anchors.top: idAcount.bottom
            anchors.topMargin: 100
            anchors.left: parent.left
            anchors.leftMargin: 50
            font.pointSize: 34
            text: "应付: ¥" + totalToPay
        }

        Rectangle {
            id: idButtonPay
            anchors.top: idTotalToPay.bottom
            anchors.topMargin: 120
            anchors.left: parent.left
            anchors.leftMargin: 60
            height: 60
            width: 200
            radius: 10
            color: "red"

            Text {
                font.pointSize: 24
                text: "确认付款"
                anchors.horizontalCenter: parent.horizontalCenter
                anchors.verticalCenter: parent.verticalCenter
            }

            MouseArea {
                anchors.fill: parent
                onClicked: {
                    var data = idAcount.acount - idTotalToPay.totalToPay
                    goodsManager.writeValidMoneyOfCard(data)
                    goodsManager.refreshSoldGoodsInfoToDB()
                    goodsManager.stopToPay()
                    sigPayFinished()
                    idRoot.destroy()
                }
            }
        }

        Rectangle {
            id: idButtonCancel
            anchors.top: idTotalToPay.bottom
            anchors.topMargin: 120
            anchors.left: idButtonPay.right
            anchors.leftMargin: 60
            anchors.verticalCenter: idButtonPay.verticalCenter
            radius: 10
            color: "red"
            height: 60
            width: 200

            Text {
                font.pointSize: 24
                text: "取消"
                anchors.verticalCenter: parent.verticalCenter
                anchors.horizontalCenter: parent.horizontalCenter
            }

            MouseArea {
                anchors.fill: parent
                onClicked: {
                    goodsManager.stopToPay()
                    sigPayFinished()
                    idRoot.destroy()
                }
            }
        }

        Rectangle {
            width: 200
            height: 60
            radius: 20
            color: "green"
            anchors.top: idButtonCancel.bottom
            anchors.topMargin: 100
            anchors.left: idRectPay.left
            anchors.leftMargin: 160

            Text {
                text: "充值: 200"
                anchors.verticalCenter: parent.verticalCenter
                anchors.horizontalCenter: parent.horizontalCenter
                font.pointSize: 24
            }

            MouseArea {
                anchors.fill: parent
                onClicked: {
                    goodsManager.writeValidMoneyOfCard(200.00)
                    goodsManager.checkCardInStatus()
                }
            }
        }
    }

    Component.onCompleted: {
        console.log("Pay detail by card page load finished.")
        goodsManager.setPayStyle(1)
        goodsManager.startToPay()
    }

}

