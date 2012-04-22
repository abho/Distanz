#ifndef SENDMENU_H
#define SENDMENU_H
#include <QtCore>
#include <QtGui>
#include <QDebug>
#include "shiptreewidget.h"

class SendMenu
{
public:
    SendMenu(ShipTreeWidget *tree);
    ~SendMenu();
    QMenu* menu();

    void addSendToMark();
    const QStringList& checkedID();
    const QString & orbitID();
    bool sendToMarkWasClicked();
    void setDisable(bool x);
    QAction* clickedSendAction();
    bool exit();
    bool checkSendClicked(QAction *ret);
private:
    enum Typ {Flotte,Squad};
    void saveChecked(Typ typ);
    void restorChecked(QAction *a);

    QMenu *mMenu;
    ShipTreeWidget *mTree;
    QAction *mSendFlotte;
    QAction *mSendSquad;
    QAction *mHierFlotte;
    QAction *mHierSquad;
    QAction *mMakiertFlotte;
    QAction *mMakiertSquad;
    QAction *mTmpClicked;
    QAction *mSetFavFlotte;
    QAction *mSetFavSquad;
    QList<QAction*> mFlottenActions;
    QList<QAction*> mSquadActions;
    bool mSendToMark;
    QString mOrbitID;
    bool mSendToMarksClicked;
    bool mExit;
    QStringList mIDs;
    QSettings mSettings;
    QMenu *mFavFlotte;
    QMenu * mFavSquad;


};

#endif // SENDMENU_H
