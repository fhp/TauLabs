/**
 ******************************************************************************
 *
 * @file       connectionmanager.cpp
 * @author     The OpenPilot Team, http://www.openpilot.org Copyright (C) 2010.
 *             Parts by Nokia Corporation (qt-info@nokia.com) Copyright (C) 2009.
 * @addtogroup GCSPlugins GCS Plugins
 * @{
 * @addtogroup CorePlugin Core Plugin
 * @{
 * @brief The Core GCS plugin
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

#include "connectionmanager.h"

#include <aggregation/aggregate.h>
#include "iconnection.h"
#include "idevice.h"
#include <extensionsystem/pluginmanager.h>
#include "qextserialport/src/qextserialenumerator.h"
#include "qextserialport/src/qextserialport.h"
#include <QDebug>
#include <QEventLoop>

namespace Core {


ConnectionManager::ConnectionManager(QObject *parent) :
    QObject(parent),
    m_ioDev(NULL),
    polling(true)
{
    qDebug() << "created Connectionmanager";
    // setup our reconnect timers
    reconnect = new QTimer(this);
    reconnectCheck = new QTimer(this);
    connect(reconnect,SIGNAL(timeout()),this,SLOT(reconnectSlot()));
    connect(reconnectCheck,SIGNAL(timeout()),this,SLOT(reconnectCheckSlot()));
}

ConnectionManager::~ConnectionManager()
{
    disconnectDevice();
    suspendPolling();
}

void ConnectionManager::init()
{
    //register to the plugin manager so we can receive
    //new connection object from plugins
    qDebug() << "init Connectionmanager";
    QObject::connect(ExtensionSystem::PluginManager::instance(), SIGNAL(objectAdded(QObject*)), this, SLOT(objectAdded(QObject*)));
    QObject::connect(ExtensionSystem::PluginManager::instance(), SIGNAL(aboutToRemoveObject(QObject*)), this, SLOT(aboutToRemoveObject(QObject*)));
    qDebug() << ExtensionSystem::PluginManager::instance();
}

/**
*   Method called when the user clicks the "Connect" button
*/
bool ConnectionManager::connectDevice(DevListItem connection_device)
{
    qDebug() << "Connect device";
    QIODevice *io_dev = connection_device.connection->openDevice(connection_device.device);
    if (!io_dev)
        return false;

    io_dev->open(QIODevice::ReadWrite);

    // check if opening the device worked
    if (!io_dev->isOpen()) {
        return false;
    }

    // we appear to have connected to the device OK
    // remember the connection/device details
    m_connectionDevice = connection_device;
    m_ioDev = io_dev;

    connect(m_connectionDevice.connection, SIGNAL(destroyed(QObject *)), this, SLOT(onConnectionDestroyed(QObject *)), Qt::QueuedConnection);

    // signal interested plugins that we connected to the device
    emit deviceConnected(io_dev);

    qDebug() << "Connected to device " << connection_device.getConName();
    
    return true;
}

/**
*   Method called by plugins who want to force a disconnection.
*   Used by Uploader gadget for instance.
*/
bool ConnectionManager::disconnectDevice()
{
    if (!m_ioDev) {
        // apparently we are already disconnected: this can
        // happen if a plugin tries to force a disconnect whereas
        // we are not connected. Just return.
        return false;
    }

    // We are connected - disconnect from the device

    // stop our timers
    if(reconnect->isActive())
        reconnect->stop();
    if(reconnectCheck->isActive())
        reconnectCheck->stop();

    // signal interested plugins that user is disconnecting his device
    emit deviceAboutToDisconnect();

    try {
        if (m_connectionDevice.connection) {
            m_connectionDevice.connection->closeDevice(m_connectionDevice.getConName());
        }
    } catch (...) {	// handle exception
        qDebug() << "Exception: m_connectionDevice.connection->closeDevice(" << m_connectionDevice.getConName() << ")";
    }

    m_connectionDevice.connection = NULL;
    m_ioDev = NULL;

    emit deviceDisconnected();
    
    return true;
}

/**
*   Slot called when a plugin added an object to the core pool
*/
void ConnectionManager::objectAdded(QObject *obj)
{
    //Check if a plugin added a connection object to the pool
    IConnection *connection = Aggregation::query<IConnection>(obj);
    if (!connection) return;
    qDebug() << "Object added" << obj;

    //register devices and populate CB
    devChanged(connection);

    // Keep track of the registration to be able to tell plugins
    // to do things
    m_connectionsList.append(connection);

    QObject::connect(connection, SIGNAL(availableDevChanged(IConnection *)), this, SLOT(devChanged(IConnection *)));
}

void ConnectionManager::aboutToRemoveObject(QObject *obj)
{
    //Check if a plugin added a connection object to the pool
    IConnection *connection = Aggregation::query<IConnection>(obj);
    if (!connection) return;

    if (m_connectionDevice.connection && m_connectionDevice.connection == connection)
    {	// we are currently using the one that is about to be removed
        disconnectDevice();
        m_connectionDevice.connection = NULL;
        m_ioDev = NULL;
    }

    if (m_connectionsList.contains(connection))
        m_connectionsList.removeAt(m_connectionsList.indexOf(connection));
}


void ConnectionManager::onConnectionDestroyed(QObject *obj)
{
    Q_UNUSED(obj)
    disconnectDevice();
}

/**
*   Slot called when the telemetry is connected
*/
void ConnectionManager::telemetryConnected()
{
    qDebug() << "TelemetryMonitor: connected";

    if(reconnectCheck->isActive())
        reconnectCheck->stop();
}

/**
*   Slot called when the telemetry is disconnected
*/
void ConnectionManager::telemetryDisconnected()
{
    qDebug() << "TelemetryMonitor: disconnected";

    if (m_ioDev){
        if(m_connectionDevice.connection->shortName()=="Serial") {
            if(!reconnect->isActive())
                reconnect->start(1000);
        }
    }
}

/**
*   Slot called when the telemetry rates are updated
*/
void ConnectionManager::telemetryUpdated(double txRate, double rxRate)
{
	Q_UNUSED(txRate)
	Q_UNUSED(rxRate)
}

void ConnectionManager::reconnectSlot()
{
    qDebug()<<"reconnect";
    if(m_ioDev->isOpen())
        m_ioDev->close();

    if(m_ioDev->open(QIODevice::ReadWrite)) {
        qDebug()<<"reconnect successfull";
        reconnect->stop();
        reconnectCheck->start(20000);
    }
    else
        qDebug()<<"reconnect NOT successfull";
}

void ConnectionManager::reconnectCheckSlot()
{
    reconnectCheck->stop();
    reconnect->start(1000);
}

/**
*   Find a device by its displayed (visible on screen) name
*/
DevListItem ConnectionManager::findDevice(const QString &devName)
{
    foreach (DevListItem d, m_devList)
    {
        if (d.getConName() == devName)
            return d;
    }

    qDebug() << "findDevice: cannot find " << devName << " in device list";

    DevListItem d;
    d.connection = NULL;
    return d;
}

/**
*   Tells every connection plugin to stop polling for devices if they
*   are doing that.
*
*/
void ConnectionManager::suspendPolling()
{
    foreach (IConnection *cnx, m_connectionsList)
    {
        cnx->suspendPolling();
    }

    polling = false;
}

/**
*   Tells every connection plugin to resume polling for devices if they
*   are doing that.
*/
void ConnectionManager::resumePolling()
{
    foreach (IConnection *cnx, m_connectionsList)
    {
        cnx->resumePolling();
    }

    polling = true;
}

/**
 * Synchronize the list of connections displayed with those physically
 * present
 * @param[in] connection Connection type that you want to forget about :)
 */
void ConnectionManager::updateConnectionList(IConnection *connection)
{
    // Get the updated list of devices
    QList <IDevice*> availableDev = connection->availableDevices();

    // Go through the list of connections of that type.  If they are not in the
    // available device list then remove them.  If they are connected, then
    // disconnect them.
    for (QLinkedList<DevListItem>::iterator iter = m_devList.begin(); iter != m_devList.end(); )
    {
        if (iter->connection != connection) {
            ++iter;
            continue;
        }

        // See if device exists in the updated availability list
        bool found = availableDev.contains(iter->device);
        if (!found) {
            // we are currently using the one we are about to erase
            if (m_connectionDevice.connection && m_connectionDevice.connection == connection && m_connectionDevice.device == iter->device) {
                disconnectDevice();
            }

            // We have to delete the IDevice in that DevListItem before getting rid
            // of the iter itself.
            delete(iter->device);

            iter = m_devList.erase(iter);
        } else
            ++iter;
    }

    // Add any back to list that don't exist
    foreach (IDevice *dev, availableDev)
    {
        bool found = m_devList.contains(DevListItem(connection, dev));
        if (!found) {
            registerDevice(connection,dev);
        }
    }
}

/**
*   Register a device from a specific connection plugin
*   @param devN contains the connection shortname + device name which is diplayed in the tooltip
*  @param disp is the name that is displayed in the dropdown menu
*  @param name is the actual device name
*/
void ConnectionManager::registerDevice(IConnection *conn, IDevice *device)
{
    DevListItem d(conn,device);
    m_devList.append(d);
}

/**
*   A device plugin notified us that its device list has changed
*   (eg: user plug/unplug a usb device)
*   \param[in] connection Connection type which signaled the update
*/
void ConnectionManager::devChanged(IConnection *connection)
{
    qDebug() << "Devlist changed" << connection;
    if(!ExtensionSystem::PluginManager::instance()->allPluginsLoaded())
    {
        connectionBackup.append(connection);
        connect(ExtensionSystem::PluginManager::instance(),SIGNAL(pluginsLoadEnded()),this,SLOT(connectionsCallBack()),Qt::UniqueConnection);
        return;
    }
    
    //remove registered devices of this IConnection from the list
    updateConnectionList(connection);

    //add all the list again to the combobox
    foreach (DevListItem d, m_devList)
    {
        if(!m_ioDev && d.getConName().startsWith("USB"))
        {
            qDebug() << "Automatically opening device";
            connectDevice(d);
            qDebug()<<"ConnectionManager::devChanged autoconnected USB device";
        }
    }
    
    qDebug() << "# devices " << m_devList.count();
    emit availableDevicesChanged(m_devList);
}

void ConnectionManager::connectionsCallBack()
{
    foreach(IConnection * con,connectionBackup)
    {
        devChanged(con);
    }
    connectionBackup.clear();
    disconnect(ExtensionSystem::PluginManager::instance(),SIGNAL(pluginsLoadEnded()),this,SLOT(connectionsCallBack()));
}

} //namespace Core
