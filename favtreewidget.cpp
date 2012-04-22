#include "favtreewidget.h"
#include "shiptreewidget.h"

FavTreeWidget::FavTreeWidget(QWidget *parent) :
    QTreeWidget(parent)
{
    connect(this,SIGNAL(itemDoubleClicked(QTreeWidgetItem*,int)),this,SLOT(onItemDoubleClicked(QTreeWidgetItem*,int)));
    connect(this,SIGNAL(customContextMenuRequested(QPoint)),this,SLOT(onCustomContextMenuRequested(QPoint)));
}

void FavTreeWidget::loadItems(const QStringList &list)
{
    QTreeWidgetItem *i;
    QStringList tmp;
    for (int var = 0; var < list.count(); ++var) {
        tmp = list.at(var).split(";;;");
        i = new QTreeWidgetItem(QStringList() << tmp.at(1) << tmp.at(2));
        i->setData(0,Qt::UserRole,tmp.at(0));
        addTopLevelItem(i);
    }
    resizeColumnToContents(0);
}

QStringList FavTreeWidget::saveItems()
{
    QStringList l;
    QTreeWidgetItem *item;
    for (int var = 0; var < topLevelItemCount(); ++var) {
        item = topLevelItem(var);
        l.append(item->data(0,Qt::UserRole).toString()+";;;"+item->text(0)+";;;"+item->text(1));
    }
    return l;
}

void FavTreeWidget::onItemDoubleClicked(QTreeWidgetItem *item, int)
{
    if(item->text(0).left(2) == "[S"){
        emit openSystem(item->data(0,Qt::UserRole).toString());
    } else {
       emit openOrbit(item->data(0,Qt::UserRole).toString());
    }
}

void FavTreeWidget::onCustomContextMenuRequested(const QPoint &pos)
{
    QTreeWidgetItem *item = itemAt(pos);
    if(item != NULL){

        SendMenu menu(ShipTreeWidget::tree());

        QMenu * m = menu.menu();

        if(item->text(0).left(2)=="[S")
            menu.setDisable(true);

        QAction *rm = m->addAction("remove");
        QAction *k = m->addAction("add kommentar");

        do{            
            QAction *ret = m->exec(mapToGlobal(pos));
            if(ret == rm){
                delete item;
            }else if(ret == k){
                QString str = QInputDialog::getText(this,"kommentar","kommentar:");
                if(!str.isEmpty()){
                    item->setText(1,str);
                }
            }else if( menu.checkSendClicked(ret)){
               qDebug() << Q_FUNC_INFO;
                 const QStringList &l = menu.checkedID();
               const  QString &oldID = menu.orbitID();
                if(l.size()>0){                    
                   QString  planiID = item->data(0,Qt::UserRole).toString();
                    if(l.size() == 1)
                        emit sentShip(l.first(),oldID,planiID);
                    else
                      emit sentShips(l,planiID);
                }
            }
        }while(!menu.exit());
    }
}
