import QtQuick 2.5

Rectangle {
    id: idRoot
    width: 300
    height: 1440
    visible: true
    color: "#CDAF95"
    opacity: 0

    SystemPalette {
        id: sysPalette
    }

    Text {
        id: idTitle1
        x: 10
        y: 20
        text: "DES加密过程"
        font.bold: true
        font.pointSize: 14
    }

    ListModel {
        id: idListModel
        ListElement {
            title: "1.对密钥取56位并进行置换"
        }
        ListElement {
            title: "2.对56位密钥进行循环左移并置换，迭代生成16组经压缩置换后的48位密钥"
        }
        ListElement {
            title: "3.对64位明文进行置换、分组；将右值扩展置换成48位，并与48位密钥进行异或"
        }
    }

    ListView {
        id: idListView
        width: parent.width - 15
        height: 200
        anchors.top: idTitle1.bottom
        anchors.topMargin: 5
        anchors.left: idTitle1.left
        anchors.leftMargin: 5
        spacing: 5
//        highlight: Rectangle { color: "lightsteelblue"; radius: 5 }
////        highlightFollowsCurrentItem: true
//        keyNavigationWraps: true
        focus: true

        model: idListModel

        delegate: TextTitle {
             text: title
        }

//        TextTitle {id: idT1_1; text: "1.对密钥取56位并进行置换"}
//        TextTitle {id: idT1_2; text: "2.对56位密钥进行循环左移并置换，迭代生成16组经压缩置换后的48位密钥"}
//        TextTitle {id: idT1_3; text: "3.对64位明文进行置换、分组；将右值扩展置换成48位，并与48位密钥进行异或"}
//        TextTitle {id: idT1_4; text: "4.对异或后的右值进行S盒运算，并置换压缩为32位新的右值"}
    }

    MouseArea {
        id: idArea
        width: 30
        height: parent.height
        hoverEnabled: true
        propagateComposedEvents: true
        onPositionChanged: {
            mouse.accepted = false
        }
        onContainsMouseChanged: {
            if (containsMouse) {
                idArea.width = idRoot.width
                parent.opacity = 1
            } else {
                idArea.width =  30
                parent.opacity = 0
            }
        }
    }

}

