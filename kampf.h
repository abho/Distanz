#ifndef KAMPF_H
#define KAMPF_H
#include <QString>
#include <QDateTime>

class Kampf
{
public:
    Kampf();

    bool operator<(const Kampf &other) const;
    QString toString() const;
    QString planiName;
    QString cords;
    QString sysid;
    QString time;
    QString owner;
    QString ally;
    QString berichtUrl;
};

#endif // KAMPF_H
