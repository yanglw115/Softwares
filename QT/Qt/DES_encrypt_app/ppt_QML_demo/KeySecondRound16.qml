import QtQuick 2.0

Rectangle {
    id: idRoot
    width: 1440
    height: 900
    color: sysPalette.window

    property alias idKeyOrig_28_2: idKeyOrig_28_2
    property var arrayMoveTo: [2, 2, 1, 1, 2, 2, 1, 2, 1, 1, 2, 2, 1, 2, 2, 1]
    property var testArray: [0, 0, 1, 1, 0, 0, 1, 0, 1, 0, 0, 1, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 0,
        1, 0, 0, 0, 0, 1, 0, 0, 0, 1, 0, 1, 0, 0, 0, 0, 1, 1, 0, 1, 1, 0, 1, 0, 1, 1, 1, 1]
    property var arrayPC2: [ 12, 22, 48, 54, 36, 53,
                        37, 26,  2, 19, 16,  3,
                        33, 43, 18, 10,  1,  6,
                        15, 38, 30, 44, 21, 25,
                        28, 20, 24, 52, 46, 42,
                        23,  9, 17, 47, 39, 40,
                        7,  8, 13,  5, 56, 14,
                        45, 31, 50, 34, 55, 29]
    property int moveTimes: 0

    signal sigGoToKeyFirst()

    SystemPalette {
        id: sysPalette
    }

    function f_keyMoveToLeft()
    {
        var count = idKeyMoved_28_2.maxRow * idKeyMoved_28_2.maxColumn
        var temp1, temp2
        var i, j
        if (moveTimes === 0) {
            idKeyMoved_28_2.listModel.clear()
            for (i = 0; i < count; ++i) {
                temp1 = idKeyOrig_28_2.listModel.get(i).value
                idKeyMoved_28_2.listModel.append({"value": temp1, "eColor": "white"})
            }
        }
        if (moveTimes == 16) {
            return
        }
        for (i = 0; i < arrayMoveTo[i]; ++i) {
            temp1 = idKeyMoved_28_2.listModel.get(0).value
            for (j = 0; j < count / 2 - 1; ++j) {
                temp2 = idKeyMoved_28_2.listModel.get(j + 1).value
                idKeyMoved_28_2.listModel.set(j, {"value": temp2, "eColor": "white"})
            }
            idKeyMoved_28_2.listModel.set(j, {"value": temp1, "eColor": "white"})

            temp1 = idKeyMoved_28_2.listModel.get(count / 2).value
            for (j = count / 2; j < count - 1; ++j) {
                temp2 = idKeyMoved_28_2.listModel.get(j + 1).value
                idKeyMoved_28_2.listModel.set(j, {"value": temp2, "eColor": "white"})
            }
            idKeyMoved_28_2.listModel.set(j, {"value": temp1, "eColor": "white"})
        }

        moveTimes++
        idKeyMoved_28_2.visible = true
    }

    function f_movedKeyTo8_7()
    {
        idKeyMoved_8_7.listModel.clear()
        var count = idKeyMoved_28_2.listModel.count
        var temp
        for (var i = 0; i < count; ++i) {
            temp = idKeyMoved_28_2.listModel.get(i).value
            idKeyMoved_8_7.listModel.append({"value": temp, "eColor": "white"})
        }
    }

    function f_setMovedKeyTo8_6()
    {
        idKeyMoved_8_6.listModel.clear()
        var count = idKeyPC2.listModel.count
        var temp
        var position
        for (var i = 0; i < count; ++i) {
            position = idKeyPC2.listModel.get(i).value
            temp = idKeyMoved_8_7.listModel.get(position - 1).value
            idKeyMoved_8_6.listModel.append({"value": temp, "eColor": "white"})
        }
    }

    /* 展示移位用的16个长度的数组 */
    ArrayGridInt {
        id: idMoveToArray_16
        x: 200
        y: 30
        title: "循环左移数组Array[16]"
        maxRow: 1
        maxColumn: 16
        strInputData: "#"
        Component.onCompleted: {
            listModel.clear()
            for (var i = 0; i < arrayMoveTo.length; ++i) {
                listModel.append({"value": arrayMoveTo[i].toString(), "eColor": "white"})
            }
        }
    }

    /* 展示移位前的56位密钥28*2结构 */
    ArrayGridInt {
        id: idKeyOrig_28_2
        x: 200
        anchors.top: idMoveToArray_16.bottom
        anchors.topMargin: 20
        title: "初次置换后的56位密钥(原始)"
        maxRow: 2
        maxColumn: 28
        strInputData: "左右"
        Component.onCompleted: {
            listModel.clear()
            for (var i = 0; i < maxRow * maxColumn; ++i) {
                listModel.append({"value": testArray[i].toString(), "eColor": "white"})
            }
        }
    }

    /* 该箭头用于密钥第1次移位 */
    Image {
        id: idArrowD_ToRound16
        width: 60
        height: 60
        anchors.top: idKeyOrig_28_2.bottom
        anchors.left: idKeyOrig_28_2.left
        anchors.leftMargin: 30
        source: "qrc:arrowD.png"
        fillMode: Image.PreserveAspectCrop
        visible: true

        MouseArea {
            anchors.fill: parent
            onClicked: {
                if (moveTimes === 0) {
                    idArrowL_ToRound16.visible = true
                    f_keyMoveToLeft()
                    idArrowD_ToKey_8_7.visible = true
                }
            }
        }
    }

    /* 第i次移位后的56位密钥28*2结构 */
    ArrayGridInt {
        id: idKeyMoved_28_2
        x: 200
        anchors.top: idArrowD_ToRound16.bottom
        title: "第" + moveTimes + "次移位后的56位密钥"
        maxRow: 2
        maxColumn: 28
        strInputData: "左右"
        visible: false

        Component.onCompleted: {
            listModel.clear()
            for (var i = 0; i < maxRow * maxColumn; ++i) {
                listModel.append({"value": "", "eColor": "white"})
            }
        }
    }

    /* 该箭头用于密钥第1次以后的移位动作触发 */
    Image {
        id: idArrowL_ToRound16
        width: 60
        height: 60
        anchors.bottom: idKeyMoved_28_2.bottom
        anchors.left: idKeyMoved_28_2.right
        anchors.leftMargin: 20
        source: "qrc:arrowL.png"
        fillMode: Image.PreserveAspectCrop
        visible: false

        MouseArea {
            anchors.fill: parent
            onClicked: {
                f_keyMoveToLeft()
                if (idKeyMoved_8_7.visible) {
                    f_movedKeyTo8_7()
                }
                if (idKeyMoved_8_6.visible) {
                    f_setMovedKeyTo8_6()
                }
            }
        }
    }

    /* 该箭头用于密钥第i次移位后，生成左右合并后的8*7结构的密钥 */
    Image {
        id: idArrowD_ToKey_8_7
        width: 60
        height: 60
        anchors.top: idKeyMoved_28_2.bottom
        anchors.left: idKeyMoved_28_2.left
        anchors.leftMargin: 20
        source: "qrc:arrowD.png"
        fillMode: Image.PreserveAspectCrop
        visible: false

        MouseArea {
            anchors.fill: parent
            onClicked: {
                idKeyMoved_8_7.visible = true
                idArrowR_ToKey_8_6.visible = true
                idArrowL_PC2.visible = true
                idKeyPC2.visible = true
                f_movedKeyTo8_7()
                if (idKeyMoved_8_6.visible) {
                    f_setMovedKeyTo8_6()
                }
            }
        }
    }

    /* 第i次移位后的56位密钥8*7结构 */
    ArrayGridInt {
        id: idKeyMoved_8_7
        x: 200
        anchors.top: idArrowD_ToKey_8_7.bottom
//        anchors.topMargin: 10
        title: "第" + moveTimes + "次移位后的56(8*7)位密钥"
        maxRow: 7
        maxColumn: 8
        visible: false

        Component.onCompleted: {
            listModel.clear()
            for (var i = 0; i < maxRow * maxColumn; ++i) {
                listModel.append({"value": "", "eColor": "white"})
            }
        }
    }

    /* 该箭头用于密钥第i次移位后，根据PC2使置位生成48位密钥 */
    Image {
        id: idArrowR_ToKey_8_6
        width: 60
        height: 60
        anchors.left: idKeyMoved_8_7.right
        anchors.leftMargin: 20
        anchors.verticalCenter: idKeyMoved_8_7.verticalCenter
        source: "qrc:arrowR.png"
        fillMode: Image.PreserveAspectCrop
        visible: false

        MouseArea {
            anchors.fill: parent
            onClicked: {
                idKeyMoved_8_6.visible = true
                f_setMovedKeyTo8_6()
            }
        }
    }

    /* 第i次移位并置换后的48位密钥8*6结构 */
    ArrayGridInt {
        id: idKeyMoved_8_6
        x: 200
        anchors.left: idArrowR_ToKey_8_6.right
        anchors.leftMargin: 20
        anchors.verticalCenter: idArrowR_ToKey_8_6.verticalCenter
        title: "第" + moveTimes + "次移位置换后的8*6密钥"
        maxRow: 6
        maxColumn: 8
        visible: false

        Component.onCompleted: {
            listModel.clear()
            for (var i = 0; i < maxRow * maxColumn; ++i) {
                listModel.append({"value": "", "eColor": "white"})
            }
        }
    }

    /* 该箭头仅用于指示从PC2到最终置换的8*6密钥数据 */
    Image {
        id: idArrowL_PC2
        width: 60
        height: 60
        anchors.left: idKeyMoved_8_6.right
        anchors.leftMargin: 20
        anchors.verticalCenter: idKeyMoved_8_6.verticalCenter
        source: "qrc:arrowL.png"
        fillMode: Image.PreserveAspectCrop
        visible: false
    }

    /* PC2: 用于对第i次移位后的密钥进行压缩置换 */
    ArrayGridInt {
        id: idKeyPC2
        anchors.left: idArrowL_PC2.right
        anchors.leftMargin: 20
        anchors.verticalCenter: idArrowL_PC2.verticalCenter
        title: "置换表PC2"
        maxRow: 6
        maxColumn: 8
        visible: false

        Component.onCompleted: {
            listModel.clear()
            for (var i = 0; i < maxRow * maxColumn; ++i) {
                listModel.append({"value": arrayPC2[i].toString(), "eColor": "white"})
            }
        }
    }


    /* 该箭头仅用于反回密钥生成的第一页 */
    Image {
        id: idArrowL_Back
        width: 80
        height: 80
        anchors.left: parent.left
        anchors.leftMargin: 5
        anchors.verticalCenter: parent.verticalCenter
        source: "qrc:arrowL.png"
        fillMode: Image.PreserveAspectCrop
        visible: true

        MouseArea {
            anchors.fill: parent
            onClicked: {
                sigGoToKeyFirst()
            }
        }
    }

}

