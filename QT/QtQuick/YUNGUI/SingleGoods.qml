import QtQuick 2.2

/* 商品列表的单个商品数据展示 */
Rectangle {
    width: 300
    height: 240
    Keys.enabled: true
    visible: true
    border.color: "black"

    /* 匿名只能在object加载完成之后才可以使用 */
    property alias mouseArea: mouseArea
    property alias source: myImage.source
    property alias text: textName.text

    /* 目前只展示商品图片与商品名称 */
    Image {
        id: myImage
        source: "photoes/photoes/YiShouWan.png"
        width: parent.width
        height: parent.height * 0.9
    }

    Text {
        id: textName
        height: parent.height - myImage.height
        color: "#8d2020"
        anchors.top: myImage.bottom
        anchors.topMargin: 5
        anchors.horizontalCenter: myImage.horizontalCenter
        text: "unknow"
        font.bold: true
        font.family: "WenQuanYi Micro Hei"
    }

    MouseArea {
        id: mouseArea
        anchors.fill: parent
        hoverEnabled: true
        onContainsMouseChanged: {
            if (containsMouse) {
                parent.opacity = 0.7
            } else {
                parent.opacity = 1.0
            }
        }
    }
}
