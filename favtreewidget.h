#ifndef FAVTREEWIDGET_H
#define FAVTREEWIDGET_H

#include <QTreeWidget>
#include "tool.h"
#include <QMenu>
#include <QAction>
#include <QInputDialog>
#include <sendmenu.h>


class FavTreeWidget : public QTreeWidget
{
    Q_OBJECT
public:
    explicit FavTreeWidget(QWidget *parent = 0);
    void loadItems(const QStringList &list);
    QStringList saveItems();
signals:
    void openOrbit(const QString&);
    void openSystem(const QString &);
    void sentShip(const QString &,const QString &,const QString &);
     void sentShips(const QList<QString> &,const QString &);
public slots:
    
private slots:
    void onItemDoubleClicked(QTreeWidgetItem*item ,int);
    void onCustomContextMenuRequested(const QPoint &pos);
};

#endif // FAVTREEWIDGET_H
