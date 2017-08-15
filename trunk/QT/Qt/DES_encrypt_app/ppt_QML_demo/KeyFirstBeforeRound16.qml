import QtQuick 2.3
import QtQuick.Window 2.2


Rectangle {
    id: idRoot
    width: 1440
    height: 900
    visible: true
    color: sysPalette.window
    property alias idGridKey_28_2: idGridKey_28_2
    property var arrayPC1: [27, 31, 38, 50, 16, 19, 58, 24,
        44,  7, 63, 54, 40, 62, 46, 10,
        41, 29, 15, 39, 55, 12, 30,  9,
        42, 49, 17,  3, 22, 47,  8, 48,
        5, 33, 11, 20, 37, 21, 60, 28,
        23, 32, 51, 61, 13, 36, 14,  2,
        25, 43, 59, 56,  6, 34,  1, 26 ]

    signal sigGotoKeyRound16()
    signal sigSetKeySecondOrig56()
    signal sigGotoDataFirst()

    SystemPalette {
        id: sysPalette
    }

    /* 原始密钥的输入及对应二进制数组展示 */
    ArrayGridBin {
        id: idKeyOrig
        x: 100
        y: 80
        strInputLabel: "原始密钥："
        strPlaceHolderText: "Key"
        strInputData: "12345678"
    }

    /* 放在原始密钥右边的箭头，点击出现28*2的空数组 */
    Image {
        id: idArrowR_OrigKey
        width: 60
        height: 60
        anchors.horizontalCenter: idArrowR_PC1.horizontalCenter
        anchors.verticalCenter: idKeyOrig.verticalCenter
        source: "qrc:arrowR.png"
        fillMode: Image.PreserveAspectCrop
        visible: false

        MouseArea {
            anchors.fill: parent
            onClicked: {
                idGridKey_28_2.visible = true
                idArrowU_PC1_28_2.visible = true
            }
        }
    }

    /* 56位初始置换表PC1对应二维表展示 */
    ArrayGridInt {
        id: idPC1
        anchors.top: idKeyOrig.bottom
        anchors.topMargin: 100
        anchors.right: idKeyOrig.right
        title: "PC1(56位密钥置换表8*7)"
        maxRow: 7
        visible: true
        Component.onCompleted: {
            listModel.clear()
            for (var i = 0; i < arrayPC1.length; ++i) {
                listModel.append({"value": arrayPC1[i].toString(), "eColor": "white"})
            }
        }
    }

    /* 放在PC1旁边的箭头，点击出现P1对应的28*2的数组 */
    Image {
        id: idArrowR_PC1
        width: 60
        height: 60
        anchors.left: idPC1.right
        anchors.leftMargin: 20
        anchors.verticalCenter: idPC1.verticalCenter
        //        anchors.bottom: idPC1.bottom
        //        anchors.bottomMargin: 20
        source: "qrc:arrowR.png"
        fillMode: Image.PreserveAspectCrop

        MouseArea {
            anchors.fill: parent
            onClicked: {
                idGridPC1_28_2.visible = true
                idArrowR_OrigKey.visible = true
            }
        }
    }

    /* 用于存放56位密钥的28*2的数组 */
    ArrayGridInt {
        id: idGridKey_28_2
        //        anchors.top: idKeyOrig.top
        anchors.left: idArrowR_OrigKey.right
        anchors.leftMargin: 20
        anchors.verticalCenter: idArrowR_OrigKey.verticalCenter
        maxRow: 2
        maxColumn: 28
        title: "按照PC1表转换后的56位密钥(28*2)"
        strInputData: "左右"
        visible: false
        Component.onCompleted: {
            listModel.clear()
            for (var i = 0; i < maxRow * maxColumn; ++i) {
                listModel.append({"value": "", "eColor": "white"})
            }
        }
    }

    /* PC1的28*2形式数组 */
    ArrayGridInt {
        id: idGridPC1_28_2
        anchors.left: idArrowR_PC1.right
        anchors.leftMargin: 20
        anchors.verticalCenter: idArrowR_PC1.verticalCenter
        maxRow: 2
        maxColumn: 28
        title: "PC1(56位密钥置换表28*2)"
        strInputData: "左右"
        visible: false
        Component.onCompleted: {
            listModel.clear()
            for (var i = 0; i < arrayPC1.length; ++i) {
                listModel.append({"value": arrayPC1[i].toString(), "eColor": "white"})
            }
        }
    }

    /* 放在PC的28*2数组旁边的箭头，点击开始将64位密钥转成56位并放在28*2数组中 */
    Image {
        id: idArrowU_PC1_28_2
        width: 60
        height: 60
        anchors.bottom: idGridPC1_28_2.top
        anchors.bottomMargin: 20
        anchors.horizontalCenter: idGridPC1_28_2.horizontalCenter
        source: "qrc:arrowU.png"
        fillMode: Image.PreserveAspectCrop
        visible: false

        MouseArea {
            anchors.fill: parent
            onClicked: {
                var position
                var temp
                for (var i = 0; i < idGridKey_28_2.viewCount; ++i) {
                    position = idGridPC1_28_2.listModel.get(i).value - 1
                    temp = idKeyOrig.listModel.get(position).value
                    idGridKey_28_2.listModel.set(i, {"value": temp, "eColor": "white"})
                    idArrowR_ToRound16.visible = true
                }
                sigSetKeySecondOrig56()
            }
        }
    }


    Column {
        anchors.verticalCenter: parent.verticalCenter
        anchors.right: parent.right
        anchors.rightMargin: 5
        spacing: 10
        /* 该箭头用于进入16轮密钥生成界面 */
        Image {
            id: idArrowR_ToRound16
            width: 80
            height: 80
            source: "qrc:arrowR.png"
            fillMode: Image.PreserveAspectCrop
            visible: true

            MouseArea {
                anchors.fill: parent
                onClicked: {
                    sigGotoKeyRound16()
                }
            }
        }

        /* 该箭头用于进入明文处理界面 */
        Image {
            id: idArrowR_ToDataFirst
            width: 80
            height: 80
            source: "qrc:arrowR.png"
            fillMode: Image.PreserveAspectCrop
            visible: true

            MouseArea {
                anchors.fill: parent
                onClicked: {
                    sigGotoDataFirst()
                }
            }
        }
    }
}
