#ifndef SCOUTWIDGET_H
#define SCOUTWIDGET_H

#include <QDebug>
#include <QtGui>
#include <QtCore>
#include <QNetworkAccessManager>
#include <QWebFrame>
#include <QWebElementCollection>
#include "dbhandler.h"
#include "tool.h"
#include "parser.h"
#include "jsobject.h"
#include "bottask.h"
#include "bot2.h"
#include "option.h"

namespace Ui {
class ScoutWidget;
}

class ScoutWidget : public QWidget
{
    Q_OBJECT
    
public:
    explicit ScoutWidget(QWidget *parent);
    void setDBAndManager(DBHandler *db, QNetworkAccessManager *manager, Option *option);
    ~ScoutWidget();
    enum Typ {System, Orbit,No};

public slots:
    void onBotParsedScoutPage(const QList<QPair<QString,QString> > & list);
private slots:

    void on_pushButtonAdd_clicked();

    void on_pushButtonCalc_clicked();
    void onTimerTimeout();
    void onSecTimer();

    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

    void on_pushButtonClear_clicked();
    void onBotParsedPostSendPage(const QList<QStringList> &list);
    void onBotLoadSystemPage(const QString &id);
    void onBotLoadSOrbitPage(const QString &id);

private:
    Ui::ScoutWidget *ui;

    Bot2 *mBot;
    Typ mTyp;
    DBHandler *mDB;
    int mIndex;
    int mChildIndex;
    QTimer *mTimer;
    QTimer *mSTimer;
    QSettings mSetting;


    void addToTree(const QString &sysID,const QString &planiID);
    void nextStep();
};

#endif // SCOUTWIDGET_H
