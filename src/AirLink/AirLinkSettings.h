#pragma once

#include "LinkManager.h"
#include "UDPLink.h"
#include "QGCToolbox.h"
#include "SettingsGroup.h"

class AirLinkManager : public QGCTool
{
    Q_OBJECT
public:
    AirLinkManager(QGCApplication* app, QGCToolbox* toolbox);
    ~AirLinkManager();

    enum AuthStatus {
        Unknown,
        Authenticated,
        Error
    };

    Q_ENUM(AuthStatus)

    Q_PROPERTY(bool                        isconnect           READ isconnect          NOTIFY connectStatusChanged)
    Q_PROPERTY(AirLinkManager::AuthStatus  authStatus          READ authStatus         NOTIFY authStatusChanged)

    Q_INVOKABLE void connect(QString login,QString pass);
    Q_INVOKABLE void disconnect(void);

private:
    void _authServer(LinkInterface* link, QString login, QString pass);

    UDPConfiguration* _udpConfig;
    const QString AirLinkHost = "air-link.space";
    const QString AirLinkUDPName = "AirLinkUDP";

    bool _isconnect;
    AirLinkManager::AuthStatus _authStatus;

    bool isconnect(void){return _isconnect;};
    AirLinkManager::AuthStatus authStatus(void){return _authStatus;};

signals:
        void connectStatusChanged();
        void authStatusChanged();
};

class AirLinkSettings : public SettingsGroup
{
    Q_OBJECT
public:
    AirLinkSettings(QObject* parent = nullptr);

    DEFINE_SETTING_NAME_GROUP()

    DEFINE_SETTINGFACT(userName)
    DEFINE_SETTINGFACT(password)

};

