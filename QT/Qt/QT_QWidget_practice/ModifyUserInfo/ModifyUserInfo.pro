#-------------------------------------------------
#
# Project created by QtCreator 2016-04-15T17:56:36
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = ModifyUserInfo
TEMPLATE = app


SOURCES += main.cpp\
        dialog.cpp \
    cbasicinfo.cpp \
    cconnectiions.cpp \
    cdetailinfo.cpp \
    ccontent.cpp

HEADERS  += dialog.h \
    cbasicinfo.h \
    cconnectiions.h \
    cdetailinfo.h \
    ccontent.h

RESOURCES += \
    resouce.qrc
