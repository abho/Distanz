#ifndef BOT2_H
#define BOT2_H

#include <QObject>
#include <QDebug>
#include <QtGui>
#include <QWebView>
#include <QtCore>
#include "tool.h"
#include "QWebFrame"
#include "QWebElement"
#include "jsobject.h"
#include "bottask.h"
#include "dbhandler.h"
#include "option.h"
#include "parser.h"
#include <QSharedPointer>

class Bot2 : public QObject
{
    Q_OBJECT
public:
    enum OrbitActions {No,MarkAll,Scannen};
    explicit Bot2(QWebView *view,DBHandler *db,Option *option,QObject *parent = 0);
    void setODDBEnable(bool isEnable);
    void loadSysteme(QHash<QString,QTreeWidgetItem*> *oldSysteme, int gala);
    void parsSystemPage(const QString &sysID);
    void parsOwnPlaneten();
    void parsPlayerLogs(const QString &playerID);
    QList<QPair<QString, QString> > parsScoutPage(QWebFrame *frame);
    void parsODDBAllyPage(QWebFrame* frame);
    void login();
    QString parsCurrentToAdd();
    void parsPlanetenPage();
    void fetchPages();
    QTreeWidgetItem *parsFavPage();
    void performOrbitAction(OrbitActions action);
    QList<QPair<QString, QString> > parsSystemFormScout();

signals:
    void setProgressBarVisible(bool);
    void parsedSystemPage(const  QList<ODPlanet> &list);
    void parsedPostSendPage(const QStringList&);
    void parsedOwnPanis(const QList<ODPlanet> &list);
    void parsedPlayerLogs(const QList<Kampf> &);
    void parsedFlottenAnsicht(const QList<Schiff>&);
    void parsedPlanetenPage(QTreeWidgetItem*);
    void updateCredits(const QString &);
    void loadedPage(const QString &);
    void loadedOrbit(const QString &);
    void loadedPlanet(const QString &);
    void loadedSystem(const QString &);
    void parsedWerft2Page(const QList<QTreeWidgetItem*>&);
    void shipsSended(const QList<QStringList> &);
public slots:
    void sendShip(const QString &shipID, const QString &orbitID, const QString &zielOrbitID,ShipSendTask::SendTyp typ = ShipSendTask::GMode);
    void sendShips(const QStringList &ships,const QString & zielID,ShipSendTask::SendTyp typ = ShipSendTask::GMode);
    void loadPage(QString str);
    void loadOrbit(const QString &orbitID);
    void loadPlaneten(const QString &orbitID);
    void loadSystem(const QString &systemID);
private slots:
    void onWebFrameLoadFinished(bool ok);
    void onLoadFinished(bool ok);
    void onRequestFinished(QNetworkReply *re);
private:
    void startIfNotRunning();
    void start();
    void nextTask();
    void loadUrl(const QString & url);

    QWebView *mWebView;
    Parser *mParser;
    JSObject mJSObject;
    QQueue<QSharedPointer<BotTask> >mTasks;
    DBHandler *mDB;
    Option *mOption;


};

#endif // BOT2_H
