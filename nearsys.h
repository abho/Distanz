#ifndef NEARSYS_H
#define NEARSYS_H

#include <QWidget>
#include <QDebug>
#include<QtCore>
#include <QtGui>
#include "tool.h"
#include "sendmenu.h"

namespace Ui {
class NearSys;
}

class NearSys : public QWidget
{
    Q_OBJECT
    
public:
    explicit NearSys(QWidget *parent = 0);
    ~NearSys();
    void paintEvent(QPaintEvent *);
public slots:
     void onGalaGVcalcedNearest(const QList<DistanzPair> & list);
signals:
     void systemChoisen(const QString &id);
     void sendShip(const QString &,const QString &,const QString &);
      void sendShips(const QList<QString> &,const QString &);
      void openSystem(const QString &sysId);
private slots:
     void on_checkBox_toggled(bool checked);
     void on_treeWidget_currentItemChanged(QTreeWidgetItem *current, QTreeWidgetItem *previous);

     void on_treeWidget_itemDoubleClicked(QTreeWidgetItem *item, int column);

     void on_treeWidget_customContextMenuRequested(const QPoint &pos);

private:
    Ui::NearSys *ui;
    QList<QTreeWidgetItem*> mItems;
};

#endif // NEARSYS_H
