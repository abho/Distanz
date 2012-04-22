#include "page.h"
#include <QDebug>

Page::Page(QObject *parent) :
    QWebPage(parent)
{
}

bool Page::acceptNavigationRequest(QWebFrame *frame, const QNetworkRequest &request, QWebPage::NavigationType type)
{

    QString url = request.url().toString();
    if(url.contains("http://www.omega-day.com/game/"))
        emit loadPage(url);
    // qDebug() << frame << request.url().toString();
    return QWebPage::acceptNavigationRequest(frame,request,type);

}

void Page::javaScriptConsoleMessage(const QString &message, int lineNumber, const QString &sourceID)
{
    qDebug() << Q_FUNC_INFO;
    qDebug() << "ERROR:" << lineNumber << message;
    QWebPage::javaScriptConsoleMessage(message,lineNumber,sourceID);
}
