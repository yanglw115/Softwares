#-------------------------------------------------
#
# Project created by QtCreator 2018-04-09T15:46:18
#
#-------------------------------------------------

QT       += core gui xlsx network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = CureOffice
TEMPLATE = app
RC_FILE = logo.rc
DEFINES += SMTP_BUILD

SOURCES += main.cpp\
        mainwindow.cpp \
    cure_salary.cpp \
    xlsx/xlsx_sheet_model.cpp \
    cure_item_delegate.cpp \
    hanz2pinyin/Hanz2Piny.cpp \
    smtpClient/emailaddress.cpp \
    smtpClient/mimeattachment.cpp \
    smtpClient/mimecontentformatter.cpp \
    smtpClient/mimefile.cpp \
    smtpClient/mimehtml.cpp \
    smtpClient/mimeinlinefile.cpp \
    smtpClient/mimemessage.cpp \
    smtpClient/mimemultipart.cpp \
    smtpClient/mimepart.cpp \
    smtpClient/mimetext.cpp \
    smtpClient/quotedprintable.cpp \
    smtpClient/smtpclient.cpp \
    cure_tableheaderview.cpp

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
    hanz2pinyin/utf8.h \
    smtpClient/emailaddress.h \
    smtpClient/mimeattachment.h \
    smtpClient/mimecontentformatter.h \
    smtpClient/mimefile.h \
    smtpClient/mimehtml.h \
    smtpClient/mimeinlinefile.h \
    smtpClient/mimemessage.h \
    smtpClient/mimemultipart.h \
    smtpClient/mimepart.h \
    smtpClient/mimetext.h \
    smtpClient/quotedprintable.h \
    smtpClient/smtpclient.h \
    smtpClient/smtpexports.h \
    smtpClient/SmtpMime.h \
    cure_table_view_header.h \
    cure_tableheaderview.h

RESOURCES += \
    resources/imgresource.qrc
