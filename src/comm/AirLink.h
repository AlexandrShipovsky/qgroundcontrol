/****************************************************************************
 *
 * (c) 2009-2020 QGROUNDCONTROL PROJECT <http://www.qgroundcontrol.org>
 *
 * QGroundControl is licensed according to the terms in the file
 * COPYING.md in the root of the source code directory.
 *
 ****************************************************************************/

#pragma once

#include <QString>
#include <QList>
#include <QMap>
#include <QMutex>
#include <QUdpSocket>
#include <QMutex>
#include <QQueue>
#include <QByteArray>

#if defined(QGC_ZEROCONF_ENABLED)
#include <dns_sd.h>
#endif

#include "QGCConfig.h"
#include "LinkConfiguration.h"
#include "LinkInterface.h"
#include "UDPLink.h"

class AirLinkConfiguration : public UDPConfiguration
{
    Q_OBJECT
public:
    AirLinkConfiguration(const QString& name);
    AirLinkConfiguration(UDPConfiguration* source);
    ~AirLinkConfiguration();

    Q_INVOKABLE void authorize(const QString login,const QString pass);

    LinkType    type                 (void) override                                        { return LinkConfiguration::TypeAirlink; }
    QString     settingsURL          (void) override                                        { return "AirLinkSetting.qml"; }
    QString     settingsTitle        (void) override                                        { return tr("AirLink Settings"); }
};

class AirLink : public UDPLink
{
    Q_OBJECT
public:
    AirLink(SharedLinkConfigurationPtr& config);
    virtual ~AirLink();

    // LinkInterface overrides
    bool _connect(void) override;
};
