TEMPLATE = lib
TARGET = Core
DEFINES += CORE_LIBRARY
QT += xml \
    network \
    script \
    svg \
    sql
include(../../taulabsgcsplugin.pri)
include(../../libs/utils/utils.pri)
include(../../shared/scriptwrapper/scriptwrapper.pri)
include(coreplugin_dependencies.pri)
SOURCES += coreplugin.cpp \
    connectionmanager.cpp \
    threadmanager.cpp \
    boardmanager.cpp \
    coreimpl.cpp
HEADERS += coreplugin.h \
    connectionmanager.h \
    threadmanager.h \
    boardmanager.h \
    coreimpl.h \
    icore.h \
    iconnection.h \
    core_global.h \
    idevice.h \
    iboardtype.h
OTHER_FILES += Core.pluginspec
