/**
 ******************************************************************************
 *
 * @file       videoplugin.h
 * @author     The OpenPilot Team, http://www.openpilot.org Copyright (C) 2010.
 * @addtogroup GCSPlugins GCS Plugins
 * @{
 * @addtogroup VideoPlugin Video Plugin
 * @{
 * @brief A minimal video plugin
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
#include "videoplugin.h"
#include <QtPlugin>
#include <QDebug>
#include <extensionsystem/pluginmanager.h>
#include "uavobjectmanager.h"
#include "uavdataobject.h"

VideoPlugin::VideoPlugin()
{
   programRunning = false;
   videoProcess = new QProcess(this);
}

VideoPlugin::~VideoPlugin()
{
   if(programRunning) {
	videoProcess->terminate();
	programRunning = false;
   }
}

bool VideoPlugin::initialize(const QStringList& args, QString *errMsg)
{
    Q_UNUSED(args);
    Q_UNUSED(errMsg);
    
    ExtensionSystem::PluginManager *pm = ExtensionSystem::PluginManager::instance();
    UAVObjectManager *objManager = pm->getObject<UAVObjectManager>();
    
    flightStatusObj = dynamic_cast<UAVDataObject*>(objManager->getObject("FlightStatus"));
    if (flightStatusObj != NULL) {
        connect(flightStatusObj, SIGNAL(objectUpdated(UAVObject*)), this, SLOT(updateFlightStatus(UAVObject*)));
	qDebug() << "FlightStatus object connected.";
    } else {
        qDebug() << "Error: Object is unknown (FlightStatus).";
    }
    
    return true;
}

void VideoPlugin::extensionsInitialized()
{
    // Do nothing
}

void VideoPlugin::shutdown()
{
    // Do nothing
}

void VideoPlugin::updateFlightStatus(UAVObject *object1) {
    qDebug() << "FlightStatus object changed.";
    UAVObjectField * arm = object1->getField(QString("Armed"));
    if(arm->getValue() == "Armed") {
        qDebug() << "Armed";
	if(!programRunning) {
		qDebug() << "starting video";
		videoProcess->startDetached("/root/video.sh");
		programRunning = true;
	}
    } else {
        qDebug() << "Not Armed";
	if(programRunning) {
		qDebug() << "stopping video";
		videoProcess->terminate();
		programRunning = false;
	}
    }
}

Q_EXPORT_PLUGIN(VideoPlugin)
