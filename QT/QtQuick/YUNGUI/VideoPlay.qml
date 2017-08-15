import QtQuick 2.4
import QtMultimedia 5.4
import QtQuick.Controls 1.3
import QtQuick.Controls.Styles 1.3
import qt.YG 1.0

Rectangle {
    id: myPlaySurface
    width: 1024
    height: 768

    property alias player: idPlayer
    property bool playlistInitOK: false

    signal sigGotoShopping()

    function f_showAndPlay()
    {
        myPlaySurface.visible = true
        idVideoarea.myVideoOutput.myPlayer.play()
    }

    /* 整个视频控制区域 */
    Rectangle {
        id: idVideoarea
        x: 0
        y: 0
        anchors.fill: parent
        color: "#000000"
        visible: true

        property alias myVideoOutput: myVideoOutput

        /* 视频输出区域 */
        VideoOutput {
            id: myVideoOutput
            source: myPlayer
            anchors.fill: parent

            property alias myPlayer: idPlayer

            /* 播放列表管理类，由C++实现 */
            PlaylistManager {
                id: myPlaylistManager
                onSigInitFinished: {
                    console.log("Play list is init OK!")
                    playlistInitOK = true
                }
            }

            /* 播放器 */
            MediaPlayer {
                id: idPlayer
                source: ""
                property bool bReadyToPlay: false

                onBReadyToPlayChanged: {
                    if (bReadyToPlay) {
                        if (idPlayer.playbackState === MediaPlayer.StoppedState) {
                            idPlayer.source = myPlaylistManager.getNextPlayMediaPath()
                            console.log("Next play:", idPlayer.source)
                            idPlayer.play()
                        }
                    }
                }

                onError: {
                    if (MediaPlayer.NoError != error) {
                        console.log("MediaPlayer play error:", errorString)
                    }
                }

                onPlaybackStateChanged: {
                    console.log("New play state:", idPlayer.playbackState)
                    if (idPlayer.playbackState == MediaPlayer.StoppedState) {
                        if (playlistInitOK) {
                            source = myPlaylistManager.getNextPlayMediaPath()
                            console.log("Next play:", idPlayer.source)
                        }
                        idPlayer.play()
                    }
                }
            }

            /* 按键处理 */
            MouseArea {
                anchors.fill: parent
                onClicked: {
                    if (idPlayer.playbackState == MediaPlayer.StoppedState) {
                        if (playlistInitOK) {
                            idPlayer.source = myPlaylistManager.getNextPlayMediaPath()
                        }
                        idPlayer.play()
                    } else if (idPlayer.playbackState == MediaPlayer.PausedState) {
                        idPlayer.play()
                    } else {
                        idPlayer.pause()
                    }
                }
            }
        }
    }

    /* 右上角展示一个按钮，用于控制进入商品购买界面 */
    Rectangle {
        id: myButton
        z: 2

        height: 40
        width: 120
        radius: 5
        anchors.right: parent.right
        anchors.rightMargin: 10
        anchors.top: parent.top
        anchors.topMargin: 20

        gradient: Gradient {
            GradientStop { position: 0 ; color: "#00CC6C"}
            GradientStop { position: 1 ; color: "#04FDCC"}
        }

        Text {
            anchors.centerIn: parent
            text: "<font color='white'>请点击购物</font>"
            font.family: "WenQuanYi Micro Hei"

        }

        MouseArea {
            anchors.fill: parent
            onClicked: {
                idVideoarea.myVideoOutput.myPlayer.pause()
                sigGotoShopping()
                myPlaySurface.visible = false
            }
        }
    }

    Component.onCompleted: {
        visible = true
    }

}

