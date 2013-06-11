/**
 ******************************************************************************
 * @file       uavtalkrelayplugin.c
 * @author     Tau Labs, http://taulabs.org, Copyright (C) 2012-2013
 * @addtogroup GCSPlugins GCS Plugins
 * @{
 * @addtogroup UAVTalk relay plugin
 * @{
 *
 * @brief Relays UAVTalk data trough UDP to another GCS
 *****************************************************************************/
/*
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
 * or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License
 * for more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program; if not, write to the Free Software Foundation, Inc.,
 * 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA
 */

#include "uavtalkrelayplugin.h"

#include <coreplugin/icore.h>
#include <coreplugin/connectionmanager.h>
#include <QtPlugin>

#include "uavobjectmanager.h"
static const QString VERSION = "1.0.0";

UavTalkRelayPlugin::UavTalkRelayPlugin() : relay(0), m_IpAddress(""),
    m_Port(2000), m_DefaultRule(UavTalkRelayComon::ReadWrite)
{
}

UavTalkRelayPlugin::~UavTalkRelayPlugin()
{

}
/**
  * Called once all the plugins which depend on us have been loaded
  */
void UavTalkRelayPlugin::extensionsInitialized()
{

}

/**
  * Called at startup, before any plugin which depends on us is initialized
  */
bool UavTalkRelayPlugin::initialize(const QStringList & arguments, QString * errorString)
{
    Q_UNUSED(arguments);
    Q_UNUSED(errorString);
    ExtensionSystem::PluginManager* pm = ExtensionSystem::PluginManager::instance();
    UAVObjectManager * objMngr = pm->getObject<UAVObjectManager>();
    relay = new UavTalkRelay(objMngr,m_IpAddress,m_Port,rules,m_DefaultRule);
    addAutoReleasedObject(relay);
    return true;
}

void UavTalkRelayPlugin::shutdown()
{

}

Q_EXPORT_PLUGIN(UavTalkRelayPlugin)
