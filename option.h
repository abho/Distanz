#ifndef OPTION_H
#define OPTION_H

#include <QWidget>
#include <QtCore>
#include <QtGui>
#include <QDebug>

namespace Ui {
class Option;
}

class Option : public QWidget
{
    Q_OBJECT
    
public:
    explicit Option(QWidget *parent = 0);
  const QStringList& ids();
  void setIds(const QStringList &ids);
    ~Option();
  void paintEvent(QPaintEvent *);
    
signals:
  void loadGala(int);
private slots:

    void on_listWidget_customContextMenuRequested(const QPoint &pos);

    void on_pushButtonOption_clicked();

    void on_pushButton_clicked();

    void on_pushButtonRmFlotte_clicked();

    void on_pushButtonEmSquad_clicked();

private:
    Ui::Option *ui;
    QStringList mIDs;
    QSettings mSettings;

    void saveFav(QComboBox *b);
    void restore(QComboBox *b);
};

#endif // OPTION_H
