#-------------------------------------------------
#
# Project created by QtCreator 2018-02-01T09:52:37
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = FaceDetect
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    CLabelImageDrag.cpp \
    CResultDetail.cpp \
    CFaceDetecter.cpp

HEADERS  += mainwindow.h \
    vs_charset.h \
    CLabelImageDrag.h \
    CResultDetail.h \
    glob.h \
    CFaceDetecter.h

RESOURCES += \
    resource.qrc
