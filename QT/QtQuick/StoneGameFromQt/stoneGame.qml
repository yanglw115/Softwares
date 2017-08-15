import QtQuick 2.5
import QtQuick.Controls 1.4
import QtQuick.Dialogs 1.2
import QtQuick.Window 2.2
import "gameControl.js" as GameControl

Window {
    visible: true
    width: 490
    height: 720

    SystemPalette {id: idSysPalette}

    Item {
        id: idPlayArea
        width: parent.width
        anchors.top: idControl.bottom
        anchors.bottom: parent.bottom
        Image {
            id: idBackgroud
            anchors.fill: parent
            source: "./share/background.jpg"
            fillMode: Image.PreserveAspectCrop
        }

        Item {
            id: idRealPlayArea
            property int widthPerStone: 40
            width: parent.width - parent.width % widthPerStone
            height: parent.height - parent.height % widthPerStone

            MouseArea {
                anchors.fill: parent
                onClicked: {
                    GameControl.handleClicked(mouseX, mouseY)
                }
            }
        }
    }

    Rectangle {
        id: idControl
        width: parent.width
        height: 30
        anchors.top: parent.top
        color: idSysPalette.window

        Button {
            id: idPlayButton
            anchors.left:parent.left
            anchors.leftMargin: 5
            height: 25
            width: 50
            text: "Go"
            anchors.verticalCenter: parent.verticalCenter

            onClicked: {
                console.log("start game ... ")
//                dialogInputUser.visible = true
                GameControl.startGame()
            }
        }

        Label {
            id: labelScore
            property int score: 0
            property string name: "?"
            property string recordName: ""
            property int gameRecord: 0
            property int playerRecord: 0
            property int goal: 800

            anchors.left: idPlayButton.right
            anchors.leftMargin: 20
            anchors.verticalCenter: parent.verticalCenter
            text: "游戏记录:" + gameRecord.toString()
                  + "|本人(" + name + ")记录:" + playerRecord.toString()
                  + "|目标分: " + goal
                  + "|得分: " + score.toString()

            function setText()
            {
                text = "Game record:" + gameRecord.toString()
                      + "| " + name + " record:" + playerRecord.toString()
                      +"| Score: " + score.toString()
            }
        }
    }

    Dialog {
        id: idDialog
        width: 200
        height: 100
        standardButtons: StandardButton.Ok
        title: "Game Over"
        visible: false

        Label {
//            width: 100
            height: 60
            anchors.centerIn: parent
//            text: "Congratulations!\n\n New record: " + labelScore.score
            text: "Game over!"
        }

        onAccepted: {
            if (clickedButton === StandardButton.Ok) {
            }
        }
    }

    Dialog {
        id: dialogInputUser
        width: 300
        height: 100
        visible: false

        standardButtons: StandardButton.Ok

        Text {
            id: idText
            anchors.left: parent.left
            anchors.leftMargin: 10
            anchors.verticalCenter: parent.verticalCenter
            text: "Input name:"
        }

        ComboBox {
            id: idComboBox
            property var modelList: ["0"]
            anchors.left: idText.right
            anchors.verticalCenter: parent.verticalCenter
            editable: true
            model: modelList

//            Component {
//                id: idModel
//                Item {

//                }
//            }
//            : {
//                model = modelList
//                console.log("Modelist changed!")
//                console.log(modelList[0])
//                console.log(model[0])
//            }

            Component.onCompleted: {
                var list = GameControl.getAllUsersIndatabase()
                if (list !== null) {
                    console.log("###list:", list[0])
                    modelList[0] = list[0]
                    console.log("###modelList:", model[0])
                }
            }

//            onModelChanged: dialogInputUser.visible = true
        }

        onAccepted: {
            if (clickedButton === StandardButton.Ok) {
                var input = idComboBox.editText.trim()
                if (input === "") {
                } else {
                    console.log("Input name:", input)
                    labelScore.name = input
                    GameControl.getRecord(1, input)
                }
                GameControl.gameDuration = new Date()
            }
        }
    }

}
