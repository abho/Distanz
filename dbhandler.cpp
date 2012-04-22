#include "dbhandler.h"

typedef QPair<QString,HDB::Typ>	DBPair;
typedef QPair< QString,QVariant> InsertPair;
DBHandler::DBHandler(QObject *parent) :
    QObject(parent)
{
    if(!mDB.open("plani.db")){
        QMessageBox::warning(0,"Fehler","DB konnt nicht geöffnet werden \n"+mDB.lastError().text(),QMessageBox::Ok);
    } else {
        QList<DBPair> list;
        list << DBPair("gala",HDB::Int) << DBPair("name",HDB::Char) << DBPair("sysnr",HDB::Char);
        list << DBPair("cords",HDB::Char) << DBPair("planis",HDB::Char) << DBPair("checked",HDB::Int);

        if(!mDB.createTableIfNoExist("systeme",list,QStringList()<<"sysnr")){
            QMessageBox::warning(0,"Fehler","systeme konnt nicht erstellt werden \n"+mDB.lastError().text(),QMessageBox::Ok);
        }
        list.clear();
        list << DBPair("gala",HDB::Int) << DBPair("sysname",HDB::Char) << DBPair("sysid",HDB::Char);
        list << DBPair("name",HDB::Char) << DBPair("planiid",HDB::Char);
        list << DBPair("slot",HDB::Int) << DBPair("bev",HDB::Int) << DBPair("erz",HDB::Int);
        list << DBPair("wolf",HDB::Int) << DBPair("flur",HDB::Int)<< DBPair("metal",HDB::Int);
        list << DBPair("kristal",HDB::Int)<< DBPair("bauauftrag",HDB::Char);

        if(!mDB.createTableIfNoExist("ownplaneten",list,QStringList()<<"planiid")){
            QMessageBox::warning(0,"Fehler","ownplanete konnt nicht erstellt werden \n"+mDB.lastError().text(),QMessageBox::Ok);
        }
        list.clear();
        list << DBPair("gala",HDB::Int) << DBPair("sysid",HDB::Char) << DBPair("planiid",HDB::Char);
        list << DBPair("name",HDB::Char) <<DBPair("spielerid",HDB::Char) <<DBPair("slot",HDB::Int) << DBPair("bev",HDB::Int);

        if(!mDB.createTableIfNoExist("planeten",list,QStringList()<<"planiid")){
            QMessageBox::warning(0,"Fehler","planeten konnt nicht erstellt werden \n"+mDB.lastError().text(),QMessageBox::Ok);
        }

        list.clear();
        list <<  DBPair("id",HDB::Char) << DBPair("planiid",HDB::Char)<< DBPair("namen",HDB::Char);
        list  <<DBPair("ally",HDB::Char) << DBPair("iswerft",HDB::Int) <<DBPair("gala",HDB::Int) <<DBPair("ischecked",HDB::Int);

        if(!mDB.createTableIfNoExist("esysteme",list,QStringList()<<"id")){
            QMessageBox::warning(0,"Fehler","esyteme konnt nicht erstellt werden \n"+mDB.lastError().text(),QMessageBox::Ok);
        }

        list.clear();
        list <<  DBPair("gala",HDB::Int) << DBPair("planiid",HDB::Char)<< DBPair("sysid",HDB::Char);

        if(!mDB.createTableIfNoExist("gates",list,QStringList()<<"gala")){
            QMessageBox::warning(0,"Fehler","gates konnt nicht erstellt werden \n"+mDB.lastError().text(),QMessageBox::Ok);
        }
    }
}

void DBHandler::insert(QTreeWidgetItem *item)
{
    QList<InsertPair > list;
    list << InsertPair("gala",item->data(0,Qt::UserRole+2).toInt());
    list << InsertPair("name",item->text(0));
    list << InsertPair("sysnr",item->data(0,Qt::UserRole+1).toString());
    list << InsertPair("cords",item->data(0,Qt::UserRole).toString());
    list << InsertPair("planis",item->text(3));
    list << InsertPair("checked",item->text(2).toInt());


    if(!mDB.insert("systeme",list)){
        qDebug() <<"Fehler"<<"insert konnt nicht erstellt werden \n"+mDB.lastError().text();
    }
}

void DBHandler::insertPlaneten(int gala, const QString &sysID, const QString &planiID, const QString &name, const QString &spielerid,int slot, int bev)
{
    QList<InsertPair > list;
    list << InsertPair("gala",gala);
    list << InsertPair("name",name);
    list << InsertPair("sysid",sysID);
    list << InsertPair("spielerid",spielerid.isEmpty()?"unbekannt":spielerid);
    list << InsertPair("planiid",planiID);
    list << InsertPair("slot",slot);
    list << InsertPair("bev",bev);

    qDebug() << gala << bev;

    if(!mDB.insert("planeten",list,true)){
        QMessageBox::warning(0,"Fehler","insert konnt nicht erstellt werden \n"+mDB.lastError().text(),QMessageBox::Ok);
    }
}

void DBHandler::updateName(const QString &cords, const QString &name)
{
    QList<InsertPair > list;
    list << InsertPair("name",name);
    if(!mDB.update("systeme",list,"cords ='"+cords+"'")){
        QMessageBox::warning(0,"Fehler","update konnt nicht ausgeführt werden \n"+mDB.lastError().text(),QMessageBox::Ok);
    }
}

void DBHandler::updatePlanis(const QString &sysid, const QString &planis)
{
    QList<InsertPair > list;
    list << InsertPair("planis",planis);
    if(!mDB.update("systeme",list,"sysnr ='"+sysid+"'")){
        QMessageBox::warning(0,"Fehler","update konnt nicht ausgeführt werden \n"+mDB.lastError().text(),QMessageBox::Ok);
    }
}

void DBHandler::startTransaction()
{
    mDB.startTransaction();
}

void DBHandler::stopTransaction()
{
    mDB.stopTransaction();
}

void DBHandler::loadItems(QHash<QString,QTreeWidgetItem *> & list,int gala)
{


    list.clear();
    QTreeWidgetItem *item;
    bool ok;
    QSqlQuery q = mDB.select(QStringList() <<"systeme",ok,QHash<QString,QStringList>(),QString("gala="+QString::number(gala)));
    if(ok){
        while(q.next()){
            item = new QTreeWidgetItem(QStringList()<<q.value(1).toString()<<"" << QString::number(q.value(5).toInt()) << q.value(4).toString());

            item->setData(0,Qt::UserRole,q.value(3).toString());
            item->setData(0,Qt::UserRole+1,q.value(2).toString());
            item->setData(0,Qt::UserRole+2,q.value(0).toInt());
            list.insert(q.value(3).toString(),item);
        }
    }else {
        QMessageBox::warning(0,"Fehler","select konnt nicht ausgeführt werden \n"+mDB.lastError().text(),QMessageBox::Ok);
    }


}

const QHash<QString, System> &DBHandler::systeme(int gala)
{
    qDebug() << Q_FUNC_INFO;
    if(!mSystem.contains(gala)){
        loadSysteme(gala);
    }
    qDebug() << mSystem.value(gala).size();
    const QHash<QString, System> &r =mSystem[gala];
    qDebug() << Q_FUNC_INFO;
    return r;
}

bool DBHandler::insertEsystem(const ESystem &sys)
{
    QList<InsertPair > list;
    list << InsertPair("gala",sys.gala);
    list << InsertPair("id",sys.id);
    list << InsertPair("planiid",sys.planiID);
    list << InsertPair("namen",sys.namen.join(";"));
    list << InsertPair("ally",sys.ally);
    list<<InsertPair("iswerft",sys.isWerft);
    list << InsertPair("ischecked",sys.isChecked);

    if(!mDB.insert("esysteme",list,true)){
        QMessageBox::warning(0,"Fehler","insert konnt nicht erstellt werden \n"+mDB.lastError().text(),QMessageBox::Ok);
        return false;
    }
    return true;
}

void DBHandler::insertEsystemList( QHash<QString, ESystem> &esys,const QString &str)
{
    qDebug() << Q_FUNC_INFO;
    QHash<QString,ESystem> & hash = mESystem[str];
    if(hash.isEmpty()){
        loadESysteme(str);
    }
    qDebug() << hash.count();
    QMutableHashIterator<QString,ESystem> it(esys);
    while(it.hasNext()){
        it.next();
        if(hash.contains(it.key())){
            ESystem &e = it.value();
            const ESystem &eOld =  hash.value(e.id);
            if(e != eOld){
                if(eOld.isWerft)
                    e.isWerft = true;
                if(eOld.isChecked)
                    e.isChecked = true;
                e.cords =eOld.cords;
                if(insertEsystem(e))
                    hash[e.id]= e;
            }
        }else {
            if( insertEsystem(it.value()))
                hash.insert(it.key(),it.value());
        }
    }
}

void DBHandler::saveOwnPlanis(const QList<ODPlanet> &planiList)
{
    mDB.startTransaction();
    QList<InsertPair > list;
    for (int var = 0; var < planiList.size(); ++var) {
        const ODPlanet &plani = planiList.at(var);
        list.clear();
        list << InsertPair("gala",plani.gala.toInt());
        list << InsertPair("sysname",plani.sysname);
        list << InsertPair("sysid",plani.sys);
        list << InsertPair("name",plani.name);
        list << InsertPair("bev",plani.bev.toInt());
        list << InsertPair("planiid",plani.index);
        list << InsertPair("slot",plani.slot.toInt());;
        list << InsertPair("erz",plani.erz.toInt());
        list << InsertPair("metal",plani.metal.toInt());;
        list << InsertPair("wolf",plani.wolf.toInt());;
        list << InsertPair("flur",plani.flur.toInt());;
        list << InsertPair("kristal",plani.kristal.toInt());
        list << InsertPair("bauauftrag",plani.bauauftrag.first+":"+plani.bauauftrag.second);


        if(!mDB.insert("ownplaneten",list,true)){
            QMessageBox::warning(0,"Fehler","insert konnt nicht erstellt werden \n"+mDB.lastError().text(),QMessageBox::Ok);
        }
    }
    mDB.stopTransaction();

}

void DBHandler::insetrGate(const Gate &g)
{
    QList<InsertPair > list;
    list << InsertPair("gala",g.gala);
    list << InsertPair("planiid",g.planiID);
    list << InsertPair("sysid",g.sysID);

    if(!mDB.insert("gates",list,true)){
        qDebug() <<"Fehler"<<"insert konnt nicht erstellt werden \n"+mDB.lastError().text();
    }


}

QList<ODPlanet> DBHandler::loadOwnPlanis(int gala)
{
    QList<ODPlanet> list;
    bool ok;
    QHash<QString,QStringList> h;
    h.insert("systeme",QStringList() << "cords");
    QString w = "ownplaneten.sysid=systeme.sysnr and ownplaneten.gala="+QString::number(gala);
    QSqlQuery q = mDB.select(QStringList() << "ownplaneten"<<"systeme",ok,h,w);
    if(!ok){
        QMessageBox::warning(0,"Fehler","select konnt nicht ausgeführt werden \n"+mDB.lastError().text(),QMessageBox::Ok);
    }else {
        while(q.next()){
            list.append(ODPlanet::fromQuery(q));
        }
    }
    return list;
}

QStringList DBHandler::containsPlaniToGala(const QString &name, int gala)
{
    bool ok;

    QHash<QString,QStringList> h;
    h.insert("planeten",QStringList() << "name");
    h.insert("systeme",QStringList() << "cords" << "sysnr");
    QString w = "planeten.name='"+name+"' and planeten.gala="+QString::number(gala)+" and planeten.sysid=systeme.sysnr";
    QSqlQuery q= mDB.select(QStringList() << "planeten" << "systeme",ok,h,w);
    if(!ok){
        QMessageBox::warning(0,"Fehler","select konnt nicht ausgeführt werden \n"+mDB.lastError().text(),QMessageBox::Ok);
        return QStringList();
    }else {
        if(q.next()){
            QStringList l;
            for (int var = 0; var < 3; ++var) {
                QVariant v = q.value(var);
                if(v.isValid())
                    l.append(v.toString());
            }
            return l;
        } else {
            return QStringList();
        }
    }
}

QList<ESystem> DBHandler::eSysteme(const QString &ally, int gala)
{
    QList<ESystem> l;

    if(!mESystem.contains(ally))
        loadESysteme(ally);

    QHashIterator<QString,ESystem> it(mESystem.value(ally));
    while(it.hasNext()){
        it.next();
        if(it.value().gala == gala)
            l.append(it.value());
    }
    return l;
}

void DBHandler::setESystemChecked(const QString &id, bool isChecked)
{

    qDebug() << Q_FUNC_INFO << id << isChecked;
    QList<InsertPair > list;
    list << InsertPair("ischecked",isChecked?1:0);
    if(!mDB.update("esysteme",list,"id="+id)){
        QMessageBox::warning(0,"Fehler","update konnt nicht ausgeführt werden \n"+mDB.lastError().text(),QMessageBox::Ok);
    }
}

void DBHandler::clearChecked(const QString &ally, int gala)
{
    qDebug() << Q_FUNC_INFO << gala;
    QList<InsertPair > list;
    list << InsertPair("ischecked",0);
    if(!mDB.update("esysteme",list,"ally='"+ally+"' and gala="+QString::number(gala))){
        QMessageBox::warning(0,"Fehler","update konnt nicht ausgeführt werden \n"+mDB.lastError().text(),QMessageBox::Ok);
    }
}

void DBHandler::setESystemIsWerft(const QString &id, bool isWerft)
{
    qDebug() << Q_FUNC_INFO << id << isWerft;
    QList<InsertPair > list;
    list << InsertPair("iswerft",isWerft?1:0);
    if(!mDB.update("esysteme",list,"id="+id)){
        QMessageBox::warning(0,"Fehler","update konnt nicht ausgeführt werden \n"+mDB.lastError().text(),QMessageBox::Ok);
    }
}

QList<Gate> DBHandler::gates()
{

    QList<Gate> l;
    bool ok;
    QSqlQuery q = mDB.select(QStringList() <<"gates",ok,QHash<QString,QStringList>());
    if(ok){
        while(q.next()){
            l.append(Gate(q));
        }
    }else {
        QMessageBox::warning(0,"Fehler","select konnt nicht ausgeführt werden \n"+mDB.lastError().text(),QMessageBox::Ok);
    }
    return l;
}

QString DBHandler::cords(const QString &id)
{
    QHash<QString,QStringList> hash;
    hash.insert("systeme",QStringList() << "cords");
    bool ok;
    QString ret;
    QSqlQuery q = mDB.select(QStringList() <<"systeme",ok,hash,QString("systeme.sysnr='"+id+"'"));
    if(ok){
        q.next();
        ret = q.value(0).toString();
    }else {
        QMessageBox::warning(0,"Fehler","select konnt nicht ausgeführt werden \n"+mDB.lastError().text(),QMessageBox::Ok);
    }
    return ret;
}



void DBHandler::loadESysteme(const QString &ally)
{
    qDebug() << Q_FUNC_INFO;
    bool ok;
    QHash<QString,QStringList> h;
    h.insert("systeme",QStringList() << "cords");
    QString w = "esysteme.id=systeme.sysnr and esysteme.ally='"+ally+"'";
    QSqlQuery q = mDB.select(QStringList() << "esysteme"<<"systeme",ok,h,w);
    if(!ok){
        QMessageBox::warning(0,"Fehler","select konnt nicht ausgeführt werden \n"+mDB.lastError().text(),QMessageBox::Ok);
    }else {
        int  var = 0;
        QHash<QString,ESystem> hash;
        while(q.next()){
            ESystem  e = ESystem::fromQuery(q);
            hash.insert(e.id,e);
            var++;
        }
        mESystem.insert(ally,hash);
    }
}

void DBHandler::loadSysteme(int gala)
{
    qDebug() << Q_FUNC_INFO;

    bool ok;
    QSqlQuery q = mDB.select(QStringList() <<"systeme",ok,QHash<QString,QStringList>(),QString("gala="+QString::number(gala)));
    if(ok){
        QHash<QString,System> hash;
        while(q.next()){
            System s =System::fromQuery(q);
            hash.insert(s.sysnr,s);
        }
        qDebug() <<hash.size();
        mSystem.insert(gala,hash);

    }else {
        QMessageBox::warning(0,"Fehler","select konnt nicht ausgeführt werden \n"+mDB.lastError().text(),QMessageBox::Ok);
    }
}
