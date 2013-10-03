
TEMPLATE = lib 
TARGET = Demo 
 
include(../../taulabsgcsplugin.pri)
# include(uavobjects_dependencies.pri)
include(../../plugins/uavobjects/uavobjects.pri)

HEADERS += demoplugin.h 
SOURCES += demoplugin.cpp 
 
OTHER_FILES += Demo.pluginspec