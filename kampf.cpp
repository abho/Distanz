#include "kampf.h"
#include <QDebug>

Kampf::Kampf() : planiName(""),cords(""),time("")
{
}

bool Kampf::operator <(const Kampf &other) const
{

    return QDateTime::fromString(time,"HH:mm:ss dd.MM.yy")< QDateTime::fromString(other.time,"HH:mm:ss dd.MM.yy");
}

QString Kampf::toString() const
{
    return time+" | "+owner+" | " +ally+" | "+planiName+" | "+cords;
}
