#-------------------------------------------------
#
# Project created by QtCreator 2015-09-18T21:58:14
#
#-------------------------------------------------

QT       += widgets opengl

TARGET = VisionWidgets
TEMPLATE = lib
CONFIG += staticlib


INCLUDEPATH += $$PWD/../../ExternalLibraries/OpenCV2.4.9/build/include
INCLUDEPATH += $$PWD/../../Core/Vision

SOURCES += \
    FrameServerControlWidget.cpp \
    ImageViewerCv.cpp \
    ProcessControl.cpp \
    ProcessWidget.cpp

HEADERS += \
    DetectorControl.h \
    FrameServerControlWidget.h \
    ImageViewerCv.h \
    ProcessControl.h \
    ProcessWidget.h \
    TrackerControl.h
unix {
    target.path = /usr/lib
    INSTALLS += target
}

FORMS += \
    FrameServerControlWidget.ui \
    ImageViewerCv.ui \
    ProcessWidget.ui
