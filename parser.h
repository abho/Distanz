#ifndef PARSER_H
#define PARSER_H

#include <QObject>
#include <QWebFrame>
#include<QWebElementCollection>
#include <QWebElement>
#include "dbhandler.h"
#include <QtGui>
#include "odplanet.h"
#include "kampf.h"
#include "option.h"
#include "tool.h"
#include "bottask.h"
class Parser : public QObject
{
    Q_OBJECT
public:
    enum SendShipTyp {Parken,Direkt};
    explicit Parser(Option *option,DBHandler *db,QObject *parent = 0);
    explicit Parser(QObject *parent = 0);
    QList<Schiff> parsFlottenAnsicht(QWebFrame *frame);
    QList<QTreeWidgetItem*> parsSystemListPage(QWebFrame *frame,QHash<QString,QTreeWidgetItem*> *systeme);
    QList<ODPlanet> parsSystemPage(QWebFrame *frame);
    QList<ODPlanet> parsOwnPlaniListe(QWebFrame *frame);
    QList<Kampf> parsPlayerLog(QWebFrame *frame);
    bool parsSendShipPage(QWebFrame *frame,const QString &zielID,ShipSendTask::SendTyp typ = ShipSendTask::GMode);
    QStringList parsPostSendPage(QWebFrame *frame);
    void login();
    QTreeWidgetItem *parsFavPage(QWebFrame *frame);
     QString parsAddPage(QWebFrame *frame);
    QPair<QTreeWidgetItem *, QString> parsPlanetenPage(QWebFrame *frame, const QString &id= QString());
    QList<QTreeWidgetItem *> parsWerft2Page(QWebFrame *f);
    bool sendShips(QWebFrame *frame, const QList<QString> &ships);
   void parsCurrentODDBPage(QWebFrame *frame);
   void setNoDB(bool noDB);
   QList<QPair<QString,QString> > parsSystem(QWebFrame *frame);
   QList<QPair<QString, QString> > parsScoutPage(QWebFrame *frame);

signals:
    
public slots:
private:

    Option *mOption;
    DBHandler *mDB;
    QString makeTime(int t);
    bool mNoDB;
};

#endif // PARSER_H
