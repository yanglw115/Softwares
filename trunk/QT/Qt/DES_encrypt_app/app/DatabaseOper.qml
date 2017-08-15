import QtQuick 2.0
import QtQuick.Controls 1.4
import QtQuick.Dialogs 1.2
import QtQuick.Layouts 1.2
import QtQml.Models 2.2
import qt.DES.component 1.0

/******************************************下面是数据库操作****************************/

GroupBox {
    id: idRoot

    title: "sqlite operation"

    checkable: false
    visible: false

    signal sigTest();

    function f_refreshVersionColumn()
    {
        var roleList = idDataModel.getRolesTableVersion();
        for(var i=0; i < roleList.length; i++)
        {
            var role  = roleList[i]
//            idTabelVersion.addColumn(temp[i])
//            idTabView.f_test(role);
//            idTabVersion.f_test1(role);
            sigTest(role)
        }
    }

    Label {
        id: idLabel
        anchors.top: parent.top
        anchors.topMargin: 10
        anchors.left: parent.left
        anchors.leftMargin: 10
        text: "sqlite文件："
    }

    /* file path field*/
    TextField {
        id: tfFileDatabase
        width: 500
        height: 30
        anchors.left: idLabel.right
        anchors.leftMargin: 5
        anchors.verticalCenter: idLabel.verticalCenter
//        text: ""

        onTextChanged: {
            idDataModel.setDatabasePath(text.toString());
            f_refreshVersionColumn();
        }

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
                    tfFileDatabase.text = strUrl.slice(8);
                } else if (strUrl.substring(0, 7) === "file://") {
                    /* 对于局域网共享的文件，其格式为:file://192.168.1.10/file,这个时候如果要传给文件读写接口，则需要解析
                        成//192.168.1.10/file这样的格式 */
                    tfFileDatabase.text = strUrl.slice(5);
                } else {
                    tfFileDatabase.text = strUrl;
                }
            }
        }

    }

    FileDialog {
        id: idFileDialogDatabase
        title: "Please choose a sqlite database file"
        folder: shortcuts.desktop
        onAccepted: {
            var strUrl = idFileDialogDatabase.fileUrl.toString();
            console.log("You chose: " + strUrl)
            console.log("xxx:" + strUrl.substring(0, 8));
            if (strUrl.substring(0, 8) === "file:///") {
                tfFileDatabase.text = strUrl.slice(8);
            } else {
                tfFileDatabase.text = strUrl;
            }
        }
        onRejected: {
            console.log("Canceled")
        }
        Component.onCompleted: visible = false
    }

    Button {
        id: idbuttonFile
        width: 40
        height: 30
        anchors.left: tfFileDatabase.right
        anchors.leftMargin: 5
        anchors.verticalCenter: idLabel.verticalCenter
        text: "浏览"

        MouseArea {
            anchors.fill: parent
            onClicked: {
                idFileDialogDatabase.visible = true
            }
        }
    }

    /* 版本号与广告分开 */
    TabView {
        id: idTabView
        anchors.left: parent.left
        anchors.leftMargin: 5
        anchors.right: parent.right
        anchors.rightMargin: 5
        anchors.top: idbuttonFile.bottom
        anchors.topMargin: 5
        anchors.bottom: parent.bottom
        anchors.bottomMargin: 5

//        function f_test(temp)
//        {
//            idTabVersion.f_test1(temp);
//        }

        DatabaseModel {
            id: idDataModel
        }

        Tab {
            id: idTabVersion
            anchors.fill: parent
            title: "版本号"


            //http://stackoverflow.com/questions/27230818/qml-tableview-with-dynamic-number-of-columns
//            TableView {
//                id: idabc
//                anchors.fill: parent

//                /* 一个箭头，点击即可进行排序 */
//                sortIndicatorVisible: true
////                model: idListModel
//                Component.onCompleted: {
//                    console.log("Create complete!.....");
//                }

//            }

            ListModel {
                id: libraryModel
                ListElement {
                    title: "A Masterpiece"
                    author: "Gabriel"
                }
                ListElement {
                    title: "Brilliance"
                    author: "Jens"
                }
                ListElement {
                    title: "Outstanding"
                    author: "Frederik"
                }
            }

            TableView {
                id: idTableView
                TableViewColumn {
                    role: "title"
                    title: "Title"
                    width: 100
                }
                TableViewColumn {
                    role: "author"
                    title: "Author"
                    width: 200
                }
                model: libraryModel

            }


            function f_test1(temp)
            {
                idTableView.addColumn(temp)
            }

        }
        Tab {
            title: "广告"
        }
    }
}



