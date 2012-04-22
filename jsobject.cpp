#include "jsobject.h"

#include <QDebug>
JSObject::JSObject(QObject *parent) :
    QObject(parent)
{

    connect(&mManager,SIGNAL(finished(QNetworkReply*)),this,SLOT(onFinished(QNetworkReply*)));
    login();
}

void JSObject::setManager(QNetworkAccessManager *manager)
{




}

bool JSObject::getState()
{
    return mState;
}

void JSObject::start(QString str, QString data)
{

    mData = data;
    QNetworkRequest re(str);
    re.setAttribute(QNetworkRequest::User,0);
    mManager.get(re);

}

void JSObject::startPost(QString url,QString data)
{


    QByteArray block;
    block.append(data);
    QNetworkRequest re(url);
    re.setAttribute(QNetworkRequest::User,1);
    re.setHeader(QNetworkRequest::ContentTypeHeader,"application/x-www-form-urlencoded");
    mManager.post(re,block);
    mTyp = PARS;
}

void JSObject::startTox(QString url)
{

    QNetworkRequest re(url);
    re.setAttribute(QNetworkRequest::User,2);
    mManager.get(re);
    mTyp = No;
}

void JSObject::setParsData(QString list)
{
    //qDebug() << Q_FUNC_INFO << list;
}

void JSObject::onFinished(QNetworkReply *re)
{
    if(re->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt() == 200){
        mResult =QString::fromUtf8(re->readAll());
        mState = false;
        if(re->request().attribute(QNetworkRequest::User).toInt() == FOW){
            emit loadReady();
        }else if(re->request().attribute(QNetworkRequest::User).toInt() == PARS){
            emit parsReady();
        }

    }else {
        mState = true;
    }




}

void JSObject::login()
{

    QByteArray block;
    block.append("username=abho&pw=fanatik1&autologin=1");
    QNetworkRequest re(QUrl("http://oddb.kryops.de/index.php?p=login&sp=login&inst=11"));
    re.setHeader(QNetworkRequest::ContentTypeHeader,"application/x-www-form-urlencoded");
    mManager.post(re,block);
    mTyp = No;
}

QString JSObject::getResult()
{

    return mResult;
}

QString JSObject::getData()
{
    return mData;
}

void JSObject::post(QString str)
{

}

void JSObject::setEnable(bool isEnable)
{
    if(isEnable){
        QFile file;
        file.setFileName(":/jquery");
        file.open(QIODevice::ReadOnly);
        mJQuery = file.readAll();
        file.close();


        //file.setFileName(":/base");
        file.setFileName("C:\\Users\\HakaH\\Documents\\QT\\PlanisDistanz\\base.js");
        file.open(QIODevice::ReadOnly);
        mInserJS = file.readAll();
        file.close();

    }else {
        mJQuery.clear();
        mInserJS.clear();
    }
}

bool JSObject::isEnable()
{
    return !mJQuery.isEmpty();
}

const QString &JSObject::jquery()
{
    return mJQuery;
}

const QString &JSObject::insertJS()
{
    return mInserJS;
}
