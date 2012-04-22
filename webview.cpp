#include "webview.h"
#include <QDebug>
#include <QMouseEvent>
WebView::WebView(QWidget *parent) :
    QWebView(parent)
{
}

void WebView::mousePressEvent(QMouseEvent *event)
{
    qDebug() << event->button();
    QWebView::mousePressEvent(event);
}
