#include "bautreewidget.h"
#include "planetentreewidget.h"

BauTreeWidget::BauTreeWidget(QWidget *parent) :
    QTreeWidget(parent)
{
    setContextMenuPolicy(Qt::CustomContextMenu);
    connect(this,SIGNAL(customContextMenuRequested(QPoint)),this,SLOT(onCustomContextMenuRequested(QPoint)));
    connect(this,SIGNAL(itemExpanded(QTreeWidgetItem*)),this,SLOT(onItemExpanded(QTreeWidgetItem*)));
}



void BauTreeWidget::onCustomContextMenuRequested(const QPoint &pos)
{
    QTreeWidgetItem *item = itemAt(pos);
    PlanetenTreeWidget *tree = PlanetenTreeWidget::tree();

    if (item != NULL){
        float ip = item->text(5).remove(",").toFloat();
        QMenu m;
        qDebug() << Q_FUNC_INFO << ip << tree->topLevelItemCount();
        for (int var = 0; var < tree->topLevelItem(0)->childCount() ; ++var) {
            QTreeWidgetItem *i = tree->topLevelItem(0)->child(var);
            float f = i->data(6,Qt::UserRole).toFloat();
            if(f!=0){
                qDebug() << f;
             m.addAction(i->text(0) +" : "+Tool::secsToTimeString(ip/f));
            }
        }
        m.exec(mapToGlobal(pos));

    }

}

void BauTreeWidget::onItemExpanded(QTreeWidgetItem *item)
{
    for (int var = 0; var < topLevelItemCount(); ++var) {
        resizeColumnToContents(var);
    }
}

void BauTreeWidget::onBotParsedWerft2Page(const QList<QTreeWidgetItem *> &list)
{
    clear();

    addTopLevelItems(list);

    for (int var = 0; var < topLevelItemCount(); ++var) {
        resizeColumnToContents(var);
    }
}

