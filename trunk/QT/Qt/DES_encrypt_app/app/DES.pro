TEMPLATE = app

QT += qml quick widgets core sql

SOURCES += main.cpp \
    des_source.cpp \
    YLW_database_model.cpp \
    YLW_database_table_model.cpp

RESOURCES += qml.qrc

RC_FILE = logo.rc

# Additional import path used to resolve QML modules in Qt Creator's code model
QML_IMPORT_PATH =

# Default rules for deployment.
include(deployment.pri)

HEADERS += \
    des_source.h \
    YLW_database_model.h \
    YLW_database_table_model.h
