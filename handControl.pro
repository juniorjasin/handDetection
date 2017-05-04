#-------------------------------------------------
#
# Project created by QtCreator 2017-05-02T16:04:03
#
#-------------------------------------------------

QT       += core gui opengl network multimedia widgets

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = handControl
TEMPLATE = app

DEFINES += NO_DEBUG_ARUCO

unix:INCLUDEPATH += "/usr/include/GL/"                             # OpenGL
#unix:LIBS += "/usr/local/lib/x86_64-linux-gnu/libglut.so"          # OpenGL

unix:LIBS += "/usr/local/lib/libopencv_core.so"         # OpenCV
unix:LIBS += "/usr/local/lib/libopencv_highgui.so"      # OpenCV
unix:LIBS += "/usr/local/lib/libopencv_imgproc.so"      # OpenCV
unix:LIBS += "/usr/local/lib/libopencv_objdetect.so"    # OpenCV
unix:LIBS += "/usr/local/lib/libopencv_calib3d.so"      # OpenCV
unix:LIBS += "/usr/local/lib/libopencv_ml.so"           # OpenCV
unix:LIBS += "/usr/local/lib/libopencv_videoio.so"      # OpenCv


SOURCES += main.cpp\
        principal.cpp \
    scene.cpp \

HEADERS  += principal.h \
    scene.h \
    texture.h \

FORMS    += principal.ui
