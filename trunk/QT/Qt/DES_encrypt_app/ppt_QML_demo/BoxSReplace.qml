import QtQuick 2.0

Rectangle {
    id: idRoot
    width: 1440
    height: 900
    color: sysPalette.window

    property alias idRight_8_6: idRight_8_6

    property var arrayIP: [  6, 13, 32,  3,
                            25, 31, 23, 14,
                             4, 27, 21, 24,
                            26,  8, 17, 20,
                            28,  7, 11, 16,
                            12,  9, 29, 30,
                            22,  5, 18,  1,
                            15, 10, 19,  2 ]

    property var arrayS0: [ 13,  3,  1,  7,  2, 11, 10,  0, 14,  6,  5, 15,  4, 12,  9,  8,
        4, 11,  8,  2, 14,  0, 10,  5,  3,  6, 12,  1, 15, 13,  7,  9,
        8, 10, 14,  3,  6,  2,  0,  1,  7,  4,  5, 15,  9, 11, 12, 13,
        10,  6,  3, 14, 13,  7, 15,  2, 12, 11,  5,  8,  4,  1,  0,  9 ]

    property var arrayS1: [ 14,  4,  2,  9, 13,  1,  8, 15, 10,  6,  5, 12,  0, 11,  7,  3,
        14, 10,  2, 15,  7,  8,  3,  1,  4,  6, 13,  9,  0, 12, 11,  5,
        3,  0, 14, 13,  5, 12,  8,  1,  7,  2,  6, 15, 11,  9,  4, 10,
        15,  9,  7,  1, 11,  6,  0,  2,  4,  3,  8, 14,  5, 13, 10, 12 ]

    property var arrayS2: [ 8,  2, 15,  0,  5,  4,  9, 11,  3,  7, 14, 10, 12, 13,  1,  6,
        5, 10,  7,  0, 11, 15,  9,  8, 14,  2,  1, 12,  6,  4, 13,  3,
        1,  4,  5,  8, 12, 15,  2, 13, 11, 14,  3,  9, 10,  6,  0,  7,
        2,  3, 15,  4,  8,  7, 10, 13, 12,  9,  5,  6, 11, 14,  1,  0]

    property var arrayS3: [ 0, 13,  9,  6, 11,  4,  2, 15,  8, 14, 12,  7, 10,  1,  3,  5,
        15, 13, 14, 12,  5, 11,  8,  1,  4,  9,  6, 10,  3,  0,  2,  7,
        12,  5,  6, 14, 15, 11,  1, 10, 13,  0,  9,  8,  4,  7,  3,  2,
        3, 13,  0,  5, 11,  8,  4,  9, 15,  2,  1,  6, 14, 10, 12,  7 ]

    property var arrayS4: [ 7,  0,  5,  1,  9, 10,  2, 14,  3, 11,  8, 12, 15,  4,  6, 13,
        14,  5,  2,  3,  6,  7,  1,  4, 11, 10, 15, 12,  8,  9,  0, 13,
        15, 10,  7, 14, 13, 12,  9,  4,  3,  8,  2,  1,  0,  6,  5, 11,
        2,  0,  4, 11, 10, 12,  5,  1, 15, 14,  8, 13,  7,  3,  6,  9 ]

    property var arrayS5: [ 14,  2,  9, 10,  8,  6, 15,  0, 11,  7,  1, 12,  4, 13,  3,  5,
        3,  2, 10,  5,  0,  9, 11, 12, 15, 13,  8,  1, 14,  4,  7,  6,
        8,  4, 12, 11, 15,  1,  7, 14, 10,  0, 13,  9,  5,  6,  2,  3,
        11, 14,  9,  7,  6, 13,  1,  5,  2,  3,  8, 10, 12, 15,  4,  0 ]

    property var arrayS6: [ 5,  6,  0,  2, 11,  8, 14,  3, 13, 10,  7,  1, 12,  9,  4, 15,
        13,  4,  9,  3, 12,  2,  8, 15, 10,  6,  1,  5,  0, 14, 11,  7,
        12,  0, 11,  8,  5, 10, 13,  2, 14,  6,  9,  4, 15,  7,  3,  1,
        15,  1,  0, 12,  4, 11,  5,  7,  8,  2, 13,  9, 14, 10,  6,  3 ]

    property var arrayS7: [ 4, 11,  7,  3,  0, 12, 15,  6,  5, 10,  8,  1, 13,  9, 14,  2,
        9, 15, 12, 13,  3,  7,  8,  1, 11,  0,  4,  5, 14, 10,  6,  2,
        1,  4, 12,  3, 10,  5,  9,  7, 13, 11, 14,  8,  2,  6, 15,  0,
        13,  1,  2,  7,  6,  3, 12,  0,  9,  4, 14,  5, 11, 10, 15,  8 ]

    signal sigGoDathandleFirst()

    function f_firstBoxSOperation()
    {
        idGridFirstBoxSOper.repeatorModel.clear()
        idGridFirstBoxSOper.repeatorModel.append({"text1": "行", "text2": "列", "text3": "值", "eColor": "gray"})
        var t1, t2, t3, v1, v11, v2, v22
        for (var i = 0; i < 8; ++i) {
            v1 = idRight_8_6.listModel.get(i * 6).value + idRight_8_6.listModel.get(i * 6 + 5).value
            v2 = idRight_8_6.listModel.get(i * 6 + 1).value + idRight_8_6.listModel.get(i * 6 + 2).value
                    + idRight_8_6.listModel.get(i * 6 + 3).value + idRight_8_6.listModel.get(i * 6 + 4).value

            v11 = parseInt(v1, 2)
            v22 = parseInt(v2, 2)

            t1 = v1 + "(" + v11.toString() + ")"
            t2 = v2 + "(" + v22.toString() + ")"

            switch (i) {
            case 0:
                t3 = arrayS0[v11 * 16 + v22]
                break;
            case 1:
                t3 = arrayS1[v11 * 16 + v22]
                break;
            case 2:
                t3 = arrayS2[v11 * 16 + v22]
                break;
            case 3:
                t3 = arrayS3[v11 * 16 + v22]
                break;
            case 4:
                t3 = arrayS4[v11 * 16 + v22]
                break;
            case 5:
                t3 = arrayS5[v11 * 16 + v22]
                break;
            case 6:
                t3 = arrayS6[v11 * 16 + v22]
                break;
            case 7:
                t3 = arrayS7[v11 * 16 + v22]
                break;
            default:
                t3 = ""
                break;
            }

            idGridFirstBoxSOper.repeatorModel.append({"text1": t1, "text2": t2, "text3": t3.toString(), "eColor": "white"})
        }
    }

    function f_setFirstBoxSResult()
    {
        var count = idGridFirstBoxSOper.repeatorModel.count
        var temp, i, j, c, len
        idGridFirstResult.model.clear()
        for (i = 0; i < count - 1; ++i) {
            switch (i) {
            case 0:
                c = "#EE9A00"
                break;
            case 1:
                c = "#EEC591"
                break;
            case 2:
                c = "#EE6AA7"
                break;
            case 3:
                c = "#EEA9B8"
                break;
            case 4:
                c = "#9A32CD"
                break;
            case 5:
                c = "#9F79EE"
                break;
            case 6:
                c = "#8B5A00"
                break;
            case 7:
                c = "#8B7355"
                break;
            default:
                c = "white"
                break;
            }
            temp = idGridFirstBoxSOper.repeatorModel.get(i + 1).text3
            //            temp = idGridFirstBoxSOper.idRepeater_01.itemAt(i + i).text3
            temp = parseInt(temp).toString(2)
            len = temp.length
            for (j = 0; j < idGridFirstResult.maxColumn; ++j) {
                if (j >= 4) {
                    idGridFirstResult.model.append({"value": "0", "eColor": "white"})
                } else {
                    if (j >= len) {
                        idGridFirstResult.model.append({"value": "0", "eColor": c})
                    } else {
                        idGridFirstResult.model.append({"value": temp.charAt(len - j - 1), "eColor": c})
                    }
                }
            }
        }
    }

    function f_setSecondBoxResult()
    {
        var count = idGridSecondResult.maxRow * idGridSecondResult.maxColumn
        var c, temp, index
        idGridSecondResult.listModel.clear()
        for (var i = 0; i < count; ++i) {
            switch (Math.floor(i / 4)) {
            case 0:
                c = "#EEC591"
                break;
            case 1:
                c = "#EE9A00"
                break;
            case 2:
                c = "#EEA9B8"
                break;
            case 3:
                c = "#EE6AA7"
                break;
            case 4:
                c = "#9F79EE"
                break;
            case 5:
                c = "#9A32CD"
                break;
            case 6:
                c = "#8B7355"
                break;
            case 7:
                c = "#8B5A00"
                break;
            default:
                c = "white"
                break;
            }
            if (Math.floor(i / 4) % 2) {
                index = (Math.floor(i / 4)- 1) * 8 + i % 4
            } else {
                index = (Math.floor(i / 4) + 1) * 8 + i % 4
            }
            temp = idGridFirstResult.listModel.get(index).value
            idGridSecondResult.listModel.append({"value": temp, "eColor": c})
        }
    }

    function f_setIPResult()
    {
        var count = idGridIPResult.maxRow * idGridIPResult.maxColumn
        var temp1, temp2
        idGridIPResult.listModel.clear()
        for (var i = 0; i < count; ++i) {
            temp1 = idGridIP.listModel.get(i).value - 1
            temp2 = idGridSecondResult.listModel.get(temp1).value
            idGridIPResult.listModel.append({"value": temp2, "eColor": "white"})
        }
    }

    SystemPalette {
        id: sysPalette
    }

    /* S0 */
    ArrayGridInt {
        id: idS0
        x: 140
        y: 10
//        anchors.left: idGridFirstBoxSOper.right
//        anchors.leftMargin: 20
//        anchors.verticalCenter: idRight_8_6.verticalCenter
        //        anchors.top: idRight_8_6.bottom
        //        anchors.topMargin: 20
        //        anchors.left: idRight_8_6.left
        title: "S0"
        maxRow: 4
        maxColumn: 16
        visible: true

        Component.onCompleted: {
            listModel.clear()
            for (var i = 0; i < maxRow * maxColumn; ++i) {
                listModel.append({"value": arrayS0[i].toString(), "eColor": "white"})
            }
        }
    }

    /* 48位右值 */
    ArrayGridInt {
        id: idRight_8_6
        //        x: 140
        //        y: 10
        anchors.left: idS0.left
        anchors.top: idS0.bottom
        anchors.topMargin: 20
        title: "48位右值"
        maxRow: 8
        maxColumn: 6
        visible: true

        Component.onCompleted: {
            listModel.clear()
            for (var i = 0; i < maxRow * maxColumn; ++i) {
                listModel.append({"value": "", "eColor": "white"})
            }
        }
    }

    /* 箭头指令：计算将从S盒中取值的行与列，及取出来的值，S盒计算第一步 */
    Image {
        id: idArrow_ToGridFirst
        width: 40
        height: 40
        anchors.left: idRight_8_6.right
        anchors.leftMargin: 20
        anchors.verticalCenter: idRight_8_6.verticalCenter
        source: "qrc:arrowR.png"
        fillMode: Image.PreserveAspectCrop
        visible: true

        MouseArea {
            anchors.fill: parent
            onClicked: {
                idGridFirstBoxSOper.visible = true
                idArrow_ToFirstResult.visible = true
                f_firstBoxSOperation()
            }
        }
    }

    /* 用于展示取到的行/列/和对应S盒中的值 */
    Grid {
        id: idGridFirstBoxSOper
        anchors.left: idArrow_ToGridFirst.right
        anchors.leftMargin: 20
        anchors.bottom: idRight_8_6.bottom
        visible: false
        columns: 1

        property alias repeatorModel: listModel_01
        property alias idRepeater_01: idRepeater_01

        Component {
            id: idDelegate_01
            Row {
                Rectangle {
                    height: idRight_8_6.cellH
                    width: idRight_8_6.cellW * 3
                    border.color: "green"
                    color: eColor
                    Text {
                        anchors.verticalCenter: parent.verticalCenter
                        anchors.horizontalCenter: parent.horizontalCenter
                        text: text1
                        font.pointSize: idRight_8_6.fontSize
                    }
                }
                Rectangle {
                    height: idRight_8_6.cellH
                    width: idRight_8_6.cellW * 4
                    border.color: "green"
                    color: eColor
                    Text {
                        anchors.verticalCenter: parent.verticalCenter
                        anchors.horizontalCenter: parent.horizontalCenter
                        text: text2
                        font.pointSize: idRight_8_6.fontSize
                    }
                }
                Rectangle {
                    height: idRight_8_6.cellH
                    width: idRight_8_6.cellW * 2
                    border.color: "green"
                    color: eColor
                    Text {
                        anchors.verticalCenter: parent.verticalCenter
                        anchors.horizontalCenter: parent.horizontalCenter
                        text: text3
                        font.pointSize: idRight_8_6.fontSize
                    }
                }
            }
        }

        ListModel {
            id: listModel_01
            ListElement {
                text1: ""
                text2: ""
                text3: ""
                eColor: ""
            }
        }

        Repeater {
            id: idRepeater_01
            model: listModel_01
            delegate: idDelegate_01
        }
    }

    /* 箭头指令：将从S盒中取出的8个数字转换为8*8 */
    Image {
        id: idArrow_ToFirstResult
        width: 40
        height: 40
        anchors.left: idGridFirstBoxSOper.right
        anchors.leftMargin: 20
        anchors.verticalCenter: idGridFirstBoxSOper.verticalCenter
        source: "qrc:arrowR.png"
        fillMode: Image.PreserveAspectCrop
        visible: false

        MouseArea {
            anchors.fill: parent
            onClicked: {
                idGridFirstResult.visible = true
                idArrow_ToSecondResult.visible = true
                f_setFirstBoxSResult()
            }
        }
    }

    /* 用于存放从S盒中取出的8个数字 */
    ArrayGridInt {
        id: idGridFirstResult
        //        anchors.top: idRight_8_6.bottom
        //        anchors.topMargin: 10
        //        anchors.left: idRight_8_6.left
        anchors.left: idArrow_ToFirstResult.right
        anchors.leftMargin: 20
        anchors.verticalCenter: idRight_8_6.verticalCenter
        title: "从S盒中取出的值"
        maxColumn: 8
        maxRow: 8
        visible: false

        Component.onCompleted: {
            listModel.clear()
            for (var i = 0; i < maxRow * maxColumn; ++i) {
                listModel.append({"value": "", "eColor": "white"})
            }
        }
    }

    /* 箭头指令：用于将8*8数据两两合并为4*8 */
    Image {
        id: idArrow_ToSecondResult
        width: 40
        height: 40
        anchors.top: idGridFirstResult.bottom
        anchors.topMargin: 20
        anchors.horizontalCenter: idGridFirstResult.horizontalCenter
        source: "qrc:arrowD.png"
        fillMode: Image.PreserveAspectCrop
        visible: false

        MouseArea {
            anchors.fill: parent
            onClicked: {
                idGridSecondResult.visible = true
                idArrow_ToIPResult.visible = true
                idGridIP.visible = true
                idArrow_ToIPResult1.visible = true
                f_setSecondBoxResult()
            }
        }
    }

    /* 对从S盒中取出的8个数字两两进行移位合并，得到4*8的32位数据 */
    ArrayGridInt {
        id: idGridSecondResult
        anchors.top: idArrow_ToSecondResult.bottom
        anchors.topMargin: 20
        anchors.horizontalCenter: idArrow_ToSecondResult.horizontalCenter
        title: "两两合并后的结果"
        maxColumn: 8
        maxRow: 4
        visible: false

        Component.onCompleted: {
            listModel.clear()
            for (var i = 0; i < maxRow * maxColumn; ++i) {
                listModel.append({"value": "", "eColor": "white"})
            }
        }
    }

    /* 箭头指令：用于将4*8数据进行IP置换 */
    Image {
        id: idArrow_ToIPResult
        width: 40
        height: 40
        anchors.left: idGridSecondResult.right
        anchors.leftMargin: 20
        anchors.verticalCenter: idGridSecondResult.verticalCenter
        source: "qrc:arrowR.png"
        fillMode: Image.PreserveAspectCrop
        visible: false

        MouseArea {
            anchors.fill: parent
            onClicked: {
                idGridIPResult.visible = true
                f_setIPResult()
            }
        }
    }

    /* 对合并后生成的4*8数据再进行IP置换后生成的4*8数据 */
    ArrayGridInt {
        id: idGridIPResult
        anchors.left: idArrow_ToIPResult.right
        anchors.leftMargin: 20
        anchors.verticalCenter: idGridSecondResult.verticalCenter
        title: "IP置换后的结果"
        maxColumn: 8
        maxRow: 4
        visible: false

        Component.onCompleted: {
            listModel.clear()
            for (var i = 0; i < maxRow * maxColumn; ++i) {
                listModel.append({"value": "", "eColor": "white"})
            }
        }
    }

    /* IP置换表 */
    ArrayGridInt {
        id: idGridIP
        anchors.bottom: idGridFirstResult.bottom
        anchors.horizontalCenter: idGridIPResult.horizontalCenter
        title: "IP置换表"
        maxColumn: 8
        maxRow: 4
        visible: false

        Component.onCompleted: {
            listModel.clear()
            for (var i = 0; i < maxRow * maxColumn; ++i) {
                listModel.append({"value": arrayIP[i].toString(), "eColor": "white"})
            }
        }
    }

    /* 箭头装饰：指向IP置换后的结果 */
    Image {
        id: idArrow_ToIPResult1
        width: 40
        height: 40
        anchors.top: idGridIP.bottom
        anchors.topMargin: 20
        anchors.horizontalCenter: idGridIP.horizontalCenter
        source: "qrc:arrowD.png"
        fillMode: Image.PreserveAspectCrop
        visible: false
    }

    /* 返回DataHandleFirst界面 */
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
                sigGoDathandleFirst();
            }
        }
    }

}

