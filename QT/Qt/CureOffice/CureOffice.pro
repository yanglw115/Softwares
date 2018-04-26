#-------------------------------------------------
#
# Project created by QtCreator 2018-04-09T15:46:18
#
#-------------------------------------------------

QT       += core gui xlsx

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = CureOffice
TEMPLATE = app
RC_FILE = logo.rc


SOURCES += main.cpp\
        mainwindow.cpp \
    cure_salary.cpp \
    xlsx/xlsx_sheet_model.cpp \
    cure_item_delegate.cpp \
    hanz2pinyin/Hanz2Piny.cpp

HEADERS  += mainwindow.h \
    msvs_charset.h \
    cure_salary.h \
    xlsx/xlsx_sheet_model.h \
    xlsx/xlsx_sheet_model_p.h \
    cure_item_delegate.h \
    hanz2pinyin/utf8/checked.h \
    hanz2pinyin/utf8/core.h \
    hanz2pinyin/utf8/unchecked.h \
    hanz2pinyin/Hanz2Piny.h \
    hanz2pinyin/utf8.h

RESOURCES += \
    resources/imgresource.qrc
