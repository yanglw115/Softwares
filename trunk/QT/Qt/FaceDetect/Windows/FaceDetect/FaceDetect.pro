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

QMAKE_CXXFLAGS += -DDLIB_JPEG_SUPPORT


win32: LIBS += -L$$PWD/libs -ldlib
win32: LIBS += -L$$PWD/libs -lopencv_calib3d331
win32: LIBS += -L$$PWD/libs -lopencv_core331
win32: LIBS += -L$$PWD/libs -lopencv_dnn331
win32: LIBS += -L$$PWD/libs -lopencv_features2d331
win32: LIBS += -L$$PWD/libs -lopencv_flann331
win32: LIBS += -L$$PWD/libs -lopencv_highgui331
win32: LIBS += -L$$PWD/libs -lopencv_imgcodecs331
win32: LIBS += -L$$PWD/libs -lopencv_imgproc331
win32: LIBS += -L$$PWD/libs -lopencv_ml331
win32: LIBS += -L$$PWD/libs -lopencv_objdetect331
win32: LIBS += -L$$PWD/libs -lopencv_photo331
win32: LIBS += -L$$PWD/libs -lopencv_shape331
win32: LIBS += -L$$PWD/libs -lopencv_stitching331
win32: LIBS += -L$$PWD/libs -lopencv_superres331
win32: LIBS += -L$$PWD/libs -lopencv_ts331
win32: LIBS += -L$$PWD/libs -lopencv_video331
win32: LIBS += -L$$PWD/libs -lopencv_videoio331
win32: LIBS += -L$$PWD/libs -lopencv_videostab331


INCLUDEPATH += $$PWD/include
DEPENDPATH += $$PWD/libs

win32:!win32-g++: PRE_TARGETDEPS += $$PWD/libs/dlib.lib
win32:!win32-g++: PRE_TARGETDEPS += $$PWD/libs/opencv_calib3d331.lib
win32:!win32-g++: PRE_TARGETDEPS += $$PWD/libs/opencv_core331.lib
win32:!win32-g++: PRE_TARGETDEPS += $$PWD/libs/opencv_dnn331.lib
win32:!win32-g++: PRE_TARGETDEPS += $$PWD/libs/opencv_features2d331.lib
win32:!win32-g++: PRE_TARGETDEPS += $$PWD/libs/opencv_flann331.lib
win32:!win32-g++: PRE_TARGETDEPS += $$PWD/libs/opencv_highgui331.lib
win32:!win32-g++: PRE_TARGETDEPS += $$PWD/libs/opencv_imgcodecs331.lib
win32:!win32-g++: PRE_TARGETDEPS += $$PWD/libs/opencv_imgproc331.lib
win32:!win32-g++: PRE_TARGETDEPS += $$PWD/libs/opencv_ml331.lib
win32:!win32-g++: PRE_TARGETDEPS += $$PWD/libs/opencv_objdetect331.lib
win32:!win32-g++: PRE_TARGETDEPS += $$PWD/libs/opencv_photo331.lib
win32:!win32-g++: PRE_TARGETDEPS += $$PWD/libs/opencv_shape331.lib
win32:!win32-g++: PRE_TARGETDEPS += $$PWD/libs/opencv_stitching331.lib
win32:!win32-g++: PRE_TARGETDEPS += $$PWD/libs/opencv_superres331.lib
win32:!win32-g++: PRE_TARGETDEPS += $$PWD/libs/opencv_ts331.lib
win32:!win32-g++: PRE_TARGETDEPS += $$PWD/libs/opencv_video331.lib
win32:!win32-g++: PRE_TARGETDEPS += $$PWD/libs/opencv_videoio331.lib
win32:!win32-g++: PRE_TARGETDEPS += $$PWD/libs/opencv_videostab331.lib
