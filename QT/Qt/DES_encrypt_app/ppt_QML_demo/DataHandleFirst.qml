import QtQuick 2.0

Rectangle {
    id: idRoot
    width: 1440
    height: 900
    visible: true
    color: sysPalette.window

    property alias idDataRightAfterEP: idDataRightAfterEP
    property var arrayIP1: [ 40,  8, 48, 16, 56, 24, 64, 32,
        39,  7, 47, 15, 55, 23, 63, 31,
        38,  6, 46, 14, 54, 22, 62, 30,
        37,  5, 45, 13, 53, 21, 61, 29,
        36,  4, 44, 12, 52, 20, 60, 28,
        35,  3, 43, 11, 51, 19, 59, 27,
        34,  2, 42, 10, 50, 18, 58, 26,
        33,  1, 41,  9, 49, 17, 57, 25
       ]
    property var arrayEP: [   7, 19, 29, 21, 32,  2,
                             20, 28,  8, 13, 26, 30,
                             27,  3, 23, 11, 25, 31,
                              6, 15,  9,  1, 12, 24,
                             17, 18, 14, 10,  4, 16,
                              5, 22, 11, 22, 23, 10,
                             26,  3, 29,  9, 20, 14,
                             21,  2, 17, 12,  8,  1]


    signal sigGoToKeyFirst()
    signal sigGoToBoxSReplace()
    signal sigSetRightDataInBoxS()

    function f_setDataAfterIP1()
    {
        var count = idDataOrig.listModel.count
        var temp1
        var temp2
        idDataAfterIP1.listModel.clear()
        for (var i = 0; i < count; ++i) {
            temp1 = idIP1.listModel.get(i).value - 1
            temp2 = idDataOrig.listModel.get(temp1).value
            idDataAfterIP1.listModel.append({"value": temp2.toString(), "eColor": "white"})
        }
    }

    function f_setDataLeftAfterIP1()
    {
        var count = idDataAfterIP1.listModel.count
        var temp
        idDataLeftIP1.listModel.clear()
        for (var i = 0; i < count / 2; ++i) {
            temp = idDataAfterIP1.listModel.get(i).value
            idDataLeftIP1.listModel.append({"value": temp, "eColor": "white"})
        }
    }

    function f_setDataRightAfterIP1()
    {
        var count = idDataAfterIP1.listModel.count
        var temp
        idDataRightIP1.listModel.clear()
        for (var i = 0; i < count / 2; ++i) {
            temp = idDataAfterIP1.listModel.get(i + count / 2).value
            idDataRightIP1.listModel.append({"value": temp, "eColor": "white"})
        }
    }

    function f_setDataLRAfterIP1()
    {
        var count = idDataAfterIP1.listModel.count
        var temp
        idDataLRIP1.listModel.clear()
        for (var i = 0; i < count; ++i) {
            temp = idDataAfterIP1.listModel.get(i).value
            idDataLRIP1.listModel.append({"value": temp, "eColor": "white"})
        }
    }

    function f_setRightDataAfterEP()
    {
        var count = idEP.listModel.count
        var temp1
        var temp2
        idDataRightAfterEP.listModel.clear()
        for (var i = 0; i < count; ++i) {
            temp1 = idEP.listModel.get(i).value - 1
            temp2 = idDataRightIP1.listModel.get(temp1).value
            idDataRightAfterEP.listModel.append({"value": temp2, "eColor": "white"})
        }
    }

    SystemPalette {
        id: sysPalette
    }

    /* 原始数据 */
    ArrayGridBin {
        id: idDataOrig
        x: 140
        y: 40
        strInputLabel: "原始数据："
        strPlaceHolderText: "Data"
        strInputData: "12345678"
    }

    /* 放在原始数据右边的箭头，点击进行64位置换 */
    Image {
        id: idArrowR_DataTo64_IP1
        width: 60
        height: 60
        anchors.left: idDataOrig.right
        anchors.leftMargin: 20
        anchors.verticalCenter: idDataOrig.verticalCenter
        source: "qrc:arrowR.png"
        fillMode: Image.PreserveAspectCrop
        visible: true

        MouseArea {
            anchors.fill: parent
            onClicked: {
                idDataAfterIP1.visible = true
                idDataLeftIP1.visible = true
                idDataRightIP1.visible = true
                idDataLRIP1.visible = true
                idArrowR_DataTo48.visible = true
                idArrowL_DataTo48.visible = true
                idEP.visible = true
                f_setDataAfterIP1()
                f_setDataLeftAfterIP1()
                f_setDataRightAfterIP1()
                f_setDataLRAfterIP1()
            }
        }
    }

    /* 64位根据IP1置换后的数据 */
    ArrayGridInt {
        id: idDataAfterIP1
        anchors.left: idArrowR_DataTo64_IP1.right
        anchors.leftMargin: 20
        anchors.bottom: idDataOrig.bottom
        title: "根据IP1置换后的8*8数据"
        maxRow: 8
        maxColumn: 8
        visible: false
        Component.onCompleted: {
            listModel.clear()
            for (var i = 0; i < maxRow * maxColumn; ++i) {
                listModel.append({"value": "", "eColor": "white"})
            }
        }
    }

    /* 装饰用 */
    Image {
        id: idArrowL_DataTo64_IP1
        width: 60
        height: 60
        anchors.left: idDataAfterIP1.right
        anchors.leftMargin: 20
        anchors.verticalCenter: idArrowR_DataTo64_IP1.verticalCenter
        source: "qrc:arrowL.png"
        fillMode: Image.PreserveAspectCrop
        visible: true

        MouseArea {
            anchors.fill: parent
            onClicked: {

            }
        }
    }

    /* 返回密钥处理第一张界面 */
    Image {
        id: idArrowL_Back
        width: 80
        height: 80
        anchors.left: parent.left
        anchors.leftMargin: 30
        anchors.verticalCenter: parent.verticalCenter
        source: "qrc:arrowL.png"
        fillMode: Image.PreserveAspectCrop
        visible: true

        MouseArea {
            anchors.fill: parent
            onClicked: {
                sigGoToKeyFirst();
            }
        }
    }

    /* IP1 64 */
    ArrayGridInt {
        id: idIP1
        anchors.left: idArrowL_DataTo64_IP1.right
        anchors.leftMargin: 20
        anchors.bottom: idDataOrig.bottom
        title: "IP1置换表"
        maxRow: 8
        maxColumn: 8
        visible: true
        Component.onCompleted: {
            listModel.clear()
            for (var i = 0; i < arrayIP1.length; ++i) {
                listModel.append({"value": arrayIP1[i].toString(), "eColor": "white"})
            }
        }
    }

    /* 置换后的32位左值 8*4 */
    ArrayGridInt {
        id: idDataLeftIP1
        anchors.left: idDataOrig.left
        anchors.top: idDataOrig.bottom
        anchors.topMargin: 20
        title: "32位左值"
        maxRow: 4
        maxColumn: 8
        visible: false
        Component.onCompleted: {
            listModel.clear()
            for (var i = 0; i < maxRow * maxColumn; ++i) {
                listModel.append({"value": "", "eColor": "white"})
            }
        }
    }

    /* 置换后的32位右值 8*4 */
    ArrayGridInt {
        id: idDataRightIP1
        anchors.top: idDataLeftIP1.bottom
        anchors.topMargin: 60
        anchors.left: idDataOrig.left
        title: "32位右值"
        maxRow: 4
        maxColumn: 8
        visible: false
        Component.onCompleted: {
            listModel.clear()
            for (var i = 0; i < maxRow * maxColumn; ++i) {
                listModel.append({"value": "", "eColor": "white"})
            }
        }
    }

    /* 置换后的32位左右值 32*2 */
    ArrayGridInt {
        id: idDataLRIP1
        anchors.left: idDataLeftIP1.right
        anchors.leftMargin: 20
        anchors.verticalCenter: idDataLeftIP1.verticalCenter
        title: "分组后的左右值"
        maxRow: 2
        maxColumn: 32
        strInputData: "左右"
        visible: false
        Component.onCompleted: {
            listModel.clear()
            for (var i = 0; i < maxRow * maxColumn; ++i) {
                listModel.append({"value": "", "eColor": "white"})
            }
        }
    }

    /* 对32位的右值进行扩展置换处理，使变成48位，便于后面的数据处理 */
    Image {
        id: idArrowR_DataTo48
        width: 60
        height: 60
        anchors.left: idDataRightIP1.right
        anchors.leftMargin: 20
        anchors.verticalCenter: idDataRightIP1.verticalCenter
        source: "qrc:arrowR.png"
        fillMode: Image.PreserveAspectCrop
        visible: false

        MouseArea {
            anchors.fill: parent
            onClicked: {
                idDataRightAfterEP.visible = true
                f_setRightDataAfterEP()
                sigSetRightDataInBoxS();
                idArrowR_SBox.visible = true
            }
        }
    }

    /* 扩展置换后的48位右值 8*6 */
    ArrayGridInt {
        id: idDataRightAfterEP
        anchors.left: idArrowR_DataTo48.right
        anchors.leftMargin: 20
        anchors.verticalCenter: idArrowR_DataTo48.verticalCenter
        title: "扩展置换后的48位右值"
        maxRow: 8
        maxColumn: 6
        visible: false

        Component.onCompleted: {
            listModel.clear()
            for (var i = 0; i < maxRow * maxColumn; ++i) {
                listModel.append({"value": "", "eColor": "white"})
            }
        }
    }

    /* 标识作用，从EP表指向EP后的48位右值 */
    Image {
        id: idArrowL_DataTo48
        width: 60
        height: 60
        anchors.left: idDataRightAfterEP.right
        anchors.leftMargin: 20
        anchors.verticalCenter: idDataRightAfterEP.verticalCenter
        source: "qrc:arrowL.png"
        fillMode: Image.PreserveAspectCrop
        visible: false
    }

    /* EP表 */
    ArrayGridInt {
        id: idEP
        anchors.left: idArrowL_DataTo48.right
        anchors.leftMargin: 20
        anchors.verticalCenter: idArrowL_DataTo48.verticalCenter
        title: "EP置换表"
        maxRow: 8
        maxColumn: 6
        visible: false

        Component.onCompleted: {
            listModel.clear()
            for (var i = 0; i < maxRow * maxColumn; ++i) {
                listModel.append({"value": arrayEP[i].toString(), "eColor": "white"})
            }
        }
    }

    /* 箭头，指向S盒运算界面 */
    Image {
        id: idArrowR_SBox
        width: 120
        height: 80
        anchors.right: parent.right
        anchors.rightMargin: 20
        anchors.verticalCenter: idEP.verticalCenter
        source: "qrc:arrowR.png"
//        fillMode: Image.PreserveAspectCrop
        visible: false

        Text {
            text: "S盒运算"
            anchors.top: parent.top
            anchors.topMargin: -12
            anchors.horizontalCenter: parent.horizontalCenter
            font.bold: true
            font.pointSize: 12
            color: "red"
        }

        MouseArea {
            anchors.fill: parent
            onClicked: {
                sigGoToBoxSReplace()
            }
        }
    }

}

