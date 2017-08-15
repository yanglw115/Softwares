TEMPLATE = app

QT += qml quick multimedia sql widgets xml serialport
DEFINES += PAY_WITH_CARD_READER=0 # pay with card reader or card releaser

SOURCES += main.cpp \
    YG_playlist_manager.cpp \
    YG_playlist.cpp \
    YG_utils.cpp \
    YG_des_source.cpp \
    devices/CoinAcceptor.cpp \
    devices/SerialDevice.cpp \
    devices/serialportcontrol.cpp \
    util/Configuration.cpp \
    http/httprequest.cpp \
    devices/CoinPayout.cpp \
    devices/NoteAcceptor.cpp \
    YG_goods_info.cpp \
    YG_goods_manager.cpp \
    devices/CardReader.cpp \
    devices/CardReleaser.cpp

RESOURCES += qml.qrc

# Additional import path used to resolve QML modules in Qt Creator's code model
QML_IMPORT_PATH =

# Default rules for deployment.
include(deployment.pri)

HEADERS += \
    YG_playlist_manager.h \
    YG_playlist.h \
    YG_utils.h \
    YG_des_source.h \
    devices/CoinAcceptor.h \
    devices/SerialDevice.h \
    devices/serialportcontrol.h \
    util/Configuration.h \
    http/httprequest.h \
    devices/CoinPayout.h \
    devices/NoteAcceptor.h \
    YG_goods_info.h \
    YG_goods_manager.h \
    devices/CardReader.h \
    devices/CardReleaser.h

