import QtQuick 2.5

Text {
    font.pointSize: 12
    lineHeight: 1.2
    width: parent.width
    wrapMode: Text.WrapAnywhere;
    MouseArea {
        anchors.fill: parent
        hoverEnabled: true
        propagateComposedEvents: true
        onContainsMouseChanged: {
            console.log("dddddddddddd")
            if (containsMouse) {
                parent.color = "Green"
                parent.font.bold = true
            } else {
                parent.color = "black"
                parent.font.bold = false
            }
        }

        onPositionChanged: {
            console.log("aaaaaaaaaa")
        }
    }
}

