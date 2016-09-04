#-------------------------------------------------
#
# Project created by QtCreator 2016-08-26T11:01:33
#
#-------------------------------------------------

QT       += core gui

CONFIG += c++11

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = ConsoleUnfoldingGUI
TEMPLATE = app


SOURCES += main.cpp \
        mainwindow.cpp \
    CalibrationWizard.cpp \
    ../../Console/ConeUnfoldingC/Cone.cpp \
    ../../Console/ConeUnfoldingC/Config.cpp \
    ../../Console/ConeUnfoldingC/DotDetection.cpp \
    ../../Console/ConeUnfoldingC/EdgeDetection.cpp \
    ../../Console/ConeUnfoldingC/Ellipse.cpp \
    ../../Console/ConeUnfoldingC/Line.cpp \
    ../../Console/ConeUnfoldingC/Misc.cpp \
    ../../Console/ConeUnfoldingC/Transformation.cpp \
    ../../Console/ConeUnfoldingC/Calibration.cpp \
    CustomWizardPage.cpp \
    QtOpencvCore.cpp \
    CustomGraphicsView.cpp

HEADERS  += mainwindow.h \
    CalibrationWizard.h \
    ../../Console/ConeUnfoldingC/Cone.h \
    ../../Console/ConeUnfoldingC/Config.h \
    ../../Console/ConeUnfoldingC/DotDetection.h \
    ../../Console/ConeUnfoldingC/EdgeDetection.h \
    ../../Console/ConeUnfoldingC/Ellipse.h \
    ../../Console/ConeUnfoldingC/Line.h \
    ../../Console/ConeUnfoldingC/Misc.h \
    ../../Console/ConeUnfoldingC/Transformation.h \
    ../../Console/ConeUnfoldingC/Calibration.h \
    CustomWizardPage.h \
    QtOpencvCore.hpp \
    CustomGraphicsView.h


FORMS    += mainwindow.ui \
    CalibrationWizard.ui


unix {
    QMAKE_CXXFLAGS += -std=c++11 -Wall -pedantic -Wno-unknown-pragmas
    INCLUDEPATH +=  /usr/include \
                    /opt/opencv/opencv-2.4/debug/installed/include \
                    ../../Console/ConeUnfoldingC

    LIBS += -L/opt/opencv/opencv-2.4/debug/installed/lib \
            -lopencv_core \
            -lopencv_highgui \
            -lopencv_imgproc \
            -lopencv_features2d \
            -lopencv_calib3d

    QMAKE_CXXFLAGS_WARN_ON = -Wno-unused-variable -Wno-reorder
}

win32 {
    INCLUDEPATH += D:/opencv2/opencv/build/include \
                    ../../Console/ConeUnfoldingC

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

}
