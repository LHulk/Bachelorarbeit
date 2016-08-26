#-------------------------------------------------
#
# Project created by QtCreator 2016-08-26T11:01:33
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = ConsoleUnfoldingGUI
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    CalibrationWizard.cpp

HEADERS  += mainwindow.h \
    CalibrationWizard.h

FORMS    += mainwindow.ui \
    CalibrationWizard.ui
