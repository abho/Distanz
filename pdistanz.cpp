#include "pdistanz.h"
#include "ui_pdistanz.h"
#include <QDebug>
#include <cmath>


PDistanz::PDistanz(QMainWindow *parent) :
    QMainWindow(parent),
    ui(new Ui::PDistanz),mSetting("hakah","PDistanz"),mChatIndex(100)
{
    ui->setupUi(this);
    ui->treeWidget->setSelectionMode(QAbstractItemView::ExtendedSelection);

    ui->webView->setPage(new Page());
    connect(ui->graphicsView,SIGNAL(loadPage(QString)),this,SLOT(onShowODPageFromODDBTool(QString)));
    connect(ui->graphicsView_2,SIGNAL(loadPage(QString)),this,SLOT(onShowODPageFromODDBTool(QString)));
    connect(ui->graphicsView,SIGNAL(sendShip(QString,QString,QString)),&mBot,SLOT(sendShip(QString,QString,QString)));
    connect(ui->graphicsView,SIGNAL(sendShips(QList<QString>,QString)),&mBot,SLOT(sendShips(QList<QString>,QString)));
    connect(ui->graphicsView_2,SIGNAL(sendShip(QString,QString,QString)),&mBot,SLOT(sendShip(QString,QString,QString)));
    connect(ui->graphicsView_2,SIGNAL(sendShips(QList<QString>,QString)),&mBot,SLOT(sendShips(QList<QString>,QString)));
    connect(ui->graphicsView,SIGNAL(calcedNearest(QList<DistanzPair>)),ui->widgetNearest,SLOT(onGalaGVcalcedNearest(QList<DistanzPair>)));
    connect(ui->widgetNearest,SIGNAL(systemChoisen(QString)),ui->graphicsView,SLOT(onSystemChosen(QString)));
    connect(ui->widgetNearest,SIGNAL(openSystem(QString)),&mBot,SLOT(loadSystem(QString)));
    connect(ui->widgetNearest,SIGNAL(sendShip(QString,QString,QString)),&mBot,SLOT(sendShip(QString,QString,QString)));
    connect(ui->widgetNearest,SIGNAL(sendShips(QList<QString>,QString)),&mBot,SLOT(sendShips(QList<QString>,QString)));

    connect(ui->webViewChat,SIGNAL(loadFinished(bool)),this,SLOT(onChatPageLoaded(bool)));
    connect(&mTimer,SIGNAL(timeout()),this,SLOT(onChatTimer()));
    connect(&mBot,SIGNAL(setProgressBarVisible(bool)),this,SLOT(onProgressbarVisible(bool)));
    connect(&mBot,SIGNAL(setMessage(QString)),this,SLOT(onPrintMessage(QString)));
    connect(&mBot,SIGNAL(parsedFlottenAnsicht(QList<Schiff>)),ui->treeWidgetSchiffe,SLOT(onBotParsedFlottenansicht(QList<Schiff>)));
    connect(&mBot,SIGNAL(parsedOwnPanis(QList<ODPlanet>)),this,SLOT(onBotParsedOwnPlaniList(QList<ODPlanet>)));
    connect(&mBot,SIGNAL(parsedPlayerLogs(QList<Kampf>)),ui->graphicsView,SLOT(setPlayerLogItems(QList<Kampf>)));
    connect(&mBot,SIGNAL(parsedPlayerLogs(QList<Kampf>)),this,SLOT(onBotParsedPlayLogs(QList<Kampf>)));
    connect(&mBot,SIGNAL(shipsSended(QList<QStringList>)),ui->treeWidgetSchiffe,SLOT(onBotSendShip(QList<QStringList>)));
    connect(ui->webViewODDB,SIGNAL(loadFinished(bool)),this,SLOT(onODDBPageLoad(bool)));
    connect(ui->chatEdit,SIGNAL(anchorClicked(QUrl)),this,SLOT(onTextBrowserLinkClicked(QUrl)));
    connect(&mBot,SIGNAL(parsedWerft2Page(QList<QTreeWidgetItem*>)),ui->treeWidgetBau,SLOT(onBotParsedWerft2Page(QList<QTreeWidgetItem*>)));
    connect(&mBot,SIGNAL(parsedPlanetenPage(QTreeWidgetItem*)),ui->treeWidgetPlaneten,SLOT(onBotParsedPlanetenPage(QTreeWidgetItem*)));
    connect(&mBot,SIGNAL(updateCredits(QString)),this,SLOT(onBotUpdateCredits(QString)));
    connect(ui->treeWidgetSchiffe,SIGNAL(openOrbit(QString)),&mBot,SLOT(loadOrbit(QString)));
    connect(ui->treeWidgetPlaneten,SIGNAL(sentShip(QString,QString,QString)),&mBot,SLOT(sendShip(QString,QString,QString)));
    connect(ui->treeWidgetPlaneten,SIGNAL(sentShips(QList<QString>,QString)),&mBot,SLOT(sendShips(QList<QString>,QString)));
    connect(ui->treeWidgetPlaneten,SIGNAL(openOrbit(QString)),&mBot,SLOT(loadPlaneten(QString)));
    connect(ui->treeWidgetFav,SIGNAL(openOrbit(QString)),&mBot,SLOT(loadOrbit(QString)));
    connect(ui->treeWidgetFav,SIGNAL(openSystem(QString)),&mBot,SLOT(loadSystem(QString)));
    connect(ui->treeWidgetFav,SIGNAL(sentShip(QString,QString,QString)),&mBot,SLOT(sendShip(QString,QString,QString)));
    connect(ui->treeWidgetFav,SIGNAL(sentShips(QList<QString>,QString)),&mBot,SLOT(sendShips(QList<QString>,QString)));
    connect(ui->option,SIGNAL(loadGala(int)),&mBot,SLOT(loadGala(int)));
    connect(&mBot,SIGNAL(parsedScoutPage(QList<QPair<QString,QString> >)),ui->widget_2,SLOT(onBotParsedScoutPage(QList<QPair<QString,QString> >)));

    if(mSetting.contains("geo"))
        restoreGeometry(mSetting.value("geo").toByteArray());

    ui->comboBox->addItems(mSetting.value("combo",QStringList()).toStringList());
    if(!ui->comboBox->currentText().isEmpty())
        ui->graphicsView->setCalcPos(ui->comboBox->currentText());


    mBar = new QProgressBar();
    mBar->setMaximumWidth(180);
    mBar->setVisible(false);
    mBar->setRange(0,0);
    mBar->setMaximumHeight(12);
    // ui->statusbar->addPermanentWidget(mBar);

    mLabelLogin = new QLabel("not logged in");
    mLabelInfo = new QLabel();
    QFont f =   mLabelLogin->font();
    f.setPointSize(9);
    mLabelLogin->setFont(f);
    mLabelInfo->setFont(f);
    mLabelInfo->setMaximumHeight(12);
    mLabelLogin->setMaximumHeight(12);
    //ui->statusbar->addPermanentWidget(mLabelLogin);


    mComboBoxLogPlayer = new QComboBox();
    mComboBoxLogPlayer->setEditable(true);
    mComboBoxLogPlayer->addItems(mSetting.value("comboBoxplayer").toStringList());
    ui->toolBar->insertWidget(ui->actionLoadLogs,mComboBoxLogPlayer);

    mComboBoxGala = new QComboBox();
    mComboBoxGala->setEditable(true);
    mComboBoxGala->addItems(mSetting.value("comboboxgala",QStringList() << "7").toStringList());
    mComboBoxGala->setCurrentIndex(mSetting.value("gala").toInt());
    ui->toolBar->insertWidget(ui->actionGalaScreen,mComboBoxGala);
    connect(mComboBoxGala,SIGNAL(currentIndexChanged(QString)),this,SLOT(onComboBoxGalaCurrentIndexChanged(QString)));



    mComboBoxAllys = new QComboBox();
    mComboBoxAllys->setEditable(true);
    mComboBoxAllys->addItems(mSetting.value("comboboxally",QStringList()).toStringList());
    ui->toolBar->insertWidget(ui->actionAlly,mComboBoxAllys);
    connect(mComboBoxAllys,SIGNAL(currentIndexChanged(QString)),this,SLOT(onComboBoxAllyCurrentIndexChanged(QString)));

    ui->graphicsView->setStatusBarAndView(mLabelInfo,ui->graphicsView_2);
    ui->graphicsView->setDB(&mDB);

    ui->webViewChat->setPage(new Page());



    restoreState(mSetting.value("dock").toByteArray());


    ui->webViewODDB->setPage(new Page());
    connect(static_cast<Page *>(ui->webViewODDB->page()),SIGNAL(loadPage(QString)),this,SLOT(onShowODPageFromODDBTool(QString)));
    QPalette p=palette();
    p.setColor(QPalette::Base,QColor(Qt::gray));
    ui->chatEdit->setPalette(p);

    mTimer.start(1000);

    QColor linkColor("#37C0FD");
    QString sheet = QString::fromLatin1("a { text-decoration: none; color: %1 } a:hover{text-decoration:underline; }").arg(linkColor.name());
    ui->chatEdit->document()->setDefaultStyleSheet(sheet);



    ui->treeWidgetFav->loadItems(mSetting.value("fav",QStringList()).toStringList());

    ui->stackedWidget->setCurrentIndex(1);
    mLastAction = ui->actionOD;
    ui->actionOD->setChecked(true);

    ui->option->setIds(mSetting.value("optionplaniids",QStringList()).toStringList());
    for (int var = 0; var < ui->treeWidgetPlaneten->columnCount(); ++var) {
        ui->treeWidgetPlaneten->resizeColumnToContents(var);
    }

    mLabelCredits = new QLabel();
    mLabelCredits->setObjectName("labelCredits");
    ui->toolBar->addWidget(mLabelCredits);

    QWidget *w  = new QWidget();
    QHBoxLayout *layout = new QHBoxLayout();
    layout->addStretch(10);

    layout->addWidget(mLabelInfo);
    layout->addWidget(mBar);
    layout->addWidget(mLabelLogin);
    w->setLayout(layout);
    w->setMaximumHeight(20);
    layout->setMargin(0);
    ui->toolBar->addWidget(w);

    mSystemIcon.setIcon(QIcon(":/odIcon"));
    mSystemIcon.show();

    ui->graphicsView->setGala(mComboBoxGala->currentText().toInt());

    ui->graphicsView_2->setScene(ui->graphicsView->scene());
    ui->graphicsView_2->setDB(&mDB);
}

PDistanz::~PDistanz()
{
    mSetting.setValue("geo",saveGeometry());
    QStringList l;
    for (int var = 0; var < ui->comboBox->count(); ++var) {
        l.append(ui->comboBox->itemText(var));
    }
    mSetting.setValue("combo",l);
    l.clear();
    for (int var = 0; var < mComboBoxLogPlayer->count(); ++var) {
        l.append(mComboBoxLogPlayer->itemText(var));
    }
    mSetting.setValue("comboboxplayer",l);
    l.clear();
    for (int var = 0; var < mComboBoxGala->count(); ++var) {
        l.append(mComboBoxGala->itemText(var));
    }
    mSetting.setValue("comboboxgala",l);
    mSetting.setValue("gala",mComboBoxGala->currentIndex());
    l.clear();
    for (int var = 0; var < mComboBoxAllys->count(); ++var) {
        l.append(mComboBoxAllys->itemText(var));
    }
    mSetting.setValue("comboboxally",l);

    mSetting.setValue("fav",ui->treeWidgetFav->saveItems());

    mSetting.setValue("dock",saveState());

    mSetting.setValue("optionplaniids",ui->option->ids());
    delete ui;

}



void PDistanz::onProgressbarVisible(bool isVisible)
{
    mBar->setVisible(isVisible);
}



int PDistanz::clacDis(const QString &str, qreal x1, qreal y1, qreal z1)
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




void PDistanz::onBotParsedSysteme(const QList<QTreeWidgetItem *> &list)
{
    ui->treeWidget->addTopLevelItems(list);
}



void PDistanz::onBotUpdateCredits(const QString &str)
{
    mLabelCredits->setText("Credits:"+str.split(":").first());
    mLabelCredits->setProperty("credits",str.split(":").last().toFloat());
}




void PDistanz::onBotParsedOwnPlaniList(const QList<ODPlanet> &list)
{
    mOwnPlanis = list;
}

void PDistanz::onPrintMessage(QString str)
{
    ui->lineEditChat->setText(str);
    ui->statusbar->showMessage(str);
}

void PDistanz::onODDBPageLoad(bool ok)
{

    QWebFrame *f =  ui->webViewODDB->page()->currentFrame();
    if(f->url().toString().contains("oddb.kryops.de")){
        QWebElementCollection c = f->findAllElements("input[name=username]");
        c.first().setAttribute("value","Abho");
        QWebElementCollection cc = f->findAllElements("input[name=pw]");
        cc.first().setAttribute("value","fanatik1");

        QWebElementCollection ccc = f->findAllElements("input[type=submit]");
        ccc.first().evaluateJavaScript("this.click()");
    }else if(f->url().toString()=="http://usc.forenworld.at/ucp.php?mode=login"){
        QWebElementCollection c = f->findAllElements("input[name=username]");
        c.first().setAttribute("value","Abho");
        QWebElementCollection cc = f->findAllElements("input[name=password]");
        cc.first().setAttribute("value","fanatik1");
        QWebElementCollection ccc = f->findAllElements("input[type=submit]");
        ccc.first().evaluateJavaScript("this.click()");
    }
}

void PDistanz::onShowODPageFromODDBTool(QString str)
{
    ui->stackedWidget->setCurrentIndex(1);
    mBot.loadPage(str);
}

void PDistanz::onTextBrowserLinkClicked(QUrl str)
{
    qDebug() << str.toString();
    mBot.loadPage("http://www.omega-day.com/game/"+str.toString());
}

void PDistanz::onBotParsedPlayLogs(const QList<Kampf> &list)
{
    ui->treeWidgetCombatLog->clear();
    for (int var = list.count()-1; 0 <= var; --var) {
        ui->treeWidgetCombatLog->addTopLevelItem(new QTreeWidgetItem(QStringList()<<list.at(var).toString()));
    }
}

void PDistanz::onActionAddToCombat()
{
    qDebug() << Q_FUNC_INFO << ui->chatEdit->textCursor().selectedText();
}



void PDistanz::loadFromDB(int gala)
{



}

void PDistanz::login()
{

    QString str;
    str.append("username=abho&passwort=fanatik1&op=login&setworld=int9&Abschicken=Abschicken");
    QNetworkRequest re(QUrl("http://www.omega-day.com/game/?setworld=int9"));
    re.setHeader(QNetworkRequest::ContentTypeHeader,"application/x-www-form-urlencoded");
    mManager.post(re,str.toUtf8());
    connect(&mManager,SIGNAL(finished(QNetworkReply*)),this,SLOT(onReply(QNetworkReply*)));
}


QTreeWidgetItem *PDistanz::colorItem(QTreeWidgetItem *item)
{

    if(item->text(3) !="0"){
        if(item->text(3).left(3).remove(0,1).toInt()>28){
            for (int var = 0; var < 4; ++var) {
                item->setBackgroundColor(var,QColor(255,0,0,100));
            }
        }else if(item->text(3).left(3).remove(0,1).toInt()>25 || item->text(3).contains("25-3 Mio")){
            for (int var = 0; var < 4; ++var) {
                item->setBackgroundColor(var,QColor(0,255,255,100));
            }
        }
    }
    return item;
}







void PDistanz::on_treeWidget_doubleClicked(const QModelIndex &index)
{
    mBot.parsSystemPage(index.data(Qt::UserRole+1).toString(),false);
}




void PDistanz::on_treeWidget_currentItemChanged(QTreeWidgetItem *current, QTreeWidgetItem *previous)
{
    ui->graphicsView->setCurrentSelected(current);
}


void PDistanz::on_pushButtonLoadPlanis_clicked()
{
    QList<QTreeWidgetItem *> items = ui->treeWidget->selectedItems();
    for (int var = 0; var < items.size(); ++var) {
        mBot.parsSystemPage(items.at(0)->data(0,Qt::UserRole+1).toString(),true);
    }
}

void PDistanz::on_pushButtonCalc_clicked()
{
    QTreeWidgetItem *tmp;
    QString str = ui->comboBox->currentText();
    if( ui->comboBox->findText(str) == -1)
        ui->comboBox->addItem(str);
    if(!str.isEmpty()){
        ui->graphicsView->setCalcPos(str);
        str.remove(0,2);
        QStringList l = str.split("/");
        qreal x1 = l.at(0).toDouble();
        qreal y1 = l.at(1).toDouble();
        qreal z1 = l.at(2).toDouble();
        QString sum;

        for (int var = 0; var < ui->treeWidget->topLevelItemCount(); ++var) {
            tmp = ui->treeWidget->topLevelItem(var);
            sum = QString::number(clacDis(tmp->data(0,Qt::UserRole).toString(),x1,y1,z1));
            switch(sum.size()){
            case 1:
                sum.prepend("00");
                break;
            case 2:
                sum.prepend("0");
                break;
            }
            tmp->setText(1,sum);
        }
    }
}



void PDistanz::on_actionLogin_triggered()
{
    login();
}




void PDistanz::on_actionShowOwn_toggled(bool arg1)
{

    ui->graphicsView->showOwn(arg1);
}





void PDistanz::on_actionLoadLogs_triggered(bool checked)
{

    qDebug() << Q_FUNC_INFO<< mComboBoxLogPlayer->currentText();
    if(checked){
        mBot.parsPlayerLogs(mComboBoxLogPlayer->currentText());
    }else {
        ui->graphicsView->rmPlayerLogItems();
    }
}





void PDistanz::onReply(QNetworkReply *re)
{


    mManager.disconnect();
    ui->webView->page()->setNetworkAccessManager(&mManager);
    ui->webViewChat->page()->setNetworkAccessManager(&mManager);
    ui->webViewChat->load(QUrl("http://www.omega-day.com/game/?op=melder"));
    ui->webViewODDB->page()->setNetworkAccessManager(&mManager);
    ui->widget_2->setDBAndManager(&mDB,&mManager,ui->option);
    mLabelLogin->setText("Last Login: "+QTime::currentTime().toString("hh:mm:ss"));
    mBot.setViewAndDBAndOption(ui->webView,mDB,ui->option);
    mBot.fetchPages();

}

void PDistanz::on_actionAddtoChat_triggered()
{
    QFile file("style.txt");
    file.open(QIODevice::ReadOnly);
    QString s = file.readAll();
    file.close();
    setStyleSheet(s);


}

void PDistanz::onChatTimer()
{
    QWebElementCollection e =  ui->webViewChat->page()->currentFrame()->findAllElements("div [id=divChat]");
    if(e.count() != 0){
        e =e.first().findAll("div");
        int index =  e.count();
        if(index != mChatIndex && index >100){
            for (int var = mChatIndex; var < index; ++var) {
                QWebElement webE = e.at(var);
                ui->chatEdit->append(webE.toOuterXml());
                QString msg = webE.toPlainText();
                QString first;
                qDebug() << msg.at(16);
                if(msg.at(16)=='<'){
                    int index = msg.indexOf(">");
                    first = msg.left(index+1);
                } else if(msg.contains("EMP")){
                    int index = msg.indexOf("EMP");
                    first = msg.left(index+3);
                }else if(msg.contains("VERLOREN")){
                    int index = msg.indexOf("VERLOREN");
                    first = msg.left(index+8);
                }else if(msg.contains("GEWONNEN")){
                    int index = msg.indexOf("GEWONNEN");
                    first = msg.left(index+8);
                }else if(msg.contains("RAID")){
                    int index = msg.indexOf("RAID");
                    first = msg.left(index+4);
                }else if(msg.contains("ANGRIFF PLANET")){
                    int index = msg.indexOf("ANGRIFF PLANET");
                    first = msg.left(index+14);
                } else if(msg.contains("ANGRIFF SCHIFFE")){
                    int index = msg.indexOf("ANGRIFF SCHIFFE");
                    first = msg.left(index+15);
                } else if(msg.contains("ABHOLUNG")){
                    int index = msg.indexOf("ABHOLUNG");
                    first = msg.left(index+8);
                }else if(msg.contains("LIEFERUNG")){
                    int index = msg.indexOf("LIEFERUNG");
                    first = msg.left(index+9);
                }
                mSystemIcon.showMessage(first,msg.remove(0,first.size()));
                ui->chatEdit->verticalScrollBar()->triggerAction(QAbstractSlider::SliderToMaximum);
            }
            mChatIndex = index;
        }
    }
    ui->treeWidgetSchiffe->updateTimeout();
    ui->treeWidgetPlaneten->updateTimeout();




    float c = mLabelCredits->text().split(":").last().remove(',').toDouble()+ mLabelCredits->property("credits").toDouble();

    mLabelCredits->setText("$:"+Tool::insertDots(QString::number(c,'f',3)));

}

void PDistanz::onChatPageLoaded(bool x)
{
    QWebElement e =  ui->webViewChat->page()->currentFrame()->findAllElements("div [id=divChat]").first();

    ui->chatEdit->setHtml(e.toOuterXml());
    QScrollBar *vScrollBar = ui->chatEdit->verticalScrollBar();
    vScrollBar->triggerAction(QScrollBar::SliderToMaximum);
    ui->chatEdit->moveCursor(QTextCursor::End);

}
void PDistanz::on_pushButtonChat_clicked()
{
    QString str = ui->lineEditChat->text();
    if(str.left(4)== "http"){
        QMessageBox::StandardButton ret = QMessageBox::question(this,"frage","Wirklich im Chat posten?");
        if(ret == QMessageBox::No || ret ==QMessageBox::Escape)
            return ;
    }
    ui->webViewChat->page()->currentFrame()->evaluateJavaScript("document.getElementById(\"chat_input\").value = \'"+ui->lineEditChat->text()+"\'");
    QWebElementCollection e =ui->webViewChat->page()->mainFrame()->findAllElements("input[name=Abschicken]");
    qDebug() << e.count();
    e.first().evaluateJavaScript("this.click()");

}

void PDistanz::on_webView_customContextMenuRequested(const QPoint &pos)
{
    QWebHitTestResult h =  ui->webView->page()->currentFrame()->hitTestContent(pos);
    QString hitUrl,hitContext;
    if(h.element().tagName()=="IMG"){
        hitUrl = h.linkUrl().toString();//<< h.element().parent().toOuterXml();
        hitContext= h.element().parent().toOuterXml();
    }

    SendMenu menu(ShipTreeWidget::tree());

    QMenu * m = menu.menu();
    menu.addSendToMark();


    QAction *back = m->addAction("back");
    QAction *fav = m->addAction("Add to Fav");
    QAction *add = m->addAction("add To GalaView");
    QAction *addGate = m->addAction("add Gala");

    QString url = ui->webView->page()->currentFrame()->url().toString();

    if(!hitContext.contains("Sprungtor"))
        addGate->setEnabled(false);
    if(!url.contains("op=orbit")&& !url.contains("op=system")){
        fav->setEnabled(false);
        add->setEnabled(false);
    }
    if(!url.contains("op=orbit")&&!url.contains("op=planet")&&!hitUrl.contains("op=orbit&index"))
        menu.setDisable(true);

    do{

        QAction *ret = m->exec(ui->webView->mapToGlobal(pos));

        if(ret == fav){
            ui->treeWidgetFav->addTopLevelItem(mBot.parsFavPage());
            ui->treeWidgetFav->resizeColumnToContents(0);

        } else if( ret == back){
            ui->webView->triggerPageAction(QWebPage::Back,true);
        } else if(menu.checkSendClicked(ret)){
            const QStringList &l = menu.checkedID();
            const QString &oldID = menu.orbitID();
            if(l.size()>0){
                QString planiID;
                if(menu.sendToMarkWasClicked()){
                    planiID = ui->webView->selectedText();
                }else{
                    if(url.contains("op=orbit&index"))
                        planiID = url.split("=").last();
                    else
                        planiID = hitUrl.split("=").last();
                }
                if(l.size() == 1)
                    mBot.sendShip(l.first(),oldID,planiID);
                else
                    mBot.sendShips(l,planiID);
            }
        }else if(ret == add){
            ui->graphicsView->setMomentPos( mBot.parsCurrentToAdd());
            if( ui->dockWidget_10->isHidden())
                ui->dockWidget_10->show();

        } else if(ret == addGate){
            bool ok;
            int gal = QInputDialog::getInt(this,"Gata","Gala eintragen:",1,1,100,1,&ok);
            if(ok){
                mDB.insetrGate(Gate(gal,hitUrl.split("=").last(),url.split("=").last()));
            }

        }
    }while(!menu.exit());

}


void PDistanz::on_pushButton_clicked()
{
    mBot.loadPage(ui->lineEditChat->text());
    ui->stackedWidget->setCurrentIndex(1);
}

void PDistanz::on_actionODDBtool_triggered(bool checked)
{
    if(!checked)
        ui->actionODDBtool->setChecked(true);
    else {
        if( mLastAction !=ui->actionODDBtool ){
            mLastAction->setChecked(false);
            mLastAction = ui->actionODDBtool;
        }
    }

    if(!ui->webViewODDB->url().toString().contains("oddb.kryops.de")){
        ui->webViewODDB->load(QUrl("http://oddb.kryops.de/index.php?inst=11"));
    }
    ui->stackedWidget->setCurrentIndex(2);
}

void PDistanz::on_actionPlaniEditor_triggered(bool checked)
{
    if(!ui->webViewODDB->url().toString().contains("simod.gdra.de")){
        ui->webViewODDB->load(QUrl("http://simod.gdra.de/pkr9/"));
    }
    ui->stackedWidget->setCurrentIndex(2);

    if(!checked)
        ui->actionPlaniEditor->setChecked(true);
    else {
        if( mLastAction !=ui->actionPlaniEditor ){
            mLastAction->setChecked(false);
            mLastAction = ui->actionPlaniEditor;
        }
    }
}

void PDistanz::on_actionShipConfig_triggered(bool checked)
{
    if(!ui->webViewODDB->url().toString().contains("shipconfigurator")){
        ui->webViewODDB->load(QUrl("http://www.omega-day.de/game/shipconfigurator/"));
    }
    ui->stackedWidget->setCurrentIndex(2);

    if(!checked)
        ui->actionShipConfig->setChecked(true);
    else {
        if( mLastAction !=ui->actionShipConfig ){
            mLastAction->setChecked(false);
            mLastAction = ui->actionShipConfig;
        }
    }
}


void PDistanz::on_actionAllyForum_triggered(bool checked)
{

    if(!ui->webViewODDB->url().toString().contains("shipconfigurator")){
        ui->webViewODDB->load(QUrl("http://usc.forenworld.at/ucp.php?mode=login"));
    }
    ui->stackedWidget->setCurrentIndex(2);

    if(!checked)
        ui->actionAllyForum->setChecked(true);
    else {
        if( mLastAction !=ui->actionAllyForum){
            mLastAction->setChecked(false);
            mLastAction = ui->actionAllyForum;
        }
    }
}

void PDistanz::on_actionChat_triggered(bool checked)
{

    ui->stackedWidget->setCurrentIndex(3);

    if(!checked)
        ui->actionChat->setChecked(true);
    else {
        if( mLastAction !=ui->actionChat){
            mLastAction->setChecked(false);
            mLastAction = ui->actionChat;
        }
    }
}

void PDistanz::on_actionOD_triggered(bool checked)
{
    ui->stackedWidget->setCurrentIndex(1);

    if(!checked)
        ui->actionOD->setChecked(true);
    else {
        if( mLastAction !=ui->actionOD){
            mLastAction->setChecked(false);
            mLastAction = ui->actionOD;
        }
    }
}

void PDistanz::on_actionGalaScreen_triggered(bool checked)
{
    ui->stackedWidget->setCurrentIndex(0);

    if(!checked)
        ui->actionGalaScreen->setChecked(true);
    else {
        if( mLastAction !=ui->actionGalaScreen){
            mLastAction->setChecked(false);
            mLastAction = ui->actionGalaScreen;
        }
    }
}



void PDistanz::on_actionOption_triggered(bool checked)
{

    ui->stackedWidget->setCurrentIndex(4);

    if(!checked)
        ui->actionOption->setChecked(true);
    else {
        if( mLastAction !=ui->actionOption){
            mLastAction->setChecked(false);
            mLastAction = ui->actionOption;
        }
    }
}





void PDistanz::on_chatEdit_customContextMenuRequested(const QPoint &pos)
{
    QMenu *m = ui->chatEdit->createStandardContextMenu();
    m->addAction("addToLog",this,SLOT(onActionAddToCombat()));
    m->exec(ui->chatEdit->mapToGlobal(pos));
}

void PDistanz::on_webViewODDB_customContextMenuRequested(const QPoint &pos)
{

    if(ui->webViewODDB->url().toString().contains("http://oddb.kryops.de")){
        QMenu m;
        QAction *pars = m.addAction("pars");
        QAction *p = m.addAction("parsScout");
        QAction *ret  = m.exec(ui->webViewODDB->mapToGlobal(pos));
        if(ret==pars){
            mBot.parsCurrent(ui->webViewODDB->page()->mainFrame());
        }else if(ret == p){
            mBot.parsScoutPage(ui->webViewODDB->page()->mainFrame());
        }
    }
}

void PDistanz::onComboBoxGalaCurrentIndexChanged(const QString &gala)
{
    qDebug() <<Q_FUNC_INFO << gala;
    ui->graphicsView->setGala(gala.toInt());
}

void PDistanz::onComboBoxAllyCurrentIndexChanged(const QString &ally)
{
}

void PDistanz::on_actionAlly_toggled(bool arg1)
{
    ui->graphicsView->showESysteme(mComboBoxAllys->currentText(),arg1);
}

void PDistanz::on_actionTest_triggered()
{

    mBot.test();
}

void PDistanz::on_actionScout_triggered(bool checked)
{
    ui->stackedWidget->setCurrentIndex(5);

    if(!checked)
        ui->actionScout->setChecked(true);
    else {
        if( mLastAction !=ui->actionScout){
            mLastAction->setChecked(false);
            mLastAction = ui->actionScout;
        }
    }
}
