import QtQuick 2.4
import QtQuick.Controls 1.2
import QtQuick.Controls.Styles 1.3

/* 当商品列表中的商品被点击之后，弹出该商品的详细信息界面 */

Rectangle {
    id: myDetail
    width: 300
    height: 300
    color: "#dbd8db"
    focus: true
    property alias imageSource: myImage.source
    property alias price: priceValue.text
    property alias spinMax: myComboBox.model /* 这里展示的是最多可选的商品数量 */
    property alias leftCount: text3.text

    signal sigDestroyDetail(QtObject obj)
    signal sigAddToCart(string count)

    /* 展示商品图片 */
    Image {
        id: myImage
        width: parent.width
        height: parent.height * 0.7
        source: ""
    }

    /* 展示商品价格和商品剩余数量等 */
    Rectangle {
        id: priceRect
        width: parent.width
        height: 40
        color: "#c6cbcc"
        anchors.top: myImage.bottom

        /* 由于这里展示多个文本信息所用的颜色不一样，又不好在同一个Text中区别显示不同颜色，所以暂时分成多个Text来实现 */
        Text {
            id: priceText
            width: 48
            height: 30
            anchors.left: priceRect.left
            anchors.top: priceRect.top
            anchors.topMargin: 10
            anchors.leftMargin: 40
            color: "#000000"
//            font.family: "Microsoft YaHei UI"
            font.family: "WenQuanYi Micro Hei"
            font.pointSize: 14
            text: qsTr("价格: ¥")
        }

        Text {
            id: priceValue
            width: 40
            height: 30
            color: "#e40d0d"
            anchors.leftMargin: 14
            anchors.left: priceText.right
            anchors.top: priceText.top
            anchors.horizontalCenter: priceText.horizontalCenter
            font.bold: true
            font.pointSize: 14
            text: qsTr("")
            horizontalAlignment: Text.AlignLeft
            verticalAlignment: Text.AlignVCenter
            anchors.horizontalCenterOffset: 4
            anchors.topMargin: -2
//            font.family: "Microsoft YaHei UI"
            font.family: "WenQuanYi Micro Hei"
        }

        Text {
            id: countName
            width: 20
            height: 30
            color: "#040000"
            anchors.leftMargin: 153
            anchors.left: priceValue.right
            anchors.top: priceValue.top
            anchors.horizontalCenter: priceValue.horizontalCenter
//            font.bold: true
            font.pointSize: 14
            text: qsTr("购买")
            verticalAlignment: Text.AlignVCenter
            font.bold: false
            anchors.horizontalCenterOffset: 143
            anchors.topMargin: 0
//            font.family: "宋体"
            font.family: "WenQuanYi Micro Hei"
        }

        /* 通过ComboBox下拉列表方便用户选择购买数量 */
        ComboBox {
            id: myComboBox
            width: 63
            height: 30
            anchors.topMargin: 0
            anchors.top: priceValue.top
            anchors.left: priceValue.right
            anchors.leftMargin: 199
            editable: true
            model: 10 /* 意思是从0~10，这个值会改变 */
            focus: true
            style: ComboBoxStyle {
                dropDownButtonWidth: 20
            }
        }

        Text {
            id: text1
            anchors.left: myComboBox.right
            anchors.top: myComboBox.top
            anchors.leftMargin: 6
            anchors.horizontalCenter: myComboBox.horizontalCenter
            width: 10
            height: 30
            text: qsTr("件")
            anchors.horizontalCenterOffset: 42
            anchors.topMargin: 0
            verticalAlignment: Text.AlignVCenter
            horizontalAlignment: Text.AlignHCenter
            font.pointSize: 14
//            font.family: "宋体"
            font.family: "WenQuanYi Micro Hei"
        }


        Row {
            id: rowText
            width: 40
            height: 30
            anchors.left: text1.right
            anchors.leftMargin: 20
            anchors.top: text1.top
            anchors.topMargin: 5

            spacing: 2

            Text {
                id: text2
                verticalAlignment: Text.AlignVCenter
                horizontalAlignment: Text.AlignHCenter
                text: qsTr("库存")
                font.pointSize: 14
//                font.family: "宋体"
                font.family: "WenQuanYi Micro Hei"
            }

            Text {
                id: text3
                verticalAlignment: Text.AlignVCenter
                horizontalAlignment: Text.AlignHCenter
                text: ""
                font.pointSize: 14
                color: "red"
//                font.family: "宋体"
                font.family: "WenQuanYi Micro Hei"
            }

            Text {
                id: text4
                verticalAlignment: Text.AlignVCenter
                horizontalAlignment: Text.AlignHCenter
                text: "件"
                font.pointSize: 14
//                font.family: "宋体"
                font.family: "WenQuanYi Micro Hei"
            }
        }

    }

    /* 展示“立即购买”和“加入购物车” */
    Rectangle {
        id: rectBuy
        width: parent.width
        height: 60
        anchors.top: priceRect.bottom
        anchors.topMargin: 20
        color: parent.color
        Rectangle {
            id: rect1
            anchors.left: parent.left
            anchors.top: parent.top
            anchors.leftMargin: 40
            color: "#ffcccc"
            border.color: "#cf0c0c"
            width: 200
            height: 40
            Text {
                id: t1
                color: "#f10c0c"
                text: qsTr("立即购买")
                font.family: "WenQuanYi Micro Hei"
                font.pointSize: 16
                font.bold: true
                anchors.horizontalCenter: rect1.horizontalCenter
                anchors.verticalCenter: rect1.verticalCenter
            }
        }

        Rectangle {
            id: rect2
            anchors.left: rect1.right
            anchors.top: parent.top
            anchors.leftMargin: 40
            color: "#ee2c2c"
            width: 200
            height: 40

            Text {
                id: t2
                color: "#ffffff"
                text: qsTr("加入购物车")
                font.family: "WenQuanYi Micro Hei"
                font.pointSize: 16
                font.bold: true
                anchors.horizontalCenter: rect2.horizontalCenter
                anchors.verticalCenter: rect2.verticalCenter
            }

            MouseArea {
                anchors.fill: parent
                onClicked: {
                    console.log("Select count:", myComboBox.currentText)
                    myDetail.sigAddToCart(myComboBox.currentText)
                    myDetail.sigDestroyDetail(myDetail)
                }
            }
        }
    }

    Rectangle {
        id: rect3
        width: 200
        height: 40
        anchors.top: rectBuy.bottom
        anchors.left: parent.left
        anchors.leftMargin: 170
        color: "#1b950b"

        Text {
            id: t3
            color: "#ffffff"
            text: qsTr("返回商品列表")
            anchors.horizontalCenter: rect3.horizontalCenter
            anchors.verticalCenter: rect3.verticalCenter
            verticalAlignment: Text.AlignVCenter
            horizontalAlignment: Text.AlignHCenter
            font.pointSize: 16
            font.family: "WenQuanYi Micro Hei"
            styleColor: "#fdfbfb"
        }

        MouseArea {
            anchors.fill: parent
            onClicked: {
                myDetail.sigDestroyDetail(myDetail)
            }
        }
    }

    Keys.onEscapePressed: {
        event.accept = true
        myDetail.sigDestroyDetail(myDetail)
    }
}
