QT       -= core gui

TARGET = VisionSensors
TEMPLATE = lib
CONFIG += staticlib
CONFIG += C++11

INCLUDEPATH += $$PWD/../../ExternalLibraries/eigen-eigen-3.1.1
INCLUDEPATH += $$PWD/../../ExternalLibraries/CsvUtil
INCLUDEPATH += $$PWD/../../ExternalLibraries/OpenCV2.4.9/build/include
INCLUDEPATH += $$PWD/../../Core/Vision

HEADERS += VisionSensors.h \
    AnalogMeterDetectorCLT.h \
    LevelTracker.h \
    PresenceTracker.h \
    DiffractionLevelTracker.h \
    AnalogMeterTrackerPC.h

SOURCES += \
    AnalogMeterDetectorCLT.cpp \
    LevelTracker.cpp \
    PresenceTracker.cpp \
    DiffractionLevelTracker.cpp \
    AnalogMeterTrackerPC.cpp

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../VisionImplementationCv/release/ -lVisionImplementationCv
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../VisionImplementationCv/debug/ -lVisionImplementationCv
else:unix:!macx: LIBS += -L$$OUT_PWD/../VisionImplementationCv/ -lVisionImplementationCv

INCLUDEPATH += $$PWD/../VisionImplementationCv
DEPENDPATH += $$PWD/../VisionImplementationCv

win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../VisionImplementationCv/release/libVisionImplementationCv.a
else:win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../VisionImplementationCv/debug/libVisionImplementationCv.a
else:win32:!win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../VisionImplementationCv/release/VisionImplementationCv.lib
else:win32:!win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../VisionImplementationCv/debug/VisionImplementationCv.lib
else:unix:!macx: PRE_TARGETDEPS += $$OUT_PWD/../VisionImplementationCv/libVisionImplementationCv.a
