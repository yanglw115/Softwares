#-------------------------------------------------
#
# Project created by QtCreator 2016-07-27T15:15:15
#
#-------------------------------------------------

QT       += core gui sql

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets network

TARGET = DES_Qt
TEMPLATE = app

RC_FILE = logo.rc


SOURCES += main.cpp\
        YLW_main_widget.cpp \
    YLW_des_encrypt_widget.cpp \
    YLW_des_decrypt_widget.cpp \
    YLW_des_source.cpp \
    YLW_file_line_edit.cpp \
    YLW_sql_table_view.cpp \
    YLW_my_table_view.cpp \
    YLW_http_reqeust_tools.cpp

HEADERS  += YLW_main_widget.h \
    YLW_des_encrypt_widget.h \
    YLW_des_decrypt_widget.h \
    YLW_des_source.h \
    YLW_file_line_edit.h \
    YLW_sql_table_view.h \
    YLW_my_table_view.h \
    YLW_VS_char_set.h \
    YLW_http_reqeust_tools.h

RESOURCES += \
    rec.qrc
