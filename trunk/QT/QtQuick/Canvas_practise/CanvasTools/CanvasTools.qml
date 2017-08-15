import QtQuick 2.5

Rectangle {
    width: 640
    height: 480
    color: "#2f2b2b"

    Row {
        id: idRow
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.top: parent.top
        anchors.topMargin: 10
        spacing: 4
        focus: true
        property string selColor: "red"

        Repeater {
            id: idRepeater
            model: ["red", "green", "blue", "black"]
            delegate: Rectangle {
                width: 50
                height: 50
                focus: true
                color: modelData
                MouseArea {
                    anchors.fill: parent
                    onClicked: {
                        idRow.selColor = color

                    }
                }
            }
        }

    }

    Rectangle {
        anchors.top: idRow.bottom
        anchors.bottom: parent.bottom
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.topMargin: 20
        anchors.bottomMargin: 5
        anchors.leftMargin: 5
        anchors.rightMargin: 5
        color: "white"
        Canvas {
            id: idCanvas
            anchors.fill: parent
            property real lastX
            property real lastY
            onPaint: {
                var ct = getContext("2d")
                if (1) {
                    parent.draw(ct)
                } else {
                    if (null !== ct) {
                        ct.lineWidth = 1
                        ct.strokeStyle = idRow.selColor
                        //                    ct.fillStyle = "steelblue"

                        ct.beginPath()
                        ct.moveTo(lastX, lastY)
                        var tempX = lastX
                        var tempY = lastY
                        lastX = area.mouseX
                        lastY = area.mouseY
                        if (lastX === tempX && lastY === tempY) {
                            lastX += 1
                            lastY += 1
                        }
                        ct.lineTo(lastX, lastY)
                        ct.stroke()

                    } else {
                        console.log("Failed to get canvas context!")
                    }
                }
            }
        }

        MouseArea {
            id: area
            anchors.fill: parent
            onPressed: {
                idCanvas.lastX = mouseX
                idCanvas.lastY = mouseY
            }
            onPositionChanged: {
                idCanvas.requestPaint()
            }
            onReleased: {
                idCanvas.requestPaint()
            }
        }

        function draw(ctx) {
            ctx.fillRect(0,0,300,300);
            for (var i=0;i<3;i++) {
                for (var j=0;j<3;j++) {
                    ctx.save();
                    ctx.strokeStyle = "#9CFF00";
                    ctx.translate(50+j*100,50+i*100);
                    drawSpirograph(ctx,20*(j+2)/(j+1),-8*(i+3)/(i+1),10);
                    ctx.restore();
                }
            }
        }

        function drawSpirograph(ctx,R,r,O){
            var x1 = R-O;
            var y1 = 0;
            var i  = 1;
            ctx.beginPath();
            ctx.moveTo(x1,y1);
            do {
                if (i>20000) break;
                var x2 = (R+r)*Math.cos(i*Math.PI/72) - (r+O)*Math.cos(((R+r)/r)*(i*Math.PI/72))
                var y2 = (R+r)*Math.sin(i*Math.PI/72) - (r+O)*Math.sin(((R+r)/r)*(i*Math.PI/72))
                ctx.lineTo(x2,y2);
                x1 = x2;
                y1 = y2;
                i++;
            } while (x2 != R-O && y2 != 0 );
            ctx.stroke();
        }
    }
}

