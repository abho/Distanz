#ifndef GALAGV_H
#define GALAGV_H

#include <QGraphicsView>
#include <QtGui>
#include <QtCore>
#include <QDebug>
#include "odplanet.h"
#include "kampf.h"
#include "tool.h"
#include "dbhandler.h"
#include "sendmenu.h"

class SceneData : public QObject {
public :
    SceneData(QObject *parent = 0):QObject(parent),showESysteme("",false),showOwn(false),momentPos("",NULL),chosen(NULL),gate(NULL){

    }


    ~SceneData();

    QPair<QString,bool> showESysteme;
     bool showOwn;
    int gala;
    QPair<QString,QGraphicsEllipseItem*> momentPos;
    QHash<int,QHash<QString,QGraphicsEllipseItem*> >own;
    QHash<int,QHash<QString, QGraphicsEllipseItem *> > systeme;
    QHash<QString,QHash<QString,QGraphicsEllipseItem*> >eSysteme;
    QList<QGraphicsItem*> mPlayerLogPlanis;
    QGraphicsEllipseItem *chosen;
    QGraphicsEllipseItem *gate;
    QHash<int,Gate> gates;

};


class GalaGV : public QGraphicsView
{
    Q_OBJECT
public:
    explicit GalaGV(QWidget *parent = 0);
    ~GalaGV();
    void setTreeItemToView(QTreeWidgetItem* item);
    void setStatusBarAndView(QLabel *l,GalaGV *secondView);
    void setDB(DBHandler *db);
    void setData( SceneData *d);

    void rmPlayerLogItems();
    QGraphicsEllipseItem* paintKreisFromItem(QTreeWidgetItem *item);
    QGraphicsEllipseItem* paintKreis(QString str,QColor color = Qt::white);
    void setCurrentSelected(QTreeWidgetItem *item);
    void setCalcPos(QString str);

    void setGala(int gala);
    void showESysteme(const QString &ally,bool show);
    void showOwn(bool show);
    void setMomentPos( QString index);
signals:
    void loadPage(QString);
    void sendShip(const QString &,const QString &,const QString &);
     void sendShips(const QList<QString> &,const QString &);
     void calcedNearest(const QList<DistanzPair> & list);
protected:
    void wheelEvent(QWheelEvent *event);
public slots:
     void setPlayerLogItems(const QList<Kampf> & list);
     void onSystemChosen(const QString &id);

private slots:
    void plus();
    void minus();
    void onSelectionChanged();
    void onContextMenuRequest(const QPoint &pos);
private:
    enum Typs {Plani,Mark,BigMarker};
    enum DataTyps { ID=Qt::UserRole,ItemTyp,Cords,Esystem,Bericht};
    enum ItemTyps { StandardSystem,Own,Ally,Moment,Log,Muh,Chosen,GateItem};

    QLabel *mLabelInfo;
    DBHandler *mDB;
    QGraphicsEllipseItem *mSelectedItem;
    QGraphicsEllipseItem *mCalcitem;
   GalaGV *mSecondView;


     QGraphicsEllipseItem * makeMarkerItem(const QString & cords,const QString &sysid, ItemTyps itemtyp , QColor color = Qt::white, Typs typ = Plani);
    QGraphicsLineItem  * makeLineItem(const QString & cords1,const QString & cords2,QColor color = Qt::white);    
    void setChecked(QGraphicsItem * item, bool isChecked);
    void setWerft(QGraphicsItem * item, bool isChecked);
    void clearAllChecked();
    void calcNearSys(const QString &cords);
    int clacDis(const QString &str, qreal x1,qreal y1,qreal z1);
    int clacDis(const QString &str1, const QString &str2);
    void hideItems(const QHash<QString,QGraphicsEllipseItem*> & hash);
    void showItems(const QHash<QString,QGraphicsEllipseItem*> & hash);
    void setGate();

    SceneData *mData;
};



#endif // GALAGV_H
