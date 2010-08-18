TEMPLATE = lib
TARGET = Config
include(../../openpilotgcsplugin.pri)
include(../../plugins/coreplugin/coreplugin.pri)
include(../../plugins/uavobjects/uavobjects.pri)
OTHER_FILES += Config.pluginspec
HEADERS += configplugin.h \
    configgadgetconfiguration.h \
    configgadgetwidget.h \
    configgadgetfactory.h \
    configgadgetoptionspage.h \
    configgadget.h \
    fancytabwidget.h
SOURCES += configplugin.cpp \
    configgadgetconfiguration.cpp \
    configgadgetwidget.cpp \
    configgadgetfactory.cpp \
    configgadgetoptionspage.cpp \
    configgadget.cpp \
    fancytabwidget.cpp
FORMS += settingswidget.ui \
    airframe.ui \
    telemetry.ui
RESOURCES += configgadget.qrc
