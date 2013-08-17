# USE .subdir AND .depends !
# OTHERWISE PLUGINS WILL BUILD IN WRONG ORDER (DIRECTORIES ARE COMPILED IN PARALLEL)

TEMPLATE  = subdirs

SUBDIRS   = plugin_coreplugin

# Core plugin
plugin_coreplugin.subdir = coreplugin
# Empty UAVGadget - Default for new splits
# plugin_emptygadget.subdir = emptygadget
# plugin_emptygadget.depends = plugin_coreplugin
# SUBDIRS += plugin_emptygadget
# 
# # Welcome plugin
# plugin_welcome.subdir = welcome
# plugin_welcome.depends = plugin_coreplugin
# SUBDIRS += plugin_welcome

# Blank Template Plugin, not compiled by default
SUBDIRS += plugin_demo
plugin_demo.subdir = demo
plugin_demo.depends = plugin_coreplugin
plugin_demo.depends = plugin_uavobjects

SUBDIRS += plugin_video
plugin_video.subdir = video
plugin_video.depends = plugin_coreplugin
plugin_video.depends = plugin_uavobjects

# RawHID connection plugin
SUBDIRS += plugin_rawhid
plugin_rawhid.subdir = rawhid
plugin_rawhid.depends = plugin_coreplugin

# # Serial port connection plugin
# SUBDIRS += plugin_serial
# plugin_serial.subdir = serialconnection
# plugin_serial.depends = plugin_coreplugin

# UAVObjects plugin
SUBDIRS += plugin_uavobjects
plugin_uavobjects.subdir = uavobjects
plugin_uavobjects.depends = plugin_coreplugin

# UAVTalk plugin
SUBDIRS += plugin_uavtalk
plugin_uavtalk.subdir = uavtalk
plugin_uavtalk.depends = plugin_uavobjects
plugin_uavtalk.depends += plugin_coreplugin

# UAVTalkRelay plugin
SUBDIRS += plugin_uavtalkrelay
plugin_uavtalkrelay.subdir = uavtalkrelay
plugin_uavtalkrelay.depends = plugin_uavobjects
plugin_uavtalkrelay.depends += plugin_coreplugin
plugin_uavtalkrelay.depends += plugin_uavtalk

# # IP connection plugin
# plugin_ipconnection.subdir = ipconnection
# plugin_ipconnection.depends = plugin_coreplugin
# SUBDIRS += plugin_ipconnection
# 
# ############################
# #  Board plugins
# # Those plugins define supported board models: each board manufacturer
# # needs to implement a manufacturer plugin that defines all their boards
# ############################
# 
# # Tau Labs project
# plugin_boards_taulabs.subdir = boards_taulabs
# plugin_boards_taulabs.depends = plugin_coreplugin
# plugin_boards_taulabs.depends = plugin_uavobjects
# SUBDIRS += plugin_boards_taulabs
# 
# # OpenPilot project
# plugin_boards_openpilot.subdir = boards_openpilot
# plugin_boards_openpilot.depends = plugin_coreplugin
# SUBDIRS += plugin_boards_openpilot
# 
# # Quantec Networks GmbH
# plugin_boards_quantec.subdir = boards_quantec
# plugin_boards_quantec.depends = plugin_coreplugin
# SUBDIRS += plugin_boards_quantec
# 
# STM boards
plugin_boards_stm.subdir = boards_stm
plugin_boards_stm.depends = plugin_coreplugin
SUBDIRS += plugin_boards_stm
# 
# plugin_sysalarmsmessaging.subdir = sysalarmsmessaging
# plugin_sysalarmsmessaging.depends = plugin_coreplugin
# plugin_sysalarmsmessaging.depends += plugin_uavobjects
# plugin_sysalarmsmessaging.depends += plugin_uavtalk
# SUBDIRS += plugin_sysalarmsmessaging
# 
