QT += core
QT -= gui

CONFIG += c++11

TARGET = Delaunay
CONFIG += console
CONFIG -= app_bundle

TEMPLATE = app

SOURCES += delaunay2.cpp



unix {
    QMAKE_CXXFLAGS += -std=c++11 -Wall -pedantic -Wno-unknown-pragmas
    INCLUDEPATH += /usr/include \
                    /opt/opencv/opencv-2.4/debug/installed/include

    LIBS += -L/opt/opencv/opencv-2.4/debug/installed/lib \
            -lopencv_core \
            -lopencv_highgui \
            -lopencv_imgproc

    QMAKE_CXXFLAGS_WARN_ON = -Wno-unused-variable -Wno-reorder
}

win32 {
    INCLUDEPATH += D:/opencv2/opencv/build/include

    Debug:LIBS +=  -LD:/opencv2/opencv/build/x86/vc12/lib \
                    -lopencv_core2413d \
                    -lopencv_highgui2413d \
                    -lopencv_imgproc2413d


    Release:LIBS +=  -LD:/opencv2/opencv/build/x86/vc12/lib \
                    -lopencv_core2413 \
                    -lopencv_highgui2413 \
                    -lopencv_imgproc2413
}
