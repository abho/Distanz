#ifndef BOT_H
#define BOT_H

#include <QObject>
#include <QWebView>
#include<QtCore>
#include <QtGui>
#include "page.h"
#include "jsobject.h"
#include "parser.h"
#include "option.h"
#include "bottask.h"
#include "tool.h"


class Bot : public QObject
{
    Q_OBJECT
public:
    explicit Bot(QObject *parent = 0);
    void setViewAndDBAndOption(QWebView *view , DBHandler &db,Option *option);
    void loadSysteme(QHash<QString,QTreeWidgetItem*> *oldSysteme, int gala);
    void parsSystemPage(const QString &sysID, bool pars);
    void parsOwnPlaneten();
    void parsPlayerLogs(const QString &playerID);
    void test();
    void parsScoutPage(QWebFrame *frame);
    void parsCurrent(QWebFrame* frame);
    void login();
    QString parsCurrentToAdd();
    void parsPlanetenPage();
    void fetchPages();
    QTreeWidgetItem *parsFavPage();

signals:
    void setProgressBarVisible(bool);
    void setMessage(QString str);
    void parsedFlottenAnsicht(const QList<Schiff>&);
    void parsedSysteme(const QList<QTreeWidgetItem*> &);
    void parsedSystem(const  QList<ODPlanet> &list);
    void parsedOwnPanis(const QList<ODPlanet> &list);
    void parsedPlayerLogs(const QList<Kampf> &);
    void loggedIn();
    void shipsSended(const QList<QStringList> &);
    void parsedPlanetenPage(QTreeWidgetItem*);
    void parsedWerft2Page(const QList<QTreeWidgetItem*>&);
    void updateCredits(const QString &);
    void parsedScoutPage(const QList<QPair<QString,QString> > &);

public slots:
  void loadPage(QString str);
  void loadOrbit(const QString &orbitID);
  void loadPlaneten(const QString &orbitID);
   void sendShip(const QString &shipID, const QString &planiID, const QString &zielID);
   void sendShips(const QList<QString> &ships,const QString & zielID);
   void loadSystem(const QString &systemID);
    void loadGala( int gala);

private slots:
    void onLoadFinished(bool);
     void insertJS();
     void onRequestFinished(QNetworkReply *re);
     void onCurrentFrameLoadFinished(bool ok);
private:
    QWebView *mView;

    enum Typ{List,Planis,PlaniListe,PlayerLog,Chat,No};
    Typ mTyp;
    QString mJQuery;
    JSObject mJSObject;
    Parser *mParser;
    QPair<QList<QString>,QString> mSendShipsID;
    QHash<QString,QTreeWidgetItem*> *mOldSysteme;
    bool mParsSystemPage;
    bool mParsPlayerLog;
    bool mParsPostSendPage;
    bool mParsGala;
    Option *mOption;
};

#endif // BOT_H
