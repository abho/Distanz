#include "odplanet.h"
#include <QDebug>

ODPlanet::ODPlanet()
{
}

void ODPlanet::pars(const QString &str)
{
    QStringList s;
    s<< "pname=\"" << "\"" << "pslots=\"" << "pverz=\""<<"pvmetall=\"" << "pvkristall=\"" << "pvwolfram=\"";
    s<<"pvflour=\"" <<"index=" << "sys="<< ">" << "<" <<"remtimea=" << "/grafik/" << ".gif" << "pbew=\"" << "<td>";
    QString tmp = str;
    int i,j;

    i = tmp.indexOf(s.at(0))+s.at(0).size();
    j = tmp.indexOf(s.at(1),i);
    name = tmp.mid(i,j-i);
//qDebug() << name;
    i = tmp.indexOf(s.at(15))+s.at(15).size();
    j = tmp.indexOf(s.at(1),i);
    bev = tmp.mid(i,j-i);
//qDebug() << bev;
    i = tmp.indexOf(s.at(2),j)+s.at(2).size();
    j = tmp.indexOf(s.at(1),i);
    slot = tmp.mid(i,j-i);
//qDebug() << slot;
    i = tmp.indexOf(s.at(3),j)+s.at(3).size();
    j = tmp.indexOf(s.at(1),i);
    erz = tmp.mid(i,j-i).remove(".");
//qDebug() << erz;
    i = tmp.indexOf(s.at(4),j)+s.at(4).size();
    j = tmp.indexOf(s.at(1),i);
    metal = tmp.mid(i,j-i).remove(".");;
//qDebug() << metal;
    i = tmp.indexOf(s.at(5),j)+s.at(5).size();
    j = tmp.indexOf(s.at(1),i);
    kristal = tmp.mid(i,j-i).remove(".");;
//qDebug() << kristal;
    i = tmp.indexOf(s.at(6),j)+s.at(6).size();
    j = tmp.indexOf(s.at(1),i);
    wolf = tmp.mid(i,j-i).remove(".");;
//qDebug() << wolf;
    i = tmp.indexOf(s.at(7),j)+s.at(7).size();
    j = tmp.indexOf(s.at(1),i);
    flur = tmp.mid(i,j-i).remove(".");;
//qDebug() << flur;
    i = tmp.indexOf(s.at(8),j)+s.at(8).size();
    j = tmp.indexOf(s.at(1),i);
    index = tmp.mid(i,j-i);
//qDebug() << index;
    i = tmp.indexOf(s.at(9),j)+s.at(9).size();    
    int ii = tmp.indexOf(s.at(10),i)+s.at(10).size();
    sys =tmp.mid(i,(ii-2)-i);
    j = tmp.indexOf(s.at(11),ii);
    sysname = tmp.mid(ii,j-ii);

    i = tmp.indexOf(s.at(16),j)+s.at(16).size();
    j = tmp.indexOf(s.at(11),i);
    gala = tmp.mid(i,j-i);
//qDebug() << cords;
    i = tmp.indexOf(s.at(12),j)+s.at(12).size();
  //  qDebug() << i;
    if((i-s.at(12).size())!= -1){
        QPair<QString,QString> b;
        int tmpJ = tmp.indexOf(s.at(10),i);
        b.second = tmp.mid(i,tmpJ-i).remove("'");
        i = tmp.indexOf(s.at(13),j)+s.at(13).size();
        //qDebug() << i;
        j = tmp.indexOf(s.at(14),i);
        //qDebug()  << j;
        b.first = tmp.mid(i,j-i);
        bauauftrag = b;
        //qDebug() << bauauftrag;
    }


}

ODPlanet ODPlanet::fromQuery(const QSqlQuery &q)
{
    ODPlanet p;
    p.gala = QString::number(q.value(0).toInt());
    p.sysname = q.value(1).toString();
    p.sys = q.value(2).toString();
    p.name =q.value(3).toString();
    p.index = q.value(4).toString();
    p.slot = QString::number(q.value(5).toInt());
    p.bev= QString::number(q.value(6).toInt());
    p.erz = QString::number(q.value(7).toInt());
    p.wolf= QString::number(q.value(8).toInt());
    p.flur = QString::number(q.value(9).toInt());
    p.metal = QString::number(q.value(10).toInt());
    p.kristal = QString::number(q.value(11).toInt());
    p.bauauftrag =QPair<QString,QString>(q.value(12).toString().split(":").first(),q.value(12).toString().split(":").at(1));
    p.cords = q.value(13).toString();
    return p;
}

QString ODPlanet::toString()
{
    QString tmp;
    tmp.append(name+":");
    tmp.append(sysname+":");
    tmp.append(index+":");
    tmp.append(slot+":");
    tmp.append("("+bauauftrag.first+","+bauauftrag.second+"):");
    tmp.append(erz+":");
    tmp.append(metal+":");
    tmp.append(kristal+":");
    tmp.append(wolf+":");
    tmp.append(flur);
    return tmp;
}

bool ODPlanet::operator ==(const ODPlanet &p) const
{
    return index == p.index;
}

bool ODPlanet::operator <(const ODPlanet &p) const
{
    return index < p.index;
}
