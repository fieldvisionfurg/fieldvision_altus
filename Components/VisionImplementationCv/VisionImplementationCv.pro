QT       -= core gui

TARGET = VisionImplementationCv
TEMPLATE = lib
CONFIG += staticlib
CONFIG += C++11


INCLUDEPATH += $$PWD/../../ExternalLibraries/eigen-eigen-3.1.1
INCLUDEPATH += $$PWD/../../ExternalLibraries/CsvUtil
INCLUDEPATH += $$PWD/../../ExternalLibraries/OpenCV2.4.9/build/include
INCLUDEPATH += $$PWD/../../Core/Vision

HEADERS += \
    ARTagDetectorBLP.h \
    CircleDetectorHTCF.h \
    CircleTrackerHTCF.h \
    ColorBlobDetectorHF.h \
    CvImageGUI.h \
    DatasetFDDB.h \
    FeatureTrackerKLTCv.h \
    FrameServerCv.h \
    LineDetectorHT.h \
    MultiObjectTrackerCCCv.h \
    ObjectDetectorCCCv.h \
    ObjectDetectorFMCv.h \
    ObjectTrackerCCCv.h \
    ObjectTrackerKLT.h \
    ObjectTrackerPFHist.h \
    VisionImplementationCv.h \
    VisionHelperCv.h \
    TemplMatchingDetector.h \
    TemplMatchingTracker.h

SOURCES += \
    ARTagDetectorBLP.cpp \
    CircleDetectorHTCF.cpp \
    CircleTrackerHTCF.cpp \
    ColorBlobDetectorHF.cpp \
    CvImageGUI.cpp \
    DatasetFDDB.cpp \
    FeatureTrackerKLTCv.cpp \
    FrameServerCv.cpp \
    LineDetectorHT.cpp \
    MultiObjectTrackerCCCv.cpp \
    ObjectDetectorCCCv.cpp \
    ObjectDetectorFMCv.cpp \
    ObjectTrackerCCCv.cpp \
    ObjectTrackerKLT.cpp \
    ObjectTrackerPFHist.cpp \
    TemplMatchingDetector.cpp \
    TemplMatchingTracker.cpp



