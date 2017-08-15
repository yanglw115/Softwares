import QtQuick 2.4

Rectangle {
    property alias mouseArea: mouseArea
    id: myRect
    width: 360
    height: 360

    BorderImage {
        id: myImage
        source: "photoes/324425b2-0405-4b0a-841a-f626bc36aa29_11.jpg"
        border.left: 5; border.top: 5
        border.right: 5; border.bottom: 5
        width: myRect.width - border.left - border.right;
        height: myRect.height - border.top - border.bottom - myText.height;
    }

    MouseArea {
        id: mouseArea
        anchors.fill: myRect
    }

    Text {
        id: myText
        width: myImage.width
        height: 30
        anchors.centerIn: myRect
        text: "dddd"
    }
}

