/**
 ******************************************************************************
 *
 * @file       altitudeplugin.h
 * @author     The OpenPilot Team, http://www.openpilot.org Copyright (C) 2010.
 * @addtogroup GCSPlugins GCS Plugins
 * @{
 * @addtogroup AltidudePlugin Altidude Plugin
 * @{
 * @brief An altitude sensor plugin
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
#include "altitudeplugin.h"
#include <QtPlugin>
#include <QTimer>
#include <QDebug>
#include <extensionsystem/pluginmanager.h>
#include "uavobjectmanager.h"
#include "uavdataobject.h"
#include "bmp085.c"

AltidudePlugin::AltidudePlugin()
{
}

AltidudePlugin::~AltidudePlugin()
{
}

bool AltidudePlugin::initialize(const QStringList& args, QString *errMsg)
{
    Q_UNUSED(args);
    Q_UNUSED(errMsg);
    bmp085_Calibration();
    
    ExtensionSystem::PluginManager *pm = ExtensionSystem::PluginManager::instance();
    UAVObjectManager *objManager = pm->getObject<UAVObjectManager>();
    
    baroAltitudeObj = dynamic_cast<UAVDataObject*>(objManager->getObject("BaroAltitude"));
    
    QTimer *timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(updateAltitude()));
    timer->start(100);
    
    qDebug() << "Altitude initialized";
    
    return true;
}

void AltidudePlugin::extensionsInitialized()
{
    // Do nothing
}

void AltidudePlugin::shutdown()
{
    // Do nothing
}

void AltidudePlugin::updateAltitude() {
    UAVObjectField * altitude = baroAltitudeObj->getField(QString("Altitude"));
    UAVObjectField * temperature = baroAltitudeObj->getField(QString("Temperature"));
    UAVObjectField * pressure = baroAltitudeObj->getField(QString("Pressure"));
    
    unsigned int temperatureValue = bmp085_GetTemperature(bmp085_ReadUT());
    unsigned int pressureValue = bmp085_GetPressure(bmp085_ReadUP());
    
    altitude->setValue(10);
    temperature->setValue(temperatureValue);
    pressure->setValue(pressureValue);
    
    qDebug() << "Altitude update";
    
    baroAltitudeObj->updated();
}


Q_EXPORT_PLUGIN(AltidudePlugin)
