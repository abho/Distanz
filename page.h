#ifndef PAGE_H
#define PAGE_H

#include <QWebPage>
#include <QWebFrame>
#include <QNetworkRequest>

class Page : public QWebPage
{
    Q_OBJECT
public:
    explicit Page(QObject *parent = 0);
    
protected:
    bool acceptNavigationRequest(QWebFrame *frame, const QNetworkRequest &request, NavigationType type);
void	javaScriptConsoleMessage(const QString &message, int lineNumber, const QString &sourceID);
signals:
void loadPage(QString);
public slots:
    
};

#endif // PAGE_H
