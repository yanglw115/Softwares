import QtQuick 2.0
import QtQuick.Controls 1.2
import QtQuick.Controls.Styles 1.2

Rectangle {
    id: idRoot
    width: 1024
    height: 768
    color: sysPalette.window

    signal sigReturnToPlayVideo()

    /* 处理购买流程结束事件 */
    function f_handlePayFinished(objDestroy)
    {
        idRoot.sigReturnToPlayVideo()
        objDestroy.destroy()
    }

    SystemPalette {
        id: sysPalette
    }

    /* 总背景 */
    Image {
        id: image2
        anchors.fill: parent
        source: "photoes/photoes/styleSelectBG.jpg"
    }

    /* 支付方式选择提示文本 */
    Rectangle {
        id: rectPay
        width: 1024
        height: 200
        color: "#caacac"
        clip: false
        //        color: "black"
        Text {
            id: textPay
            z: 1
            text: "请选择支付方式"
            font.family: "WenQuanYi Micro Hei"
            height: 60
            color: "#741950"
            font.bold: true
            font.pointSize: 38
            anchors.centerIn: parent
        }

        Image {
            id: image1
            z: 0
            x: 156
            y: 43
            anchors.fill: parent
            source: "photoes/photoes/payStyleUpbackground.jpg"
        }
    }

    /* 支付方式选择，由RadioButton实现 */
    Column {

        width: parent.width
        height: 500
        anchors.top: rectPay.bottom
        anchors.topMargin: 100
        anchors.left: parent.left
        anchors.leftMargin: 340

        spacing: 60

        RadioButton {
            id: buttonPayByCoin
            text: qsTr("")
            style: RadioButtonStyle {
                indicator: Rectangle {
                        implicitWidth: 50
                        implicitHeight: 50
                        radius: 50
                        border.color: control.activeFocus ? "darkblue" : "gray"
                        border.width: 2
                        Rectangle {
                            id: rect1
                            anchors.fill: parent
                            visible: control.checked
//                            visible: false
                            color: "#21ad3b"
                            radius: 50
                            anchors.margins: 4
                        }
                }
                label: Item {
                    width: 200
                    height: 50
                    Text {
                        text: " 投币支付"
                        font.family: "WenQuanYi Micro Hei"
                        font.pointSize: 28
                        anchors.verticalCenter: parent.verticalCenter
                    }
                }
            }

            Component.onCompleted: {
                buttonPayByCoin.checked = true
            }

            onClicked: {
                if (buttonPayByCard.checked) {
                    buttonPayByCard.checked = false
                }
                buttonPayByCoin.checked = true
            }
         }

        RadioButton {
            id: buttonPayByCard
            style: RadioButtonStyle {
                indicator: Rectangle {
                        implicitWidth: 50
                        implicitHeight: 50
                        radius: 50
                        border.color: control.activeFocus ? "darkblue" : "gray"
                        border.width: 2
                        Rectangle {
                            id: rect2
                            anchors.fill: parent
                            visible: control.checked
//                            visible: false
                            color: "#21ad3b"
                            radius: 50
                            anchors.margins: 4
                        }
                }
                label: Item {
                    implicitWidth: 200
                    implicitHeight: 50
                    Text {
                        text: " 刷卡支付"
                        font.family: "WenQuanYi Micro Hei"
                        font.pointSize: 28
                        anchors.verticalCenter: parent.verticalCenter
                    }
                }
            }

            onClicked: {
                if (buttonPayByCoin.checked) {
                    buttonPayByCoin.checked = false
                }
                buttonPayByCard.checked = true
            }
        }

        /* ‘下一步’按钮，进入投币或刷卡等界面 */
        Rectangle {
            width: 300
            height: 60
            color: "#10861a"
            Text {
                text: "下一步"
                font.family: "WenQuanYi Micro Hei"
                font.pointSize: 28
                color: "black"
                anchors.centerIn: parent
            }

            MouseArea {
                anchors.fill: parent
                onClicked: {
                    var component
                    if (buttonPayByCoin.checked) {
                        component = Qt.createComponent("PayDetailByCoin.qml")
                        if (component.status === Component.Error) {
                            console.log("Error: ", component.errorString())
                            return
                        }
                        var payByCoin = component.createObject(idRoot)
                        if (payByCoin === null) {
                            console.log("Create payByCoin object failed!")
                            return
                        }
                        payByCoin.sigPayFinished.connect(f_handlePayFinished)
                    } else {
                        component = Qt.createComponent("PayDetailByCard.qml")
                        if (component.status === Component.Error) {
                            console.log("Error: ", component.errorString())
                            return
                        }
                        var payByCard = component.createObject(idRoot)
                        if (payByCard === null) {
                            console.log("Create payByCard object failed!")
                            return
                        }
                        payByCard.sigPayFinished.connect(f_handlePayFinished)
                    }
                }
            }
        }
    }
}

