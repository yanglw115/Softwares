import QtQuick 2.0
import QtQuick.Particles 2.0

Item {
    id: idRoot
    property int type
    property alias imgOpacity: idImage.opacity
    property bool birthing: false
    property bool dying: false
    width: 40
    height: 40

    Behavior on x {
        enabled: birthing
        SpringAnimation { spring: 2; damping: 0.2 }
    }

    Behavior on y {
        SpringAnimation { spring: 2; damping: 0.2 }
    }

    Image {
        id: idImage
        anchors.fill: parent
        opacity: 0
        source: {
            if (0 == type)
                return "./share/blueStone.png"
            else if (1 == type)
                return "./share/greenStone.png"
            else if (2 == type)
                return "./share/redStone.png"
            else
                return "./share/yellowStone.png"
        }

        Behavior on opacity {
            NumberAnimation { properties: "opacity"; duration: 200}
        }
    }

    ParticleSystem {
        id: idSys
        anchors.centerIn: parent
        ImageParticle {
            source: {
                if (0 == type) {
                    return "./share/blueStar.png"
                } else if (1 == type) {
                    return "./share/greenStar.png"
                } else if (2 == type) {
                    return "./share/redStar.png"
                } else {
                    return "./share/yellowStar.png"
                }
            }
            rotationVelocityVariation: 360
        }

        Emitter {
            id: idEmitter
            anchors.centerIn: parent
            emitRate: 0
            lifeSpan: 700
            velocity: AngleDirection { angleVariation: 360; magnitude: 80; magnitudeVariation: 40 }
            size: 16
        }
    }


    states: [
        State {
            name: "AliveState"
            when: birthing == true && dying == false
            PropertyChanges {
                target: idImage
                opacity: 1
            }
        },
        State {
            name: "DeathState"
            when: dying == true
            StateChangeScript {
                script: idEmitter.burst(50)
            }
//            PropertyChanges {
//                target: idImage
//                opacity: 0
//            }
//            StateChangeScript { script: idRoot.destroy() }
        }
    ]
}
