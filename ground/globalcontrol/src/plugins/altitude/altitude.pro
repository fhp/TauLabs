
TEMPLATE = lib 
TARGET = Altitude 
 
include(../../taulabsgcsplugin.pri)
# include(uavobjects_dependencies.pri)
include(../../plugins/uavobjects/uavobjects.pri)

HEADERS += altitudeplugin.h 
SOURCES += altitudeplugin.cpp 
 
OTHER_FILES += Altitude.pluginspec