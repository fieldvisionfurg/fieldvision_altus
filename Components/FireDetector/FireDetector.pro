#-------------------------------------------------
#
# Project created by QtCreator 2015-03-06T18:53:04
#
#-------------------------------------------------

QT       += core gui
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = FireDetector
TEMPLATE = lib
CONFIG += c++11
CONFIG += staticlib

INCLUDEPATH += $$PWD/../../ExternalLibraries/eigen-eigen-3.1.1
INCLUDEPATH += $$PWD/../../ExternalLibraries/CsvUtil
INCLUDEPATH += $$PWD/../../ExternalLibraries/OpenCV2.4.9/build/include
INCLUDEPATH += $$PWD/../../Core/Vision

DEFINES += FIREDETECTOR_LIBRARY

SOURCES += \
    C45Helper.cpp \
    ContourSmooth.cpp \
    FireTrackerSA.cpp \
    RandomForestsHelper.cpp \
    TemporalDataExtractor.cpp \
    TextureGLCM.cpp \
    FrameAnnotation.cpp \
    VideoAnnotation.cpp \
    FireTrackerChenebert.cpp \
    FireTrackerCT.cpp \
    FireColorDetectorCT.cpp \
    MotionDetector.cpp \
    DatasetFire.cpp

HEADERS += \
    C45Helper.h \
    ContourSmooth.h \
    FireTrackerSA.h \
    RandomForestsHelper.h \
    TemporalDataExtractor.h \
    TextureGLCM.h \
    FrameAnnotation.h \
    VideoAnnotation.h \
    FireDetectorCv.h \
    FireTrackerChenebert.h \
    FireTrackerCT.h \
    FireColorDetectorCT.h \
    MotionDetector.h \
    DatasetFire.h

unix {
    target.path = /usr/lib
    INSTALLS += target
}

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../VisionImplementationCv/release/ -lVisionImplementationCv
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../VisionImplementationCv/debug/ -lVisionImplementationCv
else:unix: LIBS += -L$$OUT_PWD/../VisionImplementationCv/ -lVisionImplementationCv

INCLUDEPATH += $$PWD/../VisionImplementationCv
DEPENDPATH += $$PWD/../VisionImplementationCv

win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../VisionImplementationCv/release/libVisionImplementationCv.a
else:win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../VisionImplementationCv/debug/libVisionImplementationCv.a
else:win32:!win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../VisionImplementationCv/release/VisionImplementationCv.lib
else:win32:!win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../VisionImplementationCv/debug/VisionImplementationCv.lib
else:unix: PRE_TARGETDEPS += $$OUT_PWD/../VisionImplementationCv/libVisionImplementationCv.a
