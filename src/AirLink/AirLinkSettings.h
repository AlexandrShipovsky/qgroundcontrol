#ifndef AIRLINKSETTINGS_H
#define AIRLINKSETTINGS_H

#include "LinkManager.h"
#include "UDPLink.h"
#include "QGCToolbox.h"

class AirLinkManager : public QGCTool
{
    Q_OBJECT
public:
    AirLinkManager(QGCApplication* app, QGCToolbox* toolbox);
    ~AirLinkManager();

    Q_INVOKABLE void connect(QString login,QString pass);
    Q_INVOKABLE void disconnect(void);

private:
    void _authServer(LinkInterface* link, QString login, QString pass);

    UDPConfiguration* _udpConfig;
    const QString AirLinkHost = "air-link.space";
    const QString AirLinkUDPName = "AirLinkUDP";
};

#endif // AIRLINKSETTINGS_H
