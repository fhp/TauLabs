#version check qt
contains(QT_VERSION, ^4\\.[0-7]\\..*) {
    message("Cannot build globalcontrol with Qt version $${QT_VERSION}.")
    error("Cannot build globalcontrol with Qt version $${QT_VERSION}. Use at least Qt 4.8!")
}

include(globalcontrol.pri)

TEMPLATE  = subdirs
CONFIG   += ordered

SUBDIRS = src share copydata
unix:!macx:!isEmpty(copydata):SUBDIRS += bin

copydata.file = copydata.pro
