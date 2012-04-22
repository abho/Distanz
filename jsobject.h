#ifndef JSOBJECT_H
#define JSOBJECT_H

#include <QObject>
#include <QNetworkAccessManager>
#include<QNetworkReply>
#include <QFile>

class JSObject : public QObject
{
    Q_OBJECT
public:
    explicit JSObject(QObject *parent = 0);
    void setManager(QNetworkAccessManager *manager);
Q_INVOKABLE bool getState();
    Q_INVOKABLE QString getResult();
     Q_INVOKABLE QString getData();
    Q_INVOKABLE void post(QString str);

    void setEnable(bool isEnable);
    bool isEnable();
    const QString & jquery();
    const QString & insertJS();
signals:
    void loadReady();
    void parsReady();
public slots:
    void start(QString str,QString data);
    void startPost(QString url, QString data);
    void startTox(QString url);
    void setParsData(QString list);


private slots:
    void onFinished(QNetworkReply *re);

private:
    QNetworkAccessManager mManager;
enum Typ {FOW,PARS,TOX,No};
    void login();
    bool mState;
    QString mResult;
    QString mData;
    Typ mTyp;

    QString mJQuery;
    QString mInserJS;
};

#endif // JSOBJECT_H
