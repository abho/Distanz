#include "scoutwidget.h"
#include "ui_scoutwidget.h"

ScoutWidget::ScoutWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ScoutWidget),mSetting("hakah","scout")
{
    ui->setupUi(this);
    ui->splitter->restoreState(mSetting.value("splitter").toByteArray());
    mIndex = 0;
}

void ScoutWidget::setDBAndManager(DBHandler *db, QNetworkAccessManager *manager,Option *option)
{
    qDebug() << Q_FUNC_INFO;
    ui->webView->page()->setNetworkAccessManager(manager);

    mTimer = new QTimer(this);
    connect(mTimer,SIGNAL(timeout()),this,SLOT(onTimerTimeout()));
    mSTimer = new QTimer();
    connect(mSTimer,SIGNAL(timeout()),this,SLOT(onSecTimer()));
    mSTimer->start(1000);

    mDB = db;
    mBot = new Bot2(ui->webView,mDB,option,this);
    mBot->setODDBEnable(true);
    connect(mBot,SIGNAL(shipsSended(QList<QStringList>)),this,SLOT(onBotParsedPostSendPage(QList<QStringList>)));
    connect(mBot,SIGNAL(loadedSystem(QString)),this,SLOT(onBotLoadSystemPage(QString)));
    connect(mBot,SIGNAL(loadedOrbit(QString)),this,SLOT(onBotLoadSOrbitPage(QString)));

    ui->lineEditShipID->setText(mSetting.value("scoutshipid").toString());
    ui->lineEditShipPos->setText(mSetting.value("scoutshippos").toString());
    ui->lineEditStartID->setText(mSetting.value("scoutstartsys").toString());
}

ScoutWidget::~ScoutWidget()
{
    qDebug() << Q_FUNC_INFO<< mSetting.value("scoutstartid").toString();
    mSetting.setValue("scoutshipid",ui->lineEditShipID->text());
    mSetting.setValue("scoutshippos",ui->lineEditShipPos->text());
    mSetting.setValue("scoutstartsys",ui->lineEditStartID->text());
    mSetting.setValue("splitter",ui->splitter->saveState());
    delete ui;

}


void ScoutWidget::onBotParsedScoutPage(const QList<QPair<QString, QString> > &list)
{
    qDebug() << Q_FUNC_INFO;
    ui->treeWidget->clear();
    for (int var = 0; var < list.size(); ++var) {
        addToTree(list.at(var).first,list.at(var).second);
    }
}

void ScoutWidget::on_pushButtonAdd_clicked()
{
    qDebug() << Q_FUNC_INFO;
    QString str = ui->lineEditAdd->text();
    QString str2 = ui->lineEditPlaniID->text();
    if(!str.isEmpty()&&!str2.isEmpty()){
        addToTree(str,str2);
    }else {
        qDebug() << "str is empty";
    }
}

void ScoutWidget::on_pushButtonCalc_clicked()
{
    qDebug() << Q_FUNC_INFO;
    QString str = ui->lineEditStartID->text();
    if(!str.isEmpty()){
        QString cords = mDB->cords(str);
        if(!cords.isEmpty()){

            Tool tool;
            QTreeWidgetItem *item;
            QTreeWidgetItem *tmpID;
            int d = 9000000;
            int tmpD = 0;
            for (int var = 0; var < ui->treeWidget->topLevelItemCount(); ++var) {
                for (int i = 0+var; i < ui->treeWidget->topLevelItemCount(); ++i) {
                    qreal x1,y1,z1;
                    x1 = cords.section("/",0,0).toInt();
                    y1 = cords.section("/",1,1).toInt();
                    z1 = cords.section("/",2,2).toInt();
                    item = ui->treeWidget->topLevelItem(i);
                    tmpD =  tool.clacDis(item->data(0,Qt::UserRole).toString(),x1,y1,z1);
                    if(tmpD<d){
                        d= tmpD;
                        tmpID= item;
                    }
                }
                ui->treeWidget->insertTopLevelItem(var,item);
                cords = item->data(0,Qt::UserRole).toString();
            }

        }else {
            qDebug() << "system nich in der DB";
        }

    }else {
        qDebug() << "str is empty";
    }

}


void ScoutWidget::onTimerTimeout()
{
    qDebug() << Q_FUNC_INFO << mTyp ;
    mTimer->stop();
    if(mTyp == System){
        qDebug() << ui->treeWidget->topLevelItem(mIndex)->text(1);;
        mBot->loadSystem(ui->treeWidget->topLevelItem(mIndex)->text(1));
    }else if(mTyp == Orbit) {
        qDebug() << ui->treeWidget->topLevelItem(mIndex)->child(mChildIndex)->text(2);
        mBot->loadOrbit(ui->treeWidget->topLevelItem(mIndex)->child(mChildIndex)->text(2));
    }
}

void ScoutWidget::onSecTimer()
{
    if(mIndex< ui->treeWidget->topLevelItemCount()){
        int d = ui->treeWidget->topLevelItem(mIndex)->data(0,Qt::UserRole+1).toInt();
        if(d!=0){
            d--;
            ui->treeWidget->topLevelItem(mIndex)->setText(0,Tool::secsToTimeString(d));
            ui->treeWidget->topLevelItem(mIndex)->setData(0,Qt::UserRole+1,d);
        }
    }
}


void ScoutWidget::addToTree(const QString &sysID, const QString &planiID)
{
    if(ui->treeWidget->findItems(sysID,Qt::MatchExactly,1).isEmpty()){
        QString cords = mDB->cords(sysID);
        if(!cords.isEmpty()){
            QTreeWidgetItem *item = new QTreeWidgetItem(QStringList() << "" << sysID << planiID);
            item->setData(0,Qt::UserRole,cords);
            ui->treeWidget->addTopLevelItem(item);
        }else {
            qDebug() << "system nich in der DB";
        }
    }else{
        qDebug() << "id gibt es schon in der liste";
    }
}

void ScoutWidget::nextStep()
{
    qDebug() << Q_FUNC_INFO;
    if(mTyp == System){
        mChildIndex = 0;
        if(mIndex < ui->treeWidget->topLevelItemCount()){
            QString id = ui->treeWidget->topLevelItem(mIndex)->text(2);
            mBot->sendShip(ui->lineEditShipID->text(),ui->lineEditShipPos->text(),id,ShipSendTask::Direkt);
        }else {
            qDebug() << "ENDE!";
        }
    } else {
        if(mChildIndex< ui->treeWidget->topLevelItem(mIndex)->childCount()){
            QString id = ui->treeWidget->topLevelItem(mIndex)->child(mChildIndex)->text(2);
            mBot->sendShip(ui->lineEditShipID->text(),ui->lineEditShipPos->text(),id,ShipSendTask::Direkt);
        }else {
            mTyp = System;
            mIndex++;
            nextStep();
        }
    }
}

void ScoutWidget::on_pushButton_clicked()
{
    qDebug() << Q_FUNC_INFO;
    mTyp = System;
    mIndex = 0;
    nextStep();
}

void ScoutWidget::on_pushButton_2_clicked()
{

}




void ScoutWidget::on_pushButtonClear_clicked()
{
    ui->treeWidget->clear();
}

void ScoutWidget::onBotParsedPostSendPage(const QList<QStringList> &list)
{
    qDebug() << Q_FUNC_INFO;
    if(list.size() == 1){
        QStringList ts= list.first().at(2).split(":");
        if(ts.size()<4){
            qDebug() << ts;
        }else {
            QDateTime t = QDateTime::currentDateTime().addDays(ts.first().toInt()).addSecs(ts.at(1).toInt()*3600).addSecs(ts.at(2).toInt()*60).addSecs(ts.at(3).toInt());
            int secs=ts.first().toInt()*3600*24+ts.at(1).toInt()*3600+ts.at(2).toInt()*60+ts.at(3).toInt();
            QTreeWidgetItem *item;
            if (mTyp == System){
                item = ui->treeWidget->topLevelItem(mIndex);
            }else if (mTyp == Orbit) {
                item = ui->treeWidget->topLevelItem(mIndex)->child(mChildIndex);
            }
            item->setText(0,t.toString("dd.MM.yy;HH:mm:ss"));

            item->setData(0,Qt::UserRole+1,secs);
            item->setText(0,Tool::secsToTimeString(secs));
            if(secs == 0)
                secs =1;
            //qDebug() << "starttimmer " << secs*1000;
            secs++;
            mTimer->start(secs*1000);
        }
    }else {
        qDebug() << "FEHLER";
    }
}

void ScoutWidget::onBotLoadSystemPage(const QString &id)
{
    qDebug() << Q_FUNC_INFO;

    QList<QPair<QString,QString> > list= mBot->parsSystemFormScout();
    qDebug() << list.size();
    for (int var = 0; var < list.size(); ++var) {
        if(list.at(var).first != "-keiner-" && !list.at(var).first.isEmpty()){
            QTreeWidgetItem *newItem = new QTreeWidgetItem();
            newItem->setText(2,list.at(var).second);
            ui->treeWidget->topLevelItem(mIndex)->addChild(newItem);
        }
    }
    ui->lineEditShipPos->setText(ui->treeWidget->topLevelItem(mIndex)->text(2));
    mTyp = Orbit;
    nextStep();
}

void ScoutWidget::onBotLoadSOrbitPage(const QString &id)
{
    qDebug() << Q_FUNC_INFO << id;
    mBot->performOrbitAction(Bot2::MarkAll);
    mBot->performOrbitAction(Bot2::Scannen);

    ui->lineEditShipPos->setText(id);
    mChildIndex++;
    nextStep();
}
