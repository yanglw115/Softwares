import QtQuick 2.0
import QtQuick.Particles 2.0
import QtQuick.Window 2.2
//import QtQuick.Controls 1.4

Window {
    id: idRoot
    width: 800
    height: 600
    color: "black"
    visible: true
// Keys这样一点作用也没有
//    Keys.enabled: true
//    Keys.onEscapePressed: {
//        Qt.quit()
//    }

    /* 下面这样作用才可以 */
    onActiveFocusItemChanged: {
        if (activeFocusItem !== null
                && activeFocusItem.Keys !== undefined) {
            activeFocusItem.Keys.onReleased.disconnect(onKeyReleased)
        }
        if (activeFocusItem !== null) {
            activeFocusItem.Keys.onReleased.connect(onKeyReleased)
        }
    }

    function onKeyReleased(event) {
        Qt.quit()
    }


    ParticleSystem {
        id: idParticle
        //        anchors.centerIn: parent
        anchors.fill: parent
        z: 1

        ImageParticle {
            source: "qrc:/star.png"
            color: '#FFD700'
            colorVariation: 1 /* 颜色变化的范围，从0.0 ~ 1.0 */
            rotation: 75 /* 旋转 */
            rotationVariation: 5 /* 旋转变化范围 */
            rotationVelocity: 360 /* 旋转速度 */
            rotationVelocityVariation: 15 /* 旋转速度变化范围 */
            entryEffect: ImageParticle.Scale /* 图片进入效果 */
        }

        Emitter {
            id: idEmitter
            //            system: idParticle
            //            anchors.centerIn: parent
            //            anchors.fill: parent
            anchors.left: parent.left
            anchors.verticalCenter: parent.verticalCenter

            width: 1
            height: 1
            lifeSpan: 15000 /* 粒子生命周期 */
            lifeSpanVariation: 1000 /* 生命周期变化范围 */
            size: 16
            endSize: 32 /* 生命周期结束时的变化范围 */
            //            enabled: false
            emitRate: 100

            velocity: AngleDirection {
                angle: 0 /* 粒子运动的角度，0代表沿X轴向右 */
                angleVariation: 25 /* 粒子运动角度变化范围 */
                /* magnitude每秒钟显示的粒子的数量级,应该与emitRate是相关的,100代表100%,即1倍 */
                magnitude: 100
                magnitudeVariation: 50
            }
//            velocity: PointDirection {
//                x: 100 /* 在水平方向的速度，即每秒钟新产生的粒子数量，这里也是量级，与emiteRate而言，100代表100% */
//                y: 0
//                xVariation: 0
//                yVariation: 100/6
//            }

//            velocity: TargetDirection {
//                /* 运动方向朝着目标的中心进行。如果指定了targetItem，如果没有指定targetItem，则targetX和targetY需要指定 */
//                targetX: 200
//                targetY: -200
//                targetVariation: 100/6
//                magnitude: 100 /* 数量级必须指定，否则默认很可能是0，这样单位时间出来的粒子可能为0，则看不到粒子 */
//                targetItem: idRect
//            }
//            acceleration: AngleDirection {
//                angle: 90
//                magnitude: 25
//            }
        }
    }

//    Rectangle {
//        id: idRect
//        x: 500
//        y: 500
//        width: 50
//        height: 50
//        color: "green"
//    }

    MouseArea {
        anchors.fill: parent
        enabled: true
        onClicked: {
            Qt.quit()
        }
//        anchors.fill: parent
//        onClicked: {
//            //            idEmitter.burst(20)
//            if (idRoot.visibility !== 5)
//                idRoot.showFullScreen()
//            else
//                idRoot.showNormal()
//        }
    }

    Component.onCompleted: {
        idRoot.showFullScreen()
    }
}
