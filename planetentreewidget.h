#ifndef PLANETENTREEWIDGET_H
#define PLANETENTREEWIDGET_H

#include <QTreeWidget>
#include <QtGui>
#include<QtCore>
#include <QDebug>
#include "tool.h"
#include "sendmenu.h"

class PlanetenTreeWidget : public QTreeWidget
{
    Q_OBJECT
public:
    explicit PlanetenTreeWidget(QWidget *parent = 0);
    void updateTimeout();
    static  PlanetenTreeWidget *tree();
signals:
    void sentShip(const QString &,const QString &,const QString &);
     void sentShips(const QList<QString> &,const QString &);
     void openOrbit(const QString &);
public slots:
   void  onBotParsedPlanetenPage(QTreeWidgetItem * item);

private slots:
   void onCustomContextMenuRequested(const QPoint &p);
   void onItemDoubleClicked(QTreeWidgetItem*,int);

private:
    int calcDistanz(const QString &cords1,const QString & cords2);
     static PlanetenTreeWidget *mTree;
    
};

#endif // PLANETENTREEWIDGET_H
