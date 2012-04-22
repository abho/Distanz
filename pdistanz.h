#ifndef PDISTANZ_H
#define PDISTANZ_H

#include <QMainWindow>
#include <QtGui>
#include <QtCore>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QtWebKit>
#include "gviewmanager.h"
#include "odplanet.h"
#include "dbhandler.h"
#include "kampf.h"
#include "page.h"
#include "bot.h"


namespace Ui {
class PDistanz;
}

class PDistanz : public QMainWindow
{
    Q_OBJECT
    
public:
    explicit PDistanz(QMainWindow *parent = 0);
    ~PDistanz();
    
private slots:

    void onProgressbarVisible(bool isVisible);
    void onBotParsedSysteme(const QList<QTreeWidgetItem*> &list);
    void onBotUpdateCredits(const QString &str);
    void on_treeWidget_doubleClicked(const QModelIndex &index);
    void on_treeWidget_currentItemChanged(QTreeWidgetItem *current, QTreeWidgetItem *previous);
    void on_pushButtonLoadPlanis_clicked();
    void on_pushButtonCalc_clicked();
    void on_actionLogin_triggered();
    void on_actionShowOwn_toggled(bool arg1);
    void on_actionLoadLogs_triggered(bool checked);
    void onReply(QNetworkReply *re);
    void on_actionAddtoChat_triggered();
    void onChatTimer();
    void onChatPageLoaded(bool x);
    void onBotParsedOwnPlaniList(const QList<ODPlanet> &list);
    void onPrintMessage(QString str);
    void onODDBPageLoad(bool ok);
    void onShowODPageFromODDBTool(QString str);
    void onTextBrowserLinkClicked(QUrl str);
    void onBotParsedPlayLogs(const QList<Kampf> &list);
    void onActionAddToCombat();
    void on_pushButtonChat_clicked();
    void on_webView_customContextMenuRequested(const QPoint &pos);
    void on_pushButton_clicked();
    void on_actionODDBtool_triggered(bool checked);
    void on_actionPlaniEditor_triggered(bool checked);
    void on_actionShipConfig_triggered(bool checked);
    void on_actionAllyForum_triggered(bool checked);
    void on_actionChat_triggered(bool checked);
    void on_actionOD_triggered(bool checked);
    void on_actionGalaScreen_triggered(bool checked);
    void on_actionOption_triggered(bool checked);
    void on_chatEdit_customContextMenuRequested(const QPoint &pos);
    void on_webViewODDB_customContextMenuRequested(const QPoint &pos);
    void onComboBoxGalaCurrentIndexChanged(const QString &gala);
    void onComboBoxAllyCurrentIndexChanged(const QString &ally);

    void on_actionAlly_toggled(bool arg1);

    void on_actionTest_triggered();

    void on_actionScout_triggered(bool checked);

private:
    Ui::PDistanz *ui;

    QComboBox *mComboBoxLogPlayer;
    QComboBox *mComboBoxGala;
    QComboBox *mComboBoxAllys;
    QNetworkAccessManager mManager;
    QSettings mSetting;
    QList<ODPlanet> mOwnPlanis;
    QHash<QString,QTreeWidgetItem*> mSystems;
    Bot mBot;


    QTimer mTimer;
    QProgressBar *mBar;
    QList<int> mLastSizes;
    QLabel *mLabelLogin;
    int mChatIndex;
    DBHandler mDB;
    QAction *mLastAction;
    QLabel *mLabelCredits;
    QLabel *mLabelInfo;
    QSystemTrayIcon mSystemIcon;

    int clacDis(const QString &str, qreal x1,qreal y1,qreal z1);

    void loadFromDB(int gala);
    void login();
    QTreeWidgetItem * colorItem(QTreeWidgetItem *item);

};

#endif // PDISTANZ_H
