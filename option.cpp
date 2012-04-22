#include "option.h"
#include "ui_option.h"

Option::Option(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Option),mSettings("hakah","sendmenu")
{
    ui->setupUi(this);

    restore(ui->comboBoxFavFlotte);

    restore(ui->comboBoxFavSquad);
}

const QStringList &Option::ids()
{
    return mIDs;
}

void Option::setIds(const QStringList &ids)
{
    ui->listWidget->addItems(ids);
    mIDs =ids;
}

Option::~Option()
{
    delete ui;
}

void Option::paintEvent(QPaintEvent *)
{
    QStyleOption opt;
    opt.init(this);
    QPainter p(this);
    style()->drawPrimitive(QStyle::PE_Widget, &opt, &p, this);
}


void Option::on_listWidget_customContextMenuRequested(const QPoint &pos)
{
    QListWidgetItem *item = ui->listWidget->itemAt(pos);

    if(item!= NULL){
        QMenu *m = new QMenu(this);
        QAction *rm = m->addAction("RM");

        QAction *ret = m->exec(ui->listWidget->mapToGlobal(pos));

        if(ret == rm){
            mIDs.removeOne(item->text());
            delete item;
        }
    }
}

void Option::on_pushButtonOption_clicked()
{
    QString str = ui->lineEdit->text();
    if(!str.isEmpty()){
        if(!mIDs.contains(str)){
            ui->listWidget->addItem(str);
            mIDs.append(str);
        }
    }
}

void Option::on_pushButton_clicked()
{
    emit loadGala(ui->spinBoxGalaLoad->value());
}

void Option::on_pushButtonRmFlotte_clicked()
{
    ui->comboBoxFavFlotte->removeItem(ui->comboBoxFavFlotte->currentIndex());
    saveFav(ui->comboBoxFavFlotte);
}

void Option::saveFav(QComboBox *b)
{
    QString title;
    if(b == ui->comboBoxFavFlotte){
        title ="flotte";
    }else {
        title = "squad";
    }
    QStringList l;
    for (int var = 0; var < b->count(); ++var) {
        l << b->itemText(var);
    }
    mSettings.setValue(title,l);
}

void Option::restore(QComboBox *b)
{
    QString title;
    if(b == ui->comboBoxFavFlotte){
        title ="flotte";
    }else {
        title = "squad";
    }
    QStringList l = mSettings.value(title).toStringList();
    b->addItems(l);

}

void Option::on_pushButtonEmSquad_clicked()
{
    ui->comboBoxFavSquad->removeItem(ui->comboBoxFavSquad->currentIndex());
    saveFav(ui->comboBoxFavSquad);
}
