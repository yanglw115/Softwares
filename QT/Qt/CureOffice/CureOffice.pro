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

INCLUDEPATH += tools salary log

SOURCES += main.cpp\
        mainwindow.cpp \
    salary/cure_salary.cpp \
    salary/cure_tableheaderview.cpp \
    tools/hanz2pinyin/Hanz2Piny.cpp \
    tools/smtpClient/emailaddress.cpp \
    tools/smtpClient/mimeattachment.cpp \
    tools/smtpClient/mimecontentformatter.cpp \
    tools/smtpClient/mimefile.cpp \
    tools/smtpClient/mimehtml.cpp \
    tools/smtpClient/mimeinlinefile.cpp \
    tools/smtpClient/mimemessage.cpp \
    tools/smtpClient/mimemultipart.cpp \
    tools/smtpClient/mimepart.cpp \
    tools/smtpClient/mimetext.cpp \
    tools/smtpClient/quotedprintable.cpp \
    tools/smtpClient/smtpclient.cpp \
    tools/xlsx/xlsx_sheet_model.cpp \
    salary/cure_email_dialog.cpp \
    log/cure_log.cpp \
    cure_utils.cpp

HEADERS  += mainwindow.h \
    msvs_charset.h \
    cure_global.h \
    salary/cure_salary.h \
    salary/cure_tableheaderview.h \
    tools/hanz2pinyin/utf8/checked.h \
    tools/hanz2pinyin/utf8/core.h \
    tools/hanz2pinyin/utf8/unchecked.h \
    tools/hanz2pinyin/Hanz2Piny.h \
    tools/hanz2pinyin/utf8.h \
    tools/smtpClient/emailaddress.h \
    tools/smtpClient/mimeattachment.h \
    tools/smtpClient/mimecontentformatter.h \
    tools/smtpClient/mimefile.h \
    tools/smtpClient/mimehtml.h \
    tools/smtpClient/mimeinlinefile.h \
    tools/smtpClient/mimemessage.h \
    tools/smtpClient/mimemultipart.h \
    tools/smtpClient/mimepart.h \
    tools/smtpClient/mimetext.h \
    tools/smtpClient/quotedprintable.h \
    tools/smtpClient/smtpclient.h \
    tools/smtpClient/smtpexports.h \
    tools/smtpClient/SmtpMime.h \
    tools/xlsx/xlsx_sheet_model.h \
    tools/xlsx/xlsx_sheet_model_p.h \
    salary/cure_email_dialog.h \
    log/cure_log.h \
    cure_utils.h

RESOURCES += \
    resources/imgresource.qrc
