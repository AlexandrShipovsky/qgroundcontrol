/****************************************************************************
 *
 * (c) 2009-2020 QGROUNDCONTROL PROJECT <http://www.qgroundcontrol.org>
 *
 * QGroundControl is licensed according to the terms in the file
 * COPYING.md in the root of the source code directory.
 *
 ****************************************************************************/

#include <QtGlobal>
#include <QTimer>
#include <QList>
#include <QDebug>
#include <QMutexLocker>
#include <QNetworkProxy>
#include <QNetworkInterface>
#include <iostream>
#include <QHostInfo>

#include "AirLink.h"
#include "QGC.h"
#include "QGCApplication.h"
#include "SettingsManager.h"
#include "AutoConnectSettings.h"

AirLink::AirLink(SharedLinkConfigurationPtr& config)
    : UDPLink     (config)
#if defined(QGC_ZEROCONF_ENABLED)
    , _dnssServiceRef   (nullptr)
#endif
{
    if (!_udpConfig) {
        qWarning() << "Internal error";
    }
    auto allAddresses = QNetworkInterface::allAddresses();
    for (int i=0; i<allAddresses.count(); i++) {
        QHostAddress &address = allAddresses[i];
        _localAddresses.append(QHostAddress(address));
    }
    moveToThread(this);
}

AirLink::~AirLink()
{
    disconnect();
    // Tell the thread to exit
    _running = false;
    // Clear client list
    qDeleteAll(_sessionTargets);
    _sessionTargets.clear();
    quit();
    // Wait for it to exit
    wait();
    this->deleteLater();
}


//--------------------------------------------------------------------------
//-- AirLinkConfiguration

AirLinkConfiguration::AirLinkConfiguration(const QString& name) : UDPConfiguration(name)
{
    AutoConnectSettings* settings = qgcApp()->toolbox()->settingsManager()->autoConnectSettings();
    _localPort = settings->udpListenPort()->rawValue().toInt();
    QString targetHostIP = settings->udpTargetHostIP()->rawValue().toString();
    if (!targetHostIP.isEmpty()) {
        addHost(targetHostIP, settings->udpTargetHostPort()->rawValue().toUInt());
    }
}

AirLinkConfiguration::AirLinkConfiguration(UDPConfiguration* source) : UDPConfiguration(source)
{
    _copyFrom(source);
}

AirLinkConfiguration::~AirLinkConfiguration()
{
    _clearTargetHosts();
}

void AirLinkConfiguration::authorize(const QString login,const QString pass)
{

}

bool AirLink::_connect(void)
{
    if (this->isRunning() || _running) {
        _running = false;
        quit();
        wait();
    }
    _running = true;
    start(NormalPriority);
    return true;
}
