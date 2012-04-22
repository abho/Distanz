#ifndef DBHANDLER_H
#define DBHANDLER_H

#include <QObject>
#include "../HDB/hdb.h"
#include <QMessageBox>
#include <QtCore>
#include <QtGui>
#include <QDebug>
#include "odplanet.h"
#include "tool.h"
class DBHandler : public QObject
{
    Q_OBJECT
public:
    explicit DBHandler(QObject *parent = 0);
    void insert(QTreeWidgetItem* item);
    void insertPlaneten(int gala,const QString &sysID,const QString &planiID,const QString &name,const QString &spielerid,int slot,int bev);
    void updateName(const QString &cords,const QString &name);
    void updatePlanis(const QString &sysid,const QString &planis);
    void startTransaction();
    void stopTransaction();
    void loadItems(QHash<QString, QTreeWidgetItem *> &list, int gala);
    const QHash<QString,System> & systeme(int gala);
    bool insertEsystem(const ESystem & sys );
    void insertEsystemList(QHash<QString, ESystem> &esys, const QString &str);
    void saveOwnPlanis(const QList<ODPlanet> &planiList);
    void insetrGate(const Gate &g);
    QList<ODPlanet> loadOwnPlanis(int gala);
    QStringList containsPlaniToGala(const QString &name,int gala);
    QList<ESystem> eSysteme(const QString &ally,int gala );
    void setESystemChecked(const QString &id, bool isChecked);
    void clearChecked(const QString &ally, int gala );
    void setESystemIsWerft(const QString &id, bool isWerft);
    QList<Gate> gates();
    QString cords(const QString &id);

signals:
public slots:
private:
    HDB mDB;
      QHash<QString,QHash<QString,ESystem> >mESystem;
    QHash<int,QHash<QString,System> >mSystem;
    void loadESysteme(const QString &ally);
    void loadSysteme(int gala);
    
};

#endif // DBHANDLER_H
