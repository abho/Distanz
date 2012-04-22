#include "nearsys.h"
#include "ui_nearsys.h"
#include "shiptreewidget.h"

NearSys::NearSys(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::NearSys)
{
    ui->setupUi(this);
    ui->checkBox->setObjectName("nearCheclkBox");
    ui->frame->setObjectName("nearFrame");
}

NearSys::~NearSys()
{
    delete ui;
}
void NearSys::paintEvent(QPaintEvent *)
{
    QStyleOption opt;
    opt.init(this);
    QPainter p(this);
    style()->drawPrimitive(QStyle::PE_Widget, &opt, &p, this);
}

void NearSys::onGalaGVcalcedNearest(const QList<DistanzPair> &list)
{
    ui->treeWidget->clear();
    mItems.clear();
    QStringList l;
    for (int var = 0; var < 50; ++var) {
        const DistanzPair &pair = list.at(var);
        int d = pair.mDistanz;
        d=d/5;
        int h =0;
        if(d>59)
            h = d/60;

        QString t = QString::number(h)+"h:"+QString::number(d%60);
        l <<  t <<  pair.mESystem.namen.join(",") <<  (pair.mESystem.isWerft?"W":"") << (pair.mESystem.isChecked?"true":"false") << pair.mESystem.id ;
        QTreeWidgetItem * item = new QTreeWidgetItem(l);
        item->setData(0,Qt::UserRole,pair.mESystem.planiID);
        item->setData(0,Qt::UserRole+1,pair.mESystem.isChecked);
        mItems.append(item);
        ui->treeWidget->addTopLevelItem(item);
        if(!ui->checkBox->isChecked() && pair.mESystem.isChecked)
            item->setHidden(true);
        l.clear();
    }

    for (int var = 0; var < ui->treeWidget->columnCount(); ++var) {
        ui->treeWidget->resizeColumnToContents(var);
    }
}

void NearSys::on_checkBox_toggled(bool checked)
{

    qDebug() << Q_FUNC_INFO << mItems.size();
    for (int var = 0; var < mItems.size(); ++var) {
        QTreeWidgetItem *item = mItems.at(var);
        if(item->data(0,Qt::UserRole+1).toBool())
            item->setHidden(!checked);
    }
}



void NearSys::on_treeWidget_currentItemChanged(QTreeWidgetItem *current, QTreeWidgetItem *previous)
{
    if(current != NULL)
        emit systemChoisen(current->text(4));
}



void NearSys::on_treeWidget_itemDoubleClicked(QTreeWidgetItem *item, int column)
{
    emit openSystem(item->text(4));
}

void NearSys::on_treeWidget_customContextMenuRequested(const QPoint &pos)
{
    QTreeWidgetItem *item = ui->treeWidget->itemAt(pos);

    if(item != NULL){
        SendMenu menu(ShipTreeWidget::tree());

        QMenu *m = menu.menu();
        do {
            QAction * ret =m->exec(ui->treeWidget->mapToGlobal(pos));
            if(menu.checkSendClicked(ret)){
                const QStringList l =menu.checkedID();
                const QString &oldID =menu.orbitID();
                if(l.size()>0){
                    QString planiID = item->data(0,Qt::UserRole).toString();
                    if(l.size() == 1)
                        emit sendShip(l.first(),oldID,planiID);
                    else
                        emit sendShips(l,planiID);
                }
            }
        }while(!menu.exit());
    }
}
