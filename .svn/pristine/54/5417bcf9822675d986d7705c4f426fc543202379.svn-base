TEMPLATE = app
CONFIG += console
#CONFIG -= app_bundle
#CONFIG -= qt
CONFIG += c++11

SOURCES += main.cpp

TARGET = VisionTests

INCLUDEPATH += $$PWD/../../ExternalLibraries/OpenCV2.4.9/build/include

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

win32:CONFIG(release, debug|release): LIBS += -L$$PWD/../../ExternalLibraries/OpenCV2.4.9/build/x86/vc11/lib/ -lopencv_calib3d249
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../../ExternalLibraries/OpenCV2.4.9/build/x86/vc11/lib/ -lopencv_calib3d249d

INCLUDEPATH += $$PWD/../../ExternalLibraries/eigen-eigen-3.1.1
INCLUDEPATH += $$PWD/../../Core/Vision


win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../../Components/FireDetector/release/ -lFireDetector
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../../Components/FireDetector/debug/ -lFireDetector
else:unix:!macx: LIBS += -L$$OUT_PWD/../../Components/FireDetector/ -lFireDetector

INCLUDEPATH += $$PWD/../../Components/FireDetector
DEPENDPATH += $$PWD/../../Components/FireDetector

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../../Components/VisionImplementationCv/release/ -lVisionImplementationCv
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../../Components/VisionImplementationCv/debug/ -lVisionImplementationCv
else:unix: LIBS += -L$$OUT_PWD/../../Components/VisionImplementationCv/ -lVisionImplementationCv

INCLUDEPATH += $$PWD/../../Components/VisionImplementationCv
DEPENDPATH += $$PWD/../../Components/VisionImplementationCv

win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../../Components/VisionImplementationCv/release/libVisionImplementationCv.a
else:win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../../Components/VisionImplementationCv/debug/libVisionImplementationCv.a
else:win32:!win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../../Components/VisionImplementationCv/release/VisionImplementationCv.lib
else:win32:!win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../../Components/VisionImplementationCv/debug/VisionImplementationCv.lib
else:unix: PRE_TARGETDEPS += $$OUT_PWD/../../Components/VisionImplementationCv/libVisionImplementationCv.a

HEADERS +=
