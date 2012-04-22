#-------------------------------------------------
#
# Project created by QtCreator 2012-03-03T18:14:50
#
#-------------------------------------------------

QT       += core gui webkit network sql

TARGET = PlanisDistanz
TEMPLATE = app


SOURCES += main.cpp\
        pdistanz.cpp \
    odplanet.cpp \
    galagv.cpp \
    ../HDB/hdb.cpp \
    dbhandler.cpp \
    kampf.cpp \
    page.cpp \
    jsobject.cpp \
    parser.cpp \
    bot.cpp \
    webview.cpp \
    fpplani.cpp \
    option.cpp \
    bautreewidget.cpp \
    tool.cpp \
    planetentreewidget.cpp \
    shiptreewidget.cpp \
    favtreewidget.cpp \
    pageswebview.cpp \
    nearsys.cpp \
    scoutwidget.cpp \
    sendmenu.cpp \
    bot2.cpp \
    bottask.cpp

HEADERS  += pdistanz.h \
    odplanet.h \
    hdb.h \
    galagv.h \
    ../HDB/hdb.h \
    dbhandler.h \
    kampf.h \
    page.h \
    jsobject.h \
    parser.h \
    bot.h \
    webview.h \
    fpplani.h \
    option.h \
    bautreewidget.h \
    tool.h \
    planetentreewidget.h \
    shiptreewidget.h \
    favtreewidget.h \
    pageswebview.h \
    nearsys.h \
    scoutwidget.h \
    sendmenu.h \
    bot2.h \
    bottask.h

FORMS    += \
    pdistanz.ui \
    fpplani.ui \
    option.ui \
    nearsys.ui \
    scoutwidget.ui

RESOURCES += \
    res.qrc
















