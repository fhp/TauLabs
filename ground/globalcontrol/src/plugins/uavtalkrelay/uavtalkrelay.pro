QT += network
TEMPLATE = lib
TARGET = UAVTalkRelay
include(../../taulabsgcsplugin.pri)
include(../uavtalk/uavtalk.pri)
include(uavtalkrelay_dependencies.pri)
HEADERS += \
    uavtalkrelayplugin.h \
    uavtalkrelay_global.h \
    uavtalkrelay.h \
    filtereduavtalk.h
SOURCES += \
    uavtalkrelayplugin.cpp \
    uavtalkrelay.cpp \
    filtereduavtalk.cpp

DEFINES += UAVTALKRELAY_LIBRARY
OTHER_FILES += UAVTalkRelay.pluginspec
