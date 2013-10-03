
TEMPLATE = lib 
TARGET = Video 
 
include(../../taulabsgcsplugin.pri)
# include(uavobjects_dependencies.pri)
include(../../plugins/uavobjects/uavobjects.pri)

HEADERS += videoplugin.h 
SOURCES += videoplugin.cpp 
 
OTHER_FILES += Video.pluginspec