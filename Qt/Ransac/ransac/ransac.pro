QT += core
QT -= gui

CONFIG += c++11

TARGET = ransac
CONFIG += console
CONFIG -= app_bundle

TEMPLATE = app

SOURCES += main.cpp \
    ../../Console/ConeUnfoldingC/Ellipse.cpp \
    ../../Console/ConeUnfoldingC/Config.cpp \
    ../../Console/ConeUnfoldingC/Line.cpp

HEADERS += \
    ../../Console/ConeUnfoldingC/Ellipse.h \
    ../../Console/ConeUnfoldingC/Config.h \
    ../../Console/ConeUnfoldingC/Line.h

INCLUDEPATH += ../../Console/ConeUnfoldingC/




unix {
    QMAKE_CXXFLAGS += -std=c++11 -Wall -pedantic -Wno-unknown-pragmas
    INCLUDEPATH += /usr/include \
                    /opt/opencv/opencv-2.4/debug/installed/include

    LIBS += -L/opt/opencv/opencv-2.4/debug/installed/lib \
            -lopencv_core \
            -lopencv_highgui \
            -lopencv_imgproc \
            -lopencv_features2d

    QMAKE_CXXFLAGS_WARN_ON = -Wno-unused-variable -Wno-reorder
}

win32 {
    INCLUDEPATH += D:/opencv2/opencv/build/include

    Debug:LIBS +=  -LD:/opencv2/opencv/build/x86/vc12/lib \
                    -lopencv_core2413d \
                    -lopencv_highgui2413d \
                    -lopencv_imgproc2413d \
                    -lopencv_features2d2413d \
                    -lopencv_calib3d2413d


    Release:LIBS +=  -LD:/opencv2/opencv/build/x86/vc12/lib \
                    -lopencv_core2413 \
                    -lopencv_highgui2413 \
                    -lopencv_imgproc2413 \
                    -lopencv_features2d2413 \
                    -lopencv_calib3d2413

#    INCLUDEPATH += D:/opencv3/opencv/build/include

#    Debug:LIBS +=  -LD:/opencv3/opencv/build/x64/vc12/lib \
#                    -lopencv_world310d


#    Release:LIBS +=  -LD:/opencv3/opencv/build/x64/vc12/lib \
#                    -lopencv_world310


}
