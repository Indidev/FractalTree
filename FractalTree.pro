#-------------------------------------------------
#
# Project created by QtCreator 2015-09-03T16:37:37
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = FractalTree
TEMPLATE = app

QMAKE_CXXFLAGS += -std=c++11

SOURCES += main.cpp\
        FractalTree.cpp \
    FractalTreeImage.cpp \
    SpinBox.cpp \
    ExtendedButton.cpp

HEADERS  += FractalTree.h \
    FractalTreeImage.h \
    SpinBox.h \
    ExtendedButton.h

FORMS    += FractalTree.ui
