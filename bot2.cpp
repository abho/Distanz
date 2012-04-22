#include "bot2.h"

Bot2::Bot2(QWebView *view, DBHandler *db, Option *option, QObject *parent) :
    QObject(parent),mWebView(view),mParser(new Parser(mOption,mDB,this)),mDB(db),mOption(option)
{
    connect(mWebView->page()->mainFrame(),SIGNAL(loadFinished(bool)),this,SLOT(onWebFrameLoadFinished(bool)));
    connect(mWebView,SIGNAL(loadFinished(bool)),this,SLOT(onLoadFinished(bool)));
}

void Bot2::setODDBEnable(bool isEnable)
{
    mJSObject.setEnable(isEnable);
}

void Bot2::loadSysteme(QHash<QString, QTreeWidgetItem *> *oldSysteme, int gala)
{
    qDebug() << Q_FUNC_INFO;

    mTasks.append(QSharedPointer<BotTask>(new LoadSystemeTask(oldSysteme,gala)));
    startIfNotRunning();
}

void Bot2::parsSystemPage(const QString &sysID)
{
    mTasks.append(QSharedPointer<BotTask>(new ParsSystemPage(sysID)));
    startIfNotRunning();
}

void Bot2::parsOwnPlaneten()
{
    qDebug() << Q_FUNC_INFO ;
    mTasks.append(QSharedPointer<BotTask>(new BotTask(BotTask::ParsOwnPlaneten)));
    startIfNotRunning();
}

void Bot2::parsPlayerLogs(const QString &playerID)
{
    qDebug() << Q_FUNC_INFO <<  playerID;
    mTasks.append(QSharedPointer<BotTask>(new ParsPlayerLogs(playerID)));
    startIfNotRunning();

}

QList<QPair<QString,QString> >  Bot2::parsScoutPage(QWebFrame *frame)
{
    return mParser->parsScoutPage(frame);
}

void Bot2::parsODDBAllyPage(QWebFrame *frame)
{
    mParser->parsCurrentODDBPage(frame);
}

void Bot2::login()
{
    qDebug() << Q_FUNC_INFO;

    QWebFrame *frame = mWebView->page()->currentFrame();
    if( frame->toHtml().contains("loginscreen")){

        QWebElementCollection c = frame->findAllElements("input[name=username]");
        if(c.count()!= 0)
            c.first().setAttribute("value","Abho");
        c = frame->findAllElements("input[name=passwort]");
        if(c.count() != 0)
            c.first().setAttribute("value","fanatik1");
        c = frame->findAllElements("input[type=submit]");
        if(c.count()!= 0)
            c.first().evaluateJavaScript("this.click()");
    }
    emit setProgressBarVisible(true);

}

QString Bot2::parsCurrentToAdd()
{
    return mParser->parsAddPage(mWebView->page()->currentFrame());
}

void Bot2::parsPlanetenPage()
{
    mParser->parsPlanetenPage(mWebView->page()->mainFrame());
}

void Bot2::fetchPages()
{
    QNetworkRequest re(QUrl("http://www.omega-day.com/game/?op=fleet"));
    re.setAttribute(QNetworkRequest::User,"fleet");
    mWebView->page()->networkAccessManager()->get(re);

    for (int var = 0; var < mOption->ids().count(); ++var) {
        const QString &id =mOption->ids().at(var);
        QNetworkRequest re(QUrl("http://www.omega-day.com/game/?op=planet&index="+id));
        re.setAttribute(QNetworkRequest::User,id);
        mWebView->page()->networkAccessManager()->get(re);
    }
}

QTreeWidgetItem *Bot2::parsFavPage()
{
    return mParser->parsFavPage(mWebView->page()->mainFrame());
}

void Bot2::performOrbitAction(Bot2::OrbitActions action)
{
    qDebug() << Q_FUNC_INFO << action;
    QWebFrame *frame = mWebView->page()->mainFrame();

    switch(action){
    case Bot2::No:
        break;
    case Bot2::MarkAll:
        frame->evaluateJavaScript("aller(1)");
        break;
    case Bot2::Scannen:
        mTasks.append(QSharedPointer<BotTask>(new BotTask(BotTask::Scannen)));
        frame->evaluateJavaScript("scanit()");
        break;
    }
}

QList<QPair<QString,QString> >  Bot2::parsSystemFormScout()
{
    return mParser->parsSystem(mWebView->page()->mainFrame());
}

void Bot2::sendShip(const QString &shipID, const QString &orbitID,const QString &zielOrbitID,ShipSendTask::SendTyp typ)
{
    qDebug() << Q_FUNC_INFO << shipID << zielOrbitID;
    mTasks.append(QSharedPointer<BotTask>(new ShipSendTask(shipID,orbitID,zielOrbitID,typ)));
    startIfNotRunning();

}

void Bot2::sendShips(const QStringList &ships, const QString &zielID, ShipSendTask::SendTyp typ)
{
    qDebug() << Q_FUNC_INFO << ships << zielID;
    mTasks.append(QSharedPointer<BotTask>(new ShipsSendTask(ships,zielID,typ)));
    startIfNotRunning();
}

void Bot2::loadPage(QString str)
{
    qDebug() << Q_FUNC_INFO << str;
    mTasks.append(QSharedPointer<BotTask>(new LoadPageTask(str)));
    startIfNotRunning();
}

void Bot2::loadOrbit(const QString &orbitID)
{
    qDebug() << Q_FUNC_INFO << orbitID;
    mTasks.append(QSharedPointer<BotTask>(new LoadOrbitTask(orbitID)));
    startIfNotRunning();
}

void Bot2::loadPlaneten(const QString &orbitID)
{
    qDebug() << Q_FUNC_INFO << orbitID;
    mTasks.append(QSharedPointer<BotTask>(new LoadPlanetTask(orbitID)));
    startIfNotRunning();

}

void Bot2::loadSystem(const QString &systemID)
{
    qDebug() << Q_FUNC_INFO << systemID;
    mTasks.append(QSharedPointer<BotTask>(new LoadSystemTask(systemID)));
    startIfNotRunning();
}


void Bot2::onWebFrameLoadFinished(bool ok)
{
    if(mJSObject.isEnable() &ok){
        mWebView->page()->currentFrame()->evaluateJavaScript(mJSObject.jquery());
        mWebView->page()->currentFrame()->addToJavaScriptWindowObject("jsobject",&mJSObject);
        mWebView->page()->mainFrame()->evaluateJavaScript(mJSObject.insertJS());
        mWebView->page()->mainFrame()->evaluateJavaScript("oddbtool.loadPage();");
    }
}

void Bot2::onLoadFinished(bool ok)
{
    qDebug() << Q_FUNC_INFO;
    emit setProgressBarVisible(false);

    QString url(mWebView->url().toString());
    QWebFrame *frame = mWebView->page()->mainFrame();
    BotTask::Typ typ(BotTask::No);
    if(!mTasks.isEmpty())
        typ = mTasks.head().data()->typ();
    qDebug() << typ;
    switch(typ){
    case BotTask::ShipSend:{
        QSharedPointer<ShipSendTask> s = mTasks.head().staticCast<ShipSendTask>();
        if(url.contains("op=send&index="+s->orbitID()+"&ships="+s->shipID())){
            if(!mParser->parsSendShipPage(frame,s->zielID(),s->sendTyp()))
                nextTask();
        }else if(url.contains("op=send&index="+s->orbitID())){
            QStringList time =mParser->parsPostSendPage(frame);
            QList<QStringList> l;
            if(time.count()>0){
                for (int var = 0; var < 1; ++var) {
                    l.append(QStringList() << s->shipID() << s->zielID() <<time.at(var));
                }
            }
            emit shipsSended(l);
            nextTask();
        }else {
            nextTask();
        }
        break;
    }
    case BotTask::ShipsSend:{
        QSharedPointer<ShipsSendTask> s =  mTasks.head().staticCast<ShipsSendTask>();
        if(url.contains("op=fleet")){
            if(!mParser->sendShips(frame,s->shipIDs()))
                nextTask();
        }else if(url.contains("op=send&allyfl")){
            if(!mParser->parsSendShipPage(frame,s->zielID(),s->sendTyp()))
                nextTask();
        } else if (url.contains("op=send&index=")) {
            QStringList time(mParser->parsPostSendPage(frame));
            QList<QStringList> l;
            if(time.count()>0){
                for (int var = 0; var < s->shipIDs().count(); ++var) {
                    l.append(QStringList() << s->shipIDs().at(var) << s->zielID() <<time.at(var));
                }
            }
            emit shipsSended(l);
            nextTask();
        }else {
            nextTask();
        }
        break;
    }
    case BotTask::LoadSyteme:
        if(url.contains("last=5000&galax"))
            mParser->parsSystemListPage(frame,mTasks.head().staticCast<LoadSystemeTask>()->oldSysteme());
        nextTask();
        break;
    case BotTask::ParsSystemPage:
        if(url.contains("op=system") && url.contains(mTasks.head().staticCast<ParsSystemPage>()->sysID()))
            emit parsedSystemPage(mParser->parsSystemPage(frame));
        nextTask();
        break;
    case BotTask::ParsOwnPlaneten:
        if(url.contains("op=planlist"))
            emit parsedOwnPanis(mParser->parsOwnPlaniListe(frame));
        nextTask();
        break;

    case BotTask::ParsPlayerLogs:
        if(url.contains("op=warp&wessen"))
            emit parsedPlayerLogs(mParser->parsPlayerLog(frame));
        break;
    case BotTask::LoadPage:
        emit loadedPlanet(mTasks.head().staticCast<LoadPageTask>()->url());
        nextTask();
        break;
    case BotTask::LoadOrbit:
        emit loadedOrbit(mTasks.head().staticCast<LoadOrbitTask>()->orbitID());
        nextTask();
        break;
    case BotTask::LoadPlanet:
        emit loadedPlanet(mTasks.head().staticCast<LoadPlanetTask>()->orbitID());
        nextTask();
        break;
    case BotTask::LoadSystem:
        emit loadedSystem(mTasks.head().staticCast<LoadSystemTask>()->systemID());
        nextTask();
        break;
    case BotTask::Scannen:
        nextTask();
        break;
    case BotTask::No:{
        if(url.contains("op=planet")&&mOption->ids().contains(url.split("=").last())){
            QPair<QTreeWidgetItem*,QString> ret(mParser->parsPlanetenPage(frame));
            emit parsedPlanetenPage(ret.first);
            emit updateCredits(ret.second);
        }else if(url.contains("op=fleet"))
            emit parsedFlottenAnsicht(mParser->parsFlottenAnsicht(frame));
        else if(url.contains("op=werft2"))
            emit parsedWerft2Page(mParser->parsWerft2Page(frame));
        break;
    }


    }


}

void Bot2::onRequestFinished(QNetworkReply *re)
{
    QString attri = re->request().attribute(QNetworkRequest::User).toString();
    int status = re->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();

    if(status == 200){
        QWebPage p;
        p.mainFrame()->setHtml(re->readAll());

        if(attri =="fleet"){
            emit parsedFlottenAnsicht( mParser->parsFlottenAnsicht(p.mainFrame()));
        }else if(!attri.isEmpty())  {
            QPair<QTreeWidgetItem*,QString> pair = mParser->parsPlanetenPage(p.mainFrame(),attri);
            emit  parsedPlanetenPage(pair.first);
            emit updateCredits(pair.second);
        }
    }
}

void Bot2::startIfNotRunning()
{
    qDebug() << Q_FUNC_INFO << mTasks.size();
    if(mTasks.size() == 1)
        start();
}

void Bot2::start()
{
    qDebug() << Q_FUNC_INFO << mTasks.isEmpty();
    if(!mTasks.isEmpty()){
        BotTask::Typ typ(mTasks.head()->typ());
        qDebug() << typ;
        switch(typ){
        case BotTask::No:
            break;
        case BotTask::ShipSend:{
            QSharedPointer<ShipSendTask> s=  mTasks.head().staticCast<ShipSendTask>();
            loadUrl("http://www.omega-day.com/game/?op=send&index="+s->orbitID()+"&"+"ships="+s->shipID());
            break;
        }
        case BotTask::ShipsSend:
            loadUrl(QString("http://www.omega-day.com/game/?op=fleet"));
            break;
        case BotTask::LoadSyteme:
            loadUrl(QString("http://www.omega-day.com/game/?op=main&order=id&first=0&last=5000&galax="+mTasks.head().staticCast<LoadSystemeTask>()->gala()));
            break;
        case BotTask::ParsSystemPage:
            loadUrl( "http://www.omega-day.com/game/?op=system&sys="+mTasks.head().staticCast< ParsSystemPage>()->sysID());
            break;
        case BotTask::ParsOwnPlaneten:
            loadUrl(QString("http://www.omega-day.com/game/?op=planlist"));
            break;
        case BotTask::ParsPlayerLogs:
            loadUrl("http://www.omega-day.com/game/?op=warp&wessen="+mTasks.head().staticCast<ParsPlayerLogs>()->playerID());
            break;
        case BotTask::LoadPage:
            loadUrl(mTasks.head().staticCast<LoadPageTask>()->url());
            break;
        case BotTask::LoadOrbit:
            loadUrl("http://www.omega-day.com/game/?op=orbit&index="+mTasks.head().staticCast< LoadOrbitTask>()->orbitID());
            break;
        case BotTask::LoadPlanet:
            loadUrl("http://www.omega-day.com/game/?op=planet&index="+mTasks.head().staticCast< LoadPlanetTask>()->orbitID());
            break;
        case BotTask::LoadSystem:
            loadUrl("http://www.omega-day.com/game/?op=system&sys="+mTasks.head().staticCast< LoadSystemTask>()->systemID());
            break;
        }
    }
}

void Bot2::nextTask()
{
    qDebug() << Q_FUNC_INFO;
    mTasks.dequeue();
    start();
}

void Bot2::loadUrl(const QString &url)
{
    qDebug() << Q_FUNC_INFO << url;
    mWebView->load(url);
    emit setProgressBarVisible(true);
}

