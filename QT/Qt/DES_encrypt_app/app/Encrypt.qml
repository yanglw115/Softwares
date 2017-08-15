import QtQuick 2.0
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

    property bool bDatabaseOper: false

    signal sigDatabaseOper()

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

    /* select file */
    Text {
        id: textFile
        anchors.left: parent.left
        anchors.leftMargin: 10
        anchors.top: parent.top
        anchors.topMargin: 20
        text: "文件(可选):"
    }

    /* file path field*/
    TextField {
        id: tfFile
        width: 280
        height: 30
        anchors.left: textFile.right
        anchors.leftMargin: 5
        anchors.verticalCenter: textFile.verticalCenter
        text: ""

        /* 使支持外部文件拖曳功能 */
        DropArea {
            anchors.fill: parent

            Rectangle {
                color: "#90c485"
                anchors.fill: parent

                visible: parent.containsDrag
            }
            onDropped: {
                var strUrl = drop.urls.toString();
                console.log("file url:", strUrl);

                if (strUrl.substring(0, 8) === "file:///") {
                    /* 一般本地的文件获取到的url, 根据文件协议，都带有file:/// */
                    tfFile.text = strUrl.slice(8);
                } else if (strUrl.substring(0, 7) === "file://") {
                    /* 对于局域网共享的文件，其格式为:file://192.168.1.10/file,这个时候如果要传给文件读写接口，则需要解析
                        成//192.168.1.10/file这样的格式 */
                    tfFile.text = strUrl.slice(5);
                } else {
                    tfFile.text = strUrl;
                }
            }
        }

    }

    FileDialog {
        id: fileDialog
        title: "Please choose a file"
        folder: shortcuts.desktop
        onAccepted: {
            var strUrl = fileDialog.fileUrl.toString();
            console.log("You chose: " + strUrl)
            console.log("xxx:" + strUrl.substring(0, 8));
            if (strUrl.substring(0, 8) === "file:///") {
                tfFile.text = strUrl.slice(8);
            } else {
                tfFile.text = strUrl;
            }
        }
        onRejected: {
            console.log("Canceled")
        }
        Component.onCompleted: visible = false
    }

    Button {
        id: buttonFile
        width: 40
        height: 30
        anchors.left: tfFile.right
        anchors.leftMargin: 5
        anchors.verticalCenter: textFile.verticalCenter
        text: "浏览"

        MouseArea {
            anchors.fill: parent
            onClicked: {
                fileDialog.visible = true
            }
        }
    }

    Button {
        id: buttonMD5
        width: 40
        height: 30
        anchors.left: buttonFile.right
        anchors.leftMargin: 5
        anchors.verticalCenter: textFile.verticalCenter
        text: "MD5"

        MouseArea {
            anchors.fill: parent
            onClicked: {
                tfInput.text = myDES.md5ForFile(tfFile.text);
            }
        }
    }

    Rectangle {
        id: rectLine
        anchors.top: tfFile.bottom
        anchors.topMargin: 10
        anchors.left: parent.left
        anchors.leftMargin: 10
        anchors.right: parent.right
        anchors.rightMargin: 10
        width: parent.width
        color: "black"
        height: 1
    }

    /* "Key" text */
    Text {
        id: textKey
        anchors.left: parent.left
        anchors.leftMargin: 10
        anchors.top: rectLine.bottom
        anchors.topMargin: 10
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
        text: "输入明文:"
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
        text: "输出密文:"
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

    /* button for encrypt */
    Button {
        id: btEncrypt
        width: 60
        height: 30
        anchors.top: tfOutput.bottom
        anchors.topMargin: 15
        anchors.left: parent.left
        anchors.leftMargin: 10
        text: "加密"

        onClicked: {
            var inputLength = tfInput.text.trim().length
            var keyLength = tfKey.text.trim().length
            if (inputLength % 16 != 0 || inputLength == 0 || keyLength != 8) {
                if (keyLength != 8) {
                    myNote.detailedText = "The Key is empty, or the length of the Key is not 8."
                } else {
                    tfInput.focus = true
                    myNote.detailedText = "The Input data is empty, or the length of the Input is not 16*n."
                }
                myNote.title = "Error"
                myNote.text = "Invalid input."

                myNote.visible = true
            } else {
                tfOutput.text = myDES.encryptDataByDES(tfInput.text.trim(), tfKey.text.trim())
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
        anchors.left: btEncrypt.right
        anchors.leftMargin: 30
        text: "Clear all"

        onClicked: {
            tfKey.text = ""
            tfInput.text = ""
            tfOutput.text = ""
        }
    }


    CheckBox {
        id: idShowDataEdit
        anchors.top: btClearAll.bottom
        anchors.topMargin: 10
        anchors.left: parent.left
        anchors.leftMargin: 10
        checked: false
//        text: "数据库操作"

        onCheckedChanged: {
            if (checked) {
                idDatabaseOper.visible = true;
                bDatabaseOper = true;
                sigDatabaseOper();
            } else {
                idDatabaseOper.visible = false;
                bDatabaseOper = false;
                sigDatabaseOper()
            }
        }
    }

    DatabaseOper {
        id: idDatabaseOper;
        anchors.top: idShowDataEdit.bottom
        anchors.topMargin: 10
        anchors.left: parent.left
        anchors.leftMargin: 10
        anchors.right: parent.right
        anchors.rightMargin: 10
        anchors.bottom: parent.bottom
        anchors.bottomMargin: 10
    }
}
