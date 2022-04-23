#include <QQmlEngine>
#include <QtQml>

#include "QGCApplication.h"
#include "AirLinkSettings.h"
#include "air-link/mavlink_msg_airlink_auth.h"

//-----------------------------------------------------------------------------
AirLinkManager::AirLinkManager(QGCApplication* app, QGCToolbox* toolbox)
    : QGCTool(app, toolbox)
{
    qmlRegisterUncreatableType<AirLinkManager>             ("QGroundControl.Airlink",      1, 0, "AirLinkManager",                "Reference only");

    _isconnect = false;
    _authStatus = Unknown;
}

//-----------------------------------------------------------------------------
AirLinkManager::~AirLinkManager()
{
    disconnect();
}

void AirLinkManager::connect(QString login,QString pass)
{
    LinkManager* linkMgr = qgcApp()->toolbox()->linkManager();

    _udpConfig = new UDPConfiguration(AirLinkUDPName);
    _udpConfig->setDynamic(true);
    SharedLinkConfigurationPtr config = linkMgr->addConfiguration(_udpConfig);
    linkMgr->createConnectedLink(config);

    _udpConfig->addHost(AirLinkHost,10000);
    _authServer(config->link(),login,pass);
}

void AirLinkManager::disconnect()
{
    if(_udpConfig == nullptr)
    {
        return;
    }
    _udpConfig->link()->disconnect();
    if(_udpConfig->_hostList.indexOf(AirLinkHost,0) != -1)
    {
        _udpConfig->removeHost(AirLinkHost);
    }
    _isconnect = false;
    emit connectStatusChanged();
}

void AirLinkManager::_authServer(LinkInterface* link,QString login,QString pass)
{
    mavlink_airlink_auth_t airlink_auth = {.login = {0x00}, .password = {0x00}};
    uint8_t AirLinkMAVBuf[512] = {0x00};
    mavlink_message_t mavmsg;
    ssize_t len = 0;

    std::string logincpy = login.toStdString();
    std::string passcpy = pass.toStdString();

    strcpy(airlink_auth.login,logincpy.c_str());
    strcpy(airlink_auth.password,passcpy.c_str());

    mavlink_msg_airlink_auth_pack(0,0,&mavmsg,airlink_auth.login,airlink_auth.password);
    len = mavlink_msg_to_send_buffer(AirLinkMAVBuf,&mavmsg);
    link->writeBytesThreadSafe((const char *)AirLinkMAVBuf,len);

    // For debug
    qDebug() << "Connect to AirLink";

    _isconnect = true;
    emit connectStatusChanged();

    qDebug() << airlink_auth.login;
    qDebug() << airlink_auth.password;
};



DECLARE_SETTINGGROUP(AirLink, "AirLink")
{
    qmlRegisterUncreatableType<AirLinkSettings>("QGroundControl.SettingsManager", 1, 0, "AirLinkSettings", "Reference only");
}

DECLARE_SETTINGSFACT(AirLinkSettings, userName)
DECLARE_SETTINGSFACT(AirLinkSettings, password)
