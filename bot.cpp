#include "bot.h"

Bot::Bot(QObject *parent) :
    QObject(parent),mParsSystemPage(false),mParsPlayerLog(false),mParsPostSendPage(false),mParsGala(false)
{
    QFile file;
    file.setFileName(":/jquery");
    file.open(QIODevice::ReadOnly);
    mJQuery = file.readAll();
    file.close();



}

void Bot::setViewAndDBAndOption(QWebView *view, DBHandler &db,Option *option)
{
    qDebug() << Q_FUNC_INFO;
    mView =view;
    connect(mView,SIGNAL(loadFinished(bool)),this,SLOT(onLoadFinished(bool)));
    connect(mView->page()->currentFrame(),SIGNAL(loadFinished(bool)),this,SLOT(onCurrentFrameLoadFinished(bool)));
    connect(mView->page()->networkAccessManager(),SIGNAL(finished(QNetworkReply*)),this,SLOT(onRequestFinished(QNetworkReply*)));
    mView->load(QUrl("http://www.omega-day.com/game/?op=comm"));
    mOption = option;
    mParser = new Parser(option,&db,this);
}

void Bot::login(){
    qDebug() << Q_FUNC_INFO;
    QWebFrame *frame = mView->page()->currentFrame();
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

QString Bot::parsCurrentToAdd()
{
    return mParser->parsAddPage(mView->page()->currentFrame());
}

void Bot::sendShip(const QString &shipID, const QString &planiID, const QString &zielID)
{
    qDebug() << Q_FUNC_INFO << shipID << zielID;
    mView->load("http://www.omega-day.com/game/?op=send&index="+planiID+"&"+"ships="+shipID);

    mSendShipsID.first.clear();
    mSendShipsID.first.append(shipID);
    mSendShipsID.second = zielID;
}

void Bot::loadOrbit(const QString &orbitID)
{
    mView->load("http://www.omega-day.com/game/?op=orbit&index="+orbitID);
    emit setProgressBarVisible(true);
}

void Bot::loadPlaneten(const QString &orbitID)
{
    qDebug() << Q_FUNC_INFO << orbitID;
    mView->load("http://www.omega-day.com/game/?op=planet&index="+orbitID);
    emit setProgressBarVisible(true);
}

void Bot::loadSystem(const QString &systemID)
{
    mView->load("http://www.omega-day.com/game/?op=system&sys="+systemID);
    emit setProgressBarVisible(true);
}

void Bot::loadGala(int gala)
{
    qDebug() << Q_FUNC_INFO << gala;
    mParsGala = true;
    mOldSysteme= new QHash<QString,QTreeWidgetItem*>();
    mView->load(QUrl("http://www.omega-day.com/game/?op=main&order=id&first=0&last=5000&galax="+QString::number(gala)));
    emit setProgressBarVisible(true);
}

void Bot::parsPlanetenPage()
{
    mParser->parsPlanetenPage(mView->page()->mainFrame());
}

void Bot::sendShips(const QList<QString> &ships, const QString &zielID)
{
qDebug() << Q_FUNC_INFO << ships << zielID;
    mSendShipsID.first =ships;
    mSendShipsID.second=zielID;
    mView->load(QUrl("http://www.omega-day.com/game/?op=fleet"));
}

void Bot::fetchPages()
{

    QNetworkRequest re(QUrl("http://www.omega-day.com/game/?op=fleet"));
    re.setAttribute(QNetworkRequest::User,"fleet");
    mView->page()->networkAccessManager()->get(re);

    for (int var = 0; var < mOption->ids().count(); ++var) {
        const QString &id =mOption->ids().at(var);
        QNetworkRequest re(QUrl("http://www.omega-day.com/game/?op=planet&index="+id));
        re.setAttribute(QNetworkRequest::User,id);
        mView->page()->networkAccessManager()->get(re);
    }

}

QTreeWidgetItem *Bot::parsFavPage()
{
    return mParser->parsFavPage(mView->page()->mainFrame());
}

void Bot::onLoadFinished(bool)
{
    QString url = mView->url().toString();
    qDebug() << Q_FUNC_INFO << url << mOption->ids();
    QWebFrame *frame = mView->page()->mainFrame();
    if(url== "http://www.omega-day.com/game/?op=fleet")
    {
        if(mSendShipsID.first.isEmpty())
            emit parsedFlottenAnsicht( mParser->parsFlottenAnsicht(mView->page()->mainFrame()));
        else
            mParser->sendShips(frame,mSendShipsID.first);
    }
    else if(url.contains("http://www.omega-day.com/game/?op=main&order=id&first=0&last=5000&galax")&&mParsGala)
    {
        qDebug() << Q_FUNC_INFO;
      mParser->parsSystemListPage(mView->page()->mainFrame(),mOldSysteme);
      mParsGala =false;
    }
    else if(url.contains("op=system&sys=") && mParsSystemPage)
    {
        mParsSystemPage = false;
        emit parsedSystem(mParser->parsSystemPage(mView->page()->mainFrame()));
    }
    else if(url.contains("op=planlist"))
    {
        emit parsedOwnPanis(mParser->parsOwnPlaniListe(mView->page()->mainFrame()));
    }


    else if(url.contains("http://www.omega-day.com/game/?op=warp&wessen=")&&mParsPlayerLog)
    {
        mParsPlayerLog =false;
        emit parsedPlayerLogs(mParser->parsPlayerLog(mView->page()->currentFrame()));
    }
    else if(!mSendShipsID.first.isEmpty()&& url.contains("op=send")&&!mParsPostSendPage)
    {
        mParser->parsSendShipPage(frame,mSendShipsID.second);
        mParsPostSendPage = true;
    }
    else if(!mSendShipsID.first.isEmpty()&&mParsPostSendPage && url.contains("op=send"))
    {

        QStringList time =mParser->parsPostSendPage(frame);
        QList<QStringList> l;
        for (int var = 0; var < mSendShipsID.first.count(); ++var) {
            l.append(QStringList() << mSendShipsID.first.at(var) << mSendShipsID.second<<time.at(var));
        }
        emit shipsSended(l);
        mSendShipsID.first.clear();
        mParsPostSendPage =false;

        mView->load(QUrl("http://www.omega-day.com/game/?op=fleet"));
    } else if( url.contains("planet&index") && mOption->ids().contains(url.split("=").last())){
        QPair<QTreeWidgetItem *,QString> pair = mParser->parsPlanetenPage(frame);
        emit updateCredits(pair.second);
        emit parsedPlanetenPage( pair.first);
    } else if(url.contains("werft2")){
        emit parsedWerft2Page(mParser->parsWerft2Page(frame));
    } else{
        mSendShipsID.first.clear();
        mParsPostSendPage =false;
        mParsSystemPage = false;
    }

    emit setProgressBarVisible(false);

}


void Bot::insertJS()
{
    QFile file;
    //file.setFileName(":/base");
    file.setFileName("C:\\Users\\HakaH\\Documents\\QT\\PlanisDistanz\\base.js");
    file.open(QIODevice::ReadOnly);
    QString  code = file.readAll();
    file.close();
    mView->page()->currentFrame()->evaluateJavaScript(mJQuery);
    mView->page()->currentFrame()->addToJavaScriptWindowObject("jsobject",&mJSObject);
    mView->page()->mainFrame()->evaluateJavaScript(code);
    mView->page()->mainFrame()->evaluateJavaScript("oddbtool.loadPage();");

    QString url = mView->page()->currentFrame()->url().toString();
    emit setMessage(url);

}

void Bot::onRequestFinished(QNetworkReply *re)
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

void Bot::onCurrentFrameLoadFinished(bool ok)
{
    if(!mView->page()->currentFrame()->url().toString().contains("http://oddb.kryops.de"))
        insertJS();
}

void Bot::loadSysteme(QHash<QString, QTreeWidgetItem *> *oldSysteme,int gala )
{
    mOldSysteme = oldSysteme;
    mView->load(QUrl("http://www.omega-day.com/game/?op=main&order=id&first=0&last=5000&galax=7"));
    emit setProgressBarVisible(true);
}

void Bot::parsSystemPage(const QString &sysID,bool pars)
{
    mParsSystemPage = pars;
    mView->load(QUrl("http://www.omega-day.com/game/?op=system&sys="+sysID));
    emit setProgressBarVisible(true);
}

void Bot::parsOwnPlaneten()
{
    mView->load(QUrl("http://www.omega-day.com/game/?op=planlist"));
    emit setProgressBarVisible(true);
}

void Bot::parsPlayerLogs(const QString &playerID)
{
    mParsPlayerLog = true;
    mView->load("http://www.omega-day.com/game/?op=warp&wessen="+playerID);
    emit setProgressBarVisible(true);
}

void Bot::test()
{
    mParser->parsSystem(mView->page()->mainFrame());
}

void Bot::parsScoutPage(QWebFrame *frame)
{
    emit parsedScoutPage(mParser->parsScoutPage(frame));
}

void Bot::parsCurrent(QWebFrame *frame)
{
    mParser->parsCurrentODDBPage(frame);
}

void Bot::loadPage(QString str)
{
    mView->load(QUrl(str));
    emit setProgressBarVisible(true);
}

