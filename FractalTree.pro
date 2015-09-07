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
    ExtendedButton.cpp \
    ReadOnlyLineEdit.cpp

HEADERS  += FractalTree.h \
    FractalTreeImage.h \
    SpinBox.h \
    ExtendedButton.h \
    ReadOnlyLineEdit.h

FORMS    += FractalTree.ui


# Define copy command for linux and windows
QMAKE_COPY = cp -f
Win32:QMAKE_COPY = copy /y

# cp(src, dest) returns the copy command
defineReplace(cp) {
    SDIR = $$PWD/$$1
    DDIR = $$OUT_PWD/$$2

    # Replace slashes in paths with backslashes for Windows
    win32:DDIR ~= s,/,\\,g
    win32:SDIR ~= s,/,\\,g

    return($$QMAKE_COPY $$SDIR $$DDIR;)
}
cpFiles.commands += $$cp(ico.png, ./)

#Add dependencies to first
first.depends += cpFiles

#add dependencies to makefile
QMAKE_EXTRA_TARGETS += first cpFiles
