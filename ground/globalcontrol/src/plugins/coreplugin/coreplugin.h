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
#ifndef COREPLUGIN_H 
#define COREPLUGIN_H 

#include <extensionsystem/iplugin.h> 
#include "coreimpl.h" 

class CorePlugin : public ExtensionSystem::IPlugin 
{
    Q_OBJECT

public: 
   CorePlugin(); 
   ~CorePlugin(); 

   void extensionsInitialized(); 
   bool initialize(const QStringList & arguments, QString * errorString); 
   void shutdown();

public slots:
    void remoteArgument(const QString&);

private:
    Core::ICore *m_coreimpl;
};

#endif // COREPLUGIN_H