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

//    void setToolbox (QGCToolbox* toolbox) override;

    Q_INVOKABLE void _connect(void);
    void _disconnect(void);

private:
    void _authServer(LinkInterface* link);

    UDPConfiguration* _udpConfig;
    const QString AirLinkHost = "air-link.space";
    const QString AirLinkUDPName = "AirLinkUDP";
};

#endif // AIRLINKSETTINGS_H
