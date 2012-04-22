#ifndef SHIPTREEWIDGET_H
#define SHIPTREEWIDGET_H

#include <QTreeWidget>
#include <QDebug>
#include <QDateTime>
#include "tool.h"
#include <QPair>
#include <QMenu>
#include <QAction>

class ShipItem;

class ShipTreeWidget : public QTreeWidget
{
    Q_OBJECT
public:
    explicit ShipTreeWidget(QWidget *parent = 0);
    static ShipTreeWidget * tree();
    static QPair<QAction *, QList<QAction *> > menu();
    void updateTimeout();
signals:
    void openOrbit(const QString &orbit);
public slots:
    void onBotParsedFlottenansicht(const QList<Schiff> &schiffe);
    void onBotSendShip(const QList<QStringList> &list);

private slots:
    void  onItemDoubleClicked(QTreeWidgetItem* item,int);
private:
    static ShipTreeWidget *mTree;
    QTreeWidgetItemIterator mIterator;
     QHash<QString,ShipItem*> mShips;
    QList<ShipItem*>mTimerShips;
     ShipItem *mBergbau;
     ShipItem *mSquad;

    ShipItem * makeShipItem(const Schiff &s);
    void addBergbauShip(const Schiff &s);

};

class ShipItem : public QTreeWidgetItem
{
public:
    enum Typ {Time=Qt::UserRole,ID,Orbit,Href};
    bool operator<(const QTreeWidgetItem & other ) const;




};
#endif // SHIPTREEWIDGET_H
