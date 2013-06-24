/**
 ******************************************************************************
 *
 * @file       demoplugin.h
 * @author     The OpenPilot Team, http://www.openpilot.org Copyright (C) 2010.
 * @addtogroup GCSPlugins GCS Plugins
 * @{
 * @addtogroup DemoPlugin Demo Plugin
 * @{
 * @brief A minimal example plugin
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
#include "demoplugin.h"
#include <QtPlugin>
#include <QDebug>
#include <extensionsystem/pluginmanager.h>
#include "uavobjectmanager.h"
#include "uavdataobject.h"

DemoPlugin::DemoPlugin()
{
   // Do nothing
}

DemoPlugin::~DemoPlugin()
{
   // Do nothing
}

bool DemoPlugin::initialize(const QStringList& args, QString *errMsg)
{
    Q_UNUSED(args);
    Q_UNUSED(errMsg);
    
    ExtensionSystem::PluginManager *pm = ExtensionSystem::PluginManager::instance();
    UAVObjectManager *objManager = pm->getObject<UAVObjectManager>();
    
    attitudeObj = dynamic_cast<UAVDataObject*>(objManager->getObject("AttitudeActual"));
    if (attitudeObj != NULL) {
        connect(attitudeObj, SIGNAL(objectUpdated(UAVObject*)), this, SLOT(updateAttitude(UAVObject*)));
    } else {
        qDebug() << "Error: Object is unknown (AttitudeActual).";
    }
    
//     UAVDataObject* obj = dynamic_cast<UAVDataObject*>( objManager->getObject(QString("FlightStatus")) );
//     obj->getField("Armed")->setValue("Disarmed");
//     obj->updated();
    
    return true;
}

void DemoPlugin::extensionsInitialized()
{
    // Do nothing
}

void DemoPlugin::shutdown()
{
    // Do nothing
}

void DemoPlugin::updateAttitude(UAVObject *object1) {
    UAVObjectField * rollField = object1->getField(QString("Roll"));
    UAVObjectField * yawField = object1->getField(QString("Yaw"));
    UAVObjectField * pitchField = object1->getField(QString("Pitch"));
    if(rollField && yawField && pitchField) {
        qDebug() << "roll:\t" << rollField->getDouble() << "\tpitch:\t" << pitchField->getDouble() << "\tyaw:\t" << yawField->getDouble();
    } else {
        qDebug() << "Unable to get one of the fields for attitude update";
    }
}

Q_EXPORT_PLUGIN(DemoPlugin)
