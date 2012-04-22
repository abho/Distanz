#include "shiptreewidget.h"
#include <limits>

ShipTreeWidget * ShipTreeWidget::mTree = NULL;

ShipTreeWidget::ShipTreeWidget(QWidget *parent) :
    QTreeWidget(parent),mIterator(this)
{
    connect(this,SIGNAL(itemDoubleClicked(QTreeWidgetItem*,int)),this,SLOT(onItemDoubleClicked(QTreeWidgetItem*,int)));
    setSortingEnabled(true);
    mTree = this;


}


ShipTreeWidget *ShipTreeWidget::tree()
{
    return mTree;
}

QPair<QAction*, QList<QAction *> > ShipTreeWidget::menu()
{
    QMenu *m = new QMenu();
    QList<QAction*> l;
    for (int var = 0; var < mTree->topLevelItemCount(); ++var) {
        QTreeWidgetItem *item = mTree->topLevelItem(var);
        QAction *tmp = m->addAction(item->text(1)+"  :  "+item->text(2));
        tmp->setCheckable(true);
        l.append(tmp);
    }

    m->addSeparator();

    QAction *senMenu  =new QAction("send",mTree);
    senMenu->setMenu(m);


    return QPair<QAction*, QList<QAction *> >(senMenu,l);
}




void ShipTreeWidget::updateTimeout()
{

    foreach (ShipItem *item, mTimerShips) {
        int t = item->data(0,ShipItem::Time).toInt();
        if(t<(std::numeric_limits<int>::max()-6)){
            t--;
            if(t==0){
                mTimerShips.removeOne(item);
                item->setText(3,"");
                item->setData(0,ShipItem::Time,0);
            }else {
                QString str = Tool::secsToTimeString(t);
                item->setText(3,str);
                item->setData(0,ShipItem::Time,t);
            }
        }
    }
}


void ShipTreeWidget::onBotParsedFlottenansicht(const QList<Schiff> &schiffe)
{

    clear();
    mTimerShips.clear();
    mBergbau = new  ShipItem();
    mBergbau->setText(1,"BergbauSchiff");
    mBergbau->setData(0,ShipItem::Time,std::numeric_limits<int>::max());
    mSquad = new ShipItem();
    mSquad->setText(1,"Squadflotte");
    mSquad->setData(0,ShipItem::Time,std::numeric_limits<int>::max());
    for (int var = 0; var < 4; ++var) {
       mSquad->setBackgroundColor(var,QColor("#004065"));
    }
    mSquad->setExpanded(true);


    addTopLevelItem(mBergbau);
    addTopLevelItem(mSquad);

    foreach (const Schiff& s, schiffe) {
        if(!s.autoRoute.isEmpty())
            addBergbauShip(s);
        else{
            ShipItem*item =makeShipItem(s);
            if(s.owner == "Squadflotte")
                mSquad->addChild(item);
            else
                addTopLevelItem(item);
        }
    }

    for (int var = 0; var < 4; ++var) {
        resizeColumnToContents(var);
    }
}

void ShipTreeWidget::onBotSendShip(const QList<QStringList> &list)
{

    for (int var = 0; var < list.size(); ++var) {
        const QStringList &l = list.at(var);
        ShipItem * item = mShips.value(l.first());
        if(item != NULL){
            QString newHref(item->data(0,ShipItem::Href).toString().replace(item->data(0,ShipItem::ID).toString(),l.at(1)));
            item->setData(0,ShipItem::Href,newHref);
            item->setData(0,ShipItem::Orbit,l.at(1));

            item->setText(2, "StKNr:"+l.at(1));
            QStringList ts= l.at(2).split(":");
            qDebug() << ts.size();
            QDateTime t = QDateTime::currentDateTime().addDays(ts.first().toInt()).addSecs(ts.at(1).toInt()*3600).addSecs(ts.at(2).toInt()*60).addSecs(ts.at(3).toInt());
            int secs=ts.first().toInt()*3600*24+ts.at(1).toInt()*3600+ts.at(2).toInt()*60+ts.at(3).toInt();
            item->setText(0,t.toString("dd.MM.yy;HH:mm:ss"));

            item->setData(0,ShipItem::Time,secs);
            item->setText(3,Tool::secsToTimeString(secs));
            if(!mTimerShips.contains(item))
                mTimerShips.append(item);
        }
    }
}

void ShipTreeWidget::onItemDoubleClicked(QTreeWidgetItem *item, int)
{
    if(item->text(1)!="BergbauSchiff" && item->text(1)!="" && item->text(1)!="Squadflotte"){
        emit openOrbit(item->data(0,ShipItem::Orbit).toString());
    }
}

ShipItem *ShipTreeWidget::makeShipItem(const Schiff &s)
{
    ShipItem *i = new ShipItem();
    int sec;
    QString ti;
    if(s.zeit == "Unbekannt"){
        sec = std::numeric_limits<int>::max()-1;
    }else if(s.zeit.contains("TF")){
        sec = std::numeric_limits<int>::max()-2;
    } else if(s.zeit == "01.01.38;01:01:00" ){
        sec = std::numeric_limits<int>::max()-3;
    }else if(s.zeit == "Parken" ){
        sec = std::numeric_limits<int>::max()-4;
        ti = "Parken";
    }  else if(s.zeit == "V-Mode"){
        sec = std::numeric_limits<int>::max()-5;
        ti = "V-Mode";
    } else {
        QDateTime t = QDateTime::fromString(s.zeit,"dd.MM.yy;HH:mm:ss").addYears(100);

        QDateTime time=QDateTime::currentDateTime();
        int all=time.secsTo(t);
        int days=time.daysTo(t);
        int h=(all-days*24*60*60)/60/60;
        int min=(all-days*24*60*60-h*60*60)/60;
        int sek=all%60;
        sec =h*3600+min*60+sek;
        ti = Tool::secsToTimeString(sec);
        mTimerShips.append(i);
    }


    i->setText(0,s.zeit);
    i->setText(1,s.name);
    i->setText(2,s.planiName);
    i->setText(3,ti);

    i->setData(0,ShipItem::Time,sec);
    i->setData(0,ShipItem::ID,s.id);
    i->setData(0,ShipItem::Orbit,s.planiID);
    i->setData(0,ShipItem::Href,s.sendUrl);

    if(s.count>1){
        i->setBackgroundColor(0,QColor("#001A44"));
        i->setBackgroundColor(1,QColor("#001A44"));
        i->setBackgroundColor(2,QColor("#001A44"));
        i->setBackgroundColor(3,QColor("#001A44"));
    }

    i->setForeground(3,QBrush(Qt::darkGreen));
    return i;
}

void ShipTreeWidget::addBergbauShip(const Schiff &s)
{
    QStringList a = s.autoRoute.split(";");
    ShipItem *item = NULL;
    for (int var = 0; var < mBergbau->childCount(); ++var) {
        if(mBergbau->child(var)->text(0) == a.at(2)){
            item = static_cast<ShipItem*>(mBergbau->child(var));
            break;
        }
    }
    if(item == NULL){
        item = new  ShipItem();
        item->setText(0,a.at(2));
        item->setData(0,ShipItem::Time,std::numeric_limits<int>::max());
        mBergbau->addChild(item);
    }

    ShipItem *i = new ShipItem();

    i->setText(0,"Bergbau");
    i->setText(1,a.first());
    i->setText(2,"<-->");
    i->setText(3,a.at(2));

    i->setData(0,ShipItem::Time,0);
    i->setData(0,ShipItem::Orbit,a.at(1));
    item->addChild(i);



}

bool ShipItem::operator <(const QTreeWidgetItem &other) const
{
    return data(0,Qt::UserRole).toInt() > other.data(0,Qt::UserRole).toInt();
}


