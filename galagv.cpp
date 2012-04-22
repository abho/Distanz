#include "galagv.h"
#include "shiptreewidget.h"

GalaGV::GalaGV(QWidget *parent) :
    QGraphicsView(parent),mSelectedItem(NULL),mCalcitem(NULL)
{
    setScene(new QGraphicsScene());
    setSceneRect(-620,-620,1240,1240);
    QAction *openAct = new QAction("plus", this);
    openAct->setShortcuts(QKeySequence::ZoomIn);
    connect(openAct, SIGNAL(triggered()), this, SLOT(plus()));
    addAction(openAct);
    QAction *a = new QAction("minus", this);
    a->setShortcuts(QKeySequence::ZoomOut);
    connect(a, SIGNAL(triggered()), this, SLOT(minus()));
    addAction(a);
    connect(scene(),SIGNAL(selectionChanged()),this,SLOT(onSelectionChanged()));
    connect(this,SIGNAL(customContextMenuRequested(QPoint)),this,SLOT(onContextMenuRequest(QPoint)));

    mData = new SceneData(this);
}

GalaGV::~GalaGV()
{

}


void GalaGV::setTreeItemToView(QTreeWidgetItem *item)
{
    QGraphicsEllipseItem *i = makeMarkerItem(item->data(0,Qt::UserRole).toString(),item->data(0,Qt::UserRole+1).toString(),StandardSystem);

    scene()->addItem(i);
}

void GalaGV::setStatusBarAndView(QLabel *l, GalaGV *secondView)
{   
    mLabelInfo = l;
    mSecondView = secondView;
    mSecondView->setScene(scene());
    mSecondView->setData(mData);
}

void GalaGV::setCurrentSelected(QTreeWidgetItem *item)
{
    const QString str = item->data(0,Qt::UserRole).toString();
    if( mSelectedItem != NULL){
        scene()->removeItem(mSelectedItem);
    }
    mSelectedItem = makeMarkerItem(str,item->data(0,Qt::UserRole+1).toString(),Log,Qt::red,Mark);
    scene()->addItem(mSelectedItem);

}

void GalaGV::setCalcPos(QString str)
{/*
    if(mCalcitem != NULL){
        scene()->removeItem(mCalcitem);
    }
    if(str.left(2)=="7_"){
        mCalcitem = makeMarkerItem(str.mid(2),0,StandardSystem,Qt::blue,Mark);
        scene()->addItem(mCalcitem);
    }*/
}

void GalaGV::onContextMenuRequest(const QPoint &pos)
{

    QGraphicsItem *item = itemAt(pos);
    QGraphicsEllipseItem *eSystemItem;
    ESystem  e;

    SendMenu menu(ShipTreeWidget::tree());
    QMenu *m  = menu.menu();
    QAction *openSys = m->addAction("openSys");
    QAction *openLog = m->addAction("openLog");
    QAction *setCheckd = m->addAction("check");
    QAction *setWerf = m->addAction("werft");
    QAction *clearChecked = m->addAction("clear all checked");
    setCheckd->setCheckable(true);
    setWerf->setCheckable(true);

    const QHash<QString,QGraphicsEllipseItem*> &esysteme = mData->eSysteme.value(mData->showESysteme.first+";"+QString::number(mData->gala));

    if(!mData->showESysteme.second || esysteme.size() == 0)
        clearChecked->setEnabled(false);

    if(item == NULL){
        openSys->setEnabled(false);
        openLog->setEnabled(false);
        setCheckd->setEnabled(false);
        setWerf->setEnabled(false);
    } else {
        if(!mData->mPlayerLogPlanis.contains(item))
            openLog->setEnabled(false);

        eSystemItem =esysteme.value(item->data(ID).toString());
        if(eSystemItem == NULL || !mData->showESysteme.second){
            setCheckd->setEnabled(false);
            setWerf->setEnabled(false);
        }else {
            e=eSystemItem->data(Esystem).value<ESystem>();
            setCheckd->setChecked(e.isChecked);
            setWerf->setChecked(e.isWerft);
        }
    }



    if( eSystemItem == NULL || !mData->showESysteme.second)
        menu.setDisable(true);

    do {
        QAction *ret = m->exec(mapToGlobal(pos));
        if(ret == openSys){
            emit loadPage("http://www.omega-day.com/game/?op=system&sys="+item->data(Qt::UserRole).toString());
        }else if (ret == openLog){
            emit loadPage("http://www.omega-day.com/game/"+item->data(Qt::UserRole+1).toString());
        } else if(ret == setCheckd){
            setChecked(eSystemItem,setCheckd->isChecked());
        } else if(ret == clearChecked){
            int ret =QMessageBox::warning(this,"muh","wirklich löschen",QMessageBox::Yes,QMessageBox::No);
            if(ret == QMessageBox::Yes)
                clearAllChecked();

        }else if (ret == setWerf) {
            setWerft(eSystemItem,setWerf->isChecked());
        }else if( menu.checkSendClicked(ret)){
           const  QStringList & l = menu.checkedID();
            const QString &oldID = menu.orbitID();
            if(l.size()>0){
                QString planiID = eSystemItem->data(Esystem).value<ESystem>().planiID;
                if(l.size() == 1)
                    emit sendShip(l.first(),oldID,planiID);
                else
                    emit sendShips(l,planiID);
            }
        }
    }while(!menu.exit());
}

void GalaGV::setGala(int gala)
{

    hideItems(mData->systeme.value(mData->gala));
    if(mData->chosen != NULL){
        delete mData->chosen;
        mData->showOwn = NULL;
    }

    if(mData->showESysteme.second){
        hideItems(mData->eSysteme.value(mData->showESysteme.first+";"+QString::number(mData->gala)));
    }
    if( mData->showOwn){
        hideItems(mData->own.value(mData->gala));
    }
    if(mData->systeme.value(mData->gala).contains(mData->momentPos.first))
        mData->momentPos.second->hide();



    if(!mData->systeme.contains(gala)){
        const QHash<QString,System> & sys = mDB->systeme(gala);
        QHashIterator<QString, System> i(sys);
        QHash<QString, QGraphicsEllipseItem *> hash;
        while(i.hasNext()){
            i.next();
            const System &s = i.value();
            QGraphicsEllipseItem *i = makeMarkerItem(s.cords,s.sysnr,StandardSystem);
            hash.insert(s.sysnr,i);
            scene()->addItem(i);
        }
        mData->systeme.insert(gala,hash);
    }

    showItems(mData->systeme.value(gala));

    mData->gala = gala;

    if(mData->showOwn)
        showOwn(true);

    if(mData->showESysteme.second)
        showESysteme(mData->showESysteme.first,true);

    if(mData->systeme.value(gala).contains(mData->momentPos.first))
        setMomentPos(mData->momentPos.first);

    setGate();
}

void GalaGV::showESysteme(const QString &ally, bool show)
{
    if(!ally.isEmpty()){
        mData->showESysteme.first = ally;
        mData->showESysteme.second = show;
        int gala =mData->gala;
        QString key = ally+";"+QString::number(gala);

        if(!mData->eSysteme.contains(key)){
            QList<ESystem> l =mDB->eSysteme(ally,gala);
            QString cords;
            QHash<QString,QGraphicsEllipseItem*> hash;
            for (int var = 0; var < l.size(); ++var) {
                const ESystem & s = l.at(var);
                cords = s.cords;
                if(cords== "")
                    cords = mData->systeme.value(mData->gala).value(s.id)->data(Cords).toString();

                QGraphicsEllipseItem *item = makeMarkerItem(cords,s.id,Ally,s.isWerft?Qt::yellow:QColor("#E87A1A"),Mark);
                if(s.isChecked){
                    item->setOpacity(0.5);
                }
                item->setData(Esystem,QVariant::fromValue(s));
                item->setFlag(QGraphicsItem::ItemIsSelectable,true);
                item->setToolTip(s.namen.join(",")+" : "+(s.isWerft?"Werft":""));
                hash.insert(s.id,item);
                scene()->addItem(item);
            }
            mData->eSysteme.insert(key,hash);
            qDebug() << Q_FUNC_INFO << mData->eSysteme.keys()<< mData->eSysteme.value(key).count();
        }
        if(show)
            showItems(mData->eSysteme.value(key));
        else
            hideItems(mData->eSysteme.value(key));
    }
}

void GalaGV::showOwn(bool show)
{
    mData->showOwn = show;

    if(!mData->own.contains(mData->gala)){
        QList<ODPlanet> l =mDB->loadOwnPlanis(mData->gala);
        QHash<QString,QGraphicsEllipseItem*> hash;
        for (int var = 0; var < l.size(); ++var) {
            const ODPlanet & p = l.at(var);
            QGraphicsEllipseItem *item = makeMarkerItem(p.cords,p.sys,Own,Qt::green,Mark);
            item->setFlags(QGraphicsItem::ItemIsSelectable);
            item->setToolTip(p.sysname + "("+ p.name+")");
            hash.insert(p.index,item);
            scene()->addItem(item);
        }
        mData->own.insert(mData->gala,hash);
    }
    if(show){
        showItems(mData->own.value(mData->gala));
    }else {
        hideItems(mData->own.value(mData->gala));
    }
}

void GalaGV::setMomentPos(QString index)
{
    qDebug() << Q_FUNC_INFO ;
    mData->momentPos.first =index;

    QGraphicsEllipseItem *item = mData->systeme.value(mData->gala).value(mData->momentPos.first);

    if(item != NULL){
        QGraphicsEllipseItem *newItem = makeMarkerItem(item->data(Cords).toString(),mData->momentPos.first,Moment,QColor("#00D0F0"),Mark);
        qDebug() << mData->momentPos.second;
        newItem->setZValue(2);
        scene()->addItem(newItem);
        ensureVisible(newItem);
        mSecondView->ensureVisible(newItem);

        if(mData->momentPos.second != NULL)
            delete mData->momentPos.second;
        mData->momentPos.second = newItem;

        calcNearSys(newItem->data(Cords).toString());
    }


}

void GalaGV::wheelEvent(QWheelEvent *event)
{
    if(event->modifiers().testFlag(Qt::ControlModifier) || event->buttons().testFlag(Qt::RightButton)){
        if(event->delta()>0)
            plus();
        else
            minus();
    }else {
        QGraphicsView::wheelEvent(event);
    }
}

void GalaGV::plus()
{
    scale(1.1,1.1);
}

void GalaGV::minus()
{
    scale(0.9,0.9);
}

void GalaGV::onSelectionChanged()
{
    QList<QGraphicsItem*> l = scene()->selectedItems();
    if(!l.isEmpty()){
        QGraphicsItem *i = l.first();
        QString t;
        if(mData->gate != NULL){
            int d = clacDis(i->data(Cords).toString(),mData->gate->data(Cords).toString());
            d=d/5;
            int h =0;
            if(d>59)
                h = d/60;

            t = QString::number(h)+"h:"+QString::number(d%60)+"min ";
        }
        mLabelInfo->setText(t+i->toolTip());
    }
}


void GalaGV::setPlayerLogItems(const QList<Kampf> &list)
{
    qDebug() << Q_FUNC_INFO <<list.size();
    QString lastCords;
    for(int var = 0; var < list.size(); ++var) {
        const Kampf &k = list.at(var);
        QColor c =Qt::red;
        if(var == list.size()-1){
            c = Qt::darkMagenta;
        }
        QGraphicsEllipseItem *item = makeMarkerItem(k.cords,k.sysid,Log,c,Mark);
        item->setToolTip(k.toString());
        item->show();
        item->setFlags(QGraphicsItem::ItemIsSelectable);
        item->setData(Bericht,k.berichtUrl);
        scene()->addItem(item);
        mData->mPlayerLogPlanis.append(item);
        if(var>0){
            QGraphicsLineItem *lineItem = makeLineItem(k.cords,lastCords,Qt::red);
            scene()->addItem(lineItem);
            mData->mPlayerLogPlanis.append(lineItem);
        }
        lastCords = k.cords;
    }
}

void GalaGV::onSystemChosen(const QString &id)
{
    if(mData->chosen != NULL){
        delete  mData->chosen;
        mData->chosen = NULL;
    }
    if(mData->systeme.value(mData->gala).contains(id)){

        QString cords = mData->systeme.value(mData->gala).value(id)->data(Cords).toString();
        QGraphicsEllipseItem *item =  makeMarkerItem(cords,id,Chosen,QColor("#58EBED"),BigMarker);

        mData->chosen = item;
        scene()->addItem(item);
    }
}

void GalaGV::rmPlayerLogItems()
{
    for (int var = 0; var < mData->mPlayerLogPlanis.count(); ++var) {
        QGraphicsItem *item = mData->mPlayerLogPlanis.at(var);
        scene()->removeItem(item);
        delete item;
    }
    mData->mPlayerLogPlanis.clear();
}

QGraphicsEllipseItem * GalaGV::makeMarkerItem(const QString &cords,const QString& sysid, ItemTyps itemtyp,QColor color, Typs typ)
{
    QStringList cord = cords.split('/');
    qreal x1 = cord.at(0).toFloat();
    //qreal y1 = cord.at(1).toFloat();
    qreal z1 = -1*cord.at(2).toFloat();
    QGraphicsEllipseItem *item = new QGraphicsEllipseItem();
    int size;
    switch(typ){
    case GalaGV::Plani:
        item->setBrush(QBrush(Qt::white));
        size = 2;
        break;
    case GalaGV::Mark:
        size = 6;
        break;
    case GalaGV::BigMarker:
        size = 10;
        break;
    }
    item->setRect(x1-(size/2),z1-(size/2),size,size);
    item->setPen(QPen(color));
    item->setData(ID,sysid);
    item->setData(Cords,cords);
    item->setData(ItemTyp,itemtyp);
    return item;
}

QGraphicsLineItem *GalaGV::makeLineItem(const QString &cords1, const QString &cords2, QColor color)
{
    QStringList cord1 = cords1.split('/');
    qreal x1 = cord1.at(0).toFloat();
    qreal z1 = -1*cord1.at(2).toFloat();

    QStringList cord2 = cords2.split('/');
    qreal x2 = cord2.at(0).toFloat();
    qreal z2 = -1*cord2.at(2).toFloat();

    QGraphicsLineItem *item = new QGraphicsLineItem(QLineF(x1,z1,x2,z2));
    item->setPen(QPen(color));
    return item;

}


void GalaGV::setChecked(QGraphicsItem*item,bool isChecked)
{
    ESystem e = item->data(Esystem).value<ESystem>();
    e.isChecked = isChecked;
    item->setData(Esystem,QVariant::fromValue<ESystem>(e));
    item->setOpacity(isChecked?0.5:1);
    mDB->setESystemChecked(e.id,isChecked);
}

void GalaGV::setWerft(QGraphicsItem *item, bool isChecked)
{
    ESystem e = item->data(Esystem).value<ESystem>();
    e.isWerft= isChecked;
    item->setData(Esystem,QVariant::fromValue<ESystem>(e));
    static_cast<QGraphicsEllipseItem*>(item)->setPen(QPen(isChecked?QColor(Qt::yellow):QColor("#E87A1A")));

    mDB->setESystemIsWerft(e.id,isChecked);
}

void GalaGV::clearAllChecked()
{
    QHashIterator<QString, QGraphicsEllipseItem*> it(mData->eSysteme.value(mData->showESysteme.first+";"+QString::number(mData->gala)));

    while(it.hasNext()){
        it.next();
        if( it.value()->data(Esystem).value<ESystem>().isChecked){
            ESystem e= it.value()->data(Esystem).value<ESystem>();
            e.isChecked = false;
            it.value()->setData(Esystem,QVariant::fromValue<ESystem>(e));
            it.value()->setPen(QColor("#E87A1A"));
        }
    }
    mDB->clearChecked(mData->showESysteme.first,mData->gala);

}

void GalaGV::calcNearSys(const QString &cords)
{
    qDebug() << Q_FUNC_INFO << cords;
    if(mData->showESysteme.second){
        qreal x1,y1,z1;
        x1 = cords.section("/",0,0).toInt();
        y1 = cords.section("/",1,1).toInt();
        z1 = cords.section("/",2,2).toInt();
        const QHash<QString,QGraphicsEllipseItem*> &esystem = mData->eSysteme.value(mData->showESysteme.first+";"+QString::number(mData->gala));
        QHashIterator<QString,QGraphicsEllipseItem*> it(esystem);
        QList<DistanzPair> d;

        while(it.hasNext()){
            it.next();
            d.append(DistanzPair(it.value()->data(Esystem).value<ESystem>(),clacDis(it.value()->data(Cords).toString(),x1,y1,z1)));
        }
        qSort(d);
        emit calcedNearest(d);
    }
}

int GalaGV::clacDis(const QString &str, qreal x1, qreal y1, qreal z1)
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

int GalaGV::clacDis(const QString &str1, const QString &str2)
{

    qreal x1,x2,y1,y2,z1,z2;
    x1 = str1.section("/",0,0).toInt();
    y1 =str1.section("/",1,1).toInt();
    z1 = str1.section("/",2,2).toInt();

    x2 = str2.section("/",0,0).toInt();
    y2 = str2.section("/",1,1).toInt();
    z2 = str2.section("/",2,2).toInt();

    x2-=x1;
    y2-=y1;
    z2-=z1;

    x2=pow(x2,2);
    y2=pow(y2,2);
    z2=pow(z2,2);

    qreal sum = x2+y2+z2;

    return sqrt(sum);
}

void GalaGV::hideItems(const QHash<QString, QGraphicsEllipseItem *> &hash)
{
    qDebug() << Q_FUNC_INFO << hash.count();
    QHashIterator<QString, QGraphicsEllipseItem*> it(hash);
    while(it.hasNext()){
        it.next();
        it.value()->hide();
    }
}

void GalaGV::showItems(const QHash<QString, QGraphicsEllipseItem *> &hash)
{
    QHashIterator<QString, QGraphicsEllipseItem*> it(hash);
    while(it.hasNext()){
        it.next();
        it.value()->show();
    }
}

void GalaGV::setGate()
{
    qDebug() << Q_FUNC_INFO;
    if(mData->gate != NULL){
        delete mData->gate;
        mData->gate = NULL;
    }
    const Gate& g = mData->gates.value(mData->gala);

    QGraphicsEllipseItem *item = mData->systeme.value(mData->gala).value(QString(g.sysID));
    qDebug() << Q_FUNC_INFO << item;
    if( item != NULL){
        mData->gate = makeMarkerItem(item->data(Cords).toString(),g.sysID,GateItem,QColor("#E087DF"),BigMarker);
        scene()->addItem(mData->gate);
    }
}

void GalaGV::setDB(DBHandler *db)
{
    mDB = db;
    if(mData->gates.count()== 0){
        foreach (const Gate &g, mDB->gates()) {
            mData->gates.insert(g.gala,g);
        }
    }
}

void GalaGV::setData(SceneData *d)
{
    mData = d;
}

SceneData::~SceneData()
{

}


