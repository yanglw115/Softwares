#-------------------------------------------------
#
# Project created by QtCreator 2016-06-17T11:10:44
#
#-------------------------------------------------

QT       += core gui printsupport widgets

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

DEFINES += WITH_LABEL_IMAGE

TARGET = EasyWord
TEMPLATE = app


SOURCES += main.cpp\
        YLW_image_processor.cpp \
    YLW_central_widget.cpp

HEADERS  += YLW_image_processor.h \
    YLW_central_widget.h

RESOURCES += \
    resource.qrc
