import QtQuick 2.0
import QtQuick.Controls 1.4

Rectangle {
    id: idRoot
    visible: true
    width: idMainColumn.width
    height: idMainColumn.height
    property string strInputData: ""
    property string strInputLabel: ""
    property string strPlaceHolderText: ""
    property int maxColumn: 8
    property int maxRow: strInputData.length
    property int cellH: 30
    property int cellW: 30
    property color cellBorderColor: "green"
    property int fontSize: 14
    property int viewCount: idGridView.count
    property alias listModel: listModel
    color: sysPalette.window

    function f_instanceMode()
    {
        listModel.clear()

        var temp = "0";
        var postion = 0;
        console.log("----------------------")
        for (var i = 0; i < maxColumn * maxRow; ++i) {
            if (i % maxColumn === 0) {
                temp = strInputData.charCodeAt(i / maxColumn).toString(2);
                console.log(temp);
            }
            postion = i % maxColumn;
            if ((postion + 1) > temp.length) {
                listModel.append({"value": "0", "eColor": "white"})
            } else {
                listModel.append({"value": temp.charAt(postion), "eColor": "yellow"})
            }

        }
    }

    SystemPalette { id: sysPalette }

    Column {
        id: idMainColumn
        spacing: 1
        height: idRowInput.height + idItemData.height
        width: idItemData.width

        Row {
            id: idRowInput
            x: cellW
            height: cellH
            Text {
                id: keyOrignal
                text: strInputLabel
                font.pointSize: fontSize
                anchors.verticalCenter: parent.verticalCenter
            }

            TextField {
                id: idTextField
                width: 120
//                text: "#$8U3*^j"
                placeholderText: strPlaceHolderText
                font.pointSize: fontSize
                focus: true
                anchors.verticalCenter: parent.verticalCenter
                onTextChanged: {
                    if (length < 8) {
                        /* 这里使用readOnly则禁止编辑操作，也不能删除了 */
//                        readOnly = false
                        textColor = "red"
                    } else if (length > 8) {
                        text = text.substring(0, 8)
                    } else {
                        textColor = "black"
                        console.log(idTextField.text)
                        strInputData = text
                        f_instanceMode()
                    }
                }
                Component.onCompleted: {
                    if (length == 8) {
                        strInputData = text
                    }
                }
            }
        }

        Item {
            id: idItemData
            width: idRow.width + idColumn.width
            height: idRow.height + idColumn.height

            Row {
                id: idRow
                width: maxColumn * cellW
                height: cellH
                x: cellW
                Repeater {
                    model: maxColumn
                    Rectangle {
                        width: cellH
                        height: cellH
                        border.color: cellBorderColor
                        color: "grey"
                        Text {
                            id: idText
                            font.pointSize: fontSize
                            text: index
                            anchors.verticalCenter: parent.verticalCenter
                            anchors.horizontalCenter: parent.horizontalCenter
                        }
                    }
                }

            }

            Column {
                id: idColumn
                anchors.top: idRow.bottom
                anchors.right: idRow.left
                height: maxRow * cellH
                width: cellW
                Repeater {
                    model: maxRow
                    Rectangle {
                        width: cellW
                        height: cellH
                        border.color: cellBorderColor
                        color: "pink"
                        Text {
                            font.pointSize: fontSize
                            text: strInputData[index]
                            anchors.verticalCenter: parent.verticalCenter
                            anchors.horizontalCenter: parent.horizontalCenter
                        }
                    }
                }

            }

            ListModel {
                id: listModel
                ListElement {
                    value: ""
                    eColor: ""
                }
            }

            GridView {
                id: idGridView
                width: cellWidth * maxColumn
                height: cellHeight * maxRow
                anchors.left: idColumn.right
                anchors.top: idRow.bottom
                cellHeight: cellH
                cellWidth: cellW
                model: listModel
                delegate: myDelegate
                visible: true

                Component {
                    id: myDelegate
                    Rectangle {
                        width: cellH
                        height: cellH
                        border.color: cellBorderColor
                        color: model.eColor
                        Text {
                            id: idText
                            font.pointSize: fontSize
                            text: model.value
                            anchors.verticalCenter: parent.verticalCenter
                            anchors.horizontalCenter: parent.horizontalCenter
                        }
                    }
                }

            }
        }
        Component.onCompleted: {
            f_instanceMode()
        }
    }
}


