#-------------------------------------------------
#
# Project created by QtCreator 2018-03-26T16:35:37
#
#-------------------------------------------------

QT       += core gui network sql

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Macao_Router_app_control_test
TEMPLATE = app
RC_FILE = logo.rc


SOURCES += main.cpp\
        mainDialog.cpp \
    hm_encrypt.c \
    jw_des.c

HEADERS  += mainDialog.h \
    hm_encrypt.h \
    jw_des.h

FORMS    += mainDialog.ui
