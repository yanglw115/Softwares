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
    CFaceDetecter.cpp \
    CObjectResult.cpp \
    faceDetect/faceColor.cpp \
    faceDetect/faceLandmarkDetect.cpp \
    faceDetect/faceSpotsDetect.cpp \
    faceDetect/faceTamura.cpp

HEADERS  += mainwindow.h \
    vs_charset.h \
    CLabelImageDrag.h \
    CResultDetail.h \
    glob.h \
    CFaceDetecter.h \
    CObjectResult.h \
    faceDetect/faceColor.h \
    faceDetect/faceLandmarkDetect.h \
    faceDetect/faceSpotsDetect.h \
    faceDetect/faceTamura.h \
    faceDetect/faceGlobal.h

RESOURCES += \
    resource.qrc

win32: LIBS += -L$$PWD/libs/ -ldlib -lopencv_calib3d331 -lopencv_core331 -lopencv_dnn331 \
            -lopencv_features2d331 -lopencv_ffmpeg331_64 -lopencv_flann331 -lopencv_highgui331 \
            -lopencv_imgcodecs331 -lopencv_imgproc331 -lopencv_ml331 -lopencv_objdetect331 \
            -lopencv_photo331 -lopencv_shape331 -lopencv_stitching331 -lopencv_superres331 \
            -lopencv_video331 -lopencv_videoio331 -lopencv_videostab331

INCLUDEPATH += $$PWD/include
DEPENDPATH += $$PWD/libs

win32:!win32-g++: PRE_TARGETDEPS += $$PWD/libs/dlib.lib

