#include "sendmenu.h"


SendMenu::SendMenu(ShipTreeWidget *tree):mTree(tree),mMakiertFlotte(NULL),mMakiertSquad(NULL),
    mTmpClicked(NULL),mSendToMark(false),mSendToMarksClicked(false),mExit(true),mSettings("hakah","sendmenu"),
    mFavFlotte(NULL),mFavSquad(NULL),mSetFavFlotte(NULL),mSetFavSquad(NULL)
{
    mMenu = new QMenu();
    mSendFlotte = mMenu->addAction("sendFotte");
    QMenu *flottenTmp = new QMenu();

    QStringList tmpFav = mSettings.value("flotte").toStringList();
    if(!tmpFav.isEmpty()){
        mFavFlotte= new QMenu("Fav");
        foreach (const QString &str, tmpFav) {
            QAction *a = mFavFlotte->addAction(str.split(";").first());
            a->setData(str);
            qDebug() << str;
        }
        flottenTmp->addMenu(mFavFlotte);
    }
    for (int var = 0; var < mTree->topLevelItemCount(); ++var) {
        QTreeWidgetItem *item = mTree->topLevelItem(var);
        if(item->text(1)!= "Squadflotte"&& item->text(1) != "BergBauSchiff"){
            QAction *tmp = flottenTmp->addAction(item->text(1)+"  :  "+item->text(2));
            tmp->setCheckable(true);
            mFlottenActions.append(tmp);
        }
    }
    flottenTmp->addSeparator();
    mSetFavFlotte = flottenTmp->addAction("add to Fav");
    flottenTmp->addSeparator();
    mHierFlotte = flottenTmp->addAction("hierher senden");
    mSendFlotte->setMenu(flottenTmp);

    mSendSquad = mMenu->addAction("sendSquad");
    QMenu *squadTmp = new QMenu();


    tmpFav = mSettings.value("squad").toStringList();
    if(!tmpFav.isEmpty()){
        mFavSquad= new QMenu("Fav");
        foreach (const QString &str, tmpFav) {
            QAction *a = mFavFlotte->addAction(str.split(";").first());
            a->setData(str);
        }
        squadTmp->addMenu(mFavSquad);
    }
    QTreeWidgetItem *item = mTree->findItems("Squadflotte",Qt::MatchExactly,1).first();

    for (int var = 0; var < item->childCount(); ++var) {
        qDebug() << var;
        QTreeWidgetItem *i = item->child(var);
        QAction *tmp = squadTmp->addAction(i->text(1)+"  :  "+i->text(2));
        tmp->setCheckable(true);
        mSquadActions.append(tmp);
    }
    squadTmp->addSeparator();
    mSetFavSquad = squadTmp->addAction("add to Fav");
    squadTmp->addSeparator();
    mHierSquad = squadTmp->addAction("hierher senden");
    mSendSquad->setMenu(squadTmp);

}

SendMenu::~SendMenu()
{
    delete mMenu;
}

QMenu *SendMenu::menu()
{
    return mMenu;
}

void SendMenu::addSendToMark()
{
    mSendToMark = true;

    mMakiertFlotte = mSendFlotte->menu()->addAction("markSend");
    mMakiertSquad = mSendSquad->menu()->addAction("markSend");
}

bool SendMenu::checkSendClicked(QAction *ret)
{
    qDebug() << Q_FUNC_INFO;
    bool r = false;
    mTmpClicked = NULL;
    foreach (QAction* a, mFlottenActions) {
        if(a == ret){
            mTmpClicked = mSendFlotte;
            mMenu->setActiveAction(mSendFlotte);
            mExit = false;
        }
    }
    foreach (QAction* a, mSquadActions){
        if(a == ret){
            mTmpClicked = mSendSquad;
            mMenu->setActiveAction(mSendSquad);
            mExit = false;
        }
    }

    if(ret== mHierFlotte || ret == mMakiertFlotte){
        r = true;
        for (int var = 0; var <mTree->topLevelItemCount()-2; ++var) {
            if(mFlottenActions.at(var)->isChecked()){
                QTreeWidgetItem *i = mTree->topLevelItem(var);
                mIDs << i->data(0,ShipItem::ID).toString();
                mOrbitID = i->data(0,ShipItem::Orbit).toString();
            }
        }
    } else if(ret == mHierSquad || ret==mMakiertSquad){
        r = true;
        QTreeWidgetItem *item = mTree->findItems("Squadflotte",Qt::MatchExactly,1).first();
        for (int var = 0; var < mSquadActions.count(); ++var) {
            if(mSquadActions.at(var)->isChecked()){
                QTreeWidgetItem *i = item->child(var);
                mIDs << i->data(0,ShipItem::ID).toString();
                mOrbitID = i->data(0,ShipItem::Orbit).toString();
            }
        }
    }
    if(ret == mMakiertFlotte || ret==mMakiertSquad)
        mSendToMarksClicked = true;

    if(ret == mSetFavFlotte){
        saveChecked(Flotte);
    }
    if(ret == mSetFavSquad){
        saveChecked(Squad);
    }
    if(ret != NULL){
        if(ret->parent() == mFavFlotte || ret->parent() == mFavSquad)
            restorChecked(ret);
    }

    return r;
}

void SendMenu::saveChecked(Typ typ)
{
    QString str;
    QString typText;
    switch(typ){
    case SendMenu::Flotte:
        typText = "flotte";
        foreach (QAction *a, mFlottenActions) {
            if(a->isChecked())
                str.append(a->text()+";");
        }
        mMenu->setActiveAction(mSendFlotte);
        break;
    case SendMenu::Squad:
        typText = "squad";
        foreach (QAction *a, mSquadActions) {
            if(a->isChecked())
                str.append(a->text()+";");
        }
        mMenu->setActiveAction(mSendSquad);
        break;

    }
    if(!str.isEmpty()){
        QString title = QInputDialog::getText(0,"fagre","Bitte fav title eingeben" );
        if(!title.isEmpty()){
            str.remove(str.size()-1,1);
            str.prepend(title+";");

            QStringList l = mSettings.value(typText).toStringList();
            l.append(str);
            mSettings.setValue(typText,l);
        }
    }
    mExit = false;

}

void SendMenu::restorChecked(QAction *a)
{

     if(a->parent() == mFavFlotte){
         QStringList l = a->data().toString().split(";");
        for (int var = 1; var < l.size(); ++var) {
            foreach (QAction *ac, mFlottenActions) {
                qDebug() << l.at(var);
                if(ac->text() == l.at(var))
                    ac->setChecked(true);
                else
                    ac->setChecked(false);
            }
        }
        mMenu->setActiveAction(mSendFlotte);
    }else {
        QStringList l = a->data().toStringList();
        for (int var = 1; var < l.size(); ++var) {
            foreach (QAction *ac, mFlottenActions) {
                if(ac->text() == l.at(var))
                    ac->setChecked(true);
                else
                    ac->setChecked(false);
            }
        }
        mMenu->setActiveAction(mSendSquad);
    }
    mExit = false;
}

const QStringList& SendMenu::checkedID()
{
    return mIDs;
}

const QString &SendMenu::orbitID()
{
    return mOrbitID;
}

bool SendMenu::sendToMarkWasClicked()
{
    return mSendToMarksClicked;
}

void SendMenu::setDisable(bool x)
{

    mSendFlotte->setDisabled(x);
    mSendSquad->setDisabled(x);

}

QAction *SendMenu::clickedSendAction()
{
    return mTmpClicked;
}

bool SendMenu::exit()
{
    bool tmp = mExit;
    mExit =true;
    return tmp;
}

