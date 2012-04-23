#include "tool.h"
#include <cmath>

Tool::Tool()
{
}

QString Tool::insertDots(const QString &str)
{
    QString ret = str;
    int one = 3;
    int two = 6;

    if(str.contains(".")){
        one +=4;
        two +=4;
    }
    int size = str.size();
    if(size>one)
        ret.insert(size-(one),",");
    if(size>two)
        ret.insert(size-(two),",");

    return ret;
}

QString Tool::secsToTimeString(int secs)
{
    // qDebug() << Q_FUNC_INFO <<secs ;
    int d= secs/86400;
    secs = secs-d*86400;
    int h = secs/(3600);
    secs=secs-h*3600;
    int m = secs/60;
    secs = secs-m*60;
    int s=secs;

    QString dd = QString::number(d);
    if(dd.size()==1)
        dd.prepend('0');
    QString hh = QString::number(h);
    if(hh.size()==1)
        hh.prepend('0');
    QString mm = QString::number(m);
    if(mm.size()==1)
        mm.prepend('0');
    QString ss = QString::number(s);
    if(ss.size()==1)
        ss.prepend('0');
    return  dd +":"+hh+":"+mm+":"+ss;
}

int Tool::clacDis(const QString &str, qreal x1, qreal y1, qreal z1)
{
    //qDebug() << Q_FUNC_INFO << x1 << y1 << z1;
    qreal x2,y2,z2;
    x2 = str.section("/",0,0).toInt();
    y2 = str.section("/",1,1).toInt();
    z2 = str.section("/",2,2).toInt();
    //qDebug() <<  x2 << y2 << z2;
    x2-=x1;
    y2-=y1;
    z2-=z1;
    x2=pow(x2,2);
    y2=pow(y2,2);
    z2=pow(z2,2);

    qreal sum = x2+y2+z2;
    //qDebug() << x2 << y2 << z2;
    return sqrt(sum);
}


//
//SCHIFF
//

Schiff::Schiff()
{
}

QString Schiff::toString()
{
    return name+":"+id+":"+planiName+":"+planiID+":"+gala+":"+zeit+":"+QString::number(count)+":"+owner+":"+sendUrl;
}

QString Schiff::menuString() const
{
    QString time = zeit;
    if(time != "Unbekannt" && time != "Parken")
        time = zeit.split(";").at(1);
    return time+" | "+name+" | "+planiName ;
}

//
//ESYSTEM
//
bool ESystem::operator ==(const ESystem &other) const
{
    return id == other.id && gala == other.gala && namen == other.namen && ally == other.ally;
}

bool ESystem::operator !=(const ESystem &other) const
{
    return !(*this == other);
}

QString ESystem::toString()
{
    QString ret = QString::number(gala)+":"+id +":"+planiID+":";
    for (int var = 0; var < namen.count(); ++var) {
        ret.append(namen.at(var)+":");
    }
    ret.append(ally+":");
    ret.append(cords+":");
    ret.append(isWerft?"true:":"false:");
    ret.append(isChecked?"true":"false");
    return ret;
}

ESystem::ESystem():isWerft(false),isChecked(false)
{
}

ESystem::ESystem(const ESystem &other)
{
    id = other.id;
    planiID = other.planiID;
    namen = other.namen;
    ally = other.ally;
    isWerft = other.isWerft;
    cords = other.cords;
    gala = other.gala;
    isChecked = other.isChecked;
}

ESystem ESystem::fromQuery(QSqlQuery &query)
{
    ESystem e;


    e.id = query.value(0).toString();
    e.planiID = query.value(1).toString();
    e.namen = query.value(2).toString().split(";");
    e.ally = query.value(3).toString();
    e.isWerft = query.value(4).toInt()==1?true:false;
    e.gala = query.value(5).toInt();
    e.isChecked = query.value(6).toInt()==1?true:false;
    e.cords = query.value(7).toString();

    return e;
}

//
//SYSTEM
//


System::System()
{
}

QString System::toString()
{

    return QString::number(gala)+":"+name+":"+sysnr+":"+cords+":"+planis+(isChecked?":true":":false");
}
System System::fromQuery(QSqlQuery &query){

    System s;
    s.gala = query.value(0).toInt();
    s.name = query.value(1).toString();
    s.sysnr =query.value(2).toString();
    s.cords = query.value(3).toString();
    s.planis = query.value(4).toString();
    s.isChecked =query.value(5).toInt() == 1?true:false;

    return s;
}
