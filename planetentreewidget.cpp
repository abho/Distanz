#include "planetentreewidget.h"
#include "shiptreewidget.h"

PlanetenTreeWidget *PlanetenTreeWidget::mTree = NULL;
PlanetenTreeWidget::PlanetenTreeWidget(QWidget *parent) :
    QTreeWidget(parent)
{
    connect(this,SIGNAL(customContextMenuRequested(QPoint)),this,SLOT(onCustomContextMenuRequested(QPoint)));
    connect(this,SIGNAL(itemDoubleClicked(QTreeWidgetItem*,int)),this,SLOT(onItemDoubleClicked(QTreeWidgetItem*,int)));
    addTopLevelItem(new QTreeWidgetItem(QStringList()<<"Summe"));
    topLevelItem(0)->setExpanded(true);
    for (int var = 0; var <10; ++var) {
        topLevelItem(0)->setBackgroundColor( var,QColor("#001A44"));

    }
    mTree =this;
}

void PlanetenTreeWidget::updateTimeout()
{

    QVector<float> values(5,0);

    for (int var = 0; var < topLevelItem(0)->childCount(); ++var) {
        QTreeWidgetItem *item = topLevelItem(0)->child(var);
        for (int var = 1; var < 6; ++var) {
            float d =item->data(var,Qt::UserRole+1).toFloat()+item->data(var,Qt::UserRole).toFloat();
            item->setText(var,Tool::insertDots( QString::number(d,'f',0)));
            item->setData(var,Qt::UserRole+1,d);
            values[var-1]+=d;
        }

        int  t = item->data(0,Qt::UserRole).toInt();

        if(t != 0){
            t--;
            item->setData(0,Qt::UserRole,t);
            item->setText(8,Tool::secsToTimeString(t));
        }
    }
    for (int var = 0; var < 5; ++var) {
        topLevelItem(0)->setText(var+1,Tool::insertDots( QString::number(values.at(var),'f',0)));
    }
}

PlanetenTreeWidget *PlanetenTreeWidget::tree()
{
    return mTree;
}

void PlanetenTreeWidget::onBotParsedPlanetenPage(QTreeWidgetItem *item)
{
    qDebug() << Q_FUNC_INFO ;
    QString str =item->data(0,Qt::UserRole+2).toString();


    for (int var = 0; var < topLevelItem(0)->childCount(); ++var) {
        if(topLevelItem(0)->child(var)->data(0,Qt::UserRole+2).toString() == str)
            delete topLevelItem(0)->child(var);
    }
    topLevelItem(0)->addChild(item);
    for (int var = 0; var < columnCount(); ++var) {
        resizeColumnToContents(var);
    }

}

void PlanetenTreeWidget::onCustomContextMenuRequested(const QPoint &p)
{
    QTreeWidgetItem *item = itemAt(p);

    if(item != NULL && item != topLevelItem(0)){


        SendMenu menu(ShipTreeWidget::tree());
        QMenu * m = menu.menu();

        QMenu *distanz = new QMenu("distanz");
        m->addMenu(distanz);;

        for (int var = 0; var <topLevelItem(0)->childCount(); ++var) {
            QTreeWidgetItem *i = topLevelItem(0)->child(var);
            if(item!=i){
                int d = calcDistanz(item->data(0,Qt::UserRole+1).toString(),i->data(0,Qt::UserRole+1).toString());
                d=d/3;
                int h =0;
                if(d>59)
                    h = d/60;
                QString t = QString::number(h)+"h:"+QString::number(d%60)+"min nach ";

                distanz->addAction(t+i->text(0));
            }
        }

        do{
            QAction *ret = m->exec(mapToGlobal(p));
            if(menu.checkSendClicked(ret)){
                const QStringList &l = menu.checkedID();
                const QString &oldID =menu.orbitID();
                if(l.size()>0){
                    QString planiID = item->data(0,Qt::UserRole+2).toString();
                    if(l.size() == 1)
                        emit sentShip(l.first(),oldID,planiID);
                    else
                        emit sentShips(l,planiID);
                }
            }
        }while(!menu.exit());
    }
}

void PlanetenTreeWidget::onItemDoubleClicked(QTreeWidgetItem *item, int)
{
    qDebug() << Q_FUNC_INFO;
    if(item != topLevelItem(0))
        emit openOrbit(item->data(0,Qt::UserRole+2).toString());
}

int PlanetenTreeWidget::calcDistanz(const QString &cords1, const QString &cords2)
{
    qreal x1,x2,y1,y2,z1,z2;
    x1 = cords1.section("/",0,0).toInt();
    y1 = cords1.section("/",1,1).toInt();
    z1 = cords1.section("/",2,2).toInt();

    x2 = cords2.section("/",0,0).toInt();
    y2 = cords2.section("/",1,1).toInt();
    z2 = cords2.section("/",2,2).toInt();

    x2-=x1;
    y2-=y1;
    z2-=z1;

    x2=pow(x2,2);
    y2=pow(y2,2);
    z2=pow(z2,2);

    qreal sum = x2+y2+z2;

    return sqrt(sum);

}
