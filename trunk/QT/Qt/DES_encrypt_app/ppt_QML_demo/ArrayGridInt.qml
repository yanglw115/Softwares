import QtQuick 2.0

Rectangle {
    id: idRoot
    visible: true
    width: idRow.width + idColumn.width
    height: idRow.height + idColumn.height + idTitle.height
    property string strInputData: "12345678"
    property int maxColumn: 8
    property int maxRow: 8
    property int cellH: 30
    property int cellW: 30
    property color cellBorderColor: "green"
    property int fontSize: 14
    property alias model: idGridView.model
    property string title: ""
    property int viewCount: idGridView.count
    property alias gridView: idGridView
    property alias listModel: listModel
    color: sysPalette.window

    SystemPalette { id: sysPalette }

    Item {
        id: idTitle
        x: cellW
        width: cellW * 3
        height: cellH
        Text {
            text: title
            font.bold: true
            font.pointSize: fontSize
            anchors.verticalCenter: parent.verticalCenter
        }
    }

    Row {
        id: idRow
        x: cellW
        width: maxColumn * cellW
        height: cellH
        anchors.top: idTitle.bottom
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
        Repeater {
            model: maxRow
            Rectangle {
                width: cellH
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
        function setModelData()
        {
        }
    }

    GridView {
        id: idGridView
        anchors.top: idRow.bottom
        anchors.left: idColumn.right
        width: cellHeight * maxColumn
        height: cellWidth * maxRow
        cellHeight: cellH
        cellWidth: cellW
        delegate: myDelegate
        model: listModel
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
                    text: value
                    anchors.verticalCenter: parent.verticalCenter
                    anchors.horizontalCenter: parent.horizontalCenter
                }
            }
        }

        Component.onCompleted: {
        }

    }
}



