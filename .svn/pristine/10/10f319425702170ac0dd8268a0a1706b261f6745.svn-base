#-------------------------------------------------
#
# Project created by QtCreator 2014-04-23T16:37:29
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = FieldVision
TEMPLATE = app
CONFIG += c++11

SOURCES += main.cpp\
    MainWindow.cpp \
    AnalogMeterControl.cpp \
    LevelTrackerControl.cpp \
    PresenceTrackerControl.cpp \
    ModbusConfigWidget.cpp \
    AnalogMeterControl.cpp \
    FrameServerVimba.cpp \
    GeneralSettings.cpp \
    FireTrackerControl.cpp \
    AboutDialog.cpp \
    DiffractionLevelTrackerControl.cpp \
    AnalogMeterControlNF.cpp \
    FeatureTrackerControl.cpp

HEADERS  += \
    MainWindow.h \
    FrameServerControlWidget.h \
    AnalogMeterControl.h \
    LevelTrackerControl.h \
    PresenceTrackerControl.h \
    ModbusConfigWidget.h \
    FrameServerVimba.h \
    GeneralSettings.h \
    FireTrackerControl.h \
    AboutDialog.h \
    ModbusSensorWrap.h \
    DiffractionLevelTrackerControl.h \
    AnalogMeterControlNF.h \
    FeatureTrackerControl.h

FORMS    += \
    MainWindow.ui \
    ModbusConfigWidget.ui \
    GeneralSettings.ui \
    AboutDialog.ui

INCLUDEPATH += $$PWD/../../Components/VisionWidgets

win32: LIBS += -L$$PWD/../../ExternalLibraries/AVTVimba_1.3/VimbaCPP/Lib/Win32/ -lVimbaCPP

win32:!win32-g++: PRE_TARGETDEPS += $$PWD/../../ExternalLibraries/AVTVimba_1.3/VimbaCPP/Lib/Win32/VimbaCPP.lib
else:win32-g++: PRE_TARGETDEPS += $$PWD/../../ExternalLibraries/AVTVimba_1.3/VimbaCPP/Lib/Win32/libVimbaCPP.a


INCLUDEPATH += $$PWD/../../ExternalLibraries/OpenCV2.4.9/build/include


win32:CONFIG(release, debug|release): LIBS += -L$$PWD/../../Components/VisionImplementationCv/release/ -lVisionImplementationCv
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../../Components/VisionImplementationCv/debug/ -lVisionImplementationCv


win32:CONFIG(release, debug|release): LIBS += -L$$PWD/../../ExternalLibraries/OpenCV2.4.9/build/x86/vc11/lib/ -lopencv_calib3d249
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../../ExternalLibraries/OpenCV2.4.9/build/x86/vc11/lib/ -lopencv_calib3d249d

win32:CONFIG(release, debug|release): LIBS += -L$$PWD/../../ExternalLibraries/OpenCV2.4.9/build/x86/vc11/lib/ -lopencv_imgproc249
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../../ExternalLibraries/OpenCV2.4.9/build/x86/vc11/lib/ -lopencv_imgproc249d

win32:CONFIG(release, debug|release): LIBS += -L$$PWD/../../ExternalLibraries/OpenCV2.4.9/build/x86/vc11/lib/ -lopencv_core249
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../../ExternalLibraries/OpenCV2.4.9/build/x86/vc11/lib/ -lopencv_core249d

win32:CONFIG(release, debug|release): LIBS += -L$$PWD/../../ExternalLibraries/OpenCV2.4.9/build/x86/vc11/lib/ -lopencv_highgui249
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../../ExternalLibraries/OpenCV2.4.9/build/x86/vc11/lib/ -lopencv_highgui249d

win32:CONFIG(release, debug|release): LIBS += -L$$PWD/../../ExternalLibraries/OpenCV2.4.9/build/x86/vc11/lib/ -lopencv_objdetect249
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../../ExternalLibraries/OpenCV2.4.9/build/x86/vc11/lib/ -lopencv_objdetect249d

win32:CONFIG(release, debug|release): LIBS += -L$$PWD/../../ExternalLibraries/OpenCV2.4.9/build/x86/vc11/lib/ -lopencv_video249
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../../ExternalLibraries/OpenCV2.4.9/build/x86/vc11/lib/ -lopencv_video249d

win32:CONFIG(release, debug|release): LIBS += -L$$PWD/../../ExternalLibraries/OpenCV2.4.9/build/x86/vc11/lib/ -lopencv_flann249
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../../ExternalLibraries/OpenCV2.4.9/build/x86/vc11/lib/ -lopencv_flann249d

win32:CONFIG(release, debug|release): LIBS += -L$$PWD/../../ExternalLibraries/OpenCV2.4.9/build/x86/vc11/lib/ -lopencv_nonfree249
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../../ExternalLibraries/OpenCV2.4.9/build/x86/vc11/lib/ -lopencv_nonfree249d

win32:CONFIG(release, debug|release): LIBS += -L$$PWD/../../ExternalLibraries/OpenCV2.4.9/build/x86/vc11/lib/ -lopencv_features2d249
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../../ExternalLibraries/OpenCV2.4.9/build/x86/vc11/lib/ -lopencv_features2d249d

win32:CONFIG(release, debug|release): LIBS += -L$$PWD/../../ExternalLibraries/OpenCV2.4.9/build/x86/vc11/lib/ -lopencv_ml249
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../../ExternalLibraries/OpenCV2.4.9/build/x86/vc11/lib/ -lopencv_ml249d


INCLUDEPATH += $$PWD/../../ExternalLibraries/eigen-eigen-3.1.1
INCLUDEPATH += $$PWD/../../ExternalLibraries/AVTVimba_1.3
INCLUDEPATH += $$PWD/../../Core/Vision
INCLUDEPATH += $$PWD/../../Core/FieldVision
INCLUDEPATH += $$PWD/../../Components/VisionImplementationCv
DEPENDPATH += $$PWD/../../Components/VisionImplementationCv


win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$PWD/../../Components/VisionImplementationCv/release/libVisionImplementationCv.a
else:win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$PWD/../../Components/VisionImplementationCv/debug/libVisionImplementationCv.a
else:win32:!win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$PWD/../../Components/VisionImplementationCv/release/VisionImplementationCv.lib
else:win32:!win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$PWD/../../Components/VisionImplementationCv/debug/VisionImplementationCv.lib

win32: LIBS += -L$$PWD/../../ExternalLibraries/libmodbus-master/src/win32/ -lmodbus

INCLUDEPATH += $$PWD/../../ExternalLibraries/libmodbus-master/src

PRE_TARGETDEPS += $$PWD/../../ExternalLibraries/libmodbus-master/src/win32/modbus.lib



win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../../Components/VisionSensors/release/ -lVisionSensors
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../../Components/VisionSensors/debug/ -lVisionSensors
else:unix:!macx: LIBS += -L$$OUT_PWD/../../Components/VisionSensors/ -lVisionSensors

INCLUDEPATH += $$PWD/../../Components/VisionSensors
DEPENDPATH += $$PWD/../../Components/VisionSensors

win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../../Components/VisionSensors/release/libVisionSensors.a
else:win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../../Components/VisionSensors/debug/libVisionSensors.a
else:win32:!win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../../Components/VisionSensors/release/VisionSensors.lib
else:win32:!win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../../Components/VisionSensors/debug/VisionSensors.lib
else:unix:!macx: PRE_TARGETDEPS += $$OUT_PWD/../../Components/VisionSensors/libVisionSensors.a

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../../Components/FireDetector/release/ -lFireDetector
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../../Components/FireDetector/debug/ -lFireDetector

INCLUDEPATH += $$PWD/../../Components/FireDetector
DEPENDPATH += $$PWD/../../Components/FireDetector

win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../../Components/FireDetector/release/libFireDetector.a
else:win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../../Components/FireDetector/debug/libFireDetector.a
else:win32:!win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../../Components/FireDetector/release/FireDetector.lib
else:win32:!win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../../Components/FireDetector/debug/FireDetector.lib


win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../../Components/VisionWidgets/release/ -lVisionWidgets
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../../Components/VisionWidgets/debug/ -lVisionWidgets

INCLUDEPATH += $$PWD/../../Components/VisionWidgets
DEPENDPATH += $$PWD/../../Components/VisionWidgets

win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../../Components/VisionWidgets/release/libVisionWidgets.a
else:win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../../Components/VisionWidgets/debug/libVisionWidgets.a
else:win32:!win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../../Components/VisionWidgets/release/VisionWidgets.lib
else:win32:!win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../../Components/VisionWidgets/debug/VisionWidgets.lib
