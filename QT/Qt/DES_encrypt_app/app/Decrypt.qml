import QtQuick 2.5
import QtQuick.Dialogs 1.2
import QtQuick.Controls 1.3
import qt.DES.component 1.0

/* main surface of des operation */
Rectangle {
    border.color: "#62932e"

    anchors.left: parent.left
    anchors.leftMargin: 5
    anchors.right: parent.right
    anchors.rightMargin: 5
    anchors.top: parent.top
    anchors.topMargin: 5
    anchors.bottom: parent.bottom
    anchors.bottomMargin: 5

    /* note message */
    MessageDialog {
        id: myNote
        standardButtons: StandardButton.Ok
        title: ""
        text: ""
        detailedText: ""
        icon: StandardIcon.Critical
        visible: false
    }


    /* DES from C++ */
    MyDES {
        id: myDES
    }

    /* "Key" text */
    Text {
        id: textKey
        anchors.left: parent.left
        anchors.leftMargin: 10
        anchors.top: parent.top
        anchors.topMargin: 20
        text: "密钥:"
    }

    /* key input field*/
    TextField {
        id: tfKey
        width: 200
        height: 30
        anchors.left: parent.left
        anchors.leftMargin: 10
        anchors.top: textKey.bottom
        anchors.topMargin: 10
        text: "%U(^f6@R"

        onTextChanged: {
            if (length != 8) {
                textColor = "red"
            } else {
                textColor = "black"
            }
        }
    }

    /* "Input" text */
    Text {
        id: textInput
        anchors.left: parent.left
        anchors.leftMargin: 10
        anchors.top: tfKey.bottom
        anchors.topMargin: 10
        text: "输入密文:"
    }

    /* input field */
    TextField {
        id: tfInput
        width: 300
        height: 30
        anchors.left: parent.left
        anchors.leftMargin: 10
        anchors.top: textInput.bottom
        anchors.topMargin: 10
//            inputMethodHints: Qt.ImhDigitsOnly | Qt.ImhUppercaseOnly | Qt.ImhLowercaseOnly
        text: ""

        onTextChanged: {
            if (length %16 != 0) {
                textColor = "red"
            } else {
                textColor = "black"
            }
        }
    }

    /* "Output" text */
    Text {
        id: textOutput
        anchors.left: parent.left
        anchors.leftMargin: 10
        anchors.top: tfInput.bottom
        anchors.topMargin: 10
        text: "输出明文:"
    }

    /* output field */
    TextField {
        id: tfOutput
        width: 300
        height: 30
        anchors.left: parent.left
        anchors.leftMargin: 10
        anchors.top: textOutput.bottom
        anchors.topMargin: 10
        readOnly: true

    }

    /* button for reset key contents */
    Button {
        id: btKeyReset
        width: 60
        height: 30
        anchors.top: textKey.bottom
        anchors.topMargin: 10
        anchors.left: tfKey.right
        anchors.leftMargin: 10
        text: "Reset"

        onClicked: {
            tfKey.text = "%U(^f6@R"
        }
    }

    /* button for clear key contents */
    Button {
        id: btKeyClear
        width: 60
        height: 30
        anchors.top: textKey.bottom
        anchors.topMargin: 10
        anchors.left: btKeyReset.right
        anchors.leftMargin: 10
        text: "Clear"

        onClicked: {
            tfKey.text = ""
        }
    }

    /* button for clear input contents */
    Button {
        id: btInClear
        width: 60
        height: 30
        anchors.top: textInput.bottom
        anchors.topMargin: 10
        anchors.left: tfInput.right
        anchors.leftMargin: 10
        text: "Clear"

        onClicked: {
            tfInput.text = ""
        }
    }

    /* button for clear output contents */
    Button {
        id: btOutClear
        width: 60
        height: 30
        anchors.top: textOutput.bottom
        anchors.topMargin: 10
        anchors.left: tfOutput.right
        anchors.leftMargin: 10
        text: "Clear"

        onClicked: {
            tfOutput.text = ""
        }
    }

    /* button for decrypt */
    Button {
        id: btDecrypt
        width: 60
        height: 30
        anchors.top: tfOutput.bottom
        anchors.topMargin: 15
        anchors.left: parent.left
        anchors.leftMargin: 10
        text: "解密"

        onClicked: {
            var inputLength = tfInput.text.trim().length
            var keyLength = tfKey.text.trim().length
            if (inputLength % 16 != 0 || inputLength == 0 || keyLength != 8) {
                if (keyLength != 8) {
                    myNote.detailedText = "The Key is empty, or the length of the Key is not 8."
                } else {
                    myNote.detailedText = "The Input data is empty, or the length of the Input is not 16*n."
                }
                myNote.title = "Error"
                myNote.text = "Invalid input."
                myNote.visible = true
            } else {
                tfOutput.text = myDES.decryptDataByDES(tfInput.text.trim(), tfKey.text.trim())
                console.log("Result:", tfOutput.text);
            }
        }
    }

    /* button for clear all contents */
    Button {
        id: btClearAll
        width: 80
        height: 30
        anchors.top: tfOutput.bottom
        anchors.topMargin: 15
        anchors.left: btDecrypt.right
        anchors.leftMargin: 30
        text: "Clear all"

        onClicked: {
            tfKey.text = ""
            tfInput.text = ""
            tfOutput.text = ""
        }
    }

}
