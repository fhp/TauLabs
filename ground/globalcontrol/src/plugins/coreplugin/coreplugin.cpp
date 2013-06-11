/**
 ******************************************************************************
 *
 * @file       coreplugin.h
 * @author     The OpenPilot Team, http://www.openpilot.org Copyright (C) 2010.
 * @addtogroup GCSPlugins GCS Plugins
 * @{
 * @addtogroup CorePlugin Core Plugin
 * @{
 * @brief A command line core plugin
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
#include "coreplugin.h" 
#include <QtPlugin>
#include <QStringList>
#include <QDebug>
#include "coreimpl.h"

CorePlugin::CorePlugin() :
    m_coreimpl(new Core::Internal::CoreImpl)
{ 
} 

CorePlugin::~CorePlugin() 
{ 
   // Do nothing 
} 

bool CorePlugin::initialize(const QStringList& args, QString *errMsg) 
{ 
   Q_UNUSED(args); 
   Q_UNUSED(errMsg); 
   
   m_coreimpl->init();
   
   return true; 
}

void CorePlugin::extensionsInitialized() 
{ 
} 

void CorePlugin::remoteArgument(const QString& arg)
{
   Q_UNUSED(arg);
}

void CorePlugin::shutdown() 
{ 
}

Q_EXPORT_PLUGIN(CorePlugin)
