#include "bottask.h"

BotTask::BotTask(Typ typ) :mTyp (typ)
{

}

BotTask::Typ BotTask::typ()
{
    return mTyp;
}


ShipSendTask::ShipSendTask( const QString &shipID, const QString& orbitID, const QString &zielID,SendTyp typ):
    BotTask(BotTask::ShipSend), mShipID(shipID),mZielID(zielID),mOrbitID(orbitID),mSendTyp(typ)
{

}

const QString &ShipSendTask::shipID() const
{
    return mShipID;
}

const QString &ShipSendTask::zielID()const
{
    return mZielID;
}

const QString &ShipSendTask::orbitID() const
{
    return mOrbitID;
}

const ShipSendTask::SendTyp ShipSendTask::sendTyp()const
{
    return mSendTyp;
}



ShipsSendTask::ShipsSendTask(const QStringList &ships, const QString &zielID, ShipSendTask::SendTyp typ):
    BotTask(BotTask::ShipsSend), mShipIDs(ships),mZielID(zielID),mSendTyp(typ)
{
}

const QStringList &ShipsSendTask::shipIDs() const
{
    return mShipIDs;
}

const QString &ShipsSendTask::zielID() const
{
    return mZielID;
}

const ShipSendTask::SendTyp ShipsSendTask::sendTyp() const
{
    return mSendTyp;
}



LoadSystemeTask::LoadSystemeTask(QHash<QString, QTreeWidgetItem *> *oldSysteme, int gala):
    BotTask(BotTask::LoadSyteme),mOldSyteme(oldSysteme),mGala(gala)
{
}

QHash<QString, QTreeWidgetItem *> *LoadSystemeTask::oldSysteme() const
{
    return mOldSyteme;
}

int LoadSystemeTask::gala() const
{
    return mGala;
}



ParsSystemPage::ParsSystemPage(const QString &sysID): BotTask(BotTask::ParsSystemPage),mSysID(sysID)
{
}

const QString ParsSystemPage::sysID() const
{
    return mSysID;
}


ParsPlayerLogs::ParsPlayerLogs(const QString &playerID):BotTask(BotTask::ParsPlayerLogs),mPlayerID(playerID)
{
}

const QString ParsPlayerLogs::playerID() const
{
    return mPlayerID;
}


LoadPageTask::LoadPageTask(const QString &url):BotTask(BotTask::LoadPage),mUrl(url)
{
}

const QString &LoadPageTask::url() const
{
    return mUrl;
}


LoadOrbitTask::LoadOrbitTask(const QString &orbitID): BotTask(BotTask::LoadOrbit),mOrbitID(orbitID)
{
}

const QString &LoadOrbitTask::orbitID() const
{
    return mOrbitID;
}

LoadPlanetTask::LoadPlanetTask(const QString &orbitID): BotTask(BotTask::LoadPlanet),mOrbitID(orbitID)
{
}

const QString &LoadPlanetTask::orbitID() const
{
    return mOrbitID;
}

LoadSystemTask::LoadSystemTask(const QString &sysID): BotTask(BotTask::LoadSystem),mSysID(sysID)
{
}

const QString &LoadSystemTask::systemID() const
{
    return mSysID;
}
