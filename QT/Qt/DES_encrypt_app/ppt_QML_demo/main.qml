import QtQuick 2.3
import QtQuick.Window 2.2

Window {
    id: idRoot
    width: 1440
    height: 900
    visible: true
    color: sysPalette.window

    function f_goToKeyFirst()
    {
        idKeyFirst.visible = true
        idKeySecond.visible = false
        idDataFirst.visible = false
    }

    function f_goToKeyRound16()
    {
        idKeyFirst.visible = false
        idKeySecond.visible = true
        idDataFirst.visible = false
    }

    function f_goToDataFirst()
    {
        idKeyFirst.visible = false
        idBoxSReplace.visible = false
        idDataFirst.visible = true
    }

    function f_goToBoxSReplace()
    {
        idDataFirst.visible = false
        idBoxSReplace.visible = true
    }

    function f_setSecondOrig56()
    {
        idKeySecond.idKeyOrig_28_2.listModel.clear()
        var temp;
        for (var i = 0; i < idKeyFirst.idGridKey_28_2.listModel.count; ++i) {
            temp = idKeyFirst.idGridKey_28_2.listModel.get(i).value
            idKeySecond.idKeyOrig_28_2.listModel.append({"value": temp, "eColor": "white"})
        }
    }

    function f_setRightDataInBoxS()
    {
        var count = idDataFirst.idDataRightAfterEP.listModel.count
        idBoxSReplace.idRight_8_6.listModel.clear()
        var temp
        for (var i = 0; i < count; ++i) {
            temp = idDataFirst.idDataRightAfterEP.listModel.get(i).value
            idBoxSReplace.idRight_8_6.listModel.append({"value": temp, "eColor": "white"})
            if (i % 6 === 0 || (i + 1) % 6 === 0) {
                idBoxSReplace.idRight_8_6.listModel.set(i, {"eColor": "red"})
            }
        }
    }

    SystemPalette {
        id: sysPalette
    }

    KeyFirstBeforeRound16 {
        id: idKeyFirst
        onSigGotoKeyRound16: f_goToKeyRound16()
        onSigSetKeySecondOrig56: f_setSecondOrig56()
        onSigGotoDataFirst: f_goToDataFirst()
//        visible: false
    }

    KeySecondRound16 {
        id: idKeySecond
        onSigGoToKeyFirst: f_goToKeyFirst()
        visible: false
    }

    DataHandleFirst {
        id: idDataFirst
        onSigGoToKeyFirst: f_goToKeyFirst()
        onSigGoToBoxSReplace: f_goToBoxSReplace()
        onSigSetRightDataInBoxS: f_setRightDataInBoxS()
        visible: false
    }

    BoxSReplace {
        id: idBoxSReplace
        onSigGoDathandleFirst: f_goToDataFirst()
        visible: false
    }

    DESControl {
        z: 2
    }

    Component.onCompleted: {
        //        showFullScreen()
    }

    //    MouseArea {
    //        anchors.fill: parent
    //        onDoubleClicked: {
    //            if (visibility == Window.FullScreen)
    //                showNormal()
    //            else
    //                showFullScreen()
    //        }
    //    }
}
