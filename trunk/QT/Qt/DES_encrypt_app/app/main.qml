import QtQuick 2.4
import QtQuick.Controls 1.3
import QtQuick.Window 2.2


ApplicationWindow {
    id: idRoot
    title: qsTr("DES")

    minimumWidth: 500
    minimumHeight: 400
    visible: true

    function f_resize(bMax)
    {
        if (bMax) {
            idRoot.showMaximized()
        } else {
            idRoot.showNormal()
        }
    }

    TabView {
        anchors.left: parent.left
        anchors.leftMargin: 5
        anchors.right: parent.right
        anchors.rightMargin: 5
        anchors.top: parent.top
        anchors.topMargin: 5
        anchors.bottom: parent.bottom
        anchors.bottomMargin: 5
        Tab {
            title: "DES加密"
            Encrypt {
                id: idEncrpyt
                /* 这里无法通过onSigDatabaseOper给f_resize传递参数，没有效果，所以采用组件的属性值作为参数进行传递 */
                onSigDatabaseOper: f_resize(idEncrpyt.bDatabaseOper)
            }

        }
        Tab {
            title: "DES解密"
            Decrypt {id: idDecrypt }
        }
    }

}
