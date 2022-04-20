#include "QGCApplication.h"
#include "AirLinkSettings.h"
#include "air-link/mavlink_msg_airlink_auth.h"

static mavlink_airlink_auth_t airlink_auth =
{
    "shipovsky.alexandr@yandex.ru",
    "wqfgujq1995",
};

static uint8_t AirLinkMAVBuf[512];
static mavlink_message_t mavmsg;
static ssize_t len = 0;


//-----------------------------------------------------------------------------
AirLinkManager::AirLinkManager(QGCApplication* app, QGCToolbox* toolbox)
    : QGCTool(app, toolbox)
{
    _disconnect();
}

//-----------------------------------------------------------------------------
AirLinkManager::~AirLinkManager()
{
    _disconnect();
}
/*
void AirLinkManager::setToolbox(QGCToolbox* toolbox)
{

}
*/

void AirLinkManager::_connect()
{
    LinkManager* linkMgr = qgcApp()->toolbox()->linkManager();

    _udpConfig = new UDPConfiguration(AirLinkUDPName);
    _udpConfig->setDynamic(true);
    SharedLinkConfigurationPtr config = linkMgr->addConfiguration(_udpConfig);
    linkMgr->createConnectedLink(config);

    _udpConfig->addHost(AirLinkHost,10000);
    _authServer(config->link());
}

void AirLinkManager::_disconnect()
{
    if(_udpConfig == nullptr)
    {
        return;
    }
    if(_udpConfig->_hostList.indexOf(AirLinkHost,0) != -1)
    {
        _udpConfig->removeHost(AirLinkHost);
    }
}

void AirLinkManager::_authServer(LinkInterface* link)
{
    mavlink_msg_airlink_auth_pack(0,0,&mavmsg,airlink_auth.login,airlink_auth.password);
    len = mavlink_msg_to_send_buffer(AirLinkMAVBuf,&mavmsg);
     link->writeBytesThreadSafe((const char *)AirLinkMAVBuf,len);
    qDebug() << "Connect to AirLink";
};
