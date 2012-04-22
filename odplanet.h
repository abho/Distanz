#ifndef ODPLANET_H
#define ODPLANET_H
#include<QString>
#include <QStringList>
#include <QPair>
#include <QSqlQuery>
class ODPlanet
{
public:
    ODPlanet();
    void pars(const QString &str);
    static ODPlanet fromQuery(const QSqlQuery &q);
    bool operator==(const ODPlanet &p) const;
    bool operator<(const ODPlanet &p) const;
    QString toString();
    QString name;
    QString bev;
    QString sysname;
    QString gala;
    QString index;
    QString sys;
    QString slot;
    QPair<QString,QString> bauauftrag;
    QString erz;
    QString wolf;
    QString flur;
    QString metal;
    QString kristal;
    QString cords;
    QString owner;
};

#endif // ODPLANET_H
