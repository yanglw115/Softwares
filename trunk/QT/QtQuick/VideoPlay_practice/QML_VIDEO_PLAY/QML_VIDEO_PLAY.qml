import QtQuick 2.5
import QtQuick.Window 2.2
import QtMultimedia 5.5


Window {
    visible: true
    width: 640
    height: width * 9 / 16
    title: "Real Player"
    color: "black"

    VideoOutput {
        id: idVideoOut
        width: parent.width
        height: parent.height
        source: idPlayer
        fillMode: VideoOutput.PreserveAspectFit

        Row {
            id: idRow
            width: parent.width * 0.8
            height: 20
            spacing: 5
            anchors.bottom: parent.bottom
            anchors.bottomMargin: 5
            anchors.horizontalCenter: parent.horizontalCenter
            visible: true
            opacity: 0

            Rectangle {
                id: idPlayControl
                width: 20
                height: parent.height
                color: "black"

                Image {
                    id: idControlImg
                    anchors.fill: parent
                    source: "resource/player_play.png"
                }

                MouseArea {
                    anchors.fill: parent
                    hoverEnabled: true
                    onContainsMouseChanged: {
                        if (containsMouse) {
                            /* 注意下面使用透明度与使用visible是不一样的，visible为false后，hover事件不会传给该object */
                            idRow.opacity = 1
                        } else {
                            idRow.opacity = 0
                        }
                    }

                    onClicked: {
                        if (idPlayer.playbackState == MediaPlayer.PlayingState) {
                            idPlayer.pause()
                        } else {
                            idPlayer.play()
                        }
                    }
                }
            }

            Rectangle {
                id: idProgressBar
                width: parent.width - idPlayControl.width - idPlayTime.width
                height: 20
                radius: 5
                color: "gray"

                Rectangle {
                    id: idProgress
                    /* 下面的width如果不用定时器，则只能1秒动一次，因为当前player的onMediaObjectChanged信号不可用 */
                    width: 0 //(idPlayer.position > 0)? (idPlayer.position * parent.width / idPlayer.duration): 0
                    height: parent.height
                    anchors.left: idProgressBar.left
                    radius: 5
                    color: "green"
                }

                Timer {
                    interval: 50
                    repeat: true
                    running: true
                    onTriggered: {
                        idProgress.width = (idPlayer.position > 0)? (idPlayer.position * parent.width / idPlayer.duration): 0
                    }
                }

                MouseArea {
                    anchors.fill: parent
                    hoverEnabled: true

                    onContainsMouseChanged: {
                        if (containsMouse) {
                            idRow.opacity = 1
                        } else {
                            idRow.opacity = 0
                        }
                    }
                    onClicked: {
                        if (idPlayer.playbackState == MediaPlayer.PlayingState
                                || idPlayer.playbackState == MediaPlayer.PausedState) {
                            /** 这里的mouseX是相对于parent的X，所以不用转换，可以直接使用
                                 另外，seek中的offset是一个绝对位置，并不是相对于当前position的大小 */
                            var offset = mouseX * idPlayer.duration / idProgressBar.width
                            idPlayer.seek(offset)
                        }
                    }
                }
            }

            Item {
                id: idPlayTime
                height: parent.height
                width: 100
                Text {
                    text: idPlayer.positionTime + "/" + idPlayer.durationTime
                    anchors.verticalCenter: parent.verticalCenter
                }
            }
        }

    }

    MediaPlayer {
        id: idPlayer
        autoPlay: true
        loops: 2
        source: "resource/daboo.mp4"

        property int duration_s: duration / 1000 % 60
        property int duration_m: duration / 1000 / 60 % 60
        property int duration_h: duration / 1000 / 60 / 60
        property string durationTime: duration_h + ":" + duration_m + ":" + duration_s

        property int position_s: position / 1000 % 60
        property int position_m: position / 1000 / 60 % 60
        property int position_h: position / 1000 / 60 / 60
        property string positionTime: position_h + ":" + position_m + ":" + position_s

        /* 下面设置视频播放提示速度不生效，应该是QT的问题，这样进度条只能1秒一次，或者想其他方法实现进度条连贯性 */
        onMediaObjectChanged: {
            if (mediaObject) {
                mediaObject.notifyInterval = 50;
            }
        }

        onPlaybackStateChanged: {
            if (idPlayer.playbackState == MediaPlayer.PlayingState) {
                idControlImg.source = "resource/player_pause.png"
            } else {
                idControlImg.source = "resource/player_play.png"
            }
        }
    }
}

