#ifndef BAUTREEWIDGET_H
#define BAUTREEWIDGET_H

#include <QTreeWidget>
#include <QtCore>
#include "tool.h"

class BauTreeWidget : public QTreeWidget
{
    Q_OBJECT
public:
    explicit BauTreeWidget(QWidget *parent = 0);
signals:
    
public slots:
  void onBotParsedWerft2Page(const QList<QTreeWidgetItem*> &list);
 private slots:
void onCustomContextMenuRequested(const QPoint &pos);
void onItemExpanded(QTreeWidgetItem* item);
};

#endif // BAUTREEWIDGET_H
