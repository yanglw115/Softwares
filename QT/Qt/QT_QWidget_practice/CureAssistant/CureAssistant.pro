#-------------------------------------------------
#
# Project created by QtCreator 2016-08-08T15:51:13
#
#-------------------------------------------------

QT       += core gui serialport multimedia

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = CureAssistant
TEMPLATE = app
RC_FILE = logo.rc

SOURCES += main.cpp \
    main_widget.cpp \
    height_widget.cpp \
    fat_widget.cpp \
    g4_widget.cpp \
    height_device_thread.cpp \
    serial_device_thread.cpp \
    weight_device_thread.cpp \
    utils.cpp \
    measure_widget.cpp \
    myLog.cpp \
    file_line_edit.cpp \
    read_write_interface.cpp

HEADERS  += \
    main_widget.h \
    height_widget.h \
    fat_widget.h \
    g4_widget.h \
    height_device_thread.h \
    serial_device_thread.h \
    weight_device_thread.h \
    utils.h \
    measure_widget.h \
    myLog.h \
    file_line_edit.h \
    read_write_interface.h \
    vs_char_set.h
