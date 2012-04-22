#ifndef BOTTASK_H
#define BOTTASK_H
#include<QString>
#include <QStringList>
#include<QtCore>
#include <QtGui>

class BotTask
{
public:
    enum Typ{No,ShipSend,ShipsSend, LoadSyteme,ParsSystemPage,ParsOwnPlaneten,ParsPlayerLogs,LoadPage,
            LoadOrbit,LoadPlanet,LoadSystem,Scannen};
    BotTask(Typ typ);
    Typ typ();
private:
    Typ mTyp;
};


class ShipSendTask: public BotTask{
public:
    enum SendTyp {GMode,VMode,Direkt};
    ShipSendTask(const QString &shipID, const QString &orbitID, const QString &zielID, SendTyp typ = GMode);
    const QString &shipID() const;
    const QString &zielID() const;
    const QString &orbitID() const;
  const SendTyp sendTyp() const;
private:
    QString mShipID;
    QString mZielID;
    QString mOrbitID;
    SendTyp mSendTyp;
};

class ShipsSendTask:public BotTask {
public:
    ShipsSendTask(const QStringList &ships, const QString &zielID, ShipSendTask::SendTyp typ = ShipSendTask::GMode);

    const QStringList &shipIDs() const;
    const QString &zielID()  const;
    const ShipSendTask::SendTyp sendTyp() const;
private:
    QStringList mShipIDs;
    QString mZielID;
    ShipSendTask::SendTyp mSendTyp;
};

class LoadSystemeTask : public BotTask
{
public:
    LoadSystemeTask(QHash<QString, QTreeWidgetItem *> *oldSysteme, int gala);
    QHash<QString, QTreeWidgetItem *> *oldSysteme() const;
    int gala() const;
private:
    QHash<QString, QTreeWidgetItem *> *mOldSyteme;
    int mGala;
};

class ParsSystemPage : public BotTask{
public:
    ParsSystemPage(const QString &sysID);
    const QString sysID() const;
private:
    QString mSysID;
};

class ParsPlayerLogs : public BotTask{
public:
    ParsPlayerLogs(const QString &playerID);
    const QString playerID() const;
private:
    QString mPlayerID;
};

class LoadPageTask: public BotTask{
public:
    LoadPageTask(const QString &url);
    const QString &url() const;
private:
    QString mUrl;
};

class LoadOrbitTask:public BotTask
{
public:
    LoadOrbitTask(const QString &orbitID);
    const QString & orbitID()const;
private:
    QString mOrbitID;
};

class LoadPlanetTask:public BotTask
{
public:
    LoadPlanetTask(const QString &orbitID);
    const QString & orbitID()const;
private:
    QString mOrbitID;
};

class LoadSystemTask:public BotTask
{
public:
    LoadSystemTask(const QString &sysID);
    const QString & systemID()const;
private:
    QString mSysID;
};


#endif // BOTTASK_H
