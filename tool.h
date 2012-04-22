#ifndef TOOL_H
#define TOOL_H

#include <QString>
#include <QDebug>
#include <QStringList>
#include <QSqlQuery>

class Tool
{
public:
    Tool();
    static QString insertDots(const QString &str);
    static QString secsToTimeString(int secs);
    int clacDis(const QString &str, qreal x1, qreal y1, qreal z1);
};



class Schiff
{
public:
    Schiff();
    QString toString();
    QString menuString() const;

    QString name;
    QString id;
    QString planiName;
    QString planiID;
    QString gala;
    QString zeit;
    int count;
    QString owner;
    QString sendUrl;
    QString autoRoute;


};
class ESystem {
public:
    ESystem();
    ESystem (const ESystem& other);
    QString id;
    QString planiID;
    QStringList namen;
    QString ally;
    bool isWerft;
    QString cords;
    int gala;
    bool isChecked;

    static ESystem fromQuery(QSqlQuery &query);
    QString toString();
    bool operator ==(const ESystem &other) const;
    bool operator !=(const ESystem &other) const;


};

Q_DECLARE_METATYPE(ESystem)

class DistanzPair {
public:
    DistanzPair(const ESystem &e, int distanz) : mESystem(e),mDistanz(distanz){
    }

    ESystem mESystem;
    int mDistanz;

    bool operator<(const DistanzPair & other ) const{
        return mDistanz < other.mDistanz;
    }
};

class System {
public:
    System();
    int gala;
    QString name;
    QString sysnr;
    QString cords;
    QString planis;
    bool isChecked;

    static System fromQuery(QSqlQuery &query);
    QString toString();
};

class Gate {
public:
    Gate(int g,const QString& planiid, const QString &sysid):gala(g),planiID(planiid),sysID(sysid){

    }
    Gate(QSqlQuery &query){
        sysID = query.value(2).toString();
        planiID= query.value(1).toString();
       gala = query.value(0).toInt();
    }
    Gate(){

    }

    int gala;
    QString planiID;
    QString sysID;
};


#endif // TOOL_H
